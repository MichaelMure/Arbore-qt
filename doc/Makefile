all:doc

source_files=$(shell find ../src -type f -name "*cpp" -print -or -name "*.h" -print -or -name "build" -prune)

build/html/index.html:$(source_files) Makefile Doxyfile
	doxygen

.PHONY:html
html:build/html/index.html

.PHONY:doc
doc:html uml

.PHONY:uml
uml:
	plantuml $(source_files) -o ../doc/build/uml/
.PHONY:clean
clean:
	rm -r build

.PHONY:open
open:html
	mimeopen build/html/index.html
