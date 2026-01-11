@echo off
setlocal enabledelayedexpansion

REM === CONFIGURATION ===
set "CONFIG=Release"
set "PLATFORM=bin64"

if /I "%1"=="Debug" set "CONFIG=Debug"

echo Using configuration=%CONFIG% and platform=%PLATFORM%

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "ADDIN_NAME=Irit2Powerpoint"
REM === DELETE REGISTRY KEYS ===
echo Removing registry keys for the VSTO Add-in...

REG DELETE "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /f

echo Done.
pause
endlocal
