#
# App patcher makefile
# Written 2022 by d3phys
#

TOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

DEPS   = $(TOPDIR)/dependencies/
LIBS   = $(TOPDIR)/libs/

raygui = $(DEPS)raygui/src/


make: $(LIBS)/raygui/raygui.so
	g++ -c src/patch.cpp -o build/patch.o
	g++ -c src/main.cpp  -o build/main.o -I$(raygui)
	g++ build/main.o build/patch.o -o patch -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 $(LIBS)raygui/raygui.so
	./patch

raygui:
	cp $(raygui)/raygui.h $(LIBS)raygui/raygui.c
	gcc -o $(LIBS)/raygui/raygui.so $(LIBS)raygui/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


.PHONY: make raygui
