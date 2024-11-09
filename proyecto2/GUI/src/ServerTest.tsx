// In your React component or TypeScript file
import React, { useEffect, useState } from "react";

const App: React.FC = () => {
  const [response, setResponse] = useState<string>("");

  useEffect(() => {
    // Connect to WebSocket server (make sure C++ server is listening on the same port)
    const socket = new WebSocket("ws://localhost:8080");

    socket.onopen = () => {
      console.log("Connected to server");
    };

    socket.onmessage = (event) => {
      setResponse(event.data); // Display message from server
    };

    socket.onerror = (error) => {
      console.error("WebSocket Error:", error);
    };

    socket.onclose = () => {
      console.log("Disconnected from server");
    };

    // Cleanup on component unmount
    return () => socket.close();
  }, []);

  return (
    <div>
      <h1>Response from C++ Server (WebSocket):</h1>
      <p>{response}</p>
    </div>
  );
};

export default App;
