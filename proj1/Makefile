.PHONY: clean
JAVAC   = javac
JAVA    = java

default : build

build : WebServer.java
	javac WebServer.java

run : WebServer.java WebServer.class HttpRequest.class
	java WebServer

clean :
	-rm -f *.class
