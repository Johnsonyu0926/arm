# Makefile for header-only library

# Compiler settings
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic

# Header files
HEADERS := $(wildcard *.h)

# Install directory
INSTALL_DIR := /usr/local/include/mylib

.PHONY: all install clean check

all: check

install:
	@mkdir -p $(INSTALL_DIR)
	@cp $(HEADERS) $(INSTALL_DIR)
	@echo "Headers installed to $(INSTALL_DIR)"

clean:
	@echo "Nothing to clean in a header-only library"

check:
	@echo "Checking header files..."
	@for file in $(HEADERS); do \
		$(CXX) $(CXXFLAGS) -fsyntax-only $$file || exit 1; \
	done
	@echo "All headers pass syntax check"

#BY GST ARMV8 GCC 13.2