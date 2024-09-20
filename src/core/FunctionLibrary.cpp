#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include "FunctionLibrary.h"
#include "ExcelFunction.h"
#include "CellValue.h"
#include "ErrorCodes.h"

const int MAX_FUNCTION_ARGUMENTS = 255; // Maximum number of arguments for Excel functions

FunctionLibrary* FunctionLibrary::instance_ = nullptr;

// Private constructor to enforce singleton pattern
FunctionLibrary::FunctionLibrary() {
    // Initialize the functions_ map
    functions_ = std::unordered_map<std::string, std::shared_ptr<ExcelFunction>>();
    
    // Register all built-in functions
    registerBuiltInFunctions();
}

// Returns the singleton instance of FunctionLibrary
FunctionLibrary* FunctionLibrary::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new FunctionLibrary();
    }
    return instance_;
}

// Registers a new function in the library
void FunctionLibrary::registerFunction(const std::string& name, std::shared_ptr<ExcelFunction> function) {
    functions_[name] = function;
}

// Retrieves a function from the library by name
std::shared_ptr<ExcelFunction> FunctionLibrary::getFunction(const std::string& name) {
    auto it = functions_.find(name);
    if (it != functions_.end()) {
        return it->second;
    }
    return nullptr;
}

// Executes a function by name with the given arguments
CellValue FunctionLibrary::executeFunction(const std::string& name, const std::vector<CellValue>& args) {
    // Get the function using getFunction
    auto function = getFunction(name);
    
    // If function not found, return a CellValue with a #NAME? error
    if (!function) {
        return CellValue(ErrorCodes::NAME);
    }
    
    // If number of arguments exceeds MAX_FUNCTION_ARGUMENTS, return a CellValue with a #TOO_MANY_ARGS error
    if (args.size() > MAX_FUNCTION_ARGUMENTS) {
        return CellValue(ErrorCodes::TOO_MANY_ARGS);
    }
    
    // Execute the function with the provided arguments
    return function->execute(args);
}

// Registers all built-in Excel functions
void FunctionLibrary::registerBuiltInFunctions() {
    // Register SUM function
    registerFunction("SUM", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of SUM function
        return CellValue(0.0); // Placeholder
    }));

    // Register AVERAGE function
    registerFunction("AVERAGE", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of AVERAGE function
        return CellValue(0.0); // Placeholder
    }));

    // Register COUNT function
    registerFunction("COUNT", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of COUNT function
        return CellValue(0.0); // Placeholder
    }));

    // Register MAX function
    registerFunction("MAX", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of MAX function
        return CellValue(0.0); // Placeholder
    }));

    // Register MIN function
    registerFunction("MIN", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of MIN function
        return CellValue(0.0); // Placeholder
    }));

    // Register IF function
    registerFunction("IF", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of IF function
        return CellValue(false); // Placeholder
    }));

    // Register VLOOKUP function
    registerFunction("VLOOKUP", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of VLOOKUP function
        return CellValue(""); // Placeholder
    }));

    // Register CONCATENATE function
    registerFunction("CONCATENATE", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of CONCATENATE function
        return CellValue(""); // Placeholder
    }));

    // Register LEFT function
    registerFunction("LEFT", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of LEFT function
        return CellValue(""); // Placeholder
    }));

    // Register RIGHT function
    registerFunction("RIGHT", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of RIGHT function
        return CellValue(""); // Placeholder
    }));

    // Register MID function
    registerFunction("MID", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of MID function
        return CellValue(""); // Placeholder
    }));

    // Register LEN function
    registerFunction("LEN", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of LEN function
        return CellValue(0.0); // Placeholder
    }));

    // Register ROUND function
    registerFunction("ROUND", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of ROUND function
        return CellValue(0.0); // Placeholder
    }));

    // Register NOW function
    registerFunction("NOW", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of NOW function
        return CellValue(0.0); // Placeholder
    }));

    // Register TODAY function
    registerFunction("TODAY", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of TODAY function
        return CellValue(0.0); // Placeholder
    }));

    // Register AND function
    registerFunction("AND", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of AND function
        return CellValue(false); // Placeholder
    }));

    // Register OR function
    registerFunction("OR", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of OR function
        return CellValue(false); // Placeholder
    }));

    // Register NOT function
    registerFunction("NOT", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of NOT function
        return CellValue(false); // Placeholder
    }));

    // Register IFERROR function
    registerFunction("IFERROR", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of IFERROR function
        return CellValue(""); // Placeholder
    }));

    // Register SUMIF function
    registerFunction("SUMIF", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of SUMIF function
        return CellValue(0.0); // Placeholder
    }));

    // Register COUNTIF function
    registerFunction("COUNTIF", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of COUNTIF function
        return CellValue(0.0); // Placeholder
    }));

    // Register AVERAGEIF function
    registerFunction("AVERAGEIF", std::make_shared<ExcelFunction>([](const std::vector<CellValue>& args) -> CellValue {
        // Implementation of AVERAGEIF function
        return CellValue(0.0); // Placeholder
    }));
}

// Human tasks:
// - Implement error handling for edge cases in function execution
// - Add support for user-defined functions (UDFs)
// - Optimize performance for functions with large datasets
// - Implement caching mechanism for frequently used function results
// - Add more advanced Excel functions like financial and statistical functions
// - Implement multi-threading support for parallel function execution
// - Create comprehensive unit tests for all built-in functions
// - Implement function dependency tracking for efficient recalculation
// - Add support for array formulas and dynamic arrays
// - Implement localization support for function names and error messages