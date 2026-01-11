@echo off
setlocal enabledelayedexpansion

REM === CONFIGURATION ===
set "CONFIG=Release"
set "PLATFORM=bin64"

if /I "%1"=="Debug" set "CONFIG=Debug"

echo Using configuration=%CONFIG% and platform=%PLATFORM%


set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "MANIFEST_PATH=%SCRIPT_DIR%\%PLATFORM%\%CONFIG%\Irit2Powerpoint.vsto|vstolocal"
set "ADDIN_NAME=Irit2Powerpoint"

REM === UNBLOCK THE BUILD DIRECOTRY JUST INCASE ===
powershell -Command "Get-ChildItem '%SCRIPT_DIR%\%PLATFORM%' -Recurse | Unblock-File"

REM === ADD REGISTRY ENTRIES ===
echo Adding registry keys for the VSTO Add-in...

REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "Description" /t REG_SZ /d "Powerpoint plugin for real time rendering." /f
REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "FriendlyName" /t REG_SZ /d "Irit2Powerpoint" /f
REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "Manifest" /t REG_SZ /d "%MANIFEST_PATH%" /f
REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "LoadBehavior" /t REG_DWORD /d 3 /f

echo Done.
pause
endlocal
