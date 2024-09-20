using Microsoft.AspNetCore.Mvc;
using ExcelWebApp.Services;
using ExcelWebApp.Models;
using System.Threading.Tasks;
using System;

namespace ExcelWebApp.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class FormulaController : ControllerBase
    {
        private readonly ICalculationService _calculationService;

        public FormulaController(ICalculationService calculationService)
        {
            _calculationService = calculationService;
        }

        [HttpPost]
        [Route("calculate")]
        public async Task<ActionResult<FormulaResult>> CalculateFormula(FormulaRequest request)
        {
            try
            {
                // Validate the incoming request
                if (request == null || string.IsNullOrWhiteSpace(request.Formula))
                {
                    return BadRequest("Invalid formula request");
                }

                // Call _calculationService.CalculateFormulaAsync with the formula from the request
                var result = await _calculationService.CalculateFormulaAsync(request.Formula);

                // Return the calculation result as FormulaResult
                return Ok(new FormulaResult { Value = result });
            }
            catch (Exception ex)
            {
                // Handle any exceptions and return appropriate error responses
                return StatusCode(500, $"An error occurred while calculating the formula: {ex.Message}");
            }
        }

        [HttpGet]
        [Route("functions")]
        public async Task<ActionResult<IEnumerable<string>>> GetFunctionList()
        {
            try
            {
                // Call _calculationService.GetAvailableFunctionsAsync
                var functions = await _calculationService.GetAvailableFunctionsAsync();

                // Return the list of available functions
                return Ok(functions);
            }
            catch (Exception ex)
            {
                // Handle any exceptions and return appropriate error responses
                return StatusCode(500, $"An error occurred while retrieving the function list: {ex.Message}");
            }
        }
    }
}

// Human tasks:
// TODO: Implement more detailed error handling for specific formula errors
// TODO: Add input validation for the FormulaRequest
// TODO: Consider implementing pagination for large function lists
// TODO: Add caching mechanism for frequently requested function lists