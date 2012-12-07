.PHONY: test install

test: armada
	echo 'return 0;' | ./armada
	echo 'return 42;' | ./armada || true
	echo 'puts("foo");' | ./armada | xargs test foo =
	echo 'puts(argv[1]);' | ./armada bar | xargs test bar =
	echo 'for(ite *i = str("baz%i", 42); i; i = next(i)) puts(i->val);' | ./armada | xargs test baz42 =
	echo 'echo(str("foo"));' | ./armada | xargs test '(foo)' =
	-echo 'echo(glob("."));' | ./armada | xargs -I% test '(.DS_Store, .git, .gitignore, a.out, armada, armada.c, armada.dSYM, makefile, README.md)' = "%"
	echo 'echo(mkite(argv));' | ./armada a b c | xargs -I% test '(./armada, a, b, c)' = '%'

armada: armada.c
	c99 -O3 $< -o $@

install: test
	mv armada /usr/local/bin
	
