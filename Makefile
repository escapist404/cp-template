PYTHON ?= python3
TESTS ?= tests

.PHONY: help pdf pdf-colored pdf-icpc test stress test-code2pdf clean

help:
	@echo "make pdf          Generate black-and-white codebook.pdf"
	@echo "make pdf-colored  Generate colored codebook.pdf"
	@echo "make pdf-icpc     Generate compact ICPC core preset"
	@echo "make test         Run the test suite"
	@echo "make stress       Run stress tests"
	@echo "make test-code2pdf  Test the PDF generator"
	@echo "make clean        Remove generated PDFs"

pdf:
	$(PYTHON) tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --black-and-white

pdf-colored:
	$(PYTHON) tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --color

pdf-icpc:
	$(PYTHON) tools/code2pdf.py include/ -o codebook-icpc.pdf -c codebook.icpc.conf --compact --font-size 6 --line-height 7 --columns 2 --max-pages 25 --no-line-numbers --black-and-white --title "ICPC Core Codebook"

test:
	CXX="$(CXX)" ./run_tests.sh $(TESTS)

stress:
	./stress_test.sh

test-code2pdf:
	$(PYTHON) tests/tools/test_code2pdf.py

clean:
	rm -f codebook.pdf codebook-icpc.pdf
