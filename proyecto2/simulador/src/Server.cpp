#include <libwebsockets.h>
#include <iostream>
#include <string>
#include <queue>

// Declare the global wsi (WebSocket instance)
struct lws *global_wsi = nullptr;

// Declare the incoming message queue to store received messages
std::queue<std::string> incomingMessageQueue;

// Send message to the WebSocket client
void sendMessageToClient(const std::string &message)
{
    std::cout << "Sending message: " << message << std::endl;
    if (global_wsi == nullptr)
    {
        std::cerr << "No WebSocket client connected!" << std::endl;
        return;
    }

    // Format the message according to the required format
    std::string formatted_message = "@" + message + "$";
    unsigned char buf[LWS_PRE + formatted_message.size()];
    memcpy(buf + LWS_PRE, formatted_message.c_str(), formatted_message.size());

    // Send the message to the client
    lws_write(global_wsi, buf + LWS_PRE, formatted_message.size(), LWS_WRITE_TEXT);
}

// WebSocket callback function
static int callbackServer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        global_wsi = wsi;
        std::cout << "Client connected!" << std::endl;
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        // If the WebSocket is writable, send a message
        sendMessageToClient("Hello from server!");
        lws_callback_on_writable(wsi); // Request further writes if necessary
        break;

    case LWS_CALLBACK_RECEIVE:
        if (in && len > 0)
        {
            std::string received_message(static_cast<char *>(in), len);
            std::cout << "Received message from client: " << received_message << std::endl;

            // Push to incoming queue
            incomingMessageQueue.push(received_message);

            // Send acknowledgment back to the client
            sendMessageToClient("Received: " + received_message);
        }
        else
        {
            std::cout << "Empty or invalid message received." << std::endl;
        }
        break;

    case LWS_CALLBACK_CLOSED:
        std::cout << "Client disconnected!" << std::endl;
        break;

    default:
        break;
    }
    return 0;
}

// Define protocols
static const struct lws_protocols protocols[] = {
    {
        "counter_protocol",
        callbackServer,
        0, // per-session data size
        0  // rx buffer size
    },
    {NULL, NULL, 0, 0} // terminator
};

// Start WebSocket server
void startServer()
{
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = 8080;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    struct lws_context *context = lws_create_context(&info);
    if (context == NULL)
    {
        std::cerr << "Error creating WebSocket context" << std::endl;
        return;
    }

    // Main event loop
    while (1)
    {
        // Handle WebSocket events and manage writable callbacks
        lws_service(context, 100);
    }

    lws_context_destroy(context);
}
