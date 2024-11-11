import React, { useEffect, useState } from "react";
import { useWebSocketData } from "../../WebSocketProvider/WebSocketProvider";
import "./Memory.css";

interface MemoryProps {
  title?: string;
}

const Memory: React.FC<MemoryProps> = ({ title }) => {
  const [memory, setMemory] = useState<{ [key: string]: string | null }>({});
  const webSocketData = useWebSocketData();

  useEffect(() => {
    if (webSocketData) {
      const memoryData = webSocketData.memory;
      if (memoryData) {
        setMemory(memoryData);
      } else {
        setMemory({});
      }
    }
  }, [webSocketData]);

  const defaultMemory = ["0x0000", "0x0001", "0x0002", "0x0003"];

  const groupMemoryInChunks = (memoryEntries: [string, string | null][]) => {
    const chunkedMemory: [string, string | null][][] = [];
    for (let i = 0; i < memoryEntries.length; i += 12) {
      chunkedMemory.push(memoryEntries.slice(i, i + 12));
    }
    return chunkedMemory;
  };

  const groupedMemory = groupMemoryInChunks(
    Object.entries(memory).length > 0
      ? Object.entries(memory)
      : defaultMemory.map((addr) => [addr, null])
  );

  return (
    <div className="memory">
      <h1 className="memory-title">{title || "Memory"}</h1>
      <div className="memory-addresses">
        {groupedMemory.map((chunk, index) => (
          <div key={index} className="memory-row">
            {chunk.map(([addr, value]) => (
              <div key={addr} className="memory-address">
                <span className="memory-address-name">{addr}:</span>
                <span className="memory-address-value">
                  {value !== null ? value : "Null"}{" "}
                </span>
              </div>
            ))}
          </div>
        ))}
      </div>
    </div>
  );
};

export default Memory;
