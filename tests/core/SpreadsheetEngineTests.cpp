#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/core/SpreadsheetEngine.h"
#include "../../src/core/CellManager.h"
#include "../../src/core/FormulaEngine.h"
#include "../../src/shared/models/Workbook.h"
#include "../../src/shared/models/Worksheet.h"
#include "../../src/shared/models/Cell.h"

const std::string TEST_WORKBOOK_NAME = "TestWorkbook";
const std::string TEST_WORKSHEET_NAME = "TestWorksheet";

class SpreadsheetEngineTest : public ::testing::Test {
protected:
    std::unique_ptr<SpreadsheetEngine> spreadsheetEngine;
    std::shared_ptr<Workbook> testWorkbook;
    std::shared_ptr<Worksheet> testWorksheet;

    SpreadsheetEngineTest() {
        // Initialize spreadsheetEngine
        spreadsheetEngine = std::make_unique<SpreadsheetEngine>();

        // Create test workbook and worksheet
        testWorkbook = std::make_shared<Workbook>(TEST_WORKBOOK_NAME);
        testWorksheet = std::make_shared<Worksheet>(TEST_WORKSHEET_NAME);

        // Add test worksheet to test workbook
        testWorkbook->AddWorksheet(testWorksheet);
    }

    void SetUp() override {
        // Reset spreadsheetEngine
        spreadsheetEngine = std::make_unique<SpreadsheetEngine>();

        // Clear test workbook and worksheet
        testWorkbook = std::make_shared<Workbook>(TEST_WORKBOOK_NAME);
        testWorksheet = std::make_shared<Worksheet>(TEST_WORKSHEET_NAME);
        testWorkbook->AddWorksheet(testWorksheet);
    }

    void TearDown() override {
        // Release any resources if necessary
        // (No specific cleanup needed in this case)
    }
};

TEST_F(SpreadsheetEngineTest, CreateWorkbook) {
    // Call spreadsheetEngine->CreateWorkbook()
    auto createdWorkbook = spreadsheetEngine->CreateWorkbook(TEST_WORKBOOK_NAME);

    // Verify workbook is created with correct name
    ASSERT_NE(createdWorkbook, nullptr);
    EXPECT_EQ(createdWorkbook->GetName(), TEST_WORKBOOK_NAME);

    // Verify workbook contains one default worksheet
    EXPECT_EQ(createdWorkbook->GetWorksheetCount(), 1);
}

TEST_F(SpreadsheetEngineTest, AddWorksheet) {
    // Call spreadsheetEngine->AddWorksheet()
    auto initialCount = testWorkbook->GetWorksheetCount();
    auto addedWorksheet = spreadsheetEngine->AddWorksheet(testWorkbook, "NewWorksheet");

    // Verify new worksheet is added to the workbook
    ASSERT_NE(addedWorksheet, nullptr);
    EXPECT_EQ(addedWorksheet->GetName(), "NewWorksheet");

    // Verify worksheet count is increased
    EXPECT_EQ(testWorkbook->GetWorksheetCount(), initialCount + 1);
}

TEST_F(SpreadsheetEngineTest, SetGetCellValue) {
    // Set cell value using spreadsheetEngine->SetCellValue()
    std::string cellAddress = "A1";
    std::string cellValue = "Test Value";
    spreadsheetEngine->SetCellValue(testWorksheet, cellAddress, cellValue);

    // Get cell value using spreadsheetEngine->GetCellValue()
    auto retrievedValue = spreadsheetEngine->GetCellValue(testWorksheet, cellAddress);

    // Verify retrieved value matches set value
    EXPECT_EQ(retrievedValue, cellValue);
}

TEST_F(SpreadsheetEngineTest, FormulaCalculation) {
    // Set cell values for formula inputs
    spreadsheetEngine->SetCellValue(testWorksheet, "A1", "10");
    spreadsheetEngine->SetCellValue(testWorksheet, "A2", "20");

    // Set formula in a cell
    std::string formulaCell = "A3";
    std::string formula = "=SUM(A1:A2)";
    spreadsheetEngine->SetCellValue(testWorksheet, formulaCell, formula);

    // Trigger calculation using spreadsheetEngine->CalculateFormulas()
    spreadsheetEngine->CalculateFormulas(testWorksheet);

    // Verify formula result is correct
    auto result = spreadsheetEngine->GetCellValue(testWorksheet, formulaCell);
    EXPECT_EQ(result, "30");
}

TEST_F(SpreadsheetEngineTest, CellRangeOperations) {
    // Set values in a range of cells
    spreadsheetEngine->SetCellValue(testWorksheet, "B1", "10");
    spreadsheetEngine->SetCellValue(testWorksheet, "B2", "20");
    spreadsheetEngine->SetCellValue(testWorksheet, "B3", "30");

    // Perform operation on cell range (e.g., sum, average)
    std::string resultCell = "B4";
    std::string sumFormula = "=SUM(B1:B3)";
    spreadsheetEngine->SetCellValue(testWorksheet, resultCell, sumFormula);
    spreadsheetEngine->CalculateFormulas(testWorksheet);

    // Verify operation result is correct
    auto sumResult = spreadsheetEngine->GetCellValue(testWorksheet, resultCell);
    EXPECT_EQ(sumResult, "60");

    // Test average
    std::string avgCell = "B5";
    std::string avgFormula = "=AVERAGE(B1:B3)";
    spreadsheetEngine->SetCellValue(testWorksheet, avgCell, avgFormula);
    spreadsheetEngine->CalculateFormulas(testWorksheet);

    auto avgResult = spreadsheetEngine->GetCellValue(testWorksheet, avgCell);
    EXPECT_EQ(avgResult, "20");
}

TEST_F(SpreadsheetEngineTest, UndoRedoOperations) {
    // Perform a series of cell value changes
    spreadsheetEngine->SetCellValue(testWorksheet, "C1", "Value1");
    spreadsheetEngine->SetCellValue(testWorksheet, "C2", "Value2");
    spreadsheetEngine->SetCellValue(testWorksheet, "C3", "Value3");

    // Call spreadsheetEngine->Undo()
    spreadsheetEngine->Undo();

    // Verify cell values are reverted
    EXPECT_EQ(spreadsheetEngine->GetCellValue(testWorksheet, "C3"), "");
    EXPECT_EQ(spreadsheetEngine->GetCellValue(testWorksheet, "C2"), "Value2");

    // Call spreadsheetEngine->Redo()
    spreadsheetEngine->Redo();

    // Verify cell values are restored
    EXPECT_EQ(spreadsheetEngine->GetCellValue(testWorksheet, "C3"), "Value3");
}

// Human tasks:
// TODO: Implement additional test cases for edge cases and error handling
// TODO: Add performance tests for large spreadsheets
// TODO: Create mock objects for CellManager and FormulaEngine to isolate SpreadsheetEngine tests
// TODO: Implement tests for concurrent operations in a multi-threaded environment
// TODO: Add tests for saving and loading spreadsheet data