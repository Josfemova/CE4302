import React from "react";
import "./Memory.css";

interface MemoryProps {
  title?: string;
}

const Memory: React.FC<MemoryProps> = ({ title }) => {
  return (
    <div className="memory">
      <h1>{title || "Memory"}</h1>
      {/* Additional content for Memory component */}
    </div>
  );
};

export default Memory;
