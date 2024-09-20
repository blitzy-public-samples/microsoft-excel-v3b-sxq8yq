using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using ExcelWebApp.Services;
using ExcelWebApp.Models;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;

namespace ExcelWebApp.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class CellController : ControllerBase
    {
        private readonly ICellService _cellService;
        private readonly ILogger<CellController> _logger;

        public CellController(ICellService cellService, ILogger<CellController> logger)
        {
            _cellService = cellService;
            _logger = logger;
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<Cell>> GetCell(Guid id)
        {
            // Log the GetCell request
            _logger.LogInformation($"GetCell request received for id: {id}");

            // Call _cellService.GetCellAsync with the provided id
            var cell = await _cellService.GetCellAsync(id);

            // If cell is null, return NotFound
            if (cell == null)
            {
                return NotFound();
            }

            // Otherwise, return the cell
            return cell;
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateCell(Guid id, CellUpdateRequest updateRequest)
        {
            // Log the UpdateCell request
            _logger.LogInformation($"UpdateCell request received for id: {id}");

            // Validate the updateRequest
            if (!ModelState.IsValid)
            {
                return BadRequest(ModelState);
            }

            try
            {
                // Call _cellService.UpdateCellAsync with id and updateRequest
                var result = await _cellService.UpdateCellAsync(id, updateRequest);

                // If update is successful, return NoContent
                if (result)
                {
                    return NoContent();
                }
                // If cell is not found, return NotFound
                else
                {
                    return NotFound();
                }
            }
            catch (ArgumentException ex)
            {
                // If update fails due to invalid data, return BadRequest
                return BadRequest(ex.Message);
            }
        }

        [HttpGet("range")]
        public async Task<ActionResult<IEnumerable<Cell>>> GetCellsInRange(string worksheetId, string startCell, string endCell)
        {
            // Log the GetCellsInRange request
            _logger.LogInformation($"GetCellsInRange request received for worksheet: {worksheetId}, range: {startCell} to {endCell}");

            // Validate the range parameters
            if (string.IsNullOrEmpty(worksheetId) || string.IsNullOrEmpty(startCell) || string.IsNullOrEmpty(endCell))
            {
                return BadRequest("Invalid range parameters");
            }

            // Call _cellService.GetCellsInRangeAsync with worksheetId, startCell, and endCell
            var cells = await _cellService.GetCellsInRangeAsync(worksheetId, startCell, endCell);

            // Return the cells within the range
            return Ok(cells);
        }
    }
}

// Human tasks:
// TODO: Implement proper validation for CellUpdateRequest
// TODO: Add error handling for formula parsing errors
// TODO: Implement pagination for large ranges
// TODO: Add support for named ranges