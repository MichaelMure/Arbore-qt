.PHONY: all
all:build/Arbore-qt

source_files=$(shell find -type f -name "*cpp" -print -or -name "*.h" -print -or -name "build" -prune)

build/Makefile:Arbore-qt.pro Makefile
	mkdir -p build
	cd build && qtchooser -qt=5 -run-tool=qmake ..

build/Arbore-qt:build/Makefile $(source_files)
	cd build && $(MAKE)

.PHONY: run
run: build/Arbore-qt
	cd build && ./Arbore-qt

.PHONY: clean
clean:
	rm -r build
