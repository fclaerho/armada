Armada is the missing utility for C99 one-liners.

## Usage

Armada compiles and run C statements from stdin, any option is forwarded to the hosted code.

	$ echo 'printf("hello %s", argv[1]);' | armada world
	hello world

## Requirements

A posix-compliant system (Armada calls c99).