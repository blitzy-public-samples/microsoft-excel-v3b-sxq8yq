#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/core/CellManager.h"
#include "../../src/core/Cell.h"
#include "../../src/shared/models/CellAddress.h"

using namespace testing;

class CellManagerTest : public ::testing::Test {
protected:
    std::unique_ptr<CellManager> cellManager;

    CellManagerTest() {
        // Initialize cellManager with a new CellManager instance
        cellManager = std::make_unique<CellManager>();
    }

    void SetUp() override {
        // Reset cellManager to a new CellManager instance before each test
        cellManager = std::make_unique<CellManager>();
    }

    void TearDown() override {
        // Clean up any resources if necessary
    }
};

TEST_F(CellManagerTest, CreateCell) {
    // Create a new cell at address A1
    CellAddress address("A1");
    Cell* cell = cellManager->createCell(address);

    // Verify that the cell exists
    EXPECT_TRUE(cell != nullptr);

    // Verify that the cell has the correct address
    EXPECT_EQ(cell->getAddress(), address);
}

TEST_F(CellManagerTest, GetCell) {
    // Create a new cell at address B2
    CellAddress address("B2");
    cellManager->createCell(address);

    // Get the cell at address B2
    Cell* retrievedCell = cellManager->getCell(address);

    // Verify that the retrieved cell is not null
    EXPECT_NE(retrievedCell, nullptr);

    // Verify that the retrieved cell has the correct address
    EXPECT_EQ(retrievedCell->getAddress(), address);
}

TEST_F(CellManagerTest, UpdateCellValue) {
    // Create a new cell at address C3
    CellAddress address("C3");
    Cell* cell = cellManager->createCell(address);

    // Set the cell value to 42
    cell->setValue(42);

    // Verify that the cell value is 42
    EXPECT_EQ(cell->getValue(), 42);

    // Update the cell value to 100
    cell->setValue(100);

    // Verify that the cell value is now 100
    EXPECT_EQ(cell->getValue(), 100);
}

TEST_F(CellManagerTest, DeleteCell) {
    // Create a new cell at address D4
    CellAddress address("D4");
    cellManager->createCell(address);

    // Verify that the cell exists
    EXPECT_TRUE(cellManager->getCell(address) != nullptr);

    // Delete the cell at address D4
    cellManager->deleteCell(address);

    // Verify that the cell no longer exists
    EXPECT_FALSE(cellManager->getCell(address) != nullptr);
}

// Human tasks:
// TODO: Implement additional test cases for edge cases and error handling
// TODO: Add performance tests for large numbers of cells
// TODO: Create mock objects for dependencies to isolate CellManager tests
// TODO: Implement tests for concurrent access to CellManager