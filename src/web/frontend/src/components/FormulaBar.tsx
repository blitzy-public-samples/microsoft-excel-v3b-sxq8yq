import React, { useState, useEffect } from 'react';
import { useSelector, useDispatch } from 'react-redux';
import { RootState } from '../store/types';
import { updateCellValue } from '../store/actions/cellActions';
import { selectActiveCell } from '../store/selectors/worksheetSelectors';

// Define the props interface for the FormulaBar component
interface FormulaBarProps {
  workbookId: string;
  worksheetId: string;
}

// Define the FormulaBar component
const FormulaBar: React.FC<FormulaBarProps> = ({ workbookId, worksheetId }) => {
  // State to hold the current formula value
  const [formulaValue, setFormulaValue] = useState<string>('');

  // Use Redux hooks to access the store
  const dispatch = useDispatch();
  const activeCell = useSelector((state: RootState) => selectActiveCell(state, workbookId, worksheetId));

  // Update formula value when active cell changes
  useEffect(() => {
    if (activeCell) {
      setFormulaValue(activeCell.formula || activeCell.value || '');
    } else {
      setFormulaValue('');
    }
  }, [activeCell]);

  // Handle changes to the formula input
  const handleFormulaChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    setFormulaValue(event.target.value);
  };

  // Handle submission of the formula
  const handleFormulaSubmit = (event: React.FormEvent<HTMLFormElement>) => {
    event.preventDefault();
    if (activeCell) {
      dispatch(updateCellValue(workbookId, worksheetId, activeCell.id, formulaValue));
    }
  };

  return (
    <form onSubmit={handleFormulaSubmit} className="formula-bar">
      <input
        type="text"
        value={formulaValue}
        onChange={handleFormulaChange}
        placeholder="Enter formula or value"
        aria-label="Formula input"
      />
      <button type="submit" aria-label="Submit formula">
        ✓
      </button>
    </form>
  );
};

export default FormulaBar;

// Human tasks:
// TODO: Implement error handling for invalid formulas
// TODO: Add autocomplete functionality for function names
// TODO: Implement formula syntax highlighting
// TODO: Add accessibility features such as keyboard navigation and screen reader support
// TODO: Optimize performance for large formulas