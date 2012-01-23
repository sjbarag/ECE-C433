#### Contents
The following files are included in this .zip archive:
* README.md - This file
* WebServer.java - The required Java class
* Makefile - A GNU Make file
* instructions.txt - A plaintext copy of the assignment instructions
* foo.txt - A sample text file

#### Compiling
This project can be compiled using the included Makefile, assuming that GNU
`make` is installed on your system and that `javac` is in your `PATH` environment
variable.  This is the default target.

Should GNU `make` not work for you, the following command should be sufficient:

	javac WebServer.java

#### Running
The program can be executed with `make run` (assuming `java` is in your `PATH`
environment variable).  If GNU `make` is not available or you just want to run
the application directly, you can simply run:

	java WebServer
