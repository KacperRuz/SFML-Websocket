#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

//Networking
#include <winsock2.h>

//String operations
#include <string>

namespace net
{
bool init_network();
int handle_sockets();
int networking_loop();
int accept_connections();
int receive_packet();
int echo(SOCKET outSock, char[4096], int bytes);
int debug_packet();
int send_pos_packet();
int check_packet(char buf[4096]);

//Getters
std::string GetIPa();
//Setters
}


/*Class @Network in pretty useless right now but might be used in future*/

class Network{
    public:
        Network();
        std::string IP;
        std::string SetIP(std::string s);
        std::string GetIP();
};


#endif // NETWORK_H_INCLUDED
