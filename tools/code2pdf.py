#!/usr/bin/env python3
"""Turn a source tree into a print-first A4 codebook for hand-copying."""

from __future__ import annotations

import argparse
from datetime import date
import os
import sys
import unicodedata
from itertools import groupby
from pathlib import Path

try:
    from pygments import lex
    from pygments.lexers import get_lexer_for_filename, TextLexer
    from pygments.styles import get_style_by_name
    from pygments.token import Token
except ImportError as exc:  # pragma: no cover - exercised by users without deps
    raise SystemExit(
        "Missing dependency: Pygments. Install with: python3 -m pip install pygments reportlab"
    ) from exc

try:
    from reportlab.lib import colors
    from reportlab.lib.pagesizes import A4, landscape
    from reportlab.pdfbase.pdfmetrics import stringWidth
    from reportlab.pdfbase import pdfmetrics
    from reportlab.pdfbase.ttfonts import TTFont
    from reportlab.pdfbase.cidfonts import UnicodeCIDFont
    from reportlab.pdfgen.canvas import Canvas
except ImportError as exc:  # pragma: no cover - exercised by users without deps
    raise SystemExit(
        "Missing dependency: ReportLab. Install with: python3 -m pip install pygments reportlab"
    ) from exc


DEFAULT_EXTENSIONS = {
    ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx",
    ".java", ".js", ".jsx", ".ts", ".tsx", ".go", ".rs", ".py",
    ".rb", ".kt", ".swift", ".cs", ".php", ".lua", ".sh", ".sql",
}
DEFAULT_EXCLUDES = {".git", ".idea", ".vscode", "build", "dist", "target", "node_modules"}
FONT = "Courier"
CJK_FONT = None
FONT_SIZE = 8.5
LINE_HEIGHT = 11.0
TITLE_SIZE = 16.0
MARGIN = 36.0
HEADER_GAP = 28.0
COLOR_OUTPUT = False
PYGMENTS_STYLE = get_style_by_name("friendly")
DEFAULT_TITLE = "算法竞赛模板库"
AUTHOR = "evilayuria"
FILE_LABELS: dict[Path, str] = {}


def configure_font() -> None:
    """Use a real monospace font, plus a separate CJK fallback font."""
    global FONT, CJK_FONT

    def register_ttf(name: str, candidates: tuple[str, ...], needs_cjk: bool = False) -> str | None:
        for candidate in candidates:
            if not os.path.isfile(candidate):
                continue
            indices = range(8) if candidate.lower().endswith(".ttc") else (0,)
            for index in indices:
                try:
                    font_name = f"{name}{index}"
                    pdfmetrics.registerFont(TTFont(font_name, candidate, subfontIndex=index))
                    face = pdfmetrics.getFont(font_name).face
                    if needs_cjk and ord("中") not in face.charWidths:
                        continue
                    if not needs_cjk:
                        widths = face.charWidths
                        if not all(widths.get(ord(a)) == widths.get(ord("M")) for a in "Mi "):
                            continue
                    return font_name
                except Exception:
                    continue
        return None

    mono = register_ttf("CodeBookMono", (
        "/System/Library/Fonts/Menlo.ttc",
        "/System/Library/Fonts/SFNSMono.ttf",
        "/System/Library/Fonts/Supplemental/PTMono.ttc",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/local/share/fonts/DejaVuSansMono.ttf",
    ))
    if mono:
        FONT = mono

    CJK_FONT = register_ttf("CodeBookCJK", (
        "/System/Library/Fonts/STHeiti Light.ttc",
        "/System/Library/Fonts/STHeiti Medium.ttc",
        "/System/Library/Fonts/Hiragino Sans GB.ttc",
        "/System/Library/Fonts/PingFang.ttc",
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
    ), needs_cjk=True)
    if CJK_FONT is None:
        try:
            pdfmetrics.registerFont(UnicodeCIDFont("STSong-Light"))
            CJK_FONT = "STSong-Light"
        except Exception:
            CJK_FONT = FONT


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Create a print-friendly A4 PDF codebook from a source directory."
    )
    parser.add_argument("root", nargs="?", default=".", help="source directory (default: current directory)")
    parser.add_argument("-o", "--output", default="codebook.pdf", help="output PDF path")
    parser.add_argument("--title", default=DEFAULT_TITLE, help=f"PDF title (default: {DEFAULT_TITLE})")
    parser.add_argument("--author", default=AUTHOR, help=f"cover author (default: {AUTHOR})")
    parser.add_argument(
        "--config", "-c", metavar="FILE",
        help="config file listing files to include and their order (supports 'file.hpp | 中文标题')",
    )
    parser.add_argument(
        "--orientation", choices=("portrait", "landscape"), default="portrait",
        help="page orientation (default: portrait)",
    )
    color_group = parser.add_mutually_exclusive_group()
    color_group.add_argument("--color", action="store_true", dest="color", help="enable syntax colors")
    color_group.add_argument(
        "--black-and-white", action="store_false", dest="color",
        help="use monochrome output (the default, suitable for printing)",
    )
    parser.set_defaults(color=False)
    parser.add_argument(
        "--compact", action="store_true",
        help="use the old small type and tight spacing to reduce page count",
    )
    parser.add_argument("--font-size", type=float, help="override code font size in points")
    parser.add_argument("--line-height", type=float, help="override line spacing in points")
    parser.add_argument("--columns", type=int, choices=(1, 2), default=1,
                        help="code columns (default: 1; 2 packs files continuously)")
    parser.add_argument("--max-pages", type=int, help="fail before writing if total pages exceed this limit")
    parser.add_argument(
        "--style", default="friendly",
        help="Pygments style for --color (default: friendly)",
    )
    parser.add_argument("--no-line-numbers", action="store_true", help="hide line numbers")
    parser.add_argument("--no-recursive", action="store_true", help="only scan files directly under root")
    parser.add_argument(
        "--ext", action="append", metavar="EXT",
        help="extra extension to include, e.g. --ext .md (repeatable)",
    )
    parser.add_argument(
        "--exclude", action="append", metavar="NAME",
        help="directory or file name to skip (repeatable)",
    )
    return parser.parse_args()


def display_path(path: Path, root: Path) -> str:
    """Return configured Chinese title if present, else relative path."""
    if path in FILE_LABELS:
        return FILE_LABELS[path]
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return path.name


def read_config_file(config_path: Path, root: Path) -> tuple[list[Path], list[tuple[str, int]]] | None:
    """Read config file and return list of file paths and sections in order.

    Supports optional Chinese display titles via:
    filename.hpp | 中文标题
    """
    if not config_path.is_file():
        return None

    files = []
    sections = []
    FILE_LABELS.clear()
    try:
        for line in config_path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            if line.startswith("[") and line.endswith("]"):
                section_name = line[1:-1].strip()
                sections.append((section_name, len(files)))
                continue

            label = None
            if "|" in line:
                rel_part, label_part = line.split("|", 1)
                rel_name = rel_part.strip()
                label = label_part.strip()
            else:
                rel_name = line

            file_path = root / rel_name
            if file_path.is_file():
                files.append(file_path)
                if label:
                    FILE_LABELS[file_path] = label
            else:
                print(f"Warning: File not found in config: {rel_name}", file=sys.stderr)
    except Exception as e:
        print(f"Error reading config file: {e}", file=sys.stderr)
        return None

    return files, sections


def source_files(
    root: Path,
    recursive: bool,
    extensions: set[str],
    excludes: set[str],
    output: Path,
    excluded_files: set[Path],
) -> list[Path]:
    iterator = root.rglob("*") if recursive else root.glob("*")
    files = []
    for path in iterator:
        if not path.is_file() or path == output or path.resolve() in excluded_files:
            continue
        if any(part in excludes or part.startswith(".") for part in path.relative_to(root).parts):
            continue
        if path.suffix.lower() in extensions:
            files.append(path)
    return sorted(files, key=lambda p: p.relative_to(root).as_posix().lower())


def lexer_for(path: Path):
    try:
        return get_lexer_for_filename(path.name)
    except Exception:
        return TextLexer()


def token_color(token) -> colors.Color:
    if not COLOR_OUTPUT:
        return colors.black
    value = PYGMENTS_STYLE.style_for_token(token).get("color")
    return colors.HexColor(f"#{value}") if value else colors.HexColor("#24292e")


def wide_character(char: str) -> bool:
    return unicodedata.east_asian_width(char) in "WF"


def has_wide(text: str) -> bool:
    return any(wide_character(c) for c in text)


def roman(number: int) -> str:
    result = []
    for value, symbol in ((1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
                          (100, "C"), (90, "XC"), (50, "L"), (40, "XL"),
                          (10, "X"), (9, "IX"), (5, "V"), (4, "IV"), (1, "I")):
        count, number = divmod(number, value)
        result.append(symbol * count)
    return "".join(result)


def draw_mixed_fixed(canvas: Canvas, text: str, x: float, y: float, size: float) -> float:
    """Keep code monospaced while drawing CJK runs at their natural width."""
    cursor = x
    for wide, characters in groupby(text, key=wide_character):
        run = "".join(characters)
        font = CJK_FONT if wide else FONT
        canvas.setFont(font, size)
        canvas.drawString(cursor, y, run)
        cursor += stringWidth(run, font, size)
    return cursor


def mixed_proportional_width(text: str, size: float) -> float:
    return sum(
        stringWidth(run, CJK_FONT if wide else FONT, size)
        for wide, characters in groupby(text, key=wide_character)
        for run in ("".join(characters),)
    )


def draw_mixed_proportional(canvas: Canvas, text: str, x: float, y: float, size: float) -> float:
    return draw_mixed_fixed(canvas, text, x, y, size)


def draw_mixed_centered(canvas: Canvas, text: str, center: float, y: float, size: float) -> None:
    draw_mixed_proportional(
        canvas, text, center - mixed_proportional_width(text, size) / 2, y, size
    )


def wrap_fixed(text: str, max_width: float) -> list[str]:
    chunks, current, width = [], [], 0.0
    for char in text:
        char_width = stringWidth(char, CJK_FONT if wide_character(char) else FONT, FONT_SIZE)
        if current and width + char_width > max_width:
            chunks.append("".join(current))
            current, width = [], 0.0
        current.append(char)
        width += char_width
    if current or not chunks:
        chunks.append("".join(current))
    return chunks


def read_source(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def source_pages(text: str, width: float, height: float, line_numbers: bool):
    """Lay out once; rendering and the printed index use these exact pages."""
    start_y = height - MARGIN - HEADER_GAP - 5
    bottom = MARGIN + 8
    pages = [[]]
    y = start_y
    for number, line in enumerate(text.splitlines(), 1):
        prefix = f"{number:>5}  " if line_numbers else "  "
        available = width - 2 * MARGIN - stringWidth(prefix, FONT, FONT_SIZE)
        chunks = wrap_fixed(line.expandtabs(4), available)
        span = (len(chunks) - 1) * LINE_HEIGHT
        if y - span < bottom and start_y - span >= bottom and pages[-1]:
            pages.append([])
            y = start_y
        for index, chunk in enumerate(chunks):
            if y < bottom:
                pages.append([])
                y = start_y
            marker = prefix if index == 0 else " " * len(prefix)
            pages[-1].append((y, marker, chunk))
            y -= LINE_HEIGHT
    return pages


def column_pages(root: Path, files: list[Path], width: float, height: float, line_numbers: bool):
    """Pack files from the left column to the right, then to the next page."""
    gap = 18
    col_width = (width - 2 * MARGIN - gap) / 2
    start_y = height - MARGIN - HEADER_GAP - 5
    bottom = MARGIN + 8
    pages, file_page_map = [[]], {}
    column, y = 0, start_y

    def advance():
        nonlocal column, y
        column += 1
        if column == 2:
            pages.append([])
            column = 0
        y = start_y

    def heading(path: Path, continued: bool):
        nonlocal y
        text = display_path(path, root) + (" (cont.)" if continued else "")
        chunks = wrap_fixed(text, col_width)
        if y - len(chunks) * LINE_HEIGHT < bottom:
            advance()
        x = MARGIN + column * (col_width + gap)
        for chunk in chunks:
            pages[-1].append((x, y, "", chunk, path, True))
            y -= LINE_HEIGHT

    for path in files:
        heading(path, False)
        file_page_map[path] = len(pages)
        for number, line in enumerate(read_source(path).splitlines(), 1):
            prefix = f"{number:>5}  " if line_numbers else "  "
            chunks = wrap_fixed(line.expandtabs(4), col_width - stringWidth(prefix, FONT, FONT_SIZE))
            for index, chunk in enumerate(chunks):
                if y < bottom:
                    advance()
                    heading(path, True)
                marker = prefix if index == 0 else " " * len(prefix)
                x = MARGIN + column * (col_width + gap)
                pages[-1].append((x, y, marker, chunk, path, False))
                y -= LINE_HEIGHT
        y -= LINE_HEIGHT
    return pages, file_page_map


def toc_layout(root: Path, files: list[Path], sections: list[tuple[str, int]], width: float, height: float):
    """Plan both columns, keeping each section title with its first entry."""
    col_width = (width - 3 * MARGIN) / 2
    start_y = height - MARGIN - HEADER_GAP - 5
    bottom = MARGIN + 8
    pages = [[]]
    column, y = 0, start_y
    step = FONT_SIZE + 2
    current_section = ""
    section_at = {index: name for name, index in sections}
    for index, path in enumerate(files):
        name = section_at.get(index)
        entry_prefix_width = stringWidth("· ", FONT, FONT_SIZE)
        chunks = wrap_fixed(
            display_path(path, root),
            col_width - 10 - 42 - entry_prefix_width,
        )
        space = (len(chunks) - 1) * step
        if name:
            space += 2 * step
        if y - space < bottom:
            column += 1
            if column == 2:
                pages.append([])
                column = 0
            y = start_y
            if not name and current_section:
                name = current_section + ("（续）" if has_wide(current_section) else " (cont.)")
        x = MARGIN + column * (col_width + MARGIN)
        if name:
            current_section = section_at.get(index, current_section)
            if y != start_y:
                y -= step / 2
            pages[-1].append((x, y, name, None, True))
            y -= step * 1.5
        entry_x = x + 10
        for row, chunk in enumerate(chunks):
            prefix = "· " if row == 0 else "  "
            pages[-1].append((entry_x, y, prefix + chunk, path if row == 0 else None, False))
            y -= step
    return pages


def build_pdf(
    root: Path,
    output: Path,
    title: str,
    files: list[Path],
    sections: list[tuple[str, int]],
    line_numbers: bool,
    orientation: str,
    color: bool,
    style: str,
    author: str = AUTHOR,
    columns: int = 1,
    max_pages: int | None = None,
) -> int:
    global COLOR_OUTPUT, PYGMENTS_STYLE
    COLOR_OUTPUT = color
    try:
        PYGMENTS_STYLE = get_style_by_name(style)
    except Exception as exc:
        raise SystemExit(f"Unknown Pygments style: {style}") from exc
    configure_font()
    page_width, page_height = A4 if orientation == "portrait" else landscape(A4)
    contents = toc_layout(root, files, sections, page_width, page_height)
    if columns == 2:
        body, file_page_map = column_pages(root, files, page_width, page_height, line_numbers)
        body_total = len(body)
    else:
        sources = [source_pages(read_source(p), page_width, page_height, line_numbers) for p in files]
        file_page_map = {}
        next_page = 1
        for path, pages in zip(files, sources):
            file_page_map[path] = next_page
            next_page += len(pages)
        body_total = next_page - 1
    toc_total = len(contents)
    total_pages = 2 + toc_total + body_total
    if max_pages is not None and total_pages > max_pages:
        raise SystemExit(f"Codebook needs {total_pages} pages; limit is {max_pages}. Reduce the file list or adjust layout.")
    canvas = Canvas(str(output), pagesize=(page_width, page_height), pageCompression=1)
    canvas.setTitle(title)
    canvas.setAuthor(author)
    col_width = (page_width - 3 * MARGIN) / 2
    physical_page = 0

    def draw_page_number(label: str, total: str) -> None:
        canvas.setFillGray(0.3)
        canvas.setFont(FONT, 10)
        text = f"{label} / {total}"
        if physical_page % 2:
            canvas.drawRightString(page_width - MARGIN, 20, text)
        else:
            canvas.drawString(MARGIN, 20, text)

    def new_page(header: str, label: str, total: str, bold: bool = False) -> None:
        nonlocal physical_page
        if physical_page:
            canvas.showPage()
        physical_page += 1
        canvas.setFillColor(colors.black)
        if bold and not has_wide(header):
            header_size = min(TITLE_SIZE, (page_width - 2 * MARGIN) / stringWidth(header, "Helvetica-Bold", 1))
            canvas.setFont("Helvetica-Bold", header_size)
            canvas.drawCentredString(page_width / 2, page_height - MARGIN, header)
        else:
            header_size = min(TITLE_SIZE, (page_width - 2 * MARGIN) / mixed_proportional_width(header, 1))
            draw_mixed_centered(canvas, header, page_width / 2, page_height - MARGIN, header_size)
        canvas.setStrokeGray(0.55)
        canvas.setLineWidth(0.5)
        canvas.line(MARGIN, page_height - MARGIN - 10, page_width - MARGIN, page_height - MARGIN - 10)
        draw_page_number(label, total)

    canvas.setFillColor(colors.black)
    cover_size = min(24, (page_width - 2 * MARGIN) / mixed_proportional_width(title, 1))
    draw_mixed_centered(canvas, title, page_width / 2, page_height * 0.60, cover_size)
    draw_mixed_centered(canvas, author, page_width / 2, page_height * 0.49, 12)
    canvas.setFont("Helvetica", 12)
    today = date.today().strftime("%B %d, %Y").replace(" 0", " ")
    canvas.drawCentredString(page_width / 2, page_height * 0.45, today)
    canvas.showPage()
    physical_page = 2

    for index, entries in enumerate(contents):
        new_page(
            "目录" if index == 0 else "目录（续）",
            roman(index + 1).lower(),
            roman(toc_total).lower(),
            bold=True,
        )
        canvas.line(page_width / 2, page_height - MARGIN - HEADER_GAP, page_width / 2, MARGIN)
        for x, y, text, path, is_section in entries:
            canvas.setFillGray(0)
            if is_section:
                if has_wide(text):
                    draw_mixed_proportional(canvas, text, x, y, FONT_SIZE + 1)
                else:
                    canvas.setFont("Helvetica-Bold", FONT_SIZE + 1)
                    canvas.drawString(x, y, text)
            else:
                draw_mixed_proportional(canvas, text, x, y, FONT_SIZE)
            if path is not None:
                canvas.setFont(FONT, FONT_SIZE)
                canvas.drawRightString(x + col_width, y, str(file_page_map[path]))

    def draw_row(x, y, prefix, text, lexer):
        canvas.setFillGray(0.35)
        draw_mixed_fixed(canvas, prefix, x, y, FONT_SIZE)
        cursor = x + stringWidth(prefix, FONT, FONT_SIZE)
        tokens = lex(text, lexer) if color else [(Token.Text, text)]
        for token, value in tokens:
            value = value.rstrip("\r\n")
            canvas.setFillColor(token_color(token))
            cursor = draw_mixed_fixed(canvas, value, cursor, y, FONT_SIZE)

    if columns == 2:
        lexers = {path: lexer_for(path) for path in files}
        for page_index, rows in enumerate(body, 1):
            new_page(title, str(page_index), str(body_total))
            canvas.setStrokeGray(0.75)
            canvas.line(page_width / 2, page_height - MARGIN - HEADER_GAP, page_width / 2, MARGIN)
            for x, y, prefix, text, path, is_heading in rows:
                if is_heading:
                    canvas.setFillColor(colors.black)
                    draw_mixed_fixed(canvas, text, x, y, FONT_SIZE)
                    canvas.line(x, y - 2, x + (page_width - 2 * MARGIN - 18) / 2, y - 2)
                else:
                    draw_row(x, y, prefix, text, lexers[path])
        canvas.save()
        return total_pages

    for path, pages in zip(files, sources):
        relative = display_path(path, root)
        lexer = lexer_for(path)
        for page_index, rows in enumerate(pages):
            header = relative
            if len(pages) > 1:
                header += f" ({page_index + 1}/{len(pages)})"
            new_page(header, str(file_page_map[path] + page_index), str(body_total), bold=True)
            for y, prefix, text in rows:
                draw_row(MARGIN, y, prefix, text, lexer)
    canvas.save()
    return total_pages


def main() -> None:
    args = parse_args()
    global FONT_SIZE, LINE_HEIGHT, TITLE_SIZE, MARGIN, HEADER_GAP
    if args.compact:
        FONT_SIZE, LINE_HEIGHT = 7.0, 9.0
        TITLE_SIZE, MARGIN, HEADER_GAP = 15.0, 30.0, 24.0
    if args.font_size is not None:
        FONT_SIZE = args.font_size
    if args.line_height is not None:
        LINE_HEIGHT = args.line_height
    if not 5 <= FONT_SIZE <= 20 or not FONT_SIZE <= LINE_HEIGHT <= 40:
        raise SystemExit("Use font size 5-20 pt and line height between font size and 40 pt")
    if args.max_pages is not None and args.max_pages < 1:
        raise SystemExit("Page limit must be positive")
    root = Path(args.root).expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Not a directory: {root}")
    output = Path(args.output).expanduser()
    if not output.is_absolute():
        output = Path.cwd() / output
    output.parent.mkdir(parents=True, exist_ok=True)

    sections = []
    if args.config:
        config_path = Path(args.config).expanduser()
        if not config_path.is_absolute():
            config_path = Path.cwd() / config_path
        result = read_config_file(config_path, root)
        if result is None:
            raise SystemExit(f"Could not read config file: {config_path}")
        files, sections = result
        if not files:
            raise SystemExit(f"No valid files found in config: {config_path}")
    else:
        extensions = DEFAULT_EXTENSIONS | {e.lower() if e.startswith(".") else f".{e.lower()}" for e in (args.ext or [])}
        excludes = DEFAULT_EXCLUDES | set(args.exclude or [])
        excluded_files = {Path(__file__).resolve(), Path(__file__).with_name("code2pdf").resolve()}
        files = source_files(root, not args.no_recursive, extensions, excludes, output.resolve(), excluded_files)

    title = args.title or DEFAULT_TITLE
    pages = build_pdf(root, output, title, files, sections, not args.no_line_numbers,
                      args.orientation, args.color, args.style, args.author, args.columns, args.max_pages)
    print(f"Wrote {output} ({len(files)} source files, {pages} pages)")


if __name__ == "__main__":
    main()
