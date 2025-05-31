import React, { useEffect, useState } from 'react';
import { Line } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  LineElement,
  PointElement,
  LinearScale,
  TimeScale,
  Tooltip,
  Title,
  Legend,
  Filler,
} from 'chart.js';
import 'chartjs-adapter-date-fns';

ChartJS.register(LineElement, PointElement, LinearScale, TimeScale, Tooltip, Title, Legend, Filler);

export default function MemoryUsageChart() {
  const [chartData, setChartData] = useState({ datasets: [] });

  useEffect(() => {
    const now = Date.now();
    const oneHourAgo = now - 60 * 60 * 1000;

    fetch('http://127.0.0.1:5000/api/metrics?since=' + Math.floor(oneHourAgo / 1000))
      .then(res => res.json())
      .then((response) => {
        const data = response;

        const usedData = data
          .filter((pt) => pt.metric === 'system:memory_used')
          .map((pt) => ({ x: pt.timestamp * 1000, y: parseFloat((pt.value / 1024 / 1024).toFixed(2)) }));

        const totalData = data
          .filter((pt) => pt.metric === 'system:memory_total')
          .map((pt) => ({ x: pt.timestamp * 1000, y: parseFloat((pt.value / 1024 / 1024).toFixed(2)) }));

        setChartData({
          datasets: [
            {
              label: 'Used Memory (GB)',
              data: usedData,
              borderColor: 'green',
              backgroundColor: 'rgba(0, 128, 0, 0.1)',
              fill: true,
              tension: 0.3,
              pointRadius: 3,
              pointHoverRadius: 6,
              pointHitRadius: 10,
            },
            {
              label: 'Total Memory (GB)',
              data: totalData,
              borderColor: 'gray',
              borderDash: [5, 5],
              fill: false,
              tension: 0,
              pointRadius: 3,
              pointHoverRadius: 6,
              pointHitRadius: 10,
            },
          ],
        });
      })
      .catch((err) => {
        console.error('Failed to load memory stats:', err);
      });
  }, []);

  const options = {
    responsive: true,
    plugins: {
      title: {
        display: true,
        text: 'System RAM Usage (Last Hour)',
        font: {
          size: 18,
        },
      },
      legend: {
        position: 'top',
      },
    },
    scales: {
      x: {
        type: 'time',
        min: Date.now() - 30 * 60 * 1000,
        max: Date.now(),
        time: {
          unit: 'minute',
          tooltipFormat: 'HH:mm:ss',
          displayFormats: {
            minute: 'HH:mm',
          },
        },
        title: {
          display: true,
          text: 'Time',
        },
        ticks: {
          maxTicksLimit: 6,
          autoSkip: true,
        },
      },
      y: {
        beginAtZero: true,
        title: {
          display: true,
          text: 'Memory (GB)',
        },
      },
    },
  };

  return <Line data={chartData} options={options} />;
}
