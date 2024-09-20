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
    [Authorize]
    public class ChartController : ControllerBase
    {
        private readonly IChartService _chartService;

        public ChartController(IChartService chartService)
        {
            _chartService = chartService;
        }

        [HttpGet("{workbookId}/{worksheetId}")]
        public async Task<ActionResult<List<Chart>>> GetCharts(Guid workbookId, Guid worksheetId)
        {
            try
            {
                // Retrieve charts for the specified workbook and worksheet
                var charts = await _chartService.GetChartsAsync(workbookId, worksheetId);
                return Ok(charts);
            }
            catch (Exception ex)
            {
                // Return a 500 Internal Server Error if an exception occurs
                return StatusCode(500, $"An error occurred while retrieving charts: {ex.Message}");
            }
        }

        [HttpPost("{workbookId}/{worksheetId}")]
        public async Task<ActionResult<Chart>> CreateChart(Guid workbookId, Guid worksheetId, Chart chart)
        {
            try
            {
                // Create a new chart in the specified workbook and worksheet
                var createdChart = await _chartService.CreateChartAsync(workbookId, worksheetId, chart);
                return CreatedAtAction(nameof(GetCharts), new { workbookId, worksheetId }, createdChart);
            }
            catch (Exception ex)
            {
                // Return a 500 Internal Server Error if an exception occurs
                return StatusCode(500, $"An error occurred while creating the chart: {ex.Message}");
            }
        }

        [HttpPut("{workbookId}/{worksheetId}/{chartId}")]
        public async Task<ActionResult<Chart>> UpdateChart(Guid workbookId, Guid worksheetId, Guid chartId, Chart chart)
        {
            try
            {
                // Update the specified chart
                var updatedChart = await _chartService.UpdateChartAsync(workbookId, worksheetId, chartId, chart);
                if (updatedChart == null)
                {
                    return NotFound();
                }
                return Ok(updatedChart);
            }
            catch (Exception ex)
            {
                // Return a 500 Internal Server Error if an exception occurs
                return StatusCode(500, $"An error occurred while updating the chart: {ex.Message}");
            }
        }

        [HttpDelete("{workbookId}/{worksheetId}/{chartId}")]
        public async Task<ActionResult> DeleteChart(Guid workbookId, Guid worksheetId, Guid chartId)
        {
            try
            {
                // Delete the specified chart
                var result = await _chartService.DeleteChartAsync(workbookId, worksheetId, chartId);
                if (!result)
                {
                    return NotFound();
                }
                return NoContent();
            }
            catch (Exception ex)
            {
                // Return a 500 Internal Server Error if an exception occurs
                return StatusCode(500, $"An error occurred while deleting the chart: {ex.Message}");
            }
        }
    }
}