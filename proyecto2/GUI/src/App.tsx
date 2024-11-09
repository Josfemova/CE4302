// In your React component or TypeScript file
import React from "react";
import ProcessingElement from "./Components/ProcessingElement/ProcessingElement";
import BusInterconnect from "./Components/BusInterconnect/BusInterconnect";
import Memory from "./Components/Memory/Memory";
import Cache from "./Components/Cache/Cache";
import Controls from "./Components/Controls/Controls";

import "./App.css";

const App: React.FC = () => {
  return (
    <div className="background">
      <div className="titles_and_text">
        <h1>MESI Protocol</h1>
      </div>
      <div className="sim_container">
        <div className="system">
          <div className="processing-elements">
            <ProcessingElement
              title="PE 1"
              registers={{ REG0: 10, REG1: 20, REG2: 30, REG3: 40 }}
            />
            <ProcessingElement
              title="PE 2"
              registers={{ REG0: 10, REG1: 20, REG2: 30, REG3: 40 }}
            />
            <ProcessingElement
              title="PE 3"
              registers={{ REG0: 10, REG1: 20, REG2: 30, REG3: 40 }}
            />
            <ProcessingElement
              title="PE 4"
              registers={{ REG0: 10, REG1: 20, REG2: 30, REG3: 40 }}
            />
          </div>
          <div className="caches">
            <Cache
              title="Cache 1"
              blocks={[
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
              ]}
            />
            <Cache
              title="Cache 2"
              blocks={[
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
              ]}
            />
            <Cache
              title="Cache 3"
              blocks={[
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
              ]}
            />
            <Cache
              title="Cache 4"
              blocks={[
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
                "s:M tag:0x20 offset:0x1140 val:245",
              ]}
            />
          </div>
          <div className="bi-holder">
            <BusInterconnect
              title="Bus Interconnect"
              readRequests={10}
              readResponses={8}
              writeRequests={12}
              writeResponses={10}
              dataTransmitted={[50, 70, 60, 90]}
            />
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
  );
};

export default App;
