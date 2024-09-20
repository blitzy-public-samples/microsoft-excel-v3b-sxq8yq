import Cocoa
import Foundation

class FormulaBar: NSView {
    // MARK: - Properties
    private var formulaTextField: NSTextField!
    private var cellAddressLabel: NSTextField!
    private var currentCellAddress: String = ""
    private var currentFormula: String = ""

    // MARK: - Initialization
    override init(frame: NSRect) {
        super.init(frame: frame)
        
        setupCellAddressLabel()
        setupFormulaTextField()
        setupConstraints()
        registerForNotifications()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Setup Methods
    private func setupCellAddressLabel() {
        cellAddressLabel = NSTextField(frame: .zero)
        cellAddressLabel.font = NSFont.systemFont(ofSize: 12, weight: .medium)
        cellAddressLabel.alignment = .center
        cellAddressLabel.isEditable = false
        cellAddressLabel.isSelectable = true
        cellAddressLabel.backgroundColor = NSColor.controlBackgroundColor
        cellAddressLabel.isBordered = true
        addSubview(cellAddressLabel)
    }
    
    private func setupFormulaTextField() {
        formulaTextField = NSTextField(frame: .zero)
        formulaTextField.font = NSFont.systemFont(ofSize: 12)
        formulaTextField.placeholderString = "Enter formula"
        formulaTextField.delegate = self
        formulaTextField.isBordered = true
        addSubview(formulaTextField)
    }
    
    private func setupConstraints() {
        cellAddressLabel.translatesAutoresizingMaskIntoConstraints = false
        formulaTextField.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            cellAddressLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 8),
            cellAddressLabel.centerYAnchor.constraint(equalTo: centerYAnchor),
            cellAddressLabel.widthAnchor.constraint(equalToConstant: 60),
            
            formulaTextField.leadingAnchor.constraint(equalTo: cellAddressLabel.trailingAnchor, constant: 8),
            formulaTextField.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -8),
            formulaTextField.centerYAnchor.constraint(equalTo: centerYAnchor),
            formulaTextField.heightAnchor.constraint(equalTo: heightAnchor, multiplier: 0.8)
        ])
    }
    
    private func registerForNotifications() {
        NotificationCenter.default.addObserver(self, selector: #selector(updateFormulaBar), name: NSNotification.Name("SelectedCellChanged"), object: nil)
    }
    
    // MARK: - Update Methods
    @objc private func updateFormulaBar(_ notification: NSNotification) {
        guard let userInfo = notification.userInfo,
              let address = userInfo["cellAddress"] as? String,
              let formula = userInfo["cellFormula"] as? String else {
            return
        }
        
        cellAddressLabel.stringValue = address
        formulaTextField.stringValue = formula
        currentCellAddress = address
        currentFormula = formula
    }
    
    private func applyFormula() {
        let newFormula = formulaTextField.stringValue
        
        // TODO: Implement formula validation logic
        
        // Update WorksheetView
        NotificationCenter.default.post(name: NSNotification.Name("FormulaUpdated"), object: nil, userInfo: ["cellAddress": currentCellAddress, "newFormula": newFormula])
        
        // Update CellManager
        // Note: This is a placeholder. Actual implementation will depend on how CellManager is integrated.
        // CellManager.shared.updateFormula(for: currentCellAddress, formula: newFormula)
        
        currentFormula = newFormula
    }
}

// MARK: - NSTextFieldDelegate
extension FormulaBar: NSTextFieldDelegate {
    func controlTextDidEndEditing(_ notification: Notification) {
        applyFormula()
    }
}

// MARK: - Human Tasks
/*
TODO: Human tasks
- Implement formula validation logic in applyFormula() method
- Add error handling for invalid formulas
- Implement auto-completion for function names in the formula text field
- Add support for formula syntax highlighting
- Implement undo/redo functionality for formula edits
*/