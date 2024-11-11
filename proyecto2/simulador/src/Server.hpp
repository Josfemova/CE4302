#ifndef SERVER_H
#define SERVER_H

#include <libwebsockets.h>
#include <string>
#include <queue>

// Declare the function to send a message to the connected client
void sendMessageToClient(const std::string &message);

// Function to start the WebSocket server
void startServer();

// Declare a global queue to store incoming messages
extern std::queue<std::string> incomingMessageQueue; // Use 'extern' to declare it

#endif // SERVER_H
