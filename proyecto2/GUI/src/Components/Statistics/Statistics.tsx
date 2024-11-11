import React, { useEffect, useState } from "react";
import { useWebSocketData } from "../../WebSocketProvider/WebSocketProvider";
import "./Statistics.css";

const Statistics: React.FC = () => {
  const [statistics, setStatistics] = useState<{ [key: string]: string }>({});
  const webSocketData = useWebSocketData();

  useEffect(() => {
    if (webSocketData && webSocketData.statistics) {
      setStatistics(webSocketData.statistics);
    }
  }, [webSocketData]);

  return (
    <div className="statistics-table">
      <h1>Statistics</h1>
      <table>
        <thead>
          <tr>
            <th>Statistic</th>
            <th>Value</th>
          </tr>
        </thead>
        <tbody>
          {Object.entries(statistics).length > 0 ? (
            Object.entries(statistics).map(([key, value]) => (
              <tr key={key}>
                <td>{key}</td>
                <td>{value}</td>
              </tr>
            ))
          ) : (
            <tr>
              <td colSpan={2}>No data available</td>
            </tr>
          )}
        </tbody>
      </table>
    </div>
  );
};

export default Statistics;
