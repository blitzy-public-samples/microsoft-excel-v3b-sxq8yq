import { Formula } from 'src/shared/models/Formula';
import { Cell } from 'src/shared/models/Cell';
import { CellAddressConverter } from 'src/shared/utils/CellAddressConverter';
import { FunctionLibrary } from 'src/core/FunctionLibrary';

// Define global constants
const OPERATORS = ['=', '+', '-', '*', '/', '^', '&', '>', '<', '>=', '<=', '<>', '(', ')'];
const CELL_REFERENCE_REGEX = /\$?[A-Za-z]+\$?[0-9]+/;

class FormulaParser {
    private functionLibrary: FunctionLibrary;

    constructor(functionLibrary: FunctionLibrary) {
        // Initialize the FormulaParser instance
        // Set the FunctionLibrary property
        this.functionLibrary = functionLibrary;
    }

    public parse(formulaString: string): Formula {
        // Tokenize the formula string
        const tokens = this.tokenize(formulaString);

        // Validate the tokens
        this.validateTokens(tokens);

        // Convert cell references to absolute addresses
        const convertedTokens = this.convertCellReferences(tokens);

        // Build the abstract syntax tree (AST)
        const ast = this.buildAST(convertedTokens);

        // Create and return a new Formula object
        return new Formula(formulaString, ast);
    }

    public evaluate(formula: Formula, cellReferences: Map<string, Cell>): any {
        // Traverse the AST of the Formula object
        const result = this.evaluateNode(formula.ast, cellReferences);

        // Handle error conditions (e.g., #DIV/0!, #VALUE!)
        if (this.isError(result)) {
            return result;
        }

        // Return the final result
        return result;
    }

    private tokenize(formulaString: string): Array<string> {
        // Initialize an empty array for tokens
        const tokens: string[] = [];
        let currentToken = '';

        // Iterate through the formula string
        for (let i = 0; i < formulaString.length; i++) {
            const char = formulaString[i];

            // Identify operators, functions, cell references, and literals
            if (OPERATORS.includes(char)) {
                if (currentToken) {
                    tokens.push(currentToken);
                    currentToken = '';
                }
                tokens.push(char);
            } else if (char === ' ') {
                if (currentToken) {
                    tokens.push(currentToken);
                    currentToken = '';
                }
            } else if (char === '"') {
                // Handle string literals (text within quotes)
                const endQuoteIndex = formulaString.indexOf('"', i + 1);
                if (endQuoteIndex === -1) {
                    throw new Error('Unclosed string literal');
                }
                tokens.push(formulaString.slice(i, endQuoteIndex + 1));
                i = endQuoteIndex;
            } else {
                currentToken += char;
            }
        }

        // Add the last token if exists
        if (currentToken) {
            tokens.push(currentToken);
        }

        // Return the array of tokens
        return tokens;
    }

    private buildAST(tokens: Array<string>): ASTNode {
        // Initialize a stack for operators and operands
        const stack: ASTNode[] = [];

        // Iterate through the tokens
        for (const token of tokens) {
            if (this.isOperator(token)) {
                // Handle operators
                const rightOperand = stack.pop();
                const leftOperand = stack.pop();
                stack.push(new ASTNode('operator', token, [leftOperand, rightOperand]));
            } else if (this.isFunction(token)) {
                // Handle functions
                const args = [];
                while (stack.length > 0 && stack[stack.length - 1].type !== 'parenthesis') {
                    args.unshift(stack.pop());
                }
                stack.pop(); // Remove opening parenthesis
                stack.push(new ASTNode('function', token, args));
            } else if (token === '(') {
                stack.push(new ASTNode('parenthesis', token));
            } else if (token === ')') {
                // Handle closing parenthesis
                const expression = [];
                while (stack.length > 0 && stack[stack.length - 1].type !== 'parenthesis') {
                    expression.unshift(stack.pop());
                }
                stack.pop(); // Remove opening parenthesis
                if (expression.length === 1) {
                    stack.push(expression[0]);
                } else {
                    stack.push(new ASTNode('expression', '', expression));
                }
            } else {
                // Handle operands (numbers, cell references, etc.)
                stack.push(new ASTNode('operand', token));
            }
        }

        // Return the root node of the AST
        return stack[0];
    }

    private validateTokens(tokens: Array<string>): void {
        // Implement token validation logic
    }

    private convertCellReferences(tokens: Array<string>): Array<string> {
        return tokens.map(token => {
            if (CELL_REFERENCE_REGEX.test(token)) {
                return CellAddressConverter.toAbsolute(token);
            }
            return token;
        });
    }

    private evaluateNode(node: ASTNode, cellReferences: Map<string, Cell>): any {
        // Implement node evaluation logic
    }

    private isOperator(token: string): boolean {
        return OPERATORS.includes(token);
    }

    private isFunction(token: string): boolean {
        return this.functionLibrary.hasFunction(token);
    }

    private isError(value: any): boolean {
        // Implement error checking logic
        return false;
    }
}

interface ASTNode {
    type: string;
    value: string;
    children?: ASTNode[];
}

// Human tasks:
// - Implement error handling for invalid formulas
// - Add support for array formulas
// - Optimize performance for large, complex formulas
// - Implement caching mechanism for frequently used formulas
// - Add unit tests for edge cases and complex formulas