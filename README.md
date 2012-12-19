### SYNOPSIS

	armada ...

### DESCRIPTION

Armada compiles and run C99 statements from stdin,
any command line option is forwarded to the hosted code.

### EXIT STATUS

Armada exits with the exit status of the hosted code, or 1 if any operation preceeding its execution fails.
Armada raises any uncaught signal during the hosted code execution.

### EXAMPLE

	$ echo 'printf("hello %s", argv[1]); return 42;' | armada world
	hello world
	$ echo $?
	42

### REQUIREMENT

A posix-compliant platform.
Armada uses your local **c99** compiler and you'll need **make** for the installation.

### INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ sudo make -C armada install