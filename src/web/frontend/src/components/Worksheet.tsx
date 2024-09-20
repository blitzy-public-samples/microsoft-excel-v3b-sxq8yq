import React, { useState, useEffect, useCallback } from 'react';
import { useSelector, useDispatch } from 'react-redux';
import styled from 'styled-components';
import Cell from './Cell';
import FormulaBar from './FormulaBar';
import SheetTabs from './SheetTabs';
import useWebSocket from '../hooks/useWebSocket';
import { updateCell } from '../slices/worksheetSlice';
import { selectActiveSheet, selectCells } from '../selectors/worksheetSelectors';
import { CellAddress } from '../../shared/utils/CellAddressConverter';

// Constants for maximum rows and columns
const MAX_ROWS = 1048576;
const MAX_COLS = 16384;

// Interface for Worksheet component props
interface WorksheetProps {
  workbookId: string;
}

// Styled components for layout
const WorksheetContainer = styled.div`
  display: flex;
  flex-direction: column;
  height: 100%;
  width: 100%;
  overflow: hidden;
`;

const GridContainer = styled.div`
  flex: 1;
  overflow: auto;
`;

const Worksheet: React.FC<WorksheetProps> = ({ workbookId }) => {
  // State for visible rows and columns
  const [visibleRows, setVisibleRows] = useState({ start: 0, end: 50 });
  const [visibleCols, setVisibleCols] = useState({ start: 0, end: 20 });
  const [selectedCell, setSelectedCell] = useState<string | null>(null);

  // Redux selectors and dispatch
  const dispatch = useDispatch();
  const activeSheet = useSelector(selectActiveSheet);
  const cells = useSelector(selectCells);

  // WebSocket hook for real-time updates
  const { sendMessage } = useWebSocket();

  // Scroll handler for virtual scrolling
  const handleScroll = useCallback((event: React.UIEvent<HTMLDivElement>) => {
    const { scrollTop, scrollLeft, clientHeight, clientWidth } = event.currentTarget;
    const newVisibleRows = {
      start: Math.floor(scrollTop / 25),
      end: Math.min(Math.ceil((scrollTop + clientHeight) / 25), MAX_ROWS),
    };
    const newVisibleCols = {
      start: Math.floor(scrollLeft / 100),
      end: Math.min(Math.ceil((scrollLeft + clientWidth) / 100), MAX_COLS),
    };
    setVisibleRows(newVisibleRows);
    setVisibleCols(newVisibleCols);
  }, []);

  // Cell selection handler
  const handleCellClick = useCallback((cellAddress: string) => {
    setSelectedCell(cellAddress);
  }, []);

  // Cell edit handler
  const handleCellEdit = useCallback((cellAddress: string, newValue: string) => {
    dispatch(updateCell({ cellAddress, value: newValue }));
    sendMessage({
      type: 'CELL_UPDATE',
      payload: { workbookId, sheetId: activeSheet, cellAddress, value: newValue },
    });
  }, [dispatch, sendMessage, workbookId, activeSheet]);

  // Render grid of cells
  const renderGrid = useCallback(() => {
    const grid = [];
    for (let row = visibleRows.start; row < visibleRows.end; row++) {
      for (let col = visibleCols.start; col < visibleCols.end; col++) {
        const cellAddress = CellAddress.fromRowCol(row, col);
        grid.push(
          <Cell
            key={cellAddress}
            address={cellAddress}
            value={cells[cellAddress]?.value || ''}
            isSelected={cellAddress === selectedCell}
            onClick={handleCellClick}
            onEdit={handleCellEdit}
          />
        );
      }
    }
    return grid;
  }, [visibleRows, visibleCols, cells, selectedCell, handleCellClick, handleCellEdit]);

  return (
    <WorksheetContainer>
      <FormulaBar
        selectedCell={selectedCell}
        value={selectedCell ? cells[selectedCell]?.value || '' : ''}
        onEdit={(value) => selectedCell && handleCellEdit(selectedCell, value)}
      />
      <GridContainer onScroll={handleScroll}>
        {renderGrid()}
      </GridContainer>
      <SheetTabs workbookId={workbookId} />
    </WorksheetContainer>
  );
};

export default Worksheet;

// Human tasks:
// TODO: Implement performance optimizations for large datasets
// TODO: Add support for cell formatting and styles
// TODO: Implement undo/redo functionality
// TODO: Add support for merged cells
// TODO: Implement cell range selection
// TODO: Add support for copy/paste operations