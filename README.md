## Irit2Powerpoint

3D renderer functionality for powerpoint.

Leverages the Win32 API to create windows and openTK to create openGL contexts over them.

# COMPILATION

The project is built and distributed with Visual Studio 2019 (Make sure it's up to date)

The latest build was compiled under MSVC V142
Dependancies are:

* .NET framework **4.8** SDK and targeting pack.
* Visual Studio Tools for Office (VSTO).
* ClickOnce Publishing.
* Microsoft Visual Studio Installer Projects Extension:
  - Can be found at https://marketplace.visualstudio.com/items?itemName=VisualStudioClient.MicrosoftVisualStudio2017InstallerProjects
  - Click Download then follow the instructions, this will modify visual studio to include the installer project preset.

Preferred compilation is for x64 Release, debug wont create the installer.

To compile, open the top level solution and compile all the projects, compilation may take a few minutes.
Once compiled, an installer will be created in the root directory of the projet with the name I2P.exe.
Run I2P.exe then follow the instructions to register the plugin with powerpoint.
If desired, the plugin can be uninstalled from Window's control panel normally.


# AFTER COMPILATION
navigate to the docs directory, it provides documentation for the command syntax. An example powerpoint presentation that demonstrates the plugin is also provided.

**By Abed Naa'ran and Ismaeel Bder**

