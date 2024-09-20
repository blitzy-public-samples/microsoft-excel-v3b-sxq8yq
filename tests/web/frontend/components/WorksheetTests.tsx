import React from 'react';
import { render, fireEvent } from '@testing-library/react';
import { Worksheet, WorksheetProps } from 'src/web/frontend/src/components/Worksheet';
import { Cell } from 'src/shared/models/Cell';

// Helper function to render the Worksheet component with default props
const renderWorksheet = (props: Partial<WorksheetProps> = {}) => {
  const defaultProps: WorksheetProps = {
    rows: 10,
    columns: 10,
    cells: {},
    onCellValueChange: jest.fn(),
    onCellSelect: jest.fn(),
  };
  const mergedProps = { ...defaultProps, ...props };
  return render(<Worksheet {...mergedProps} />);
};

describe('Worksheet component', () => {
  beforeEach(() => {
    jest.clearAllMocks();
  });

  it('renders without crashing', () => {
    const { getByTestId } = renderWorksheet();
    expect(getByTestId('worksheet')).toBeInTheDocument();
  });

  it('displays correct number of rows and columns', () => {
    const { container } = renderWorksheet({ rows: 5, columns: 7 });
    const rows = container.querySelectorAll('[data-testid^="row-"]');
    const cells = container.querySelectorAll('[data-testid^="cell-"]');
    expect(rows).toHaveLength(5);
    expect(cells).toHaveLength(5 * 7);
  });

  it('updates cell value on user input', () => {
    const onCellValueChange = jest.fn();
    const { getByTestId } = renderWorksheet({ onCellValueChange });
    const cell = getByTestId('cell-0-0');
    fireEvent.change(cell, { target: { value: 'New Value' } });
    expect(cell).toHaveValue('New Value');
    expect(onCellValueChange).toHaveBeenCalledWith(0, 0, 'New Value');
  });

  it('handles cell selection', () => {
    const onCellSelect = jest.fn();
    const { getByTestId } = renderWorksheet({ onCellSelect });
    const cell = getByTestId('cell-1-2');
    fireEvent.click(cell);
    expect(cell).toHaveClass('selected');
    expect(onCellSelect).toHaveBeenCalledWith(1, 2);
  });

  it('applies correct styles to cells', () => {
    const styledCell: Cell = {
      value: 'Styled',
      style: {
        fontWeight: 'bold',
        color: 'red',
        backgroundColor: 'yellow',
      },
    };
    const { getByTestId } = renderWorksheet({
      cells: { '0-0': styledCell },
    });
    const cell = getByTestId('cell-0-0');
    expect(cell).toHaveStyle({
      fontWeight: 'bold',
      color: 'red',
      backgroundColor: 'yellow',
    });
  });

  it('handles formula evaluation', () => {
    const formulaCell: Cell = {
      value: '=SUM(A1:A3)',
      formula: '=SUM(A1:A3)',
    };
    const { getByTestId } = renderWorksheet({
      cells: { '0-0': formulaCell },
    });
    const cell = getByTestId('cell-0-0');
    // Assuming the formula evaluation is handled by the component
    expect(cell).toHaveTextContent('=SUM(A1:A3)');
    // You might need to mock the formula evaluation service and check for the evaluated result
  });
});

// Human tasks:
// TODO: Implement additional test cases for complex interactions like drag-and-drop cell selection
// TODO: Add performance tests for rendering large worksheets
// TODO: Create tests for accessibility features of the Worksheet component
// TODO: Implement tests for collaborative editing scenarios