#include "network.h"


PeerNode::PeerNode(int port) : port(port) {}

void PeerNode::start() 
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "[X] Failed to bind to port " << port << std::endl;
        return;
    }

    if (listen(server_socket, 3) < 0)
    {
        std::cerr << "[X] Failed to listen on port " << port << std::endl;
        return;
    }

    // Get the IP address of the server
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, ip, INET_ADDRSTRLEN); // Convert IP address to string

    std::cout << "[*] PeerNode started on " << ip << ":" << port << std::endl;

    std::thread server_thread(&PeerNode::serverLoop, this);
    server_thread.detach();
}


// Server loop is the main loop that listens for incoming connections
void PeerNode::serverLoop()
{
    if (server_socket < 0) {
        std::cerr << "[X] Invalid server socket!" << std::endl;
        return; // Exit or handle error as appropriate
    }
    
    std::cout << "[*] Waiting for incoming connections" << std::endl;

    while (true)
    {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0)
        {
            if (errno == EINTR) {
                std::cerr << "[!] Interrupted system call, retrying..." << std::endl;
                continue; // Interrupted system call, retry
            } else {
                std::cerr << "[X] Failed to accept connection: " << errno << std::endl;
                continue; // Log the error and continue the loop
            }
        }

        std::cout << "[+] Connection accepted" << std::endl;

        {
            std::lock_guard<std::mutex> lock(peer_mutex);
            peer_sockets.insert(client_socket);
        }

        std::thread client_thread(&PeerNode::clientLoop, this, client_socket);
        client_thread.detach();
    }

    // Clean up server socket
    close(server_socket);
    std::cout << "[*] Server socket closed" << std::endl;
}

void PeerNode::clientLoop(int client_socket)
{
    char buffer[1024];
    while (true)
    {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            // Connection closed or error
            std::cerr << "[!] Connection lost with peer" << std::endl;

            std::lock_guard<std::mutex> lock(peer_mutex);
            peer_sockets.erase(client_socket);  // Remove the peer from the set
            close(client_socket);
            std::cout << "[*] Closed client socket" << std::endl;
            break;
        }

        buffer[bytes_received] = '\0';  // Null-terminate the received data
        std::cout << "[MSG] Received from peer: " << buffer << std::endl;
    }
}


// Connect to a peer 
void PeerNode::connectToPeer(const std::string& ip_address, int port)
{
    int peer_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in peer_addr{};
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &peer_addr.sin_addr);

    if (connect(peer_socket, (sockaddr*)&peer_addr, sizeof(peer_addr)) < 0)
    {
        std::cerr << "[X] Failed to connect to peer" << std::endl;
        return;
    }

    std::cout << "[+] Connected to peer" << std::endl;

    std::lock_guard<std::mutex> lock(peer_mutex);
    peer_sockets.insert(peer_socket);
}

// Send a message to all peers connected to the node
void PeerNode::sendToPeer(const std::string& message)
{
    std::lock_guard<std::mutex> lock(peer_mutex);
    for (int peer_socket : peer_sockets)
    {
        ssize_t bytes_sent = send(peer_socket, message.c_str(), message.size(), 0);
        if (bytes_sent == -1)
        {
            std::cerr << "[!] Error sending message to peer" << std::endl;
        }
    }

    std::cout << "[MSG] Sent to connections: " << message << std::endl;
}

