import assert from 'assert';
import { FormulaParser } from '../../../src/shared/utils/FormulaParser';

describe('FormulaParser', () => {
  it('should parse basic arithmetic formulas correctly', () => {
    // Create a new instance of FormulaParser
    const parser = new FormulaParser();

    // Test parsing of basic arithmetic formulas
    const result1 = parser.parse('=1+2');
    const result2 = parser.parse('=10-5');
    const result3 = parser.parse('=3*4');
    const result4 = parser.parse('=20/5');

    // Assert that the parsed result matches the expected output
    assert.strictEqual(result1, 3);
    assert.strictEqual(result2, 5);
    assert.strictEqual(result3, 12);
    assert.strictEqual(result4, 4);
  });

  it('should parse complex nested formulas correctly', () => {
    // Create a new instance of FormulaParser
    const parser = new FormulaParser();

    // Test parsing of complex nested formulas
    const result1 = parser.parse('=(1+2)*(3+4)');
    const result2 = parser.parse('=((10-5)*2)/3');

    // Assert that the parsed result matches the expected output
    assert.strictEqual(result1, 21);
    assert.strictEqual(result2, 3.33);
  });

  it('should handle cell references correctly', () => {
    // Create a new instance of FormulaParser
    const parser = new FormulaParser();

    // Test parsing of formulas with cell references
    const result1 = parser.parse('=A1+B2', { A1: 10, B2: 5 });
    const result2 = parser.parse('=C3*D4', { C3: 3, D4: 4 });

    // Assert that the parsed result correctly identifies and handles cell references
    assert.strictEqual(result1, 15);
    assert.strictEqual(result2, 12);
  });

  it('should parse formulas with Excel functions correctly', () => {
    // Create a new instance of FormulaParser
    const parser = new FormulaParser();

    // Test parsing of formulas containing Excel functions (e.g., SUM, AVERAGE, IF)
    const result1 = parser.parse('=SUM(1,2,3,4,5)');
    const result2 = parser.parse('=AVERAGE(10,20,30)');
    const result3 = parser.parse('=IF(A1>10,TRUE,FALSE)', { A1: 15 });

    // Assert that the parsed result correctly identifies and handles Excel functions
    assert.strictEqual(result1, 15);
    assert.strictEqual(result2, 20);
    assert.strictEqual(result3, true);
  });

  it('should handle error cases gracefully', () => {
    // Create a new instance of FormulaParser
    const parser = new FormulaParser();

    // Test parsing of invalid or malformed formulas
    assert.throws(() => parser.parse('=1+'), Error);
    assert.throws(() => parser.parse('=SUM(1,)'), Error);
    assert.throws(() => parser.parse('=INVALID_FUNCTION()'), Error);

    // Assert that the parser throws appropriate errors or returns expected error values
    assert.strictEqual(parser.parse('=1/0'), '#DIV/0!');
    assert.strictEqual(parser.parse('=A1', {}), '#REF!');
  });
});