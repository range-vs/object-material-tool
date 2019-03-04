#include "Lines.h"

std::string Lines::systemToStdString(System::String^ s)
{
	if (!s)
		return "";
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	std::string os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
	return os;
}