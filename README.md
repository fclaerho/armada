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

### BUILT-IN FUNCTIONS

As both the C99 standard library and posix API are too low-level to write compact code, Armada is shipped with its own.

* char* str(const char *fmt, ...), returns the string interpolation;
* ite* split(const char*, const char *sep);
* char* join(const char *sep, ite*);
* ite* glob(const char *pattern), return an iterator to a list of filenames;
* ite* push(ite*, const char*);
* ite* shift(ite*), free the current entry and fetch the next one;
* void echo(ite*), output (and release) the list;

### REQUIREMENT

A posix-compliant platform.
Armada uses your local **c99** command and you'll need **make** for the installation.

### INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo make install