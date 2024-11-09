import React from "react";
import "./BusInterconnect.css";

// Define the props for the component
interface BusInterconnectProps {
  title?: string;
  readRequests: number;
  readResponses: number;
  writeRequests: number;
  writeResponses: number;
  dataTransmitted: number[]; // Array of numbers representing data to each PE
}

const BusInterconnect: React.FC<BusInterconnectProps> = ({
  title,
  readRequests,
  readResponses,
  writeRequests,
  writeResponses,
  dataTransmitted,
}) => {
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
        <h3>Cantidad de datos transmitidos a cada PE</h3>
        <div className="pe-data">
          {dataTransmitted.map((data, index) => (
            <div key={index} className="pe-data-item">
              <p>
                PE {index + 1}: {data}
              </p>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default BusInterconnect;
