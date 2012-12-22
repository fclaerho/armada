### SYNOPSIS

	armada ...

### DESCRIPTION

Armada compiles and run C99 statements from stdin,
any command line option is forwarded to the hosted code.

Neither the standard library nor the posix API allow to write compact code,
Armada therefore allows you to customize its behavior throught two environment variables:
  * ARMADA_INCLUDE to override the code preceeding main() (defaults to include string.h, stdlib.h, stdio.h and math.h);
  * ARMADA_CFLAGS to override compilation options (defaults to -O3).

### EXIT STATUS

Armada exits with the exit status of the hosted code,
or 1 if any operation preceeding its execution fails.

### EXAMPLE

	$ echo 'printf("hello %s", argv[1]); return 42;' | armada world
	hello world
	$ echo $?
	42

### REQUIREMENT

Armada uses the posix **c99** command internally to compile the hosted code.

If you use the provided makefile to install Armada,
you'll need the c99 posix command and a posix-compliant make.

### INSTALLATION

	$ git clone https://github.com/claerhout/armada.git
	$ sudo make -C armada install

### DE-INSTALLATION

	$ sudo make -C armada uninstall