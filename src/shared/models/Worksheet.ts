import { Cell } from './Cell';
import { Chart } from './Chart';
import { PivotTable } from './PivotTable';
import { NamedRange } from './NamedRange';
import { v4 as uuidv4 } from 'uuid';

// Interface for optional parameters when creating a Worksheet
export interface WorksheetOptions {
    name?: string;
    index?: number;
    visible?: boolean;
    tabColor?: string;
}

// Represents a single worksheet within an Excel workbook
export class Worksheet {
    id: string;
    name: string;
    index: number;
    visible: boolean;
    tabColor: string;
    cells: Map<string, Cell>;
    charts: Chart[];
    pivotTables: PivotTable[];
    namedRanges: NamedRange[];

    constructor(options: WorksheetOptions = {}) {
        // Generate a unique ID for the worksheet using UUID v4
        this.id = uuidv4();

        // Initialize worksheet properties with provided options or default values
        this.name = options.name || 'Sheet1';
        this.index = options.index || 0;
        this.visible = options.visible !== undefined ? options.visible : true;
        this.tabColor = options.tabColor || '#FFFFFF';

        // Initialize empty collections for cells, charts, pivot tables, and named ranges
        this.cells = new Map<string, Cell>();
        this.charts = [];
        this.pivotTables = [];
        this.namedRanges = [];
    }

    getCell(address: string): Cell | undefined {
        // Validate the cell address format
        if (!this.isValidCellAddress(address)) {
            throw new Error('Invalid cell address');
        }

        // Return the cell from the cells Map if it exists
        return this.cells.get(address);
    }

    setCell(address: string, value: string | number | boolean): Cell {
        // Validate the cell address format
        if (!this.isValidCellAddress(address)) {
            throw new Error('Invalid cell address');
        }

        // Create a new Cell instance if it doesn't exist
        let cell = this.cells.get(address);
        if (!cell) {
            cell = new Cell(address);
        }

        // Update the cell's value
        cell.setValue(value);

        // Add or update the cell in the cells Map
        this.cells.set(address, cell);

        // Return the updated or new cell
        return cell;
    }

    addChart(chart: Chart): void {
        // Validate the chart object
        if (!(chart instanceof Chart)) {
            throw new Error('Invalid chart object');
        }

        // Add the chart to the charts array
        this.charts.push(chart);
    }

    addPivotTable(pivotTable: PivotTable): void {
        // Validate the pivot table object
        if (!(pivotTable instanceof PivotTable)) {
            throw new Error('Invalid pivot table object');
        }

        // Add the pivot table to the pivotTables array
        this.pivotTables.push(pivotTable);
    }

    addNamedRange(namedRange: NamedRange): void {
        // Validate the named range object
        if (!(namedRange instanceof NamedRange)) {
            throw new Error('Invalid named range object');
        }

        // Add the named range to the namedRanges array
        this.namedRanges.push(namedRange);
    }

    toJSON(): object {
        // Create a new object with all Worksheet properties
        const json: any = {
            id: this.id,
            name: this.name,
            index: this.index,
            visible: this.visible,
            tabColor: this.tabColor,
            charts: this.charts,
            pivotTables: this.pivotTables,
            namedRanges: this.namedRanges,
        };

        // Convert the cells Map to an array of cell objects
        json.cells = Array.from(this.cells.values()).map(cell => cell.toJSON());

        // Return the serializable object
        return json;
    }

    private isValidCellAddress(address: string): boolean {
        // Simple regex to validate cell address format (e.g., A1, B2, AA100)
        const cellAddressRegex = /^[A-Z]+[1-9]\d*$/;
        return cellAddressRegex.test(address);
    }
}

// Human tasks:
// TODO: Implement data validation for cell values
// TODO: Add support for worksheet-level formulas and calculations
// TODO: Implement undo/redo functionality for worksheet operations
// TODO: Add support for conditional formatting
// TODO: Implement worksheet protection features