"""Run with: python3 tests/tools/test_code2pdf.py (requires reportlab and pygments)."""
import importlib.util
from pathlib import Path
from unittest.mock import Mock

spec = importlib.util.spec_from_file_location('code2pdf', Path(__file__).resolve().parents[2] / 'tools/code2pdf.py')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
module.configure_font()

size = module.FONT_SIZE
width = module.stringWidth
assert width('iii', module.FONT, size) == width('MMM', module.FONT, size)
canvas = Mock()
text = '中文 English'
end = module.draw_mixed_fixed(canvas, text, 0, 0, size)
expected = width('中文', module.CJK_FONT, size) + width(' English', module.FONT, size)
assert abs(end - expected) < 1e-9
assert canvas.drawString.call_count == 2  # Whole runs, without artificial letter spacing.
canvas.reset_mock()
module.draw_mixed_proportional(canvas, text, 0, 0, size)
assert canvas.setFont.call_count == 2
assert canvas.drawString.call_count == 2
chunks = module.wrap_fixed('abc中文xyz' * 3, 40)
assert ''.join(chunks) == 'abc中文xyz' * 3
for chunk in chunks:
    assert module.draw_mixed_fixed(Mock(), chunk, 0, 0, size) <= 40 + 1e-9
print('Font metrics and mixed-text rendering checks passed')

# Exercise wrapped rows longer than an entire page, including CJK and tabs.
from tempfile import TemporaryDirectory
from unittest.mock import patch

page_width, page_height = module.A4
source = '\t// 中文 mixed text ' + 'abc中文xyz ' * 1500
pages = module.source_pages(source, page_width, page_height, True)
assert len(pages) > 1
assert ''.join(row[2] for page in pages for row in page) == source.expandtabs(4)
assert all(module.MARGIN + 8 <= y <= page_height - module.MARGIN - module.HEADER_GAP - 5
           for page in pages for y, prefix, chunk in page)
assert all(module.MARGIN + module.mixed_proportional_width(prefix + chunk, size)
           <= page_width - module.MARGIN + 1e-8
           for page in pages for y, prefix, chunk in page)
assert pages[0][0][1].strip() == '1'
assert all(prefix.strip() == '' for page in pages for y, prefix, chunk in page
           if prefix.strip() != '1')

# Record actual drawing calls to compare printed TOC references with source pages.
with TemporaryDirectory() as directory:
    root = Path(directory)
    files = [root / f'file{i:03}.hpp' for i in range(170)]
    for i, path in enumerate(files):
        path.write_text(source if i == 0 else 'int value = 0;\n' * (150 if i == 1 else 1))
    sections = [('测试分类', 0), ('另一分类', 90)]
    recorded = [[]]
    canvas = Mock()
    canvas.showPage.side_effect = lambda: recorded.append([])
    canvas.drawString.side_effect = lambda x, y, text: recorded[-1].append(('text', x, y, text))
    canvas.drawCentredString.side_effect = lambda x, y, text: recorded[-1].append(('center', x, y, text))
    canvas.drawRightString.side_effect = lambda x, y, text: recorded[-1].append(('right', x, y, text))
    with patch.object(module, 'Canvas', return_value=canvas):
        module.build_pdf(root, root / 'book.pdf', 'test', files, sections, True, 'portrait', False, 'friendly', 'Team ABC')
    assert canvas.setTitle.call_args.args[0] == 'test'
    assert canvas.setAuthor.call_args.args[0] == 'Team ABC'
    assert any(entry[3] == 'test' for entry in recorded[0])
    assert any(entry[3] == 'Team ABC' for entry in recorded[0])
    references = {}
    actual = {}
    toc_count = len(module.toc_layout(root, files, sections, page_width, page_height))
    for page_number, entries in enumerate(recorded, 1):
        for position, (kind, x, y, text) in enumerate(entries):
            name = text.lstrip('· ').strip()
            name = name.split(' (', 1)[0]
            if kind not in ('text', 'center') or not name.startswith('file'):
                continue
            if y == page_height - module.MARGIN:
                actual.setdefault(name, page_number - toc_count - 2)
            else:
                next_entry = entries[position + 1]
                assert next_entry[0] == 'right' and next_entry[2] == y
                references[name] = int(next_entry[3])
    assert toc_count > 1
    assert references == actual and len(actual) == len(files)
    body, file_page_map = module.column_pages(root, files, page_width, page_height, True)
    first_pages = {}
    recovered = {path: [] for path in files}
    col_width = (page_width - 2 * module.MARGIN - 18) / 2
    for page_index, rows in enumerate(body, 1):
        for x, y, prefix, text, path, is_heading in rows:
            assert module.MARGIN + 8 <= y <= page_height - module.MARGIN - module.HEADER_GAP - 5
            assert x + module.mixed_proportional_width(prefix + text, size) <= x + col_width + 1e-8
            if is_heading:
                first_pages.setdefault(path, page_index)
            else:
                recovered[path].append(text)
    assert first_pages == file_page_map
    for path in files:
        assert ''.join(recovered[path]) == ''.join(line.expandtabs(4) for line in path.read_text().splitlines())
    assert len(body) < sum(len(module.source_pages(module.read_source(p), page_width, page_height, True)) for p in files)
    recorded = [[]]
    canvas.reset_mock()
    with patch.object(module, 'Canvas', return_value=canvas):
        total = module.build_pdf(root, root / 'columns.pdf', 'test', files, sections, True,
                                 'portrait', False, 'friendly', columns=2)
    assert total == len(recorded) == len(body) + toc_count + 2
    references, actual = {}, {}
    for page_number, entries in enumerate(recorded, 1):
        for position, (kind, x, y, text) in enumerate(entries):
            name = text.lstrip('· ').strip().split(' (', 1)[0]
            if kind != 'text' or not name.startswith('file'):
                continue
            if page_number > toc_count + 2:
                actual.setdefault(name, page_number - toc_count - 2)
            elif page_number > 2:
                next_entry = entries[position + 1]
                assert next_entry[0] == 'right' and next_entry[2] == y
                references[name] = int(next_entry[3])
    assert references == actual and len(actual) == len(files)
    with patch.object(module, 'Canvas') as canvas_factory:
        try:
            module.build_pdf(root, root / 'limited.pdf', 'test', files, sections, True,
                             'portrait', False, 'friendly', columns=2, max_pages=1)
        except SystemExit as exc:
            assert 'limit is 1' in str(exc)
        else:
            raise AssertionError('Expected the page limit to reject this book')
        canvas_factory.assert_not_called()

    # Test --with-docs and --docs-only interleaving and section index remapping
    docs_dir = root / 'docs'
    docs_dir.mkdir()
    doc0 = docs_dir / 'file000.md'
    doc1 = docs_dir / 'file001.md'
    doc0.write_text('# File 000 Doc\nUsage details\n')
    doc1.write_text('# File 001 Doc\nUsage details\n')
    interleaved_files, interleaved_sections = module.apply_docs_mode(
        [files[0], files[1]], [('Sec A', 0), ('Sec B', 1)], docs_dir, False
    )
    assert interleaved_files == [doc0, files[0], doc1, files[1]]
    assert interleaved_sections == [('Sec A', 0), ('Sec B', 2)]
    docs_only_files, docs_only_sections = module.apply_docs_mode(
        [files[0], files[1]], [('Sec A', 0), ('Sec B', 1)], docs_dir, True
    )
    assert docs_only_files == [doc0, doc1]
    assert docs_only_sections == [('Sec A', 0), ('Sec B', 1)]
print('Pagination, overflow, and printed TOC page references passed')
