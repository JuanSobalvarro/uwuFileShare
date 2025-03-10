#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <unordered_set>
#include <string>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class PeerNode
{
    public:
        PeerNode(int port);

        void start();
        
        void connectToPeer(const std::string& ip_address, int port);
        void sendToPeer(const std::string& message);
        // void receiveFromPeer(std::string& message);

    private:
        int server_socket;
        int port;
        std::unordered_set<int> peer_sockets;
        std::mutex peer_mutex; // Why do we need a mutex?

        void serverLoop();
        void clientLoop(int client_socket);
};

#endif // NETWORK_H