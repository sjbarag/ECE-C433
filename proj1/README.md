#### Compiling
This project can be compiled using the included Makefile, assuming that GNU
`make` is installed on your system and that `javac` is in your `PATH` environment
variable.  This is the default target.

Should GNU `make` not work for you, the following command should be sufficient:

	javac WebServer.java

#### Running
The program can be executed with `make run` (assuming `java` is in your `PATH` environment variable).  If GNU `make` is not available or you just want to run
the application directly, you can simply run:

	java WebServer

#### Example Output
Because the output for this application is mostly system dependant, it is not
included here in its entirety.  However, the application should terminally
output a table of the following form:

                         Threads    Single    Multi    Match
    Scriptable Output:        8    3.292550  3.16328   true
