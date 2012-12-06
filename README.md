## INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo c99 -O3 armada.c -o /usr/local/bin/armada

* * *

### SYNOPSIS

	armada ...

### DESCRIPTION

Armada compiles and run C99 statements from stdin,
any command line option is forwarded to the hosted code.

### EXIT STATUS

Armada exits with the hosted code exit status or 0 if any operation preceeding the hosted code execution fails.

### EXAMPLE

	$ echo 'printf("hello %s", argv[1]); return 42;' | armada world
	hello world
	$ echo $?
	42
	