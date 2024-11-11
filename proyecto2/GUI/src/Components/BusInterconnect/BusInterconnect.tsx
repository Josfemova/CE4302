import React, { useEffect, useState } from "react";
import { useWebSocketData } from "../../WebSocketProvider/WebSocketProvider"; // Import the WebSocket context
import "./BusInterconnect.css";

interface BusInterconnectProps {
  title?: string;
  id: string; // Unique identifier for the bus interconnect
}

const BusInterconnect: React.FC<BusInterconnectProps> = ({ title, id }) => {
  const [readRequests, setReadRequests] = useState<string>("0");
  const [readResponses, setReadResponses] = useState<string>("0");
  const [writeRequests, setWriteRequests] = useState<string>("0");
  const [writeResponses, setWriteResponses] = useState<string>("0");
  const [dataTransmitted, setDataTransmitted] = useState<{
    [peId: string]: string;
  }>({}); // Object to hold data transmitted to each PE

  const webSocketData = useWebSocketData(); // Get WebSocket data from context

  useEffect(() => {
    if (webSocketData) {
      // Retrieve the bus interconnect data from WebSocket using the provided id
      const busInterconnectData = webSocketData.busInterconnects[0];

      if (busInterconnectData) {
        // Update state based on data from WebSocket, converting values to strings
        setReadRequests(String(busInterconnectData.readRequests));
        setReadResponses(String(busInterconnectData.readResponses));
        setWriteRequests(String(busInterconnectData.writeRequests));
        setWriteResponses(String(busInterconnectData.writeResponses));

        // Update dataTransmitted by merging the incoming data with the existing state
        setDataTransmitted((prevData) => ({
          ...prevData, // Keep previous PE data
          ...busInterconnectData.dataTransmitted, // Update with new data from WebSocket
        }));
      } else {
        // If no data available, reset to default values
        setReadRequests("0");
        setReadResponses("0");
        setWriteRequests("0");
        setWriteResponses("0");
        setDataTransmitted({});
      }
    }
  }, [webSocketData, id]);

  // Ensure dataTransmitted always has at least as many entries as needed (e.g., 4 PEs)
  const transmittedData =
    Object.keys(dataTransmitted).length > 0
      ? dataTransmitted
      : { "1": "0", "2": "0", "3": "0", "4": "0" }; // Default 4 PEs with "0" data

  return (
    <div className="bus-interconnect">
      <h1>{title || "Bus Interconnect"}</h1>

      <div className="request-response">
        <div className="read-section">
          <h3>Read</h3>
          <p>Requests: {readRequests}</p>
          <p>Responses: {readResponses}</p>
        </div>
        <div className="write-section">
          <h3>Write</h3>
          <p>Requests: {writeRequests}</p>
          <p>Responses: {writeResponses}</p>
        </div>
      </div>

      <div className="data-transmitted">
        <h3>Amount of data transmitted to each PE</h3>
        <div className="pe-data">
          {Object.keys(transmittedData).map((peId) => (
            <div key={peId} className="pe-data-item">
              <p>
                PE {peId}: {transmittedData[peId]}
              </p>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default BusInterconnect;
