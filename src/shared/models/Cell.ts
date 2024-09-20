import { Formula } from './Formula';
import { Style } from './Style';
import { CellAddress } from '../utils/CellAddressConverter';
import { Observable, BehaviorSubject } from 'rxjs';

// Interface representing the core data of a cell
interface ICellData {
    value: string;
    formula: Formula | null;
    style: Style | null;
    isLocked: boolean;
}

// Represents a single cell in an Excel worksheet
export class Cell {
    public readonly address: CellAddress;
    public readonly valueChanged$: Observable<string>;
    public readonly formulaChanged$: Observable<Formula | null>;
    public readonly styleChanged$: Observable<Style | null>;

    private valueSubject: BehaviorSubject<string>;
    private formulaSubject: BehaviorSubject<Formula | null>;
    private styleSubject: BehaviorSubject<Style | null>;
    private isLockedSubject: BehaviorSubject<boolean>;

    constructor(address: CellAddress, initialData: ICellData) {
        // Initialize address property
        this.address = address;

        // Create BehaviorSubjects for value, formula, and style
        this.valueSubject = new BehaviorSubject<string>(initialData.value);
        this.formulaSubject = new BehaviorSubject<Formula | null>(initialData.formula);
        this.styleSubject = new BehaviorSubject<Style | null>(initialData.style);
        this.isLockedSubject = new BehaviorSubject<boolean>(initialData.isLocked);

        // Initialize observables from BehaviorSubjects
        this.valueChanged$ = this.valueSubject.asObservable();
        this.formulaChanged$ = this.formulaSubject.asObservable();
        this.styleChanged$ = this.styleSubject.asObservable();

        // Set initial values from initialData
        this.setValue(initialData.value);
        this.setFormula(initialData.formula);
        this.setStyle(initialData.style);
    }

    // Gets the current value of the cell
    public getValue(): string {
        // Return the current value from the BehaviorSubject
        return this.valueSubject.getValue();
    }

    // Sets the value of the cell
    public setValue(newValue: string): void {
        // Update the value BehaviorSubject with the new value
        this.valueSubject.next(newValue);
        // Clear the formula if a new value is set
        this.setFormula(null);
    }

    // Gets the current formula of the cell
    public getFormula(): Formula | null {
        // Return the current formula from the BehaviorSubject
        return this.formulaSubject.getValue();
    }

    // Sets the formula for the cell
    public setFormula(newFormula: Formula | null): void {
        // Update the formula BehaviorSubject with the new formula
        this.formulaSubject.next(newFormula);
    }

    // Gets the current style of the cell
    public getStyle(): Style | null {
        // Return the current style from the BehaviorSubject
        return this.styleSubject.getValue();
    }

    // Sets the style for the cell
    public setStyle(newStyle: Style | null): void {
        // Update the style BehaviorSubject with the new style
        this.styleSubject.next(newStyle);
    }

    // Locks the cell to prevent editing
    public lock(): void {
        // Set the isLocked property to true
        this.isLockedSubject.next(true);
    }

    // Unlocks the cell to allow editing
    public unlock(): void {
        // Set the isLocked property to false
        this.isLockedSubject.next(false);
    }

    // Checks if the cell is locked
    public isLocked(): boolean {
        // Return the current value of the isLocked property
        return this.isLockedSubject.getValue();
    }

    // Converts the cell to a JSON representation
    public toJSON(): ICellData {
        // Create an object with current value, formula, style, and lock status
        return {
            value: this.getValue(),
            formula: this.getFormula(),
            style: this.getStyle(),
            isLocked: this.isLocked()
        };
    }
}

// Human tasks:
// TODO: Implement data validation logic for cell values
// TODO: Add support for cell comments
// TODO: Implement cell dependency tracking for formula calculations
// TODO: Add support for conditional formatting
// TODO: Implement undo/redo functionality for cell changes