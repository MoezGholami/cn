all: serverCore.cpp serverCore.h serverMain.cpp ../packetHandler/packet.h ../packetHandler/packet.cpp ../packetHandler/message.h ../packetHandler/message.cpp ../packetHandler/packetHandler.h ../packetHandler/packetHandler.cpp ../packetHandler/crc.h ../packetHandler/crc.cpp
	g++ -Wall serverCore.cpp serverMain.cpp ../packetHandler/packet.cpp ../packetHandler/message.cpp ../packetHandler/packetHandler.cpp ../packetHandler/crc.cpp -o server.out

clean:
	rm -rfv *.o *.out
