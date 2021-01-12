//Networking
#include <winsock2.h>

//Files and strings operating
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "network.h"
#include "actor.h"
#include "render.h"

SOCKET mainSocket;
fd_set master;
fd_set copy;


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
    ids = 0;
};


bool net::init_network(){
    // Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	u_long arg=1;
    mainSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mainSocket == INVALID_SOCKET) {
		return false;
	}
	ioctlsocket(mainSocket, FIONBIO, &arg);

	// Get IP from textfile (TODO: temporary)
    std::string line;
	std::ifstream file("conf/ip.txt");
    getline(file,line);
	file.close();
	Network.SetIP(line);

	// Bind server
	sockaddr_in service;
	memset( & service, 0, sizeof( service ) );
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(line.c_str());
	service.sin_port = htons( 27015 );

	bind(mainSocket, (sockaddr*)&service, sizeof(service));

	// Set server socket to master
	listen(mainSocket, SOMAXCONN);
	FD_ZERO(&master);
	FD_SET(mainSocket, &master);

	return true;
}

int net::accept_connections(){
    // Accept new connection
    SOCKET client = accept(mainSocket, 0, 0);
    if(client == INVALID_SOCKET){
    return 0;
    }
    else{
    std::cout << "Client #" << client << " connected";
    // Add new connection
    FD_SET(client, &master);
    // Send ID to actor
    send_id_for_actor(client);

    act::CreateActor(0,0,client); // <- Create server side Actor object with dumb x,y

    return client;
    }
}

int net::send_id_for_actor(SOCKET sock){
    std::stringstream st;
    st << "I" << Network.ids; // <- ID packet header
    std::string str = st.str();
    send(sock, str.c_str(), 2, 0);
    Network.ids++;
}

int net::send_pos_for_actors(){

    std::ostringstream st;
    for(int n = 0; n < act::GetActorsSize(); n++){
        char buf[4096];
        st << "P" << n; // <- Position packet header + ID for actor !!!;
        if(act::GetX(n) > 9) // <- If Player X is < 9 then add '#' char - goal is to make every packet look like: P(ID)XXYY!
            st << act::GetX(n);
        else
            st << act::GetX(n) << "#";
        if(act::GetY(n) > 9) // <- same
            st << act::GetY(n);
        else
            st << act::GetY(n) << "#";
    }
    st << "!"; // <- end of pos packet header!

    fd_set copy = master;
    for(int i = 0; i < master.fd_count; i++) {
        SOCKET sock = copy.fd_array[i];
        if (sock == mainSocket){
            //
        }
        else{
            std::string debug = st.str();
            send(sock,debug.c_str(),debug.size() + 1,0);
        }

    }
}

int net::check_packet(SOCKET sock, char buf[4096]){
    switch(buf[0]){
    case 'P': // <- Position packet header
            for(int i = 0; i < act::GetActorsSize(); i++){
                if(act::GetID(i) == sock){ // <- Compare socket ids
                    if(buf[2] == '#'){ // <- Check if position is 1 number
                    int ibuf = buf[1] - '0';
                    act::SetX(i,ibuf);
                    }
                    else{
                        std::stringstream st;
                        st << buf[1] << buf[2];
                        int ibuf;
                        st >> ibuf;
                        act::SetX(i,ibuf);
                    }
                    if(buf[4] == '#'){ // <- Check if position is 1 number
                    int ibuf = buf[3] - '0';
                    act::SetY(i,ibuf);
                    }
                    else{
                        std::stringstream st;
                        st << buf[3] << buf[4];
                        int ibuf;
                        st >> ibuf;
                        act::SetY(i,ibuf);
                    }
                }
            }
            send_pos_for_actors();
        break;
    }
}

int net::receive_packet(SOCKET sock){
    char buf[4096];
    ZeroMemory(buf, 4096);
    int bytesIn = recv(sock, buf, 4096, 0);
    if (bytesIn <= 0){
        // Drop client
    }
    else{
        /*Echo the packet to the client (better synchro probably? TODO:check)*/
        std::ostringstream st;
        st << buf;
        std::string server_echo = st.str();
        gotoxy(0,16);
        std::cout << "           ";
        gotoxy(0,16);
        std::cout << server_echo;
        check_packet(sock,buf);
        //echo(sock,buf,bytesIn);
    }
}

int net::echo(SOCKET outSock, char buf[4096], int bytes){
    send(outSock,buf,bytes,0);
}

int net::handle_sockets(){
    fd_set copy = master;
    for(int i = 0; i < master.fd_count; i++) {
        SOCKET sock = copy.fd_array[i];
        if (sock == mainSocket){
            if(!accept_connections()){

            }
        }
        else{
            receive_packet(sock);
        }

    }
}

int net::networking_loop(){
    handle_sockets();
}
