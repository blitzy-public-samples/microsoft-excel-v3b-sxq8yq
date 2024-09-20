package com.microsoft.excel.mobile.android;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.GridView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.microsoft.excel.core.WorksheetEngine;
import com.microsoft.excel.core.CellManager;
import com.microsoft.excel.mobile.android.CellAdapter;
import com.microsoft.excel.mobile.android.FormulaBarView;
import com.microsoft.excel.shared.models.Worksheet;
import com.microsoft.excel.shared.models.Cell;

public class WorksheetActivity extends AppCompatActivity {

    private Worksheet worksheet;
    private WorksheetEngine worksheetEngine;
    private CellManager cellManager;
    private GridView gridView;
    private FormulaBarView formulaBar;
    private CellAdapter cellAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_worksheet);

        // Initialize the WorksheetEngine and CellManager
        worksheetEngine = new WorksheetEngine();
        cellManager = new CellManager();

        // Load or create a new Worksheet
        worksheet = worksheetEngine.loadWorksheet(); // Assume this method exists

        // Initialize the GridView for displaying cells
        gridView = findViewById(R.id.gridView);

        // Set up the FormulaBarView
        formulaBar = findViewById(R.id.formulaBar);
        formulaBar.setOnUpdateListener(this::onFormulaBarUpdate);

        // Create and set the CellAdapter for the GridView
        cellAdapter = new CellAdapter(this, worksheet);
        gridView.setAdapter(cellAdapter);

        // Set up event listeners for user interactions
        gridView.setOnItemClickListener((parent, view, position, id) -> {
            Cell cell = cellAdapter.getItem(position);
            onCellClick(cell);
        });
    }

    private void onCellClick(Cell cell) {
        // Update the FormulaBarView with the selected cell's content
        formulaBar.setContent(cell.getContent());

        // Highlight the selected cell in the GridView
        cellAdapter.setSelectedCell(cell);
        cellAdapter.notifyDataSetChanged();
    }

    private void onFormulaBarUpdate(String newContent) {
        // Update the selected cell's content using CellManager
        Cell selectedCell = cellAdapter.getSelectedCell();
        cellManager.updateCellContent(selectedCell, newContent);

        // Recalculate affected cells
        worksheetEngine.recalculateAffectedCells(selectedCell);

        // Update the GridView to reflect changes
        cellAdapter.notifyDataSetChanged();
    }

    public void onSaveWorksheet(View view) {
        // Call WorksheetEngine to save the current worksheet
        boolean success = worksheetEngine.saveWorksheet(worksheet);

        // Show a Toast message indicating successful save
        if (success) {
            Toast.makeText(this, "Worksheet saved successfully", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "Failed to save worksheet", Toast.LENGTH_SHORT).show();
        }
    }
}

// TODO: Human tasks
// - Implement error handling for worksheet loading and saving
// - Add support for different cell types (text, number, date, etc.)
// - Implement undo/redo functionality
// - Add support for gestures (pinch-to-zoom, swipe to scroll)
// - Optimize performance for large worksheets
// - Implement data validation for cell inputs
// - Add support for offline mode and syncing