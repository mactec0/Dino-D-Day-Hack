#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <stdint.h>

bool bCompare(const uint8_t* pData, const uint8_t* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)   return 0;
	return (*szMask) == NULL;
}

uint32_t FindPattern(uint32_t dwAddress, uint32_t dwLen, uint8_t *bMask, char * szMask, int offset = 0x00) {
	for (uint32_t i = 0; i < dwLen; i++)
		if (bCompare((uint8_t*)(dwAddress + i), bMask, szMask))  return (uint32_t)(dwAddress + i + offset);
	return 0;
}

uint32_t FindPatternIDA(uint32_t dwAddress, std::string idaPattern) {
	std::stringstream ss(idaPattern);
	std::string pattern = "", mask = "";
	std::string strByte;
	while (ss >> strByte) {
		if (strByte == "?") {
			pattern += 0xff;
			mask += '?';
		}
		else {
			pattern += (char)strtol(strByte.c_str(), NULL, 16);
			mask += 'x';
		}
	}
	return FindPattern(dwAddress, 0xfffffff, (uint8_t*)pattern.c_str(), (char*)mask.c_str());
}

HMODULE GetModuleHandleSafe(const char * moduleName) {
	HMODULE module = NULL;
	while (!module) {
		module = GetModuleHandle(moduleName);
		Sleep(100);
	}
	return module;
}

