using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Microsoft.Office.Interop.Excel;
using ExcelCore;
using ExcelDesktop.Windows;
using ExcelDesktop.Windows.Utils;

namespace ExcelDesktop.Windows
{
    public partial class WorksheetView : UserControl
    {
        private WorksheetEngine worksheetEngine;
        private CellManager cellManager;
        private FormulaEngine formulaEngine;
        private FormulaBar formulaBar;
        private DataGrid worksheetGrid;

        public WorksheetView(WorksheetEngine engine, CellManager cellMgr, FormulaEngine formulaEng, FormulaBar formBar)
        {
            InitializeComponent();

            // Set dependencies
            worksheetEngine = engine;
            cellManager = cellMgr;
            formulaEngine = formulaEng;
            formulaBar = formBar;

            // Initialize worksheet grid
            InitializeWorksheetGrid();

            // Set up event handlers
            worksheetGrid.PreviewKeyDown += Cell_PreviewKeyDown;
            worksheetGrid.BeginningEdit += Cell_BeginEdit;
            worksheetGrid.CellEditEnding += Cell_CellEditEnding;
        }

        private void InitializeWorksheetGrid()
        {
            // Get row and column count from worksheetEngine
            int rowCount = worksheetEngine.RowCount;
            int columnCount = worksheetEngine.ColumnCount;

            // Create and add columns to worksheetGrid
            for (int i = 0; i < columnCount; i++)
            {
                worksheetGrid.Columns.Add(new DataGridTextColumn
                {
                    Header = CellAddressConverter.GetColumnName(i),
                    Binding = new System.Windows.Data.Binding($"[{i}]")
                });
            }

            // Create and add rows to worksheetGrid
            for (int i = 0; i < rowCount; i++)
            {
                worksheetGrid.Items.Add(new string[columnCount]);
            }

            // Apply initial styles to cells
            ApplyInitialCellStyles();
        }

        private void Cell_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter || e.Key == Key.Tab)
            {
                // Move to the next cell
                worksheetGrid.CommitEdit();
                worksheetGrid.MoveFocus(new TraversalRequest(FocusNavigationDirection.Next));
                e.Handled = true;
            }
            else if (e.Key == Key.F2)
            {
                // Enter edit mode for the current cell
                worksheetGrid.BeginEdit();
                e.Handled = true;
            }
            else if (e.Key == Key.Escape)
            {
                // Cancel editing and revert changes
                worksheetGrid.CancelEdit();
                e.Handled = true;
            }
        }

        private void Cell_BeginEdit(object sender, DataGridBeginningEditEventArgs e)
        {
            // Get the current cell's value
            string cellValue = cellManager.GetCellValue(e.Row.GetIndex(), e.Column.DisplayIndex);

            // Update the formula bar with the cell's value
            formulaBar.SetText(cellValue);

            // Set focus to the formula bar for editing
            formulaBar.Focus();
        }

        private void Cell_CellEditEnding(object sender, DataGridCellEditEndingEventArgs e)
        {
            // Get the edited cell's new value
            string newValue = (e.EditingElement as TextBox)?.Text ?? string.Empty;

            // Update the cell value in the CellManager
            int row = e.Row.GetIndex();
            int column = e.Column.DisplayIndex;
            cellManager.SetCellValue(row, column, newValue);

            // If the value starts with '=', process it as a formula using FormulaEngine
            if (newValue.StartsWith("="))
            {
                string result = formulaEngine.EvaluateFormula(newValue);
                cellManager.SetCellValue(row, column, result);
            }

            // Update dependent cells
            cellManager.UpdateDependentCells(row, column);

            // Refresh the worksheet view
            RefreshWorksheet();
        }

        private void UpdateCell(string cellAddress, string value)
        {
            // Convert cell address to row and column indices
            (int row, int column) = CellAddressConverter.ConvertToRowColumn(cellAddress);

            // Update cell value in the worksheetGrid
            ((string[])worksheetGrid.Items[row])[column] = value;

            // Apply formatting based on cell type (number, date, etc.)
            ApplyCellFormatting(row, column, value);

            // Update cell style (font, color, etc.) based on CellManager data
            ApplyCellStyle(row, column);
        }

        private void RefreshWorksheet()
        {
            // Iterate through all cells in the worksheetEngine
            for (int row = 0; row < worksheetEngine.RowCount; row++)
            {
                for (int col = 0; col < worksheetEngine.ColumnCount; col++)
                {
                    string cellAddress = CellAddressConverter.ConvertToCellAddress(row, col);
                    string cellValue = cellManager.GetCellValue(row, col);
                    UpdateCell(cellAddress, cellValue);
                }
            }

            // Refresh the worksheetGrid display
            worksheetGrid.Items.Refresh();
        }

        // Helper methods (to be implemented)
        private void ApplyInitialCellStyles() { /* Implementation */ }
        private void ApplyCellFormatting(int row, int column, string value) { /* Implementation */ }
        private void ApplyCellStyle(int row, int column) { /* Implementation */ }
    }
}

// Human tasks:
// TODO: Implement performance optimizations for large worksheets
// TODO: Add support for cell range selection and multi-cell editing
// TODO: Implement undo/redo functionality
// TODO: Add context menu for additional cell operations
// TODO: Implement cell formatting options (merge, split, etc.)
// TODO: Add support for inserting and deleting rows/columns
// TODO: Implement zoom functionality for the worksheet view