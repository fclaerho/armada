.PHONY: test clean install uninstall

BINPATH := /usr/local/bin

test: armada
	echo 'return 0;' | ./armada
	echo 'return 42;' | ./armada || true
	echo 'puts("foo");' | ./armada | xargs test foo =
	echo 'puts(argv[1]);' | ./armada bar | xargs test bar =
	echo 'printf("%.0f", cos(2 * 3.141592653));' | ./armada | xargs test 1 -eq
	echo 'hello();' | ARMADA_INCLUDE='extern int puts(char*); void hello(void) { puts("hello"); }' ./armada | xargs test hello =
	echo 'puts(FOO);' | ARMADA_CFLAGS='-D FOO=\"foo\"' ./armada | xargs test foo =

armada: armada.c
	c99 -O3 $< -o $@

clean: armada
	-rm -fr $< *.dSYM

install: test
	mv armada $(BINPATH)

uninstall:
	-rm -f $(BINPATH)/armada
