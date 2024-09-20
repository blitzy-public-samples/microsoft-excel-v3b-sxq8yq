const { Builder } = require('selenium-webdriver');
const assert = require('assert');
const { TestHelpers } = require('../shared/utils/TestHelpers');
const { TEST_DATA } = require('../shared/constants/TestData');

const BASE_URL = 'https://excel.office.com';
const TIMEOUT = 30000;

// Sets up the WebDriver for browser automation
async function setupDriver(browserType) {
    const builder = new Builder();
    switch (browserType.toLowerCase()) {
        case 'chrome':
            return builder.forBrowser('chrome').build();
        case 'firefox':
            return builder.forBrowser('firefox').build();
        // Add more browser options as needed
        default:
            throw new Error(`Unsupported browser type: ${browserType}`);
    }
}

// Logs in to Microsoft Excel online
async function login(driver, username, password) {
    await driver.get(`${BASE_URL}/login`);
    await driver.findElement({ id: 'username' }).sendKeys(username);
    await driver.findElement({ id: 'password' }).sendKeys(password);
    await driver.findElement({ id: 'submitButton' }).click();
    await driver.wait(until.elementLocated({ id: 'dashboard' }), TIMEOUT);
}

// Creates a new workbook in Excel online
async function createNewWorkbook(driver) {
    await driver.findElement({ id: 'newWorkbookButton' }).click();
    await driver.wait(until.elementLocated({ id: 'workbookContainer' }), TIMEOUT);
    const workbookTitle = await driver.getTitle();
    assert(workbookTitle.includes('New Workbook'), 'New workbook was not created');
}

// Enters data into a specific cell
async function enterDataInCell(driver, cellAddress, value) {
    const cell = await driver.findElement({ css: `[data-cell-address="${cellAddress}"]` });
    await cell.click();
    await cell.sendKeys(value, Key.ENTER);
    const cellValue = await cell.getAttribute('value');
    assert.strictEqual(cellValue, value, `Value in cell ${cellAddress} does not match entered value`);
}

// Tests a basic calculation in Excel
async function testBasicCalculation(driver) {
    await enterDataInCell(driver, 'A1', '10');
    await enterDataInCell(driver, 'A2', '20');
    await enterDataInCell(driver, 'A3', '=SUM(A1:A2)');
    
    const resultCell = await driver.findElement({ css: '[data-cell-address="A3"]' });
    const result = await resultCell.getAttribute('value');
    assert.strictEqual(result, '30', 'Basic calculation result is incorrect');
}

// Tests creating a basic chart in Excel
async function testChartCreation(driver) {
    // Enter test data for chart
    await enterDataInCell(driver, 'A1', 'Category');
    await enterDataInCell(driver, 'B1', 'Value');
    await enterDataInCell(driver, 'A2', 'Item 1');
    await enterDataInCell(driver, 'B2', '10');
    await enterDataInCell(driver, 'A3', 'Item 2');
    await enterDataInCell(driver, 'B3', '20');

    // Select data range
    await driver.actions().keyDown(Key.CONTROL).sendKeys('a').keyUp(Key.CONTROL).perform();

    // Open chart creation dialog
    await driver.findElement({ id: 'insertChartButton' }).click();

    // Select chart type (e.g., column chart)
    await driver.findElement({ css: '[data-chart-type="column"]' }).click();

    // Create chart
    await driver.findElement({ id: 'createChartButton' }).click();

    // Verify chart is created and visible
    await driver.wait(until.elementLocated({ css: '.chart-container' }), TIMEOUT);
    const chartElement = await driver.findElement({ css: '.chart-container' });
    assert(await chartElement.isDisplayed(), 'Chart is not visible after creation');
}

// Runs all end-to-end tests
async function runEndToEndTests() {
    let driver;
    try {
        driver = await setupDriver('chrome');
        await login(driver, TEST_DATA.username, TEST_DATA.password);
        await createNewWorkbook(driver);
        await testBasicCalculation(driver);
        await testChartCreation(driver);
    } catch (error) {
        console.error('End-to-end test failed:', error);
        throw error;
    } finally {
        if (driver) {
            await driver.quit();
        }
    }
}

module.exports = {
    runEndToEndTests
};

// Human tasks:
// TODO: Implement additional test scenarios for complex formulas, pivot tables, and data analysis features
// TODO: Add tests for collaborative editing features
// TODO: Implement cross-browser testing logic
// TODO: Add error handling and retry mechanisms for flaky tests
// TODO: Integrate with CI/CD pipeline for automated test execution