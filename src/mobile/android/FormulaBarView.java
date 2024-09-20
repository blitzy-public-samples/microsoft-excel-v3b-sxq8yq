package com.microsoft.excel.mobile;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.EditText;
import android.widget.LinearLayout;
import androidx.annotation.Nullable;
import com.microsoft.excel.mobile.R;
import com.microsoft.excel.shared.models.Cell;
import com.microsoft.excel.shared.utils.FormulaParser;

public class FormulaBarView extends LinearLayout implements TextWatcher {
    private EditText formulaEditText;
    private Cell currentCell;
    private FormulaParser formulaParser;

    public FormulaBarView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);

        // Inflate the formula bar layout
        LayoutInflater.from(context).inflate(R.layout.view_formula_bar, this, true);

        // Initialize formulaEditText
        formulaEditText = findViewById(R.id.formula_edit_text);

        // Set up TextWatcher for formulaEditText
        formulaEditText.addTextChangedListener(this);

        // Initialize formulaParser
        formulaParser = new FormulaParser();
    }

    public void setCurrentCell(Cell cell) {
        // Update currentCell
        this.currentCell = cell;

        // Set formulaEditText text to cell's formula or value
        formulaEditText.setText(cell.getFormula() != null ? cell.getFormula() : cell.getValue());

        // Move cursor to end of text
        formulaEditText.setSelection(formulaEditText.getText().length());
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
        // Not used, but required by TextWatcher interface
    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {
        // Not used, but required by TextWatcher interface
    }

    @Override
    public void afterTextChanged(Editable s) {
        // Check if currentCell is not null
        if (currentCell != null) {
            // Get the new text from formulaEditText
            String newText = s.toString();

            // Parse the text using formulaParser
            String parsedResult = formulaParser.parse(newText);

            // Update the currentCell with the parsed result
            if (formulaParser.isFormula(newText)) {
                currentCell.setFormula(newText);
                currentCell.setValue(parsedResult);
            } else {
                currentCell.setFormula(null);
                currentCell.setValue(newText);
            }

            // Notify listeners of the cell update
            // TODO: Implement a listener interface and notify registered listeners
        }
    }
}

// TODO: Human tasks
// 1. Implement error handling for invalid formulas
// 2. Add support for formula suggestions and auto-completion
// 3. Optimize performance for large formulas
// 4. Implement undo/redo functionality for formula edits
// 5. Add accessibility features for screen readers