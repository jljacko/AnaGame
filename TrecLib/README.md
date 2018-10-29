## TrecLib - Component of AnaGame
#### Language: C++
#### Compiler: Visual C++
#### IDE: Visual Studio 2015 (created), 2017 (Current) - both Community edition
##### Note: Professional or above should work, so long as MFC part of your installation
#### Project Type: MFC Extension DLL

## Set-up

No Additional Configuration should be necessary

## Classes

##### AnaGame Hierarchy (Derives from TObject (or is TObject))
* TObject
* TString
* Parser_
* ParseReader_
* HTML_Reader
* TML_Reader_
* TFile
* TArray[Base]
* TDataArray[Base]
* TMap[Base]

##### Outside AnaGame Hierarchy (Used by AnaGame but does not derive from TObject)
* TrecPointerBase
* TrecPointer
* TrecComPointer

## Attribute Codes:
NoAtt: No Attributes - this class does not have any attributes
Gen: Generic - this class uses template so most of not all impelemntation is done in the header file
Abs: Abstract - The class has a pure vitual function and such you Need to extend the class in order to use it

### TObject
#### Attributes: NoAtt
Serves as the root class of most of AnaGame. It provides common AnaGame functions such as toString and the ability to extract variables in String form and perform AnaGame level type-checking (last feature not usable right now)

### TString
#### Attributes: NoAtt
Serves as the Dedicated String class for AnaGame. While its CString (from MFC) superclass may have attributes, TString offers no new attributes, only new functionality such as number extraction (only for signed decimal based numbers)

### Parser_
#### Attributes: NoAtt, "Abs"
Provides a base class for parsing through mark-up files by being attached to a "Reader" and having the reader call the Parser methods. This class is not a pure abstract class yet but likely will be soon.

### ParseReader_
#### Attributes: "Abs"
Provides a base class for reading mark-up files or files intended to hold data. Like the Parser_ class, this class is not properly abstract but should be soon. Currently, the class supports both MFC's CArchive and AnaGame's TFile. Treat CArchive support as DEPRECATED.

### HTML_Reader
#### Attributes:
Serves as AnaGame's dedicated HTML reader, reading through the HTML file and sending tags and attributes to an external HTML Parser (based off of Parser_)

### TML_Reader
#### Attributes:
Reads a mark-up format currently unique to (and introduced in) AnaGame. More information on TML should be available in the following months

### TFile
#### Attributes:
Extends MFC's CFile class while adding support for tha ability to read and write line by line and offers support for multiple file encodings

### TArrayBase
#### Attributes: abs
Provides a base for the TArray class while providing the ability to extract TObjects from the Array's contents (if present).

### TArray
#### Attributes: gen
Holds references to various objects. Data type NEEDS to be polymorphous as it relies on dynamic_cast in order to fulfill the TObject functionality seen in TArrayBase

### TDataArrayBase
#### Attributes: gen
Provides a base for the TDataArray class while offering the ability to extract TObjects from it's contents (if present)

### TDataArray
#### Attributes: gen
Holds raw data in a single array, but can hold pointers. If storing primitives, you CANNOT store them as pointers and TObjects are null. Otherwise, pointers need to be to polymorphous objects (dynamic_cast is used)

### TMap[Base]
#### Attributes: 
Holds objects as well as a TString key to those objects


### TrecPointerBase
#### Attributes:
Provides a base for AnaGame's mechanism of reference management, keeping a counter stored in a Table and managing the references that are returned to the specific sub-classes

### TrecPointer
#### Attributes: gen
Serves as AnaGame's dedicated smart pointer, holding the reference as well as becoming null of the reference is deleted by another TrecPointer

### TrecPointer
#### Attributes: gen
Similar to TrecPointer but used for DirectX objects as those objects are COM objects that need to be "Released" rather than deleted

## Functions

There are exported functions from TrecLib that is used by the rest of AnaGame:
* TString _TREC_LIB_DLL GetDirectory(CentralDirectories cd);
* TString _TREC_LIB_DLL GetDirectoryWithSlash(CentralDirectories cd);

The central directories is an enum that tells AnaGame which common folder to return. The major difference between the two is that latter function adds a slash at the end of it's directory.

## TrecPointer rules

1. Do NOT have an Object pointed referenced by both a TrecPointer and an std::shared_ptr.
    * While TrecPointer is prefered, if you choose to use a different smart pointer, stick with it.
    * There is NO mechanism through which the TrecPointer's counter can be synced with that of an external smart pointer
2. Do NOT hold objects that are on the Stack. The TrecPointer system currently has no way of telling whether its object is in the stack or the heap
    * If you use the "new" keyword and a pointer for your object/data, its on the heap and thus safe to hold in a TrecPointer
    * If you do not use pointers for an object, it is stored on the STACK and thus dangerous to host in a TrecPointer
3. If a given Object derives from IUnknown, i.e. you're working with a DirectX/COM object, use the TrecComPointer instead
    * COM objects play by a different set of rules than the usual new/delete set.
4. Before using an object held by a TrecPointer/TrecComPointer, perform a NULL check.
    * Simply call it's .get() method within an if statement

## TArray vs. TDataArray

##### TArray
* Holds TrecPointers for various objects generated during AnaGame's run
* Backed by a fixed size array and a link to the Next TArray
* Combines the concept of an array with a linked list
* If you are holding objects without concern for where they are stored in memory, and wish for AnaGame to manage it's memory, use a TArray
* CANNOT hold primitives

##### TDataArray
* Holds Raw data, whether they be pointers or primitive values
* Backed by a variable sized array that is copied over into a larger array when curret capacity is reached
* If storing primitives and need to hold data in one contigous sequence in memory, use a TDataArray
* If storing primitives, you CANNOT store them as pointers