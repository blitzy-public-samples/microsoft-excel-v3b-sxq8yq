using System.Threading.Tasks;
using System.Collections.Generic;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using ExcelWebApp.Models;
using ExcelWebApp.Data;
using ExcelWebApp.Interfaces;
using ExcelWebApp.Exceptions;

namespace ExcelWebApp.Services
{
    public class WorkbookService : IWorkbookService
    {
        private readonly ApplicationDbContext _context;

        public WorkbookService(ApplicationDbContext context)
        {
            // Assign the provided context to the _context field
            _context = context;
        }

        public async Task<Workbook> CreateWorkbookAsync(string name, int userId)
        {
            // Create a new Workbook object with the provided name and userId
            var workbook = new Workbook { Name = name, UserId = userId };

            // Create a default Worksheet object
            var defaultWorksheet = new Worksheet { Name = "Sheet1" };

            // Add the Worksheet to the Workbook
            workbook.Worksheets.Add(defaultWorksheet);

            // Add the Workbook to the database context
            await _context.Workbooks.AddAsync(workbook);

            // Save changes to the database
            await _context.SaveChangesAsync();

            // Return the created Workbook
            return workbook;
        }

        public async Task<Workbook> GetWorkbookAsync(int workbookId)
        {
            // Query the database for the workbook with the given ID
            // Include related worksheets in the query
            var workbook = await _context.Workbooks
                .Include(w => w.Worksheets)
                .FirstOrDefaultAsync(w => w.Id == workbookId);

            // If the workbook is not found, throw a WorkbookNotFoundException
            if (workbook == null)
            {
                throw new WorkbookNotFoundException(workbookId);
            }

            // Return the found workbook
            return workbook;
        }

        public async Task<Workbook> UpdateWorkbookAsync(Workbook workbook)
        {
            // Update the workbook in the database context
            _context.Workbooks.Update(workbook);

            // Save changes to the database
            await _context.SaveChangesAsync();

            // Return the updated workbook
            return workbook;
        }

        public async Task DeleteWorkbookAsync(int workbookId)
        {
            // Query the database for the workbook with the given ID
            var workbook = await _context.Workbooks.FindAsync(workbookId);

            // If the workbook is not found, throw a WorkbookNotFoundException
            if (workbook == null)
            {
                throw new WorkbookNotFoundException(workbookId);
            }

            // Remove the workbook from the database context
            _context.Workbooks.Remove(workbook);

            // Save changes to the database
            await _context.SaveChangesAsync();
        }

        public async Task<Worksheet> AddWorksheetAsync(int workbookId, string worksheetName)
        {
            // Query the database for the workbook with the given ID
            var workbook = await _context.Workbooks
                .Include(w => w.Worksheets)
                .FirstOrDefaultAsync(w => w.Id == workbookId);

            // If the workbook is not found, throw a WorkbookNotFoundException
            if (workbook == null)
            {
                throw new WorkbookNotFoundException(workbookId);
            }

            // Create a new Worksheet object with the provided name
            var worksheet = new Worksheet { Name = worksheetName };

            // Add the Worksheet to the Workbook
            workbook.Worksheets.Add(worksheet);

            // Save changes to the database
            await _context.SaveChangesAsync();

            // Return the created Worksheet
            return worksheet;
        }
    }
}