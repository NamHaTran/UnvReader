#include "DGAuxUltilitiesLib.h"
#include <windows.h>

namespace auxUlti
{
	std::string workingdir()
	{
		char buf[256];
		GetCurrentDirectoryA(256, buf);
		return std::string(buf);
	}
}