#!/bin/bash

# Get Excel version from version.txt file
EXCEL_VERSION=$(cat version.txt)

# Set deployment environment, default to production if not specified
DEPLOYMENT_ENV="${1:-production}"

# Function to check prerequisites
check_prerequisites() {
    # Check if necessary environment variables are set
    if [ -z "$AZURE_SUBSCRIPTION_ID" ] || [ -z "$AZURE_TENANT_ID" ]; then
        echo "Error: Azure environment variables not set"
        return 1
    fi

    # Verify Azure CLI is installed and configured
    if ! command -v az &> /dev/null; then
        echo "Error: Azure CLI is not installed"
        return 1
    fi

    # Ensure Docker is running
    if ! docker info &> /dev/null; then
        echo "Error: Docker is not running"
        return 1
    fi

    # Validate access to necessary Azure resources
    az account show &> /dev/null || {
        echo "Error: Not logged into Azure CLI"
        return 1
    }

    return 0
}

# Function to deploy Windows desktop version
deploy_windows_desktop() {
    echo "Deploying Windows desktop version..."
    
    # Build the Windows desktop application
    msbuild /p:Configuration=Release WindowsExcel.sln || return 1

    # Sign the executable
    signtool sign /f certificate.pfx /p password /t http://timestamp.digicert.com ExcelDesktop.exe || return 1

    # Package the application
    makepri new /pr . /cf priconfig.xml || return 1

    # Upload to Microsoft Store
    msstore publish --app ExcelDesktop.appxupload || return 1

    # Trigger gradual rollout
    msstore update --app ExcelDesktop --percentage 10

    return 0
}

# Function to deploy macOS desktop version
deploy_macos_desktop() {
    echo "Deploying macOS desktop version..."
    
    # Build the macOS desktop application
    xcodebuild -project MacExcel.xcodeproj -scheme MacExcel -configuration Release || return 1

    # Sign the application bundle
    codesign --sign "Developer ID Application: Microsoft Corporation" MacExcel.app || return 1

    # Package the application
    productbuild --component MacExcel.app /Applications MacExcel.pkg || return 1

    # Upload to Mac App Store
    xcrun altool --upload-app -f MacExcel.pkg -t osx -u apple@microsoft.com -p @keychain:AC_PASSWORD || return 1

    echo "macOS app submitted for review"

    return 0
}

# Function to deploy web app
deploy_web_app() {
    echo "Deploying web version..."
    
    # Build the frontend React application
    npm run build || return 1

    # Build the backend ASP.NET Core application
    dotnet publish -c Release || return 1

    # Package both into a Docker container
    docker build -t excelweb:$EXCEL_VERSION . || return 1

    # Push the container to Azure Container Registry
    az acr login --name msftexcelregistry
    docker push msftexcelregistry.azurecr.io/excelweb:$EXCEL_VERSION || return 1

    # Deploy to Azure App Service
    az webapp config container set --name ExcelWebApp --resource-group ExcelRG --docker-custom-image-name msftexcelregistry.azurecr.io/excelweb:$EXCEL_VERSION || return 1

    # Update Azure CDN endpoints
    az cdn endpoint update --name ExcelWebEndpoint --profile-name ExcelCDN --resource-group ExcelRG || return 1

    return 0
}

# Function to deploy mobile apps
deploy_mobile_apps() {
    echo "Deploying mobile versions..."
    
    # Build iOS application
    xcodebuild -project iOSExcel.xcodeproj -scheme iOSExcel -configuration Release || return 1

    # Sign and package iOS app
    xcodebuild -exportArchive -archivePath iOSExcel.xcarchive -exportOptionsPlist exportOptions.plist -exportPath . || return 1

    # Upload to App Store Connect
    xcrun altool --upload-app -f iOSExcel.ipa -t ios -u apple@microsoft.com -p @keychain:AC_PASSWORD || return 1

    # Build Android application
    ./gradlew assembleRelease || return 1

    # Sign and package Android app
    jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore my-release-key.keystore app-release-unsigned.apk alias_name || return 1
    zipalign -v 4 app-release-unsigned.apk AndroidExcel.apk || return 1

    # Upload to Google Play Console
    fastlane supply --track production --apk AndroidExcel.apk || return 1

    echo "Mobile apps uploaded, phased rollout triggered"

    return 0
}

# Function to update database
update_database() {
    echo "Updating database..."
    
    # Connect to Azure SQL Database
    connection_string=$(az sql db show-connection-string --server exceldbserver --name exceldb -c ado.net)

    # Check for pending migrations and apply them
    dotnet ef database update --connection "$connection_string" || return 1

    echo "Database updated successfully"

    return 0
}

# Function to configure CDN
configure_cdn() {
    echo "Configuring CDN..."
    
    # Update Azure CDN endpoints
    az cdn endpoint update --name ExcelCDNEndpoint --profile-name ExcelCDNProfile --resource-group ExcelRG || return 1

    # Purge CDN cache
    az cdn endpoint purge --content-paths "/*" --name ExcelCDNEndpoint --profile-name ExcelCDNProfile --resource-group ExcelRG || return 1

    # Configure caching rules for static assets
    az cdn endpoint rule add --name ExcelCDNEndpoint --profile-name ExcelCDNProfile --resource-group ExcelRG --order 1 --rule-name "CacheStaticAssets" --action-name "CacheExpiration" --cache-behavior "SetIfMissing" --cache-duration "00:05:00" --match-variable "RequestFilename" --operator "EndsWith" --match-values ".js" ".css" ".png" ".jpg" || return 1

    echo "CDN configured successfully"

    return 0
}

# Function to run post-deployment tests
run_post_deployment_tests() {
    echo "Running post-deployment tests..."
    
    # Run integration tests against deployed services
    npm run test:integration || return 1

    # Perform load testing on web services
    artillery run loadtest.yml || return 1

    # Check database connectivity and performance
    sqlcmd -S exceldbserver.database.windows.net -d exceldb -U admin -P "$DB_PASSWORD" -Q "SELECT TOP 1 * FROM Users" || return 1

    # Verify CDN content delivery
    curl -I https://excelcdn.azureedge.net/static/main.js || return 1

    echo "Post-deployment tests completed successfully"

    return 0
}

# Function to notify stakeholders
notify_stakeholders() {
    echo "Notifying stakeholders..."
    
    # Compile deployment report
    echo "Excel $EXCEL_VERSION deployed to $DEPLOYMENT_ENV" > deployment_report.txt

    # Send email to development team
    mail -s "Excel $EXCEL_VERSION Deployed" dev-team@microsoft.com < deployment_report.txt

    # Update status in project management tool
    curl -X POST -H "Content-Type: application/json" -d '{"status":"deployed","version":"'$EXCEL_VERSION'"}' https://api.projecttool.com/projects/excel

    # Notify customer support about the new release
    mail -s "New Excel Version Released" support@microsoft.com < deployment_report.txt

    echo "Stakeholders notified"
}

# Main function
main() {
    check_prerequisites || { echo "Prerequisites check failed"; exit 1; }
    deploy_windows_desktop || { echo "Windows desktop deployment failed"; exit 1; }
    deploy_macos_desktop || { echo "macOS desktop deployment failed"; exit 1; }
    deploy_web_app || { echo "Web app deployment failed"; exit 1; }
    deploy_mobile_apps || { echo "Mobile apps deployment failed"; exit 1; }
    update_database || { echo "Database update failed"; exit 1; }
    configure_cdn || { echo "CDN configuration failed"; exit 1; }
    run_post_deployment_tests || { echo "Post-deployment tests failed"; exit 1; }
    notify_stakeholders

    echo "Deployment completed successfully"
    return 0
}

# Run main function
main

# Human tasks:
# - Review and approve App Store submissions
# - Monitor gradual rollout and be prepared to rollback if issues arise
# - Update internal documentation with new version details
# - Prepare release notes for end-users
# - Schedule and conduct post-deployment review meeting