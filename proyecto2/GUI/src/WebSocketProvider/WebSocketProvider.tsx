import React, {
  createContext,
  useState,
  useEffect,
  useContext,
  ReactNode,
} from "react";

interface BusInterconnect {
  id: string;
  readRequests: string;
  readResponses: string;
  writeRequests: string;
  writeResponses: string;
  dataTransmitted: { [key: string]: string };
}

interface WebSocketData {
  processingElements: { [key: string]: { [register: string]: number } };
  caches: { [key: string]: { [block: string]: string } };
  busInterconnects: BusInterconnect[];
  memory: { [address: string]: string };
}

interface WebSocketContextData extends WebSocketData {
  sendMessage: (message: string) => void;
}

const WebSocketContext = createContext<WebSocketContextData | null>(null);

interface WebSocketProviderProps {
  children: ReactNode;
}

export const WebSocketProvider: React.FC<WebSocketProviderProps> = ({
  children,
}) => {
  const [socket, setSocket] = useState<WebSocket | null>(null);
  const [data, setData] = useState<WebSocketData>({
    processingElements: {
      "1": { "1": 0, "2": 0, "3": 0, "4": 0 },
      "2": { "1": 0, "2": 0, "3": 0, "4": 0 },
      "3": { "1": 0, "2": 0, "3": 0, "4": 0 },
      "4": { "1": 0, "2": 0, "3": 0, "4": 0 },
    },
    caches: {
      "1": {
        "1": "",
        "2": "",
        "3": "",
        "4": "",
        "5": "",
        "6": "",
        "7": "",
        "8": "",
      },
      "2": {
        "1": "",
        "2": "",
        "3": "",
        "4": "",
        "5": "",
        "6": "",
        "7": "",
        "8": "",
      },
      "3": {
        "1": "",
        "2": "",
        "3": "",
        "4": "",
        "5": "",
        "6": "",
        "7": "",
        "8": "",
      },
      "4": {
        "1": "",
        "2": "",
        "3": "",
        "4": "",
        "5": "",
        "6": "",
        "7": "",
        "8": "",
      },
    },
    busInterconnects: [
      {
        id: "1",
        readRequests: "",
        readResponses: "",
        writeRequests: "",
        writeResponses: "",
        dataTransmitted: {
          "1": "0",
          "2": "0",
          "3": "0",
          "4": "0",
        },
      },
    ],
    memory: {
      "0x0001": "00000000",
      "0x0002": "00000000",
      "0x0003": "00000000",
      "0x0004": "00000000",
      "0x0005": "00000000",
      "0x0006": "00000000",
      "0x0007": "00000000",
      "0x0008": "00000000",
      "0x0009": "00000000",
      "0x000x": "00000000",
      "0x000d": "00000000",
      "0x000r": "00000000",
      "0x000a": "00000000",
      "0x000f": "00000000",
      "0x000u": "00000000",
      "0x000g": "00000000",
      "0x000b": "00000000",
    },
  });

  const sendMessage = (message: string) => {
    if (socket?.readyState === WebSocket.OPEN) {
      socket.send(message);
      console.log("Message sent:", message);
    } else {
      console.warn("WebSocket is not open, message not sent.");
    }
  };

  const parseMessage = (messageString: string) => {
    console.log("Raw message received:", messageString);

    if (messageString.startsWith("@") && messageString.endsWith("$")) {
      const content = messageString.slice(1, -1);
      const parts = content.split(",");

      if (parts.length >= 2) {
        const cmd = parts[0];
        const args = parts.slice(1, parts.length - 1).join(",");
        const dataMatch = parts[parts.length - 1].match(/^\[(.*)\]$/);
        const data = dataMatch ? dataMatch[1] : null;

        console.log("Parsed message:", { cmd, args, data });

        return { cmd, args, data };
      }
    }

    console.warn("Message format is invalid:", messageString);
    return null;
  };

  useEffect(() => {
    const socket = new WebSocket("ws://localhost:8080");

    socket.onopen = () => {
      console.log("WebSocket connection established!");
    };

    socket.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    socket.onmessage = (event) => {
      const parsedMessage = parseMessage(event.data);

      if (parsedMessage) {
        const { cmd, args, data } = parsedMessage;

        if (cmd === "update_register" && data) {
          console.log(`Handling update_register with data: ${data}`);
          const peId = args[0];
          const registerId = args[2];
          const newValue = Number(data);

          setData((prevData) => {
            const updatedProcessingElements = {
              ...prevData.processingElements,
            };

            if (
              updatedProcessingElements[peId] &&
              updatedProcessingElements[peId][registerId] !== undefined
            ) {
              updatedProcessingElements[peId][registerId] = newValue;
              console.log(
                `Updated PE ${peId} register ${registerId} to ${newValue}`
              );
            } else {
              console.warn(
                `Invalid update: PE ${peId} or register ${registerId} does not exist.`
              );
            }

            return {
              ...prevData,
              processingElements: updatedProcessingElements,
            };
          });
        } else if (cmd === "update_block" && data) {
          console.log(`Handling update_block with data: ${data}`);
          const cacheId = args[0];
          const blockId = args[2];
          const newCacheData = data;

          setData((prevData) => {
            const updatedCaches = {
              ...prevData.caches,
              [cacheId]: {
                ...(prevData.caches[cacheId] || {}),
                [blockId]: newCacheData,
              },
            };

            console.log(
              `Updated Cache ${cacheId} Block ${blockId} with data: ${newCacheData}`
            );

            return {
              ...prevData,
              caches: updatedCaches,
            };
          });
        } else if (cmd === "update_interconnect" && data) {
          console.log(`Handling update_interconnect with data: ${data}`);
          const arg1 = args[0];
          const arg1Case = Number(arg1);
          const arg2 = args[2];
          const newCacheData = data;

          setData((prevData) => {
            const updatedBusInterconnects = [...prevData.busInterconnects];
            const busIndex = 0;
            const busInterconnect = updatedBusInterconnects[busIndex];

            switch (arg1Case) {
              case 1:
                busInterconnect.readRequests = newCacheData;
                console.log(`Updated readRequests: ${newCacheData}`);
                break;

              case 2:
                busInterconnect.readResponses = newCacheData;
                console.log(`Updated readResponses: ${newCacheData}`);
                break;

              case 3:
                busInterconnect.writeRequests = newCacheData;
                console.log(`Updated writeRequests: ${newCacheData}`);
                break;

              case 4:
                busInterconnect.writeResponses = newCacheData;
                console.log(`Updated writeResponses: ${newCacheData}`);
                break;

              case 5:
                busInterconnect.dataTransmitted[arg2] = newCacheData;
                console.log(
                  `Updated dataTransmitted for PEId ${arg2}: ${newCacheData}`
                );
                break;

              default:
                console.warn(
                  `Unexpected argument 1 case: ${arg1}, skipping update.`
                );
            }

            return {
              ...prevData,
              busInterconnects: updatedBusInterconnects,
            };
          });
        } else if (cmd === "update_memory" && data) {
          const address = args;
          const newMemoryValue = data;
          console.log(
            `Handling update_memory with data: ${data} and ${address} and ${args}`
          );

          setData((prevData) => {
            const updatedMemory = {
              ...prevData.memory,
              [address]: newMemoryValue,
            };

            console.log(
              `Updated memory at ${address} with value: ${newMemoryValue}`
            );

            return {
              ...prevData,
              memory: updatedMemory,
            };
          });
        }
      }
    };

    setSocket(socket);

    return () => {
      socket.close();
    };
  }, []);

  return (
    <WebSocketContext.Provider value={{ ...data, sendMessage }}>
      {children}
    </WebSocketContext.Provider>
  );
};

export const useWebSocketData = () => {
  const context = useContext(WebSocketContext);
  if (!context) {
    throw new Error("useWebSocketData must be used within a WebSocketProvider");
  }
  return context;
};
