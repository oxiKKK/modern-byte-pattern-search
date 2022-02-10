#include <windows.h>

#include "patternsearch.h"

using pfnMessageBoxA_t = INT32(WINAPI*)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
pfnMessageBoxA_t pfnMessageBoxA;

int main(int, char**)
{
	CBytePattern Pattern("\x8B\xFF\x55\x8B\xEC\x83\x3D\x00\x00\x00\x00\x00\x74\x00\x00\x00\x00\x00\x00\x00\xBA\x00\x00"
		"\x00\x00\x8B\x48\x24\x33\xC0\xF0\x0F\xB1\x0A\x85\xC0\x75\x00\xC7\x05\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x6A\xFF\x6A\x00\xFF\x75\x14\xFF\x75\x10\xFF\x75\x0C\xFF\x75\x08\xE8\x00\x00\x00\x00"
		"\x5D\xC2\x10\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x8B\xFF\x55\x8B\xEC\x6A");

	printf("[+] Auto-generated mask -> %s\n", Pattern.Mask().c_str());

	auto hModule = LoadLibraryA("user32.dll");

	if (!hModule)
	{
		printf("[!] user32.dll not found\n");
		return 1;
	}

	uint32_t dwModuleBase = reinterpret_cast<uint32_t>(hModule);

	printf("[+] user32.dll at 0x%08X\n", dwModuleBase);

	auto pDOSHdr = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
	auto pNtHdr = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(pDOSHdr) + pDOSHdr->e_lfanew);
	auto pOptHdr = &pNtHdr->OptionalHeader;

	auto pdwAddress = Pattern.Search(dwModuleBase, dwModuleBase + pOptHdr->SizeOfImage - 1);

	if (!pdwAddress)
	{
		printf("[!] Couldn't find the address.\n");
		return 1;
	}

	pfnMessageBoxA = reinterpret_cast<pfnMessageBoxA_t>(pdwAddress);

	printf("[+] MessageBoxA at 0x%08X\n", reinterpret_cast<uint32_t>(pfnMessageBoxA));

	printf("[+] Testing MessageBoxA...");

	pfnMessageBoxA(NULL, "Hello from hooked MessageBoxA", "Information", NULL);

	printf("Success\n");
	std::cin.get();

	return 0;
}