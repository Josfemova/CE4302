import React from "react";
import "./ProcessingElement.css";

interface ProcessingElementProps {
  title?: string;
  registers?: { [key: string]: number }; // Object with register values
}

const ProcessingElement: React.FC<ProcessingElementProps> = ({
  title,
  registers,
}) => {
  return (
    <div className="processing-element">
      <h1 className="pe-title">{title || "Processing Element"}</h1>
      <div className="pe-registers">
        {registers ? (
          Object.entries(registers).map(([reg, value]) => (
            <div key={reg} className="pe-register">
              <span className="pe-register-name">{reg}:</span>
              <span className="pe-register-value">{value}</span>
            </div>
          ))
        ) : (
          <p>No register data</p>
        )}
      </div>
    </div>
  );
};

export default ProcessingElement;
