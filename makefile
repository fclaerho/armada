.PHONY: test install

test: armada
	echo 'return 0;' | ./armada
	echo 'return 42;' | ./armada || true
	echo 'puts("foo");' | ./armada | xargs test foo =
	echo 'puts(argv[1]);' | ./armada bar | xargs test bar =

armada: armada.c
	c99 -O3 $< -o $@

install: test
	mv armada /usr/local/bin
	
