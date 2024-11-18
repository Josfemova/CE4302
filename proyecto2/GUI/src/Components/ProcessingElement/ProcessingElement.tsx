import React, { useEffect, useState } from "react";
import { useWebSocketData } from "../../WebSocketProvider/WebSocketProvider";
import "./ProcessingElement.css";

interface ProcessingElementProps {
  title?: string;
  id: string;
}

const ProcessingElement: React.FC<ProcessingElementProps> = ({ title, id }) => {
  const [registers, setRegisters] = useState<{ [key: string]: number | null }>(
    {}
  );
  const webSocketData = useWebSocketData();

  useEffect(() => {
    if (webSocketData) {
      console.log(webSocketData);
      const processingElementData = webSocketData.processingElements[id];

      if (processingElementData) {
        setRegisters(processingElementData);
      } else {
        setRegisters({});
      }
    }
  }, [webSocketData, id]);

  const defaultRegisters = ["REG 0", "REG 1", "REG 2", "REG 3"];

  return (
    <div className="processing-element">
      <h1 className="pe-title">{title || "Processing Element"}</h1>
      <div className="pe-registers">
        {Object.entries(registers).length > 0
          ? Object.entries(registers).map(([reg, value]) => (
              <div key={reg} className="pe-register">
                <span className="pe-register-name">REG{reg}:</span>
                <span className="pe-register-value">
                  {value !== null ? value : "Null"}
                </span>
              </div>
            ))
          : defaultRegisters.map((reg) => (
              <div key={reg} className="pe-register">
                <span className="pe-register-name">{reg}:</span>
                <span className="pe-register-value">Null</span>
              </div>
            ))}
      </div>
    </div>
  );
};

export default ProcessingElement;
