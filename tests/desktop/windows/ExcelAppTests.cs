using System;
using System.Windows;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Moq;
using ExcelApp.Windows;
using ExcelApp.Core;
using ExcelApp.Windows.UI;

namespace ExcelApp.Tests.Windows
{
    [TestClass]
    public class ExcelAppTests
    {
        private ExcelApp excelApp;
        private Mock<SpreadsheetEngine> mockSpreadsheetEngine;
        private Mock<WorkbookManager> mockWorkbookManager;
        private Mock<WorksheetEngine> mockWorksheetEngine;

        public ExcelAppTests()
        {
            // Initialize mock objects for SpreadsheetEngine, WorkbookManager, and WorksheetEngine
            mockSpreadsheetEngine = new Mock<SpreadsheetEngine>();
            mockWorkbookManager = new Mock<WorkbookManager>();
            mockWorksheetEngine = new Mock<WorksheetEngine>();

            // Create an instance of ExcelApp with mock dependencies
            excelApp = new ExcelApp(mockSpreadsheetEngine.Object, mockWorkbookManager.Object, mockWorksheetEngine.Object);
        }

        [TestInitialize]
        public void TestInitialize()
        {
            // Reset all mock objects
            mockSpreadsheetEngine.Reset();
            mockWorkbookManager.Reset();
            mockWorksheetEngine.Reset();

            // Reinitialize ExcelApp instance with fresh mock dependencies
            excelApp = new ExcelApp(mockSpreadsheetEngine.Object, mockWorkbookManager.Object, mockWorksheetEngine.Object);
        }

        [TestMethod]
        public void TestExcelAppInitialization()
        {
            // Verify that RibbonUI is properly initialized
            Assert.IsNotNull(excelApp.RibbonUI);

            // Verify that WorksheetView is properly initialized
            Assert.IsNotNull(excelApp.WorksheetView);

            // Verify that FormulaBar is properly initialized
            Assert.IsNotNull(excelApp.FormulaBar);

            // Assert that SpreadsheetEngine, WorkbookManager, and WorksheetEngine are correctly set
            Assert.AreEqual(mockSpreadsheetEngine.Object, excelApp.SpreadsheetEngine);
            Assert.AreEqual(mockWorkbookManager.Object, excelApp.WorkbookManager);
            Assert.AreEqual(mockWorksheetEngine.Object, excelApp.WorksheetEngine);
        }

        [TestMethod]
        public void TestNewWorkbookCreation()
        {
            // Call excelApp.NewWorkbook()
            excelApp.NewWorkbook();

            // Verify that WorkbookManager.CreateWorkbook() is called
            mockWorkbookManager.Verify(wm => wm.CreateWorkbook(), Times.Once);

            // Assert that a new WorksheetView is created and added to the UI
            Assert.IsTrue(excelApp.WorksheetView.Children.Count > 0);

            // Verify that the new workbook becomes the active workbook
            mockWorkbookManager.Verify(wm => wm.SetActiveWorkbook(It.IsAny<Workbook>()), Times.Once);
        }

        [TestMethod]
        public void TestOpenWorkbook()
        {
            // Mock file dialog to return a valid file path
            var mockFileDialog = new Mock<IFileDialog>();
            mockFileDialog.Setup(fd => fd.ShowDialog()).Returns(true);
            mockFileDialog.Setup(fd => fd.FileName).Returns("C:\\test\\workbook.xlsx");
            excelApp.FileDialog = mockFileDialog.Object;

            // Call excelApp.OpenWorkbook()
            excelApp.OpenWorkbook();

            // Verify that FileIO.ReadWorkbook() is called with the correct file path
            mockWorkbookManager.Verify(wm => wm.OpenWorkbook("C:\\test\\workbook.xlsx"), Times.Once);

            // Assert that WorksheetViews are created for each worksheet in the opened workbook
            Assert.IsTrue(excelApp.WorksheetView.Children.Count > 0);

            // Verify that the opened workbook becomes the active workbook
            mockWorkbookManager.Verify(wm => wm.SetActiveWorkbook(It.IsAny<Workbook>()), Times.Once);
        }

        [TestMethod]
        public void TestSaveWorkbook()
        {
            // Create a mock workbook with some data
            var mockWorkbook = new Mock<Workbook>();
            mockWorkbookManager.Setup(wm => wm.GetActiveWorkbook()).Returns(mockWorkbook.Object);

            // Mock file dialog to return a valid save path
            var mockFileDialog = new Mock<IFileDialog>();
            mockFileDialog.Setup(fd => fd.ShowDialog()).Returns(true);
            mockFileDialog.Setup(fd => fd.FileName).Returns("C:\\test\\save_workbook.xlsx");
            excelApp.FileDialog = mockFileDialog.Object;

            // Call excelApp.SaveWorkbook()
            excelApp.SaveWorkbook();

            // Verify that WorkbookManager.GetWorkbookData() is called
            mockWorkbookManager.Verify(wm => wm.GetActiveWorkbook(), Times.Once);

            // Verify that FileIO.WriteWorkbook() is called with the correct data and file path
            mockWorkbookManager.Verify(wm => wm.SaveWorkbook(mockWorkbook.Object, "C:\\test\\save_workbook.xlsx"), Times.Once);

            // Assert that the workbook's modified state is reset after saving
            mockWorkbook.Verify(wb => wb.ResetModifiedState(), Times.Once);
        }

        [TestMethod]
        public void TestFormulaCalculation()
        {
            // Create a mock worksheet with some cells containing formulas
            var mockWorksheet = new Mock<Worksheet>();
            mockWorksheet.Setup(ws => ws.GetCell("A1")).Returns(new Cell("A1", "=B1+C1"));
            mockWorksheet.Setup(ws => ws.GetCell("B1")).Returns(new Cell("B1", "5"));
            mockWorksheet.Setup(ws => ws.GetCell("C1")).Returns(new Cell("C1", "10"));

            mockWorkbookManager.Setup(wm => wm.GetActiveWorksheet()).Returns(mockWorksheet.Object);

            // Set up expectations for SpreadsheetEngine and FormulaEngine interactions
            mockSpreadsheetEngine.Setup(se => se.CalculateFormula(It.IsAny<string>(), It.IsAny<Worksheet>()))
                .Returns((string formula, Worksheet ws) => 
                {
                    if (formula == "=B1+C1") return 15;
                    return 0;
                });

            // Trigger formula calculation (e.g., by editing a cell value)
            excelApp.UpdateCell("B1", "5");

            // Verify that FormulaEngine.Calculate() is called for cells with formulas
            mockSpreadsheetEngine.Verify(se => se.CalculateFormula("=B1+C1", It.IsAny<Worksheet>()), Times.Once);

            // Assert that cell values are updated correctly after calculation
            Assert.AreEqual("15", mockWorksheet.Object.GetCell("A1").Value);

            // Verify that the UI is updated to reflect the new cell values
            // Note: This might require additional setup and mocking of the UI components
            // which is not fully represented in this test method.
        }
    }
}