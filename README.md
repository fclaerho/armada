## Installation

	$ git clone https://github.com/claerhout/armada.git
	$ cd armada
	$ sudo c99 -O3 armada.c -o /usr/local/bin/armada

## Usage

Armada compiles and run C statements from stdin, any option is forwarded to the hosted code.

	$ echo 'printf("hello %s", argv[1]);' | armada world
	hello world

## Requirements

A posix-compliant system (Armada calls c99).