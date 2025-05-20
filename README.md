## Irit2Powerpoint

3D renderer functionality for powerpoint.

Leverages the Win32 API to create windows and openTK to create openGL contexts over them.

# COMPILATION

The project is built and distributed with Visual Studio 2019

The latest build was compiled under MSVC V142
Dependancies are:
* .NET framework 4.8 SDK and targeting pack
* Visual Studio Tools for Office (VSTO)
* ClickOnce Publishing
* Microsoft Visual Studio Installer Projects Extension

Both Debug and Release builds are provided and have been tested under x86/x64.
Preferred compilation is for 64 bits.

To compile, open the top level solution and compile all the projects.

An installer will be generated under Irit2PowerPointInstaller/(Debug|Release). For testing purposes, once the build succeeds the plugin will be automatically installed by Visual Studio.
You may start a debug instance by hitting run which will launch PowerPoint with the Plugin preconfigured.

# INSTRUCTIONS
After installation, please see the example directory for a presentation that introduces the plugin and showcases it.
the docs directory contains documentation for the supported import settings.

**By Abed Naa'ran and Ismaeel Bder**
