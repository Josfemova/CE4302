#include <libwebsockets.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> // For sleep()

static int counter = 0;

static int callback_counter(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        // Start sending messages when a client connects
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
    {
        // Allocate buffer with LWS_PRE extra bytes
        unsigned char message[LWS_PRE + 32];
        unsigned char *message_ptr = &message[LWS_PRE]; // Offset buffer by LWS_PRE

        // Prepare and send the counter message to the client
        snprintf((char *)message_ptr, 32, "Counter: %d", counter++);
        lws_write(wsi, message_ptr, strlen((char *)message_ptr), LWS_WRITE_TEXT);

        // Schedule the next writable callback after a delay
        sleep(1); // 1-second delay
        lws_callback_on_writable(wsi);
        break;
    }

    default:
        break;
    }
    return 0;
}

// Define protocols
static const struct lws_protocols protocols[] = {
    {
        "counter_protocol", // protocol name
        callback_counter,   // callback function
        0,                  // per-session data size
        0,                  // rx buffer size
    },
    {NULL, NULL, 0, 0} // terminator
};

void start_server()
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
        fprintf(stderr, "Error creating WebSocket context\n");
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
