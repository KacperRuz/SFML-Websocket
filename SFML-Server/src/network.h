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
int receive_packet(SOCKET);
int echo(SOCKET outSock, char buf[4096], int bytes);
int check_packet(SOCKET sock, char buf[4096]);
int send_id_for_actor(SOCKET sock);
int send_pos_for_actors();

//Getters
std::string GetIPa();
//Setters
}


/*Class @Network in pretty useless right now but might be used in future*/

class Network{
    public:
        Network();
        int ids;
        std::string IP;
        std::string SetIP(std::string s);
        std::string GetIP();
};


#endif // NETWORK_H_INCLUDED
