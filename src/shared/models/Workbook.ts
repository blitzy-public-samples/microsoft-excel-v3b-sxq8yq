import { Worksheet } from './Worksheet';
import { NamedRange } from './NamedRange';
import { Style } from './Style';
import { User } from './User';
import { v4 as uuidv4 } from 'uuid';
import { Observable, Subject } from 'rxjs';

// Define the WorkbookChangeEvent type
type WorkbookChangeEvent = {
  type: 'add' | 'remove' | 'update';
  target: 'worksheet' | 'namedRange' | 'style' | 'user' | 'workbook';
  data: any;
};

export class Workbook {
  id: string;
  name: string;
  worksheets: Worksheet[];
  namedRanges: NamedRange[];
  styles: Map<string, Style>;
  owner: User;
  sharedUsers: User[];
  createdAt: Date;
  modifiedAt: Date;
  changes$: Observable<WorkbookChangeEvent>;

  private changesSubject: Subject<WorkbookChangeEvent>;

  constructor(name: string, owner: User) {
    // Generate a unique ID for the workbook
    this.id = uuidv4();

    // Set the workbook name and owner
    this.name = name;
    this.owner = owner;

    // Initialize empty arrays for worksheets and named ranges
    this.worksheets = [];
    this.namedRanges = [];

    // Create a default worksheet
    this.addWorksheet('Sheet1');

    // Initialize the styles map
    this.styles = new Map<string, Style>();

    // Set creation and modification timestamps
    this.createdAt = new Date();
    this.modifiedAt = new Date();

    // Initialize the changes Observable
    this.changesSubject = new Subject<WorkbookChangeEvent>();
    this.changes$ = this.changesSubject.asObservable();

    // Initialize shared users array
    this.sharedUsers = [];
  }

  addWorksheet(name: string): Worksheet {
    // Create a new Worksheet instance
    const newWorksheet = new Worksheet(name);

    // Add the worksheet to the worksheets array
    this.worksheets.push(newWorksheet);

    // Emit a change event
    this.changesSubject.next({
      type: 'add',
      target: 'worksheet',
      data: newWorksheet
    });

    // Return the new worksheet
    return newWorksheet;
  }

  removeWorksheet(worksheetId: string): boolean {
    // Find the worksheet by ID
    const index = this.worksheets.findIndex(ws => ws.id === worksheetId);

    if (index !== -1) {
      // Remove the worksheet from the worksheets array
      const removedWorksheet = this.worksheets.splice(index, 1)[0];

      // Emit a change event
      this.changesSubject.next({
        type: 'remove',
        target: 'worksheet',
        data: removedWorksheet
      });

      // Return true if removed
      return true;
    }

    // Return false if not found
    return false;
  }

  addNamedRange(name: string, range: string): NamedRange {
    // Create a new NamedRange instance
    const newNamedRange = new NamedRange(name, range);

    // Add the named range to the namedRanges array
    this.namedRanges.push(newNamedRange);

    // Emit a change event
    this.changesSubject.next({
      type: 'add',
      target: 'namedRange',
      data: newNamedRange
    });

    // Return the new named range
    return newNamedRange;
  }

  removeNamedRange(name: string): boolean {
    // Find the named range by name
    const index = this.namedRanges.findIndex(nr => nr.name === name);

    if (index !== -1) {
      // Remove the named range from the namedRanges array
      const removedNamedRange = this.namedRanges.splice(index, 1)[0];

      // Emit a change event
      this.changesSubject.next({
        type: 'remove',
        target: 'namedRange',
        data: removedNamedRange
      });

      // Return true if removed
      return true;
    }

    // Return false if not found
    return false;
  }

  addStyle(name: string, style: Style): void {
    // Add the style to the styles map
    this.styles.set(name, style);

    // Emit a change event
    this.changesSubject.next({
      type: 'add',
      target: 'style',
      data: { name, style }
    });
  }

  removeStyle(name: string): boolean {
    // Remove the style from the styles map
    const removed = this.styles.delete(name);

    if (removed) {
      // Emit a change event
      this.changesSubject.next({
        type: 'remove',
        target: 'style',
        data: { name }
      });
    }

    // Return true if removed, false if not found
    return removed;
  }

  async save(): Promise<void> {
    // Update the modifiedAt timestamp
    this.modifiedAt = new Date();

    // Serialize the workbook data
    const serializedData = JSON.stringify(this);

    // Call the storage service to save the data
    // This is a placeholder and should be replaced with actual storage service call
    await new Promise(resolve => setTimeout(resolve, 100));

    // Emit a change event
    this.changesSubject.next({
      type: 'update',
      target: 'workbook',
      data: this
    });
  }

  share(user: User): void {
    // Add the user to the sharedUsers array
    this.sharedUsers.push(user);

    // Emit a change event
    this.changesSubject.next({
      type: 'add',
      target: 'user',
      data: user
    });
  }
}

// Human tasks:
// TODO: Implement error handling for edge cases (e.g., duplicate worksheet names)
// TODO: Add support for workbook-level formulas and data validation
// TODO: Implement version history tracking
// TODO: Add methods for importing and exporting different file formats
// TODO: Implement undo/redo functionality