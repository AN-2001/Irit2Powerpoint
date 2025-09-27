## Irit2Powerpoint

3D renderer functionality for powerpoint.

Leverages the Win32 API to create windows and openTK to create openGL contexts over them.

# COMPILATION

The project is built and distributed with Visual Studio 2019 (Make sure it's up to date)

The latest build was compiled under MSVC V142
Dependancies are:

* .NET framework **4.8** SDK and targeting pack.
* Visual Studio Tools for Office (VSTO).

To compile, open the top level solution and compile all the projects, compilation may take a few minutes.

After Compilation the /bin directory will be created in the root of the project, this directory contains all the
required DLLs and manifests to register the plugin.
Installation can proceed manually, but for comfort we provide the following two scripts:


Install.bat:
Once compiled, the plugin may be installed my running the "install.bat" script with admin privelages.
the script registers the DLL and create a Windows Registry entry for the plugin so that powerpoint can load it.
the script can take arguments, either "Debug" or "Release" (case sensitive), this determines which version of the plugin
gets installed.

uninstall.bat:
to uninstall the plugin simply run uninstall.bat with either "Release" or "Debug", this will deregister the DLL and 
remove the Windows Registry entry for the plugin.


# AFTER COMPILATION
navigate to the docs directory, it provides documentation on how to use the plugin. An example powerpoint presentation that demonstrates the plugin is also provided.

**By Abed Naa'ran and Ismaeel Bder**

