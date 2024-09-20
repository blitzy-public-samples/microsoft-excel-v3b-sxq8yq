using System;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Moq;
using Excel.Web.Backend.Controllers;
using Excel.Web.Backend.Services;
using Excel.Shared.Models;

[TestClass]
public class WorkbookControllerTests
{
    private Mock<IWorkbookService> _mockWorkbookService;
    private WorkbookController _controller;

    public WorkbookControllerTests()
    {
        // Initialize _mockWorkbookService
        _mockWorkbookService = new Mock<IWorkbookService>();

        // Create WorkbookController instance with mock service
        _controller = new WorkbookController(_mockWorkbookService.Object);
    }

    [TestMethod]
    [AsyncStaticMethod]
    public async Task TestGetWorkbook()
    {
        // Arrange: Set up mock workbook service to return a test workbook
        var testWorkbook = new Workbook { Id = 1, Name = "Test Workbook" };
        _mockWorkbookService.Setup(s => s.GetWorkbookAsync(It.IsAny<int>())).ReturnsAsync(testWorkbook);

        // Act: Call GetWorkbook method on the controller
        var result = await _controller.GetWorkbook(1);

        // Assert: Verify the result is OkObjectResult with correct workbook data
        Assert.IsInstanceOfType(result, typeof(OkObjectResult));
        var okResult = result as OkObjectResult;
        Assert.IsNotNull(okResult);
        Assert.AreEqual(testWorkbook, okResult.Value);
    }

    [TestMethod]
    [AsyncStaticMethod]
    public async Task TestCreateWorkbook()
    {
        // Arrange: Create a new workbook object
        var newWorkbook = new Workbook { Name = "New Workbook" };
        _mockWorkbookService.Setup(s => s.CreateWorkbookAsync(It.IsAny<Workbook>())).ReturnsAsync(newWorkbook);

        // Act: Call CreateWorkbook method on the controller
        var result = await _controller.CreateWorkbook(newWorkbook);

        // Assert: Verify the result is CreatedAtActionResult with correct workbook data
        Assert.IsInstanceOfType(result, typeof(CreatedAtActionResult));
        var createdResult = result as CreatedAtActionResult;
        Assert.IsNotNull(createdResult);
        Assert.AreEqual("GetWorkbook", createdResult.ActionName);
        Assert.AreEqual(newWorkbook, createdResult.Value);
    }

    [TestMethod]
    [AsyncStaticMethod]
    public async Task TestUpdateWorkbook()
    {
        // Arrange: Set up mock workbook service for update
        var updatedWorkbook = new Workbook { Id = 1, Name = "Updated Workbook" };
        _mockWorkbookService.Setup(s => s.UpdateWorkbookAsync(It.IsAny<int>(), It.IsAny<Workbook>())).Returns(Task.CompletedTask);

        // Act: Call UpdateWorkbook method on the controller
        var result = await _controller.UpdateWorkbook(1, updatedWorkbook);

        // Assert: Verify the result is NoContentResult
        Assert.IsInstanceOfType(result, typeof(NoContentResult));
    }

    [TestMethod]
    [AsyncStaticMethod]
    public async Task TestDeleteWorkbook()
    {
        // Arrange: Set up mock workbook service for delete
        _mockWorkbookService.Setup(s => s.DeleteWorkbookAsync(It.IsAny<int>())).Returns(Task.CompletedTask);

        // Act: Call DeleteWorkbook method on the controller
        var result = await _controller.DeleteWorkbook(1);

        // Assert: Verify the result is NoContentResult
        Assert.IsInstanceOfType(result, typeof(NoContentResult));
    }
}