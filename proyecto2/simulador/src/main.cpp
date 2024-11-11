#include <format>
#include <iostream>
#include <string>
#include "Cache.hpp"
#include "BusInterconnect.hpp"
#include <unistd.h>
#include "Server.hpp"

#include <thread>
#include <chrono>

void sendTestMessages()
{
    while (1)
    {
        // Wait for the server to be ready to send messages
        std::this_thread::sleep_for(std::chrono::seconds(10));

        // Send the message
        sendMessageToClient("update_register,1,3,[35]");
        sendMessageToClient("update_block,1,3,[s:I tag:0 offset:0 val:0]");
        sendMessageToClient("update_memory,0x0001,[1111]");
        sendMessageToClient("update_interconnect,1,0,[1]");
        sendMessageToClient("update_interconnect,2,0,[1]");
        sendMessageToClient("update_interconnect,3,0,[1]");
        sendMessageToClient("update_interconnect,4,0,[1]");
        sendMessageToClient("update_interconnect,5,1,[1]");
        sendMessageToClient("update_interconnect,5,2,[12]");
        sendMessageToClient("update_interconnect,5,3,[12314]");
        sendMessageToClient("update_interconnect,5,4,[221]");

        // Wait for the server to be ready to send messages
        std::this_thread::sleep_for(std::chrono::seconds(10));

        // Send the message
        sendMessageToClient("update_register,2,2,[35]");
        sendMessageToClient("update_block,2,8,[s:I tag:0 offset:0 val:0]");
        sendMessageToClient("update_memory,0x0001,[2222]");
        sendMessageToClient("update_interconnect,1,0,[2]");
        sendMessageToClient("update_interconnect,2,0,[3]");
        sendMessageToClient("update_interconnect,3,0,[4]");
        sendMessageToClient("update_interconnect,4,0,[5]");
        sendMessageToClient("update_interconnect,5,1,[6]");
        sendMessageToClient("update_interconnect,5,2,[3]");
        sendMessageToClient("update_interconnect,5,3,[314]");
        sendMessageToClient("update_interconnect,5,4,[2]");
    }
}

// Function to read and print incoming messages from the queue
void readIncomingMessages()
{
    while (true)
    {
        if (!incomingMessageQueue.empty())
        {
            // Fetch and print the message
            std::string message = incomingMessageQueue.front();
            std::cout << "Received message: " << message << std::endl;

            // Remove the message from the queue after processing
            incomingMessageQueue.pop();
        }
        else
        {
            // Sleep briefly to avoid busy-waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main()
{
    // Start the WebSocket server in the main thread
    std::thread send_thread(sendTestMessages);

    // Start the thread to read incoming messages
    std::thread read_thread(readIncomingMessages);

    // Start the server
    startServer();

    // Join the threads to keep them running
    send_thread.join();
    read_thread.join();

    return 0;
}