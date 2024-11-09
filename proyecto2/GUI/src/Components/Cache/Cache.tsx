import React from "react";
import "./Cache.css";

interface CacheProps {
  title?: string;
  blocks?: string[]; // Array of block values, with each block representing 32 bytes
}

const Cache: React.FC<CacheProps> = ({ title, blocks }) => {
  return (
    <div className="cache">
      <h1 className="cache-title">{title || "Cache"}</h1>
      <div className="cache-blocks">
        {blocks && blocks.length >= 8 ? (
          blocks.slice(0, 8).map((value, index) => (
            <div key={index} className="cache-block">
              <span className="cache-block-name">Block {index}:</span>
              <span className="cache-block-value">{value}</span>
            </div>
          ))
        ) : (
          <p>No block data</p>
        )}
      </div>
    </div>
  );
};

export default Cache;
