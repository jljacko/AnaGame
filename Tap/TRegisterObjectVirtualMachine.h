#pragma once
#include "TVirtualMachine.h"

/*
 * class TRegisterObjectVirtualMachine
 * Purpose: Provide a Virtual Machine that tracks objects generated by the code that runs on it. 
 * Utilizing Registers to support arithmetic operations, this machine is one that is intended for 
 * higher-level languages. Any object created in such code will either implicitly or explicitly extand
 * AnaGame's TObject class
 */
class TRegisterObjectVirtualMachine :
	public TVirtualMachine
{
};

