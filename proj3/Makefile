.PHONY: clean

VID=movie.Mjpeg
CSRC=Client.java RTPpacket.java
SSRC=RTPpacket.java Server.java VideoStream.java
SOURCES=${CSRC} ${SSRC}

PRTNO=55555

default : buildAll

client : ${CSRC}
	javac Client.java

server : ${SSRC}
	javac Server.java

buildAll : ${SOURCES}
	javac Client.java
	javac Server.java

runClient : ${CSRC} ${VID} Client.class RTPpacket.class
	java Client localhost ${PRTNO} ${VID}

runServer : ${SSRC} RTPpacket.class Server.class VideoStream.class
	java Server ${PRTNO}

clean :
	-rm -f *.class
