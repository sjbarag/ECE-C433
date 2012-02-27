#### Info
Author: Sean Barag
Date: 27 February 2012
Course: ECEC433 (Winter 2012)

#### Contents
The following files are included in this .zip archive:
* README.md - This file
* Makefile - A GNU Make file

* Client.java - Streaming client class
* RTPpacket.java - RTP packet handler class
* Server.java - Streaming server class
* VideoStream.java - Video stream handler class
* movie.Mjpeg - Video to stream with the above classes

* instructions.pdf - A PDF copy of the assignment instructions
* LICENSE - Software license for this project

#### Compiling
This project can be compiled using the included Makefile, assuming that GNU
`make` is installed on your system and that `javac` is in your `PATH` environment
variable.  This is the default target.

Should GNU `make` not work for you, the following commands should be sufficient:

	javac Server.java
	javac Client.java


#### Running
The program can be executed with `make run` (assuming `java` is in your `PATH`
environment variable).  If GNU `make` is not available or you just want to run
the application directly, you can simply run:

	java Server 55555
	java Client localhost 55555 movie.Mjpeg
