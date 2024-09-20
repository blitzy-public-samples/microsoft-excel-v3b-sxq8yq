using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Authorization;
using ExcelWebApp.Services;
using ExcelWebApp.Models;
using System.Threading.Tasks;
using System.Collections.Generic;
using System;

namespace ExcelWebApp.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class WorkbookController : ControllerBase
    {
        private readonly WorkbookService _workbookService;

        public WorkbookController(WorkbookService workbookService)
        {
            _workbookService = workbookService;
        }

        [HttpGet]
        [Authorize]
        public async Task<ActionResult<IEnumerable<Workbook>>> GetWorkbooks()
        {
            // Retrieve all workbooks for the authenticated user
            var workbooks = await _workbookService.GetWorkbooksAsync();
            return Ok(workbooks);
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<Workbook>> GetWorkbook(Guid id)
        {
            // Retrieve a specific workbook by ID
            var workbook = await _workbookService.GetWorkbookAsync(id);
            
            if (workbook == null)
            {
                return NotFound();
            }

            return Ok(workbook);
        }

        [HttpPost]
        public async Task<ActionResult<Workbook>> CreateWorkbook(Workbook workbook)
        {
            // Create a new workbook
            var createdWorkbook = await _workbookService.CreateWorkbookAsync(workbook);
            return CreatedAtAction(nameof(GetWorkbook), new { id = createdWorkbook.Id }, createdWorkbook);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateWorkbook(Guid id, Workbook workbook)
        {
            // Update an existing workbook
            if (id != workbook.Id)
            {
                return BadRequest();
            }

            var result = await _workbookService.UpdateWorkbookAsync(workbook);
            
            if (!result)
            {
                return NotFound();
            }

            return NoContent();
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteWorkbook(Guid id)
        {
            // Delete a workbook
            var result = await _workbookService.DeleteWorkbookAsync(id);
            
            if (!result)
            {
                return NotFound();
            }

            return NoContent();
        }
    }
}