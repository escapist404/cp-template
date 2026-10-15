PYTHON ?= python3
TESTS ?= tests

.PHONY: help pdf pdf-colored test stress test-code2pdf clean

help:
	@echo "make pdf          Generate black-and-white codebook.pdf (Chinese title & TOC)"
	@echo "make pdf-colored  Generate colored codebook.pdf (Chinese title & TOC)"
	@echo "make test         Run the test suite"
	@echo "make stress       Run stress tests"
	@echo "make test-code2pdf  Test the PDF generator"
	@echo "make clean        Remove generated PDFs"

pdf:
	$(PYTHON) tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --black-and-white --title "算法竞赛模板库"

pdf-colored:
	$(PYTHON) tools/code2pdf.py include/ -o codebook.pdf -c codebook.conf --color --title "算法竞赛模板库"

test:
	CXX="$(CXX)" ./run_tests.sh $(TESTS)

stress:
	./stress_test.sh

test-code2pdf:
	$(PYTHON) tests/tools/test_code2pdf.py

clean:
	rm -f codebook.pdf
