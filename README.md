## Irit2Powerpoint

**3D Renderer Plugin for Microsoft PowerPoint**

Irit2Powerpoint integrates a 3D rendering engine directly into PowerPoint.  
It leverages the **Win32 API** to create windows and **OpenTK** to build OpenGL rendering contexts on top of them.

---

### Compilation

The project is developed and distributed using **Visual Studio 2019** (ensure it’s fully updated).

**Compiler & Dependencies**
- **MSVC v142**
- **.NET Framework 4.8 SDK** and **Targeting Pack**
- **Visual Studio Tools for Office (VSTO)**
- PowerPoint 64 bits (Can be checked in PowerPoint settings by clicking on File -> Account -> About PowerPoint, look for 64 bits)

**To compile:**
1. Open `Irit2Powerpoint.sln` in Visual Studio.
2. Right click the solution in the solution explorer (usually on the right side of the screen).
3. Select Batch Build
4. Select all rows and click on Rebuild.

After compilation, a `/bin` directory will be created at the root of the project — this contains all the required DLLs and manifests for plugin registration.

Note 1: x86 builds are NOT supported, this project does not work with PowerPoint 32 bits, I have attempted to compile for 32 bits but it keeps crashing for no obvious reason.

Note 2: When installing PowerPoint 64 bits, PowerPoint 32 bits needs to be completely deleted off the computer...

---

### Installation

We provide two convenience scripts for installation and removal:

#### **Install.bat**
Runs the plugin installer.  

To run the install script you can run one of the following in the command line:

// defaults to Release
install.bat 

// Explicitly set Release
install.bat Release

// Explicitly set Debug
install.bat Debug

This script modifies the registry to register the plugin so that PowerPoint loads it on the next startup.

> The installer will only install the plugin for the current user, therefore admin privilages aren't required.
> If the plugin was compiled on one computer and installed on another, PowerPoint may warn that it’s a “foreign plugin.”  
> You can safely choose to proceed — it will install normally.

If installation fails (e.g., the plugin doesn't appear in Powerpoint), please refer to the **Possible Errors** section below.  
If those steps don’t resolve the issue, compile the plugin directly on the target machine.

#### **Uninstall.bat**
To run the uninstall script you can run one of the following in the command line:

// defaults to Release
uninstall.bat 

// Explicitly set Release
uninstall.bat Release

// Explicitly set Debug
uninstall.bat Debug

This will deregister the plugin from PowerPoint.


Note: Compilation and installation has been tested on 3 seperate machines, compiling on one machine and installing on another have also been tested.

---

### Possible Errors

#### **Unsigned certificate in manifests**
If you see an error related to an unsigned or missing certificate:

1. Open the solution in Visual Studio.  
2. Right-click the **Irit2Powerpoint** C# project → **Properties**.  
3. Go to the **Signing** tab.  
4. Click **Create Test Certificate...**  
5. Leave all fields empty and click **OK**.  
6. Rebuild the project.

#### **Blocked files (Mark of the Web)**
Windows may block DLLs or EXEs copied from another computer.  
The installation script attempts to automatically remove the **Mark of the Web**, but if it fails:

- Right-click the files under the `/bin` directory.
- Select **Properties** → Check **Unblock** → **Apply**.

---

### After Compilation

Navigate to the `/docs` directory for full documentation and usage examples.  
An example PowerPoint presentation demonstrating the plugin’s capabilities is also included.

---

**Authors:**  
Abed Naa'ran  
Ismaeel Bder
