@echo off
setlocal enabledelayedexpansion

:: Title
echo =============================
echo Building the Project
echo =============================

:: Set build directory
set BUILD_DIR=build

:: Check if build directory exists
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

:: Run CMake configuration
echo Running CMake configuration...
cmake .. || goto :error

:: Build the project
echo Building the project...
cmake --build . || goto :error

:: Success message
echo =============================
echo Build completed successfully!
echo =============================
goto :end

:: Error handler
:error
echo.
echo =============================
echo An error occurred during the build.
echo =============================
pause
exit /b 1

:end
pause
