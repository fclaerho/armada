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

As both the C99 standard library and posix API are too low-level to write compact code, Armada is shipped with its own.
Its core type is an iterator and all constructors return one.

* ite* str(const char *fmt, ...), returns a singleton containing the string interpolation;
* ite* glob(const char *pattern), return an iterator to a list of filenames;
* void next(ite* *), fetch the next entry in the list, the current entry is released;
* void echo(ite*), output (and release) the list;

Under work...

### REQUIREMENT

A posix-compliant platform.
Armada uses your local **c99** command and you'll need **make** for the installation.

### INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo make install