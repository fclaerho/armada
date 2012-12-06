### SYNOPSIS

	armada ...

### DESCRIPTION

Armada compiles and run C99 statements from stdin,
any command line option is forwarded to the hosted code.

### EXIT STATUS

Armada exits with the exit status of the hosted code, or EXIT_FAILURE if any operation preceeding its execution fails.

### EXAMPLE

	$ echo 'printf("hello %s", argv[1]); return 42;' | armada world
	hello world
	$ echo $?
	42

### LIBRARY

As both the C99 standard library and posix API are too low-level to write compact code, Armada is shipped with its own library.

* ite* str(const char *fmt, ...)
* ite* glob(const char *pattern)
* void echo(ite*);
* ite* next(ite*);

Under work...

### REQUIREMENT

A posix-compliant platform.
Armada uses your local **c99** command and you'll need **make** for the installation.

### INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo make install