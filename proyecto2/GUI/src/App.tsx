// In your React component or TypeScript file
import React from "react";
import ProcessingElement from "./Components/ProcessingElement/ProcessingElement";
import BusInterconnect from "./Components/BusInterconnect/BusInterconnect";
import Memory from "./Components/Memory/Memory";
import Cache from "./Components/Cache/Cache";
import Controls from "./Components/Controls/Controls";
import { WebSocketProvider } from "./WebSocketProvider/WebSocketProvider";

import "./App.css";

const App: React.FC = () => {
  return (
    <WebSocketProvider>
      <div className="background">
        <div className="titles_and_text">
          <h1>MESI Protocol</h1>
        </div>
        <div className="sim_container">
          <div className="system">
            <div className="processing-elements">
              <ProcessingElement title="PE 1" id="1" />
              <ProcessingElement title="PE 2" id="2" />
              <ProcessingElement title="PE 3" id="3" />
              <ProcessingElement title="PE 4" id="4" />
            </div>
            <div className="caches">
              <Cache title="Cache 1" id="1" />
              <Cache title="Cache 2" id="2" />
              <Cache title="Cache 3" id="3" />
              <Cache title="Cache 4" id="4" />
            </div>
            <div className="bi-holder">
              <BusInterconnect title="Bus Interconnect" id="1" />
            </div>
            <div className="memory-holder">
              <Memory title="Memory" />
            </div>
          </div>
          <div className="controls">
            <Controls />
          </div>
        </div>
        <div className="statistics">
          <h1>Statistics</h1>
        </div>
      </div>
    </WebSocketProvider>
  );
};

export default App;
