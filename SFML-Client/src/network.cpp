//Networking
#include <winsock2.h>
#include <Ws2tcpip.h>

//Files and strings operating
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "network.h"
#include "actor.h"

fd_set master;
fd_set copy;
SOCKET sock;


Network Network;

std::string Network::GetIP(){
    return IP;
}
std::string net::GetIPa(){
    return Network.IP;
}


std::string Network::SetIP(std::string s){
    IP = s;
}

Network::Network(){
};


bool net::init_network(){

    WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	u_long arg=1;
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
        std::cout << "Invalid socket.";
		return false;
	}
	ioctlsocket(sock, FIONBIO, &arg);

	// Hint
	std::string line;
	std::ifstream file("conf/ip.txt");
    getline(file,line);
	file.close();
	sockaddr_in service;
	memset( & service, 0, sizeof( service ) );
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(line.c_str());
	service.sin_port = htons( 27015 );

	int connResult = connect(sock, (sockaddr*)&service, sizeof(service));
	/*if (connResult == SOCKET_ERROR) {
        Network.SetIP(line);
		closesocket(sock);
		WSACleanup();
		return false;
	} dunno why it droping everytime*/

	fd_set wrset;
	fd_set exset;
	FD_ZERO(&wrset);
	FD_ZERO(&exset);

	FD_SET(sock, &wrset);
	FD_SET(sock, &exset);

	//wait till connect finishes
	select(0, 0, &wrset, &exset, 0);
	Network.SetIP(line);
	return true;
}

int net::receive_packet(){
    char buf[4096];
    ZeroMemory(buf, 4096);
    int bytesIn = recv(sock, buf, 4096, 0);
    if (bytesIn <= 0){
        // Drop client
    }
    else{
        std::ostringstream st;
        st << buf;
        std::string client_echo = st.str();
        std::cout << client_echo;
        check_packet(buf);
        //echo(sock,buf,bytesIn);
    }
}

// POS PACKET: P(ID)XXYY!
//             P01010P12#2#!
//             0123456789111
//                       012

int net::check_packet(char buf[4096]){
    if(buf[0] == 'I'){ // <- ID packet header
        int ibuf = buf[1] - '0';
        act::AddID(ibuf);
    }

    if(buf[0] == 'P'){ // <- Position packet header
        /*Check packet length*/
        std::stringstream st;
        st << buf;
        std::string buf_len = st.str();
        int packets_parts = buf_len.length() / 6; // <- one packet part is 6 bytes
        std::cout << "Packet parts: " << packets_parts;

        /*
        Remember that id sent in packet != local id
        ie.
        client 0:
        has 0 id and 0 place in memory locally
        client 1 has locally id 1 and 1 place in memory

        client 1:
        has 1 id and 0 place in memory locally
        client 0 has locally id 0 and 1 place in memory
        */


        for(int l = 0; l < packets_parts; l++){ // <- loops reading packet for every packer part
        int shift = l*6;
        int exists = -1;
        int idbuf = buf[1+shift] - '0';
        for(int i = 0; i < act::GetActorsSize(); i++){ // <- checks if theres an actor with id sent in packet; if no -> create new actor with this id
            if(act::GetID(i) == idbuf){
                exists = i;
            }
        }
        if(exists == -1){
            act::CreateActor(0,0);// <- placeholder x and y
            act::AddID(idbuf);
        }

        if(buf[3+shift] == '#'){ // <- Check if position is 1 number
        int ibuf = buf[2+shift] - '0';
        act::SetX(exists,ibuf);
        }
        else{
            std::stringstream st;
            st << buf[2+shift] << buf[3+shift];
            int ibuf;
            st >> ibuf;
            act::SetX(exists,ibuf);
        }
        if(buf[5+shift] == '#'){ // <- Check if position is 1 number
        int ibuf = buf[4+shift] - '0';
        act::SetY(exists,ibuf);
        }
        else{
            std::stringstream st;
            st << buf[4+shift] << buf[5+shift];
            int ibuf;
            st >> ibuf;
            act::SetY(exists,ibuf);
        }
        }
    }
}

int net::send_pos_packet(){
    char buf[4096];
    std::ostringstream st;
    st << "P"; // <- Position packet header;
    if(act::GetX(PLAYER) > 9) // <- If Player X is < 9 then add '#' char (goal is to make every packet like: PXXYY)
        st << act::GetX(PLAYER);
    else
        st << act::GetX(PLAYER) << "#";
    if(act::GetY(PLAYER) > 9) // <- same
        st << act::GetY(PLAYER);
    else
        st << act::GetY(PLAYER) << "#";
    std::string debug = st.str();
    send(sock,debug.c_str(),debug.size() + 1,0);
}

int net::debug_packet(){
    char buf[4096];
    std::ostringstream st;
    st << act::GetX(0) << act::GetY(0);
    std::string debug = st.str();
    send(sock,debug.c_str(),debug.size() + 1,0);
}

int net::echo(SOCKET outSock, char buf[4096], int bytes){
    send(outSock,buf,bytes,0);
}

int net::networking_loop(){
    receive_packet();
}
