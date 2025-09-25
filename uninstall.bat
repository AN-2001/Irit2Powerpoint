@echo off
setlocal enabledelayedexpansion

REM === CONFIGURATION ===
set "CONFIG=Release"
if /I "%1"=="Debug" set "CONFIG=Debug"

echo Using configuration: %CONFIG%

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "DLL_PATH=%SCRIPT_DIR%\bin\%CONFIG%\Irit2Powerpoint.dll"
set "ADDIN_NAME=Irit2Powerpoint"
set "REGASM_PATH=%SystemRoot%\Microsoft.NET\Framework64\v4.0.30319\RegAsm.exe"

REM === UNREGISTER THE DLL ===
echo Unregistering the DLL with RegAsm...
"%REGASM_PATH%" "%DLL_PATH%" /unregister

if errorlevel 1 (
    echo Warning: Failed to unregister the DLL. It may not have been registered.
)

REM === DELETE REGISTRY KEYS ===
echo Removing registry keys for the VSTO Add-in...

REG DELETE "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /f

echo Done.
pause
endlocal
