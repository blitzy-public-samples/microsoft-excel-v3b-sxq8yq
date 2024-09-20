# Microsoft Excel API Documentation

## Introduction

This document outlines the API endpoints and methods available for interacting with Microsoft Excel programmatically. It covers RESTful APIs, WebSocket APIs for real-time collaboration, and authentication protocols.

## Authentication

All API requests must be authenticated using OAuth 2.0. Obtain an access token from the Microsoft identity platform before making API calls.

### Obtaining an Access Token

1. Register your application in the Azure portal
2. Implement the OAuth 2.0 authorization code flow
3. Exchange the authorization code for an access token
4. Include the access token in the Authorization header of API requests

## RESTful API Endpoints

### Workbooks

#### GET /api/workbooks
Retrieve a list of workbooks

Parameters:
- `limit` (integer): Number of workbooks to return (default: 50, max: 100)
- `offset` (integer): Number of workbooks to skip (for pagination)

#### POST /api/workbooks
Create a new workbook

Body:
- `name` (string)
- `template` (string, optional)

#### GET /api/workbooks/{id}
Retrieve a specific workbook

Parameters:
- `id` (string): Unique identifier of the workbook

#### PUT /api/workbooks/{id}
Update a workbook

Parameters:
- `id` (string): Unique identifier of the workbook

Body:
- `name` (string, optional)
- `properties` (object, optional)

#### DELETE /api/workbooks/{id}
Delete a workbook

Parameters:
- `id` (string): Unique identifier of the workbook

### Worksheets

#### GET /api/workbooks/{id}/worksheets
Retrieve worksheets in a workbook

#### POST /api/workbooks/{id}/worksheets
Add a new worksheet to a workbook

#### GET /api/workbooks/{id}/worksheets/{sheet_id}
Retrieve a specific worksheet

#### PUT /api/workbooks/{id}/worksheets/{sheet_id}
Update a worksheet

#### DELETE /api/workbooks/{id}/worksheets/{sheet_id}
Delete a worksheet

### Cells

#### GET /api/workbooks/{id}/worksheets/{sheet_id}/cells
Retrieve cells from a worksheet

#### POST /api/workbooks/{id}/worksheets/{sheet_id}/cells
Update multiple cells in a worksheet

#### GET /api/workbooks/{id}/worksheets/{sheet_id}/cells/{cell_id}
Retrieve a specific cell

#### PUT /api/workbooks/{id}/worksheets/{sheet_id}/cells/{cell_id}
Update a specific cell

### Formulas

#### POST /api/calculate
Calculate a formula or set of formulas

#### GET /api/functions
Retrieve a list of available Excel functions

### Charts

#### GET /api/workbooks/{id}/worksheets/{sheet_id}/charts
Retrieve charts in a worksheet

#### POST /api/workbooks/{id}/worksheets/{sheet_id}/charts
Create a new chart

#### GET /api/workbooks/{id}/worksheets/{sheet_id}/charts/{chart_id}
Retrieve a specific chart

#### PUT /api/workbooks/{id}/worksheets/{sheet_id}/charts/{chart_id}
Update a chart

#### DELETE /api/workbooks/{id}/worksheets/{sheet_id}/charts/{chart_id}
Delete a chart

### PivotTables

#### GET /api/workbooks/{id}/worksheets/{sheet_id}/pivottables
Retrieve pivot tables in a worksheet

#### POST /api/workbooks/{id}/worksheets/{sheet_id}/pivottables
Create a new pivot table

#### GET /api/workbooks/{id}/worksheets/{sheet_id}/pivottables/{pivot_id}
Retrieve a specific pivot table

#### PUT /api/workbooks/{id}/worksheets/{sheet_id}/pivottables/{pivot_id}
Update a pivot table

#### DELETE /api/workbooks/{id}/worksheets/{sheet_id}/pivottables/{pivot_id}
Delete a pivot table

## WebSocket API

The WebSocket API enables real-time collaboration and live updates in Excel workbooks.

Connection URL: `wss://api.excel.com/v1/workbooks/{workbook_id}/collaborate`

Authentication: Include the access token as a query parameter: `?access_token=<your_token>`

### Events

#### cell_update
Fired when a cell is updated

Payload:
- `worksheet_id` (string)
- `cell_id` (string)
- `value` (any)
- `formula` (string, optional)
- `user_id` (string)

#### range_update
Fired when a range of cells is updated

Payload:
- `worksheet_id` (string)
- `range` (string, e.g., 'A1:B10')
- `values` (array)
- `user_id` (string)

#### worksheet_add
Fired when a new worksheet is added

Payload:
- `worksheet_id` (string)
- `name` (string)
- `user_id` (string)

#### worksheet_delete
Fired when a worksheet is deleted

Payload:
- `worksheet_id` (string)
- `user_id` (string)

#### chart_update
Fired when a chart is updated

Payload:
- `worksheet_id` (string)
- `chart_id` (string)
- `properties` (object)
- `user_id` (string)

#### pivottable_update
Fired when a pivot table is updated

Payload:
- `worksheet_id` (string)
- `pivottable_id` (string)
- `properties` (object)
- `user_id` (string)

## Error Handling

All API errors follow a standard format:

```json
{
  "error": {
    "code": "string",
    "message": "string",
    "details": ["array", "optional"]
  }
}
```

### Common Error Codes

- 400: Bad Request - The request was invalid or cannot be served
- 401: Unauthorized - Authentication failed or user lacks necessary permissions
- 403: Forbidden - The request is understood, but it has been refused or access is not allowed
- 404: Not Found - The requested resource could not be found
- 429: Too Many Requests - Request limit exceeded
- 500: Internal Server Error - Something went wrong on the server

## Rate Limiting

API requests are rate-limited to ensure fair usage and system stability

### Limits

- Unauthenticated requests: 60 requests per hour
- Authenticated requests: 5000 requests per hour

### Headers

- `X-RateLimit-Limit`: The maximum number of requests you're permitted to make per hour
- `X-RateLimit-Remaining`: The number of requests remaining in the current rate limit window
- `X-RateLimit-Reset`: The time at which the current rate limit window resets in UTC epoch seconds

## Changelog

### Version 1.0 (2023-06-01)
- Initial release of the Excel API

### Version 1.1 (2023-09-15)
- Added support for pivot tables
- Improved error handling and reporting
- Increased rate limits for authenticated users

<!-- Human Tasks:
- Review and validate the API documentation for completeness and accuracy
- Ensure all endpoint descriptions and parameters are up-to-date with the latest implementation
- Add code examples for common API operations in popular programming languages
- Create a separate security considerations section detailing best practices for API usage
- Develop interactive API documentation using tools like Swagger or Postman
- Set up a system for automatically generating and updating this documentation from code annotations
-->