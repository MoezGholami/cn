all: serviceProviderMain.cpp serviceProviderCore.h serviceProviderCore.cpp ../packetHandler/packet.h ../packetHandler/packet.cpp ../packetHandler/message.h ../packetHandler/message.cpp ../packetHandler/packetHandler.h ../packetHandler/packetHandler.cpp ../packetHandler/crc.h ../packetHandler/crc.cpp ../util/util.h ../util/util.cpp
	g++ -Wall serviceProviderMain.cpp serviceProviderCore.cpp ../packetHandler/packet.cpp ../packetHandler/message.cpp ../packetHandler/packetHandler.cpp ../packetHandler/crc.cpp ../util/util.cpp -o serviceProvider.out

clean:
	rm -rfv *.o *.out
