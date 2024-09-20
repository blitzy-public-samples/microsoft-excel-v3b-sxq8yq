import { Workbook } from 'src/shared/models/Workbook';
import { Worksheet } from 'src/shared/models/Worksheet';
import { Cell } from 'src/shared/models/Cell';
import { Range } from 'src/shared/utils/Range';

// Enum representing different types of charts available in Excel
enum ChartType {
    Column,
    Bar,
    Line,
    Pie,
    Area,
    XY,
    Stock,
    Radar,
    Surface,
    Combo
}

// Interface for chart configuration options
interface ChartOptions {
    showLegend: boolean;
    showDataLabels: boolean;
    title: string;
    xAxisTitle: string;
    yAxisTitle: string;
}

// Interface representing a series of data in a chart
interface ChartSeries {
    name: string;
    xValues: Range;
    yValues: Range;
    color: string;
}

// Main class representing an Excel chart
class Chart {
    id: string;
    type: ChartType;
    options: ChartOptions;
    series: ChartSeries[];
    worksheet: Worksheet;

    // Initializes a new Chart instance
    constructor(type: ChartType, worksheet: Worksheet, options: ChartOptions) {
        this.id = Date.now().toString(); // Generate a unique ID
        this.type = type;
        this.worksheet = worksheet;
        this.options = options;
        this.series = [];
    }

    // Adds a new series to the chart
    addSeries(series: ChartSeries): void {
        // Validate the series data using the Range utility
        if (series.xValues.isValid() && series.yValues.isValid()) {
            this.series.push(series);
        } else {
            throw new Error('Invalid series data range');
        }
    }

    // Removes a series from the chart by index
    removeSeries(index: number): boolean {
        if (index >= 0 && index < this.series.length) {
            this.series.splice(index, 1);
            return true;
        }
        return false;
    }

    // Updates the chart options
    updateOptions(newOptions: ChartOptions): void {
        this.options = { ...this.options, ...newOptions };
    }

    // Gets the total data range covered by all series in the chart
    getDataRange(): Range {
        if (this.series.length === 0) {
            return new Range(''); // Return an empty range if no series
        }

        // Find the minimum and maximum cell references across all series
        const allRanges = this.series.flatMap(s => [s.xValues, s.yValues]);
        const minCell = Cell.getMinCell(allRanges.map(r => r.startCell));
        const maxCell = Cell.getMaxCell(allRanges.map(r => r.endCell));

        return new Range(`${minCell.reference}:${maxCell.reference}`);
    }

    // Converts the Chart instance to a JSON representation
    toJSON(): object {
        return {
            id: this.id,
            type: ChartType[this.type],
            options: this.options,
            series: this.series.map(s => ({
                name: s.name,
                xValues: s.xValues.toString(),
                yValues: s.yValues.toString(),
                color: s.color
            })),
            worksheet: this.worksheet.name
        };
    }
}

// Human tasks:
// TODO: Implement additional chart-specific properties and methods for each chart type
// TODO: Add validation logic for chart data ranges and series
// TODO: Implement chart rendering logic (possibly in a separate file)
// TODO: Add support for chart animations and transitions
// TODO: Implement chart theme support for consistent styling across the application