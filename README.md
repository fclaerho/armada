### SYNOPSIS

	armada ...

### DESCRIPTION

Armada compiles and run C99 statements from stdin,
any command line option is forwarded to the hosted code.

### EXIT STATUS

Armada exits with the hosted code exit status or EXIT_FAILURE if any operation preceeding the hosted code execution fails.

### EXAMPLE

	$ echo 'printf("hello %s", argv[1]); return 42;' | armada world
	hello world
	$ echo $?
	42

### REQUIREMENT

A posix-compliant installation.
Armada uses your local **c99** command and you'll need **make** for the installation.

### INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo make install