import React, { useState, useEffect, useRef } from 'react';
import { Chart as ChartJS, ChartConfiguration } from 'chart.js';
import { useSelector, useDispatch } from 'react-redux';
import { RootState } from '../store/types';
import { updateChartData } from '../store/actions/chartActions';
import { ChartData } from '../types/ChartTypes';
import { api } from '../services/api';

// Define the props interface for the Chart component
interface ChartProps {
  chartId: string;
  worksheetId: string;
}

const Chart: React.FC<ChartProps> = ({ chartId, worksheetId }) => {
  // State hooks
  const [chartInstance, setChartInstance] = useState<ChartJS | null>(null);
  const [chartData, setChartData] = useState<ChartData | null>(null);
  
  // Ref for the chart container
  const chartContainer = useRef<HTMLCanvasElement>(null);
  
  // Redux hooks
  const selectedCells = useSelector((state: RootState) => state.spreadsheet.selectedCells);
  const dispatch = useDispatch();

  // Initialize chart and handle updates
  useEffect(() => {
    if (chartId && worksheetId) {
      initializeChart();
    }
  }, [chartId, worksheetId, selectedCells]);

  // Initialize the Chart.js instance
  const initializeChart = async (): Promise<void> => {
    if (chartContainer.current) {
      const ctx = chartContainer.current.getContext('2d');
      if (ctx) {
        // Fetch initial chart data from API
        const initialData = await api.getChartData(chartId);
        setChartData(initialData);

        // Create Chart.js instance
        const newChartInstance = new ChartJS(ctx, {
          type: initialData.type,
          data: initialData.data,
          options: initialData.options
        } as ChartConfiguration);

        setChartInstance(newChartInstance);
      }
    }
  };

  // Update chart data based on selected cells
  const updateChartData = () => {
    if (chartInstance && selectedCells) {
      // Process selected cells and update chart data
      const newData = processSelectedCells(selectedCells);
      setChartData(newData);
      chartInstance.data = newData.data;
      chartInstance.update();

      // Dispatch action to update chart data in Redux store
      dispatch(updateChartData(chartId, newData));
    }
  };

  // Handle chart type change from user input
  const handleChartTypeChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
    if (chartInstance) {
      const newType = event.target.value;
      chartInstance.config.type = newType as keyof ChartConfiguration['type'];
      chartInstance.update();

      // Update chart data in state and Redux store
      setChartData(prevData => ({ ...prevData!, type: newType }));
      dispatch(updateChartData(chartId, { ...chartData!, type: newType }));
    }
  };

  // Handle changes to chart options
  const handleChartOptionChange = (option: string, value: any) => {
    if (chartInstance) {
      // Update chart options
      chartInstance.options = {
        ...chartInstance.options,
        [option]: value
      };
      chartInstance.update();

      // Update chart data in state and Redux store
      setChartData(prevData => ({
        ...prevData!,
        options: {
          ...prevData!.options,
          [option]: value
        }
      }));
      dispatch(updateChartData(chartId, {
        ...chartData!,
        options: {
          ...chartData!.options,
          [option]: value
        }
      }));
    }
  };

  // Render the chart and controls
  return (
    <div className="chart-component">
      <canvas ref={chartContainer} />
      <div className="chart-controls">
        <select onChange={handleChartTypeChange}>
          <option value="bar">Bar</option>
          <option value="line">Line</option>
          <option value="pie">Pie</option>
          {/* Add more chart type options */}
        </select>
        {/* Add more chart option controls */}
      </div>
    </div>
  );
};

export default Chart;

// Human tasks:
// TODO: Implement accessibility features for chart interactions
// TODO: Add unit tests for Chart component and its functions
// TODO: Optimize chart rendering performance for large datasets
// TODO: Implement error handling for API calls and chart initialization
// TODO: Add support for exporting charts as images or PDF