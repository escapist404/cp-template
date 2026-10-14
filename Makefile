PYTHON ?= python3
TESTS ?= tests

.PHONY: help pdf pdf-colored pdf-code pdf-docs test stress test-code2pdf clean

help:
	@echo "make pdf            Generate combined docs + code black-and-white codebook.pdf"
	@echo "make pdf-colored    Generate combined docs + code colored codebook.pdf"
	@echo "make pdf-code       Generate code-only codebook-code.pdf"
	@echo "make pdf-docs       Generate docs-only codebook-docs.pdf"
	@echo "make test           Run the test suite"
	@echo "make stress         Run stress tests"
	@echo "make test-code2pdf  Test the PDF generator"
	@echo "make clean          Remove generated PDFs"

pdf:
	$(PYTHON) tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --with-docs docs/ --black-and-white --title "CP Template Library & Docs"

pdf-colored:
	$(PYTHON) tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --with-docs docs/ --color --title "CP Template Library & Docs"

pdf-code:
	$(PYTHON) tools/code2pdf.py include/ -o codebook-code.pdf -c codebook.conf --black-and-white --title "CP Template Codebook"

pdf-docs:
	$(PYTHON) tools/code2pdf.py include/ -o codebook-docs.pdf -c codebook.conf --docs-only docs/ --no-line-numbers --black-and-white --title "CP Template Documentation"

test:
	CXX="$(CXX)" ./run_tests.sh $(TESTS)

stress:
	./stress_test.sh

test-code2pdf:
	$(PYTHON) tests/tools/test_code2pdf.py

clean:
	rm -f codebook.pdf codebook-code.pdf codebook-docs.pdf codebook-with-docs.pdf codebook-icpc.pdf
