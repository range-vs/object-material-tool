#pragma once

// c++ / cli
#include <msclr/marshal.h>

using namespace System;
using System::IntPtr;
using System::Runtime::InteropServices::Marshal;

// c++
#include <string>

class Lines
{
public:
	static std::string systemToStdString(System::String^ s);
};