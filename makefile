.PHONY: test install

test: armada
	echo 'return 0;' | ./armada
	echo 'return 42;' | ./armada || true
	echo 'puts("foo");' | ./armada | xargs test foo =
	echo 'puts(argv[1]);' | ./armada bar | xargs test bar =
	echo 'puts(str("%s%i...", "baz", 42));' | ./armada | xargs test baz42... =
	echo 'echo(glob("."));' | ./armada | xargs -I% test '(.DS_Store, .git, .gitignore, a.out, armada, armada.c, armada.dSYM, makefile, README.md)' = "%"
	echo 'echo(mkite(argv));' | ./armada a b c | xargs -I% test '(./armada, a, b, c)' = '%'
	echo 'return argc == count(mkite(argv))? 0: 1;' | ./armada a b c
	echo 'echo(split("a:b:c/de", ":/"));' | ./armada | xargs -I% test '(a, b, c, de)' = '%'

armada: armada.c
	c99 -O3 $< -o $@

install: test
	mv armada /usr/local/bin
	
