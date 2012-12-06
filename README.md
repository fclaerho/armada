## Installation

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo c99 -O3 armada.c -o /usr/local/bin/armada

## Usage

Armada compiles and run C99 statements from stdin,
any option is forwarded to the hosted code,
the exit status is the exit status of the hosted code.

	$ echo 'printf("hello %s", argv[1]); return 42;' | armada world
	hello world
	$ echo $?
	42

## Requirements

A posix-compliant system (Armada uses the c99 utility).