import axios, { AxiosInstance } from 'axios';
import { Workbook } from '../models/Workbook';
import { Worksheet } from '../models/Worksheet';
import { Cell } from '../models/Cell';
import { Chart } from '../models/Chart';
import { PivotTable } from '../models/PivotTable';

const API_BASE_URL = process.env.REACT_APP_API_BASE_URL || 'https://api.excel.com/v1';

class ExcelApiService {
  private axiosInstance: AxiosInstance;

  constructor() {
    // Create an Axios instance with the base URL and default headers
    this.axiosInstance = axios.create({
      baseURL: API_BASE_URL,
      headers: {
        'Content-Type': 'application/json',
      },
    });

    // Set up request interceptors for adding authentication tokens
    this.axiosInstance.interceptors.request.use((config) => {
      const token = localStorage.getItem('authToken');
      if (token) {
        config.headers['Authorization'] = `Bearer ${token}`;
      }
      return config;
    });

    // Set up response interceptors for handling errors globally
    this.axiosInstance.interceptors.response.use(
      (response) => response,
      (error) => {
        // Handle global errors here (e.g., unauthorized, server errors)
        console.error('API Error:', error);
        return Promise.reject(error);
      }
    );
  }

  async getWorkbooks(): Promise<Workbook[]> {
    // Send a GET request to /workbooks
    const response = await this.axiosInstance.get('/workbooks');
    // Transform the response data into Workbook objects
    const workbooks = response.data.map((workbookData: any) => new Workbook(workbookData));
    // Return the array of Workbook objects
    return workbooks;
  }

  async getWorkbook(id: string): Promise<Workbook> {
    // Send a GET request to /workbooks/{id}
    const response = await this.axiosInstance.get(`/workbooks/${id}`);
    // Transform the response data into a Workbook object
    const workbook = new Workbook(response.data);
    // Return the Workbook object
    return workbook;
  }

  async createWorkbook(workbookData: Partial<Workbook>): Promise<Workbook> {
    // Send a POST request to /workbooks with the workbookData
    const response = await this.axiosInstance.post('/workbooks', workbookData);
    // Transform the response data into a Workbook object
    const createdWorkbook = new Workbook(response.data);
    // Return the created Workbook object
    return createdWorkbook;
  }

  async updateWorkbook(id: string, workbookData: Partial<Workbook>): Promise<Workbook> {
    // Send a PUT request to /workbooks/{id} with the workbookData
    const response = await this.axiosInstance.put(`/workbooks/${id}`, workbookData);
    // Transform the response data into a Workbook object
    const updatedWorkbook = new Workbook(response.data);
    // Return the updated Workbook object
    return updatedWorkbook;
  }

  async deleteWorkbook(id: string): Promise<void> {
    // Send a DELETE request to /workbooks/{id}
    await this.axiosInstance.delete(`/workbooks/${id}`);
    // Handle the response to ensure successful deletion
    // If no error is thrown, the deletion was successful
  }

  async getWorksheets(workbookId: string): Promise<Worksheet[]> {
    // Send a GET request to /workbooks/{workbookId}/worksheets
    const response = await this.axiosInstance.get(`/workbooks/${workbookId}/worksheets`);
    // Transform the response data into Worksheet objects
    const worksheets = response.data.map((worksheetData: any) => new Worksheet(worksheetData));
    // Return the array of Worksheet objects
    return worksheets;
  }

  async updateCell(workbookId: string, worksheetId: string, cellId: string, cellData: Partial<Cell>): Promise<Cell> {
    // Send a PUT request to /workbooks/{workbookId}/worksheets/{worksheetId}/cells/{cellId} with the cellData
    const response = await this.axiosInstance.put(
      `/workbooks/${workbookId}/worksheets/${worksheetId}/cells/${cellId}`,
      cellData
    );
    // Transform the response data into a Cell object
    const updatedCell = new Cell(response.data);
    // Return the updated Cell object
    return updatedCell;
  }

  async createChart(workbookId: string, worksheetId: string, chartData: Partial<Chart>): Promise<Chart> {
    // Send a POST request to /workbooks/{workbookId}/worksheets/{worksheetId}/charts with the chartData
    const response = await this.axiosInstance.post(
      `/workbooks/${workbookId}/worksheets/${worksheetId}/charts`,
      chartData
    );
    // Transform the response data into a Chart object
    const createdChart = new Chart(response.data);
    // Return the created Chart object
    return createdChart;
  }

  async createPivotTable(workbookId: string, worksheetId: string, pivotTableData: Partial<PivotTable>): Promise<PivotTable> {
    // Send a POST request to /workbooks/{workbookId}/worksheets/{worksheetId}/pivottables with the pivotTableData
    const response = await this.axiosInstance.post(
      `/workbooks/${workbookId}/worksheets/${worksheetId}/pivottables`,
      pivotTableData
    );
    // Transform the response data into a PivotTable object
    const createdPivotTable = new PivotTable(response.data);
    // Return the created PivotTable object
    return createdPivotTable;
  }
}

export function createApiService(): ExcelApiService {
  // Create a new instance of ExcelApiService
  const apiService = new ExcelApiService();
  // Return the created instance
  return apiService;
}

// Human tasks:
// - Implement error handling and retry logic for network failures
// - Add caching mechanism for frequently accessed data
// - Implement request cancellation for long-running requests
// - Add support for batch operations to optimize network usage
// - Implement offline support and synchronization