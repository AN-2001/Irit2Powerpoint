@echo off
setlocal enabledelayedexpansion

REM === CONFIGURATION ===
set "CONFIG=Release"
if /I "%1"=="Debug" set "CONFIG=Debug"

echo Using configuration: %CONFIG%


set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "DLL_PATH=%SCRIPT_DIR%\bin\%CONFIG%\Irit2Powerpoint.dll"
set "VSTO_MANIFEST_PATH=%SCRIPT_DIR%\bin\%CONFIG%\Irit2Powerpoint.vsto|vstolocal"
set "ADDIN_NAME=Irit2Powerpoint"
set "REGASM_PATH=%SystemRoot%\Microsoft.NET\Framework64\v4.0.30319\RegAsm.exe"

REM === REGISTER THE DLL ===
echo Registering the DLL with RegAsm...
"%REGASM_PATH%" "%DLL_PATH%" /codebase

if errorlevel 1 (
    echo Error: Failed to register the DLL.
    pause
    endlocal
    exit /b 1
)

REM === ADD REGISTRY ENTRIES ===
echo Adding registry keys for the VSTO Add-in...

REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "Description" /t REG_SZ /d "Powerpoint plugin for real time rendering." /f
REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "FriendlyName" /t REG_SZ /d "Irit2Powerpoint" /f
REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "Manifest" /t REG_SZ /d "%VSTO_MANIFEST_PATH%" /f
REG ADD "HKCU\Software\Microsoft\Office\PowerPoint\Addins\%ADDIN_NAME%" /v "LoadBehavior" /t REG_DWORD /d 3 /f

echo Done.
pause
endlocal
