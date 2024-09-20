#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/core/FormulaEngine.h"
#include "../../src/core/CellManager.h"
#include "../../src/core/FunctionLibrary.h"

const double EPSILON = 1e-6;

class FormulaEngineTest : public ::testing::Test {
protected:
    std::unique_ptr<FormulaEngine> formulaEngine;
    std::unique_ptr<CellManager> cellManager;
    std::unique_ptr<FunctionLibrary> functionLibrary;

    FormulaEngineTest() {
        // Initialize cellManager
        cellManager = std::make_unique<CellManager>();
        
        // Initialize functionLibrary
        functionLibrary = std::make_unique<FunctionLibrary>();
        
        // Initialize formulaEngine with cellManager and functionLibrary
        formulaEngine = std::make_unique<FormulaEngine>(cellManager.get(), functionLibrary.get());
    }

    void SetUp() override {
        // Reset cellManager to a clean state
        cellManager->reset();
        
        // Reset functionLibrary to default functions
        functionLibrary->resetToDefaults();
        
        // Reset formulaEngine state
        formulaEngine->reset();
    }

    void TearDown() override {
        // Release any resources acquired in SetUp
        // (In this case, smart pointers will handle cleanup automatically)
    }
};

TEST_F(FormulaEngineTest, BasicArithmetic) {
    // Set cell A1 to 10
    cellManager->setCellValue("A1", 10);

    // Set cell A2 to 5
    cellManager->setCellValue("A2", 5);

    // Test addition: =A1+A2
    EXPECT_NEAR(formulaEngine->evaluate("=A1+A2"), 15.0, EPSILON);

    // Test subtraction: =A1-A2
    EXPECT_NEAR(formulaEngine->evaluate("=A1-A2"), 5.0, EPSILON);

    // Test multiplication: =A1*A2
    EXPECT_NEAR(formulaEngine->evaluate("=A1*A2"), 50.0, EPSILON);

    // Test division: =A1/A2
    EXPECT_NEAR(formulaEngine->evaluate("=A1/A2"), 2.0, EPSILON);
}

TEST_F(FormulaEngineTest, FormulaParsingAndEvaluation) {
    // Set cell B1 to 100
    cellManager->setCellValue("B1", 100);

    // Set cell B2 to 20
    cellManager->setCellValue("B2", 20);

    // Test complex formula: =(B1+B2)*2-30/3
    EXPECT_NEAR(formulaEngine->evaluate("=(B1+B2)*2-30/3"), 230.0, EPSILON);
}

TEST_F(FormulaEngineTest, BuiltInFunctions) {
    // Test SUM function
    cellManager->setCellValue("C1", 10);
    cellManager->setCellValue("C2", 20);
    cellManager->setCellValue("C3", 30);
    EXPECT_NEAR(formulaEngine->evaluate("=SUM(C1:C3)"), 60.0, EPSILON);

    // Test AVERAGE function
    EXPECT_NEAR(formulaEngine->evaluate("=AVERAGE(C1:C3)"), 20.0, EPSILON);

    // Test MAX function
    EXPECT_NEAR(formulaEngine->evaluate("=MAX(C1:C3)"), 30.0, EPSILON);

    // Test MIN function
    EXPECT_NEAR(formulaEngine->evaluate("=MIN(C1:C3)"), 10.0, EPSILON);

    // Test COUNT function
    EXPECT_NEAR(formulaEngine->evaluate("=COUNT(C1:C3)"), 3.0, EPSILON);
}

TEST_F(FormulaEngineTest, ErrorHandling) {
    // Test division by zero
    EXPECT_EQ(formulaEngine->evaluate("=1/0"), "#DIV/0!");

    // Test invalid cell reference
    EXPECT_EQ(formulaEngine->evaluate("=A999999"), "#REF!");

    // Test circular reference
    cellManager->setCellFormula("D1", "=D2");
    cellManager->setCellFormula("D2", "=D1");
    EXPECT_EQ(formulaEngine->evaluate("=D1"), "#CIRCULAR!");

    // Test invalid function name
    EXPECT_EQ(formulaEngine->evaluate("=INVALID_FUNCTION()"), "#NAME?");
}

TEST_F(FormulaEngineTest, FormulaDependencies) {
    // Create a chain of dependent cells
    cellManager->setCellValue("E1", 10);
    cellManager->setCellFormula("E2", "=E1*2");
    cellManager->setCellFormula("E3", "=E2+5");
    cellManager->setCellFormula("E4", "=E3*3");

    // Verify initial values
    EXPECT_NEAR(formulaEngine->evaluate("=E4"), 75.0, EPSILON);

    // Update a cell at the start of the chain
    cellManager->setCellValue("E1", 20);

    // Verify all dependent cells are updated correctly
    EXPECT_NEAR(formulaEngine->evaluate("=E2"), 40.0, EPSILON);
    EXPECT_NEAR(formulaEngine->evaluate("=E3"), 45.0, EPSILON);
    EXPECT_NEAR(formulaEngine->evaluate("=E4"), 135.0, EPSILON);
}

// Human tasks:
// TODO: Implement additional tests for more complex scenarios
// TODO: Add performance tests for large formula evaluations
// TODO: Create tests for custom user-defined functions
// TODO: Implement tests for array formulas and dynamic arrays
// TODO: Add tests for internationalization and localization of formulas