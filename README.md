# AnaGame (Draft - to be updated soon)
#### Language: C++
#### Compiler: Visual C++
#### IDE: Visual Studio 2015 (created), 2017 (Current) - both Community edition
##### Note: Professional or above should work, so long as MFC part of your installation

## Set-up

1. Clone the Project from Github (*git clone https://github.com/TrecApps/AnaGame.git *)
2. Open the solution in Visual Studio
3. Build the TrecLib project (you'll need a debug folder)
4. For each project (other than TrecLib), Open their properties window
5. Update the "Library Directories" and "Include Directories" in "VC++ Directories"
--Make sure they point to the directories on _your_ system
6. Update the lib file list ("Linker --> Input --> Additional Dependencies")

### TrecLib

No configurtion needs to be done

### VideoGraphics

Include Directories:
* DirectXTex\DirectXTex-master\DirectXTex (3rd Party Project)
* TrecLib
 
Library Directories: 
* DirectXTex\DirectXTex-master\DirectXTex\Bin\Desktop_2015\Win32\Debug (3rd Party Project)
* Debug

Lib Files:
* TrecLib.lib
* d3d11.lib
* evr.lib
* mf.lib
* mfplay.lib
* mfreadwrite.lib
* mfuuid.lib
* mfplat.lib
* dxva2.lib
* Strmiids.lib
* D3DCompiler.lib
* DirectXTex.lib

__Note:__ Starting with Windows 8, the development kit for Windows included DirectX. However, one common
	function involved with Textures in prior DirectX Development was removed from the kit. Consequently, 
	because AnaGame was developed on Windows 8 and 10, you'll need the DirectXTex project found here:
	https://github.com/Microsoft/DirectXTex

### Anaface

Include Directories:
* TrecLib
* VideoGraphics

Library Directories:
* Debug

Lib Files:
* dwrite.lib
* d2d1.lib
* TrecLib.lib
* VideoGraphics.lib
* evr.lib
* mf.lib
* mfplay.lib
* mfreadwrite.lib
* mfuuid.lib
* mfplat.lib
* dxva2.lib
* Strmiids.lib

__Note:__ Some of these Lib files might not be needed (I know the first four ARE needed).

### Tap

Include Directories:
* TrecLib
* VideoGraphics
* Anaface

Library Directories:
* Debug

Lib Files:
* TrecLib.lib
* Anaface.lib
* VideoGraphics.lib
* d2d1.lib

### AnaGame Applications (AnaGame_Builder, AnimateBuilder, AnimateCentral, Web_Tours)

Include Directories:
* Tap
* Anaface
* VideoGraphics
* TrecLib

Library Directories:
* Debug

Lib Files:
* Tap.lib
* Anaface.lib
* VideoGraphics.lib
* TrecLib.lib
* d2d1.lib

## Pointers in AnaGame

When coding the AnaGame project, you will likely create a few objects on the heap. Unless you
	are keeping track of your objects extremely well, you wan to put them in a TrecPointer<Obj_type>.
	TrecPointers are AnaGame's way of managing objects on the heap. They allow for counters to be
	maintained and memory returned to the heap once the count reaches 0. You can also manually delete
	an object and any other TrecPointer referencing it will convert itself to NULL (as opposed to dangling.

TObject* obj = new TObject();
TrecPointer<TObject> t_obj1(obj);
TrecPointer<TObject> t_obj2(obj); // If this were a standard C++ shared_ptr, this would lead to 2 counters and eventually a dangling pointer

if(t_obj1.get())
	t_obj1->someMethod();

t_obj1.Delete(); // t_obj2 won't become null at this point, but will when it is used again

_Note:_ More Documentation should become available in the coming months

## Overall Structure

AnaGame Currently Uses 4 Extension DLL's (libraries that provide C++ classes to multiple projects).
There might soon be more in the coming months as more features are added to AnaGame. These DLL's are used
by four MFC applications in the solution.

### DLLs

AnaGame has a stacked library structure where higher up libraries are dependent on the lower ones

* TrecLib: (Lowest layer) Provides the base of AnaGame including the TObject, TString, TFile, TrecPointers, and more
* VideoGraphics: Provides the 3D Graphics engine, manages 3D shaders, and 3D model management
* Anaface: User Interface Engine that provides the TControl and subclasses that draw themselves to screen and react to user input
* Tap: (Highest library (right now)) Ties up the libraries together and provides an interface for applications to use AnaGame libraries effectively

### Applications

* AnaGame_Builder: the application intended to serve as AnaGame's IDE as well as oneday support video editing and other productivity functionality
* AnamateBuilder: Older version of _AnaGame_Builder_ that relies more on MFC and should be used for developing features in isolation
* AnimateCentral: Intended to serve as AnaGame's hub for watching videos and playing games. Currently used to develop Anaface
* Web_Tours: Serves as AnaGame's dedicated web browser, facilitating HTML/CSS support in Anaface

### Others

Currently, there are two other projects in AnaGame that were created but not under current development

* AnagameCompiler: Functional DLL that's going to be superceeded by an Extension DLL
* AnafaceSupport: Functional DLL that's intended to manage possible UI styling attributes, supporting CSS attributes as well as Anaface attributes


