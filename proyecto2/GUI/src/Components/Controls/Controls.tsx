import React, { useState } from "react";
import "./Controls.css";
import { useWebSocketData } from "../../WebSocketProvider/WebSocketProvider";

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
        placeholder={
          peId !== 100
            ? `Enter file path for PE ${peId}`
            : "Enter file path for memory initialization"
        }
        className="file-path-input"
      />
      <button onClick={handleSubmit} className="submit-button">
        Submit
      </button>
    </div>
  );
};

const Controls: React.FC = () => {
  const { sendMessage, events } = useWebSocketData();

  const handleFileSubmit = (peId: number, filePath: string) => {
    console.log(`File path for PE ${peId}: ${filePath}`);
    sendMessage(`@load_pe,${peId},[${filePath}]$`);
  };
  const handleFileSubmitMem = (peId: number, filePath: string) => {
    sendMessage(`@lmem,0,[${filePath}]$`);
  };

  const handlePlay = () => {
    console.log("Play button clicked");
    sendMessage("@play,0,[]$");
  };

  const handleStepping = () => {
    console.log("Stepping button clicked");
    sendMessage("@step,0,[]$");
  };

  const handlePause = () => {
    console.log("Pause button clicked");
    sendMessage("@pause,0,[]$");
  };

  return (
    <div className="controls-container">
      <div className="file-inputs">
        {[0, 1, 2, 3].map((peId) => (
          <FilePathInput key={peId} peId={peId} onSubmit={handleFileSubmit} />
        ))}
        <FilePathInput key={0} peId={100} onSubmit={handleFileSubmitMem} />
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
      <div className="events-box">
        <h3>Events</h3>
        <div className="events-list">
          {events.length > 0 ? (
            events.map((event, index) => (
              <div key={index} className="event-item">
                {event}
              </div>
            ))
          ) : (
            <div>No events received yet.</div>
          )}
        </div>
      </div>
    </div>
  );
};

export default Controls;
