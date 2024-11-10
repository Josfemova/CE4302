import React, { useState } from "react";
import "./Controls.css";

interface FilePathInputProps {
  peId: number;
  onSubmit: (peId: number, filePath: string) => void;
}

const FilePathInput: React.FC<FilePathInputProps> = ({ peId, onSubmit }) => {
  const [filePath, setFilePath] = useState("");

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setFilePath(e.target.value);
  };

  const handleSubmit = () => {
    onSubmit(peId, filePath);
  };

  return (
    <div className="file-input-container">
      <input
        type="text"
        value={filePath}
        onChange={handleFileChange}
        placeholder={`Enter file path for PE ${peId}`}
        className="file-path-input"
      />
      <button onClick={handleSubmit} className="submit-button">
        Submit
      </button>
    </div>
  );
};

const Controls: React.FC = () => {
  const handleFileSubmit = (peId: number, filePath: string) => {
    console.log(`File path for PE ${peId}: ${filePath}`);
  };

  const handlePlay = () => {
    console.log("Play button clicked");
  };

  const handleStepping = () => {
    console.log("Stepping button clicked");
  };

  const handlePause = () => {
    console.log("Pause button clicked");
  };

  return (
    <div className="controls-container">
      <div className="file-inputs">
        {[1, 2, 3, 4].map((peId) => (
          <FilePathInput key={peId} peId={peId} onSubmit={handleFileSubmit} />
        ))}
      </div>
      <div className="action-buttons">
        <button onClick={handlePlay} className="action-button">
          Play
        </button>
        <button onClick={handleStepping} className="action-button">
          Step
        </button>
        <button onClick={handlePause} className="action-button">
          Pause
        </button>
      </div>
    </div>
  );
};

export default Controls;