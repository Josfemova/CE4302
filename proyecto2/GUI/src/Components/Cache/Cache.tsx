import React, { useEffect, useState } from "react";
import { useWebSocketData } from "../../WebSocketProvider/WebSocketProvider"; // Import the WebSocket context
import "./Cache.css";

interface CacheProps {
  title?: string;
  id: string; // Unique identifier for the cache
}

const Cache: React.FC<CacheProps> = ({ title, id }) => {
  const [blocks, setBlocks] = useState<string[]>([]); // State to store cache blocks
  const webSocketData = useWebSocketData(); // WebSocket data from context

  useEffect(() => {
    if (webSocketData) {
      const cacheData = webSocketData.caches[id];

      // If cache data exists, update the blocks array with the current cache data
      if (cacheData) {
        setBlocks(Object.values(cacheData)); // Extract the cache values into an array
        console.log(cacheData);
      } else {
        // If no cache data, initialize blocks with empty strings
        setBlocks(Array(8).fill("")); // Default to 8 empty blocks
      }
    }
  }, [webSocketData, id]);

  const defaultBlocks = 8; // Default number of blocks to display

  return (
    <div className="cache">
      <h1 className="cache-title">{title || "Cache"}</h1>
      <div className="cache-blocks">
        {blocks.length >= defaultBlocks
          ? blocks.slice(0, defaultBlocks).map((value, index) => (
            <div key={index} className="cache-block">
              <span className="cache-block-name">Block {index}:</span>
              {(value.includes("State:I")) ? 
                <span className="cache-block-value-I">{value}</span>
                :((value.includes("State:E")) ? 
                <span className="cache-block-value-E">{value}</span>
                :((value.includes("State:M")) ? 
                <span className="cache-block-value-M">{value}</span>
                :<span className="cache-block-value">{value}</span>))
              }
            </div>
          ))
          : // When not enough blocks are available, fill with "Null"
          Array.from({ length: defaultBlocks }).map((_, index) => (
            <div key={index} className="cache-block">
              <span className="cache-block-name">Block {index}:</span>
              <span className="cache-block-value"></span>
            </div>
          ))}
      </div>
    </div>
  );
};

export default Cache;
