#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <cmath>
#include "FormulaEngine.h"
#include "CellManager.h"
#include "FunctionLibrary.h"
#include "ExcelException.h"

// Maximum allowed length for a formula
const int MAX_FORMULA_LENGTH = 1024;

FormulaEngine::FormulaEngine(std::shared_ptr<CellManager> cellManager, std::shared_ptr<FunctionLibrary> functionLibrary)
    : m_cellManager(cellManager), m_functionLibrary(functionLibrary) {
    // Clear the cached results
    m_cachedResults.clear();
}

double FormulaEngine::evaluateFormula(const std::string& formula, const std::string& cellAddress) {
    // Check if formula length exceeds MAX_FORMULA_LENGTH
    if (formula.length() > MAX_FORMULA_LENGTH) {
        throw ExcelException("Formula exceeds maximum allowed length");
    }

    // Check cache for existing result
    auto cacheIt = m_cachedResults.find(cellAddress);
    if (cacheIt != m_cachedResults.end()) {
        return cacheIt->second;
    }

    // Parse the formula
    std::vector<FormulaComponent> components = parseFormula(formula);

    // Calculate the result using parsed components
    double result = calculateResult(components, cellAddress);

    // Cache the result
    m_cachedResults[cellAddress] = result;

    return result;
}

std::vector<FormulaComponent> FormulaEngine::parseFormula(const std::string& formula) {
    // Tokenize the formula string
    std::vector<std::string> tokens = tokenizeFormula(formula);

    // Validate the tokens
    if (!validateTokens(tokens)) {
        throw ExcelException("Invalid formula syntax");
    }

    // Convert tokens to FormulaComponent objects
    std::vector<FormulaComponent> components;
    for (const auto& token : tokens) {
        components.push_back(FormulaComponent(token));
    }

    return components;
}

double FormulaEngine::calculateResult(const std::vector<FormulaComponent>& components, const std::string& cellAddress) {
    std::vector<double> resultStack;

    for (const auto& component : components) {
        if (component.type == ComponentType::OPERAND) {
            // If operand, push to stack
            if (component.isNumber) {
                resultStack.push_back(std::stod(component.value));
            } else {
                // Fetch cell value
                resultStack.push_back(m_cellManager->getCellValue(component.value));
            }
        } else if (component.type == ComponentType::OPERATOR) {
            // If operator, pop operands and perform operation
            if (resultStack.size() < 2) {
                throw ExcelException("Invalid formula: insufficient operands");
            }
            double operand2 = resultStack.back();
            resultStack.pop_back();
            double operand1 = resultStack.back();
            resultStack.pop_back();

            double opResult;
            if (component.value == "+") opResult = operand1 + operand2;
            else if (component.value == "-") opResult = operand1 - operand2;
            else if (component.value == "*") opResult = operand1 * operand2;
            else if (component.value == "/") {
                if (operand2 == 0) throw ExcelException("Division by zero");
                opResult = operand1 / operand2;
            }
            else throw ExcelException("Unknown operator");

            resultStack.push_back(opResult);
        } else if (component.type == ComponentType::FUNCTION) {
            // If function, evaluate function with arguments
            std::vector<double> args;
            for (int i = 0; i < component.argCount; ++i) {
                if (resultStack.empty()) throw ExcelException("Invalid formula: insufficient function arguments");
                args.insert(args.begin(), resultStack.back());
                resultStack.pop_back();
            }
            double funcResult = m_functionLibrary->executeFunction(component.value, args);
            resultStack.push_back(funcResult);
        }
    }

    if (resultStack.size() != 1) {
        throw ExcelException("Invalid formula: unexpected number of results");
    }

    return resultStack.back();
}

void FormulaEngine::clearCache() {
    // Clear m_cachedResults unordered_map
    m_cachedResults.clear();
}

std::vector<std::string> tokenizeFormula(const std::string& formula) {
    std::vector<std::string> tokens;
    std::string currentToken;
    bool inQuotes = false;

    for (char c : formula) {
        if (c == '"') {
            inQuotes = !inQuotes;
            currentToken += c;
        } else if (inQuotes) {
            currentToken += c;
        } else if (std::isalnum(c) || c == '.') {
            currentToken += c;
        } else if (std::isspace(c)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}

bool validateTokens(const std::vector<std::string>& tokens) {
    int parenthesesCount = 0;
    bool expectOperand = true;

    for (const auto& token : tokens) {
        if (token == "(") {
            parenthesesCount++;
            expectOperand = true;
        } else if (token == ")") {
            parenthesesCount--;
            if (parenthesesCount < 0) return false;
            expectOperand = false;
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (expectOperand) return false;
            expectOperand = true;
        } else {
            if (!expectOperand) return false;
            if (isValidCellReference(token) || isValidFunctionName(token)) {
                // Additional checks for cell references and function names
            } else {
                // Check if it's a valid number
                try {
                    std::stod(token);
                } catch (const std::invalid_argument&) {
                    return false;
                }
            }
            expectOperand = false;
        }
    }

    return parenthesesCount == 0 && !expectOperand;
}

// Human tasks:
// TODO: Implement support for array formulas
// TODO: Add error handling for division by zero and other mathematical errors
// TODO: Optimize performance for large, complex formulas
// TODO: Implement support for custom user-defined functions
// TODO: Implement circular reference detection
// TODO: Optimize cache management for large spreadsheets