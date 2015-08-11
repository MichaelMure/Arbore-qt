all:doc

source_files=$(shell find ../src -type f -name "*cpp" -print -or -name "*.h" -print -or -name "build" -prune)

build/html/index.html:$(source_files) Makefile Doxyfile
	doxygen

html:build/html/index.html

doc:html
