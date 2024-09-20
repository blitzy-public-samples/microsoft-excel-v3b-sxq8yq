import { range } from 'lodash';

export class CellAddressConverter {
    /**
     * Converts an Excel-style cell address to zero-based row and column indices.
     * @param address The Excel-style cell address (e.g., 'A1')
     * @returns An object with zero-based row and column indices
     */
    public static toIndices(address: string): { row: number, col: number } {
        // Validate the input address format
        if (!CellAddressConverter.isValidAddress(address)) {
            throw new Error('Invalid cell address format');
        }

        // Split the address into column letters and row number
        const match = address.match(/^([A-Z]+)(\d+)$/);
        if (!match) {
            throw new Error('Unable to parse cell address');
        }

        const [, colLetters, rowString] = match;

        // Convert column letters to zero-based index
        const col = colLetters.split('').reduce((acc, letter) => {
            return acc * 26 + letter.charCodeAt(0) - 64;
        }, 0) - 1;

        // Convert row number to zero-based index
        const row = parseInt(rowString, 10) - 1;

        // Return an object with row and col properties
        return { row, col };
    }

    /**
     * Converts zero-based row and column indices to an Excel-style cell address.
     * @param row The zero-based row index
     * @param col The zero-based column index
     * @returns The Excel-style cell address
     */
    public static toAddress(row: number, col: number): string {
        // Validate input row and col are non-negative integers
        if (!Number.isInteger(row) || !Number.isInteger(col) || row < 0 || col < 0) {
            throw new Error('Invalid row or column index');
        }

        // Convert column index to letter(s)
        let colString = '';
        let colIndex = col + 1;
        while (colIndex > 0) {
            colIndex--;
            colString = String.fromCharCode(65 + (colIndex % 26)) + colString;
            colIndex = Math.floor(colIndex / 26);
        }

        // Increment row index by 1 (Excel uses 1-based indexing for rows)
        const rowString = (row + 1).toString();

        // Combine column letters and row number
        return colString + rowString;
    }

    /**
     * Checks if a given string is a valid Excel-style cell address.
     * @param address The string to check
     * @returns True if valid, false otherwise
     */
    public static isValidAddress(address: string): boolean {
        // Define a regular expression pattern for valid Excel-style addresses
        const pattern = /^[A-Z]+[1-9]\d*$/;

        // Test the input address against the pattern
        return pattern.test(address);
    }
}

// Human tasks:
// TODO: Implement error handling for invalid inputs in toIndices and toAddress methods
// TODO: Add support for absolute cell references (e.g., $A$1)
// TODO: Optimize performance for large-scale conversions
// TODO: Add unit tests to cover edge cases and ensure accuracy