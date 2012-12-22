.PHONY: test clean install uninstall

BINNAME := armada
BINPATH := /usr/local/bin

test: $(BINNAME)
	echo 'return 0;' | ./$(BINNAME)
	echo 'return 42;' | ./$(BINNAME) || true
	echo 'puts("foo");' | ./$(BINNAME) | xargs test foo =
	echo 'puts(argv[1]);' | ./$(BINNAME) bar | xargs test bar =
	echo 'printf("%.0f", cos(2 * 3.141592653));' | ./$(BINNAME) | xargs test 1 -eq
	echo 'hello();' | ARMADA_INCLUDE='extern int puts(char*); void hello(void) { puts("hello"); }' ./$(BINNAME) | xargs test hello =
	echo 'puts(FOO);' | ARMADA_CFLAGS='-D FOO=\"foo\"' ./$(BINNAME) | xargs test foo =

$(BINNAME): $(BINNAME).c
	c99 -O3 $< -o $@

clean: $(BINNAME)
	-rm -fr $< *.dSYM

install: test
	mv $(BINNAME) $(BINPATH)

uninstall:
	-rm -f $(BINPATH)/$(BINNAME)