import { Cell } from 'src/shared/models/Cell';
import { CellAddressConverter } from 'src/shared/utils/CellAddressConverter';
import { FormulaParser } from 'src/shared/utils/FormulaParser';

export class Formula {
    expression: string;
    parsedExpression: any;
    cell: Cell;
    isValid: boolean;

    constructor(expression: string, cell: Cell) {
        // Set the expression property
        this.expression = expression;
        
        // Set the cell property
        this.cell = cell;
        
        // Parse the expression using FormulaParser
        const parseResult = FormulaParser.parse(expression);
        
        // Set the parsedExpression property
        this.parsedExpression = parseResult.ast;
        
        // Set the isValid property based on parsing result
        this.isValid = parseResult.isValid;
    }

    evaluate(): any {
        // Check if the formula is valid
        if (!this.isValid) {
            return new Error('Invalid formula');
        }

        try {
            // Traverse the parsedExpression tree
            const result = this.evaluateNode(this.parsedExpression);
            
            // Return the final calculated value
            return result;
        } catch (error) {
            return error;
        }
    }

    private evaluateNode(node: any): any {
        if (typeof node === 'number') {
            return node;
        }

        if (typeof node === 'string') {
            // For each cell reference, use CellAddressConverter to get the cell value
            const cellValue = CellAddressConverter.getCellValue(node);
            return cellValue;
        }

        if (Array.isArray(node)) {
            // Perform operations as defined in the expression
            const operator = node[0];
            const operands = node.slice(1).map(operand => this.evaluateNode(operand));

            switch (operator) {
                case '+':
                    return operands.reduce((a, b) => a + b, 0);
                case '-':
                    return operands.reduce((a, b) => a - b);
                case '*':
                    return operands.reduce((a, b) => a * b, 1);
                case '/':
                    return operands.reduce((a, b) => a / b);
                // Add more operators as needed
                default:
                    throw new Error(`Unknown operator: ${operator}`);
            }
        }

        throw new Error('Invalid node type in formula AST');
    }

    getDependencies(): Cell[] {
        const dependencies: Cell[] = [];

        // Traverse the parsedExpression tree
        this.collectDependencies(this.parsedExpression, dependencies);

        // Return the array of dependent Cells
        return dependencies;
    }

    private collectDependencies(node: any, dependencies: Cell[]): void {
        if (typeof node === 'string' && /^[A-Z]+[0-9]+$/.test(node)) {
            // Convert cell references to Cell objects using CellAddressConverter
            const cell = CellAddressConverter.getCellFromReference(node);
            if (cell && !dependencies.includes(cell)) {
                dependencies.push(cell);
            }
        } else if (Array.isArray(node)) {
            // Recursively collect dependencies from child nodes
            node.forEach(child => this.collectDependencies(child, dependencies));
        }
    }

    updateExpression(newExpression: string): void {
        // Set the new expression
        this.expression = newExpression;

        // Re-parse the expression using FormulaParser
        const parseResult = FormulaParser.parse(newExpression);

        // Update the parsedExpression property
        this.parsedExpression = parseResult.ast;

        // Update the isValid property based on parsing result
        this.isValid = parseResult.isValid;
    }
}

// Human tasks:
// TODO: Implement support for array formulas
// TODO: Add more complex formula functions (e.g., financial, statistical)
// TODO: Implement caching mechanism for frequently used formulas
// TODO: Implement error handling for circular references
// TODO: Optimize performance for large, complex formulas