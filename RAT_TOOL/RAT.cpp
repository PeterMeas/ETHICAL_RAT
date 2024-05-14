#include <Windows.h>

BOOL APIENTRY DllMAin(HMODULE Base, DWORD Callback, LPVOID Param)
{
	switch (Callback)
	{
	case DLL_PROCESS_ATTACH:
		
		break;
	case DLL_PROCESS_DETACH:

		break;
	default:
		break;
	}
	return 1;
}

extern "C" __declspec(dllexport) int Entry()
{
	return MessageBoxA(0, "RAT DLL TEST", 0, 0);

}