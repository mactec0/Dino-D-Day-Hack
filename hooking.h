#pragma once  
#include <windows.h>  
#include <stdint.h>
 
uint32_t hook_vmt(void *class_pointer, int id, uint32_t hook_addr) {
	uint32_t orginal, *vmt = *(uint32_t**)(class_pointer);
	DWORD current_protection;
	VirtualProtect((void*)&vmt[id], sizeof(uintptr_t), PAGE_READWRITE, &current_protection);
	orginal = vmt[id];
	vmt[id] = (uint32_t)hook_addr;
	VirtualProtect((void*)&vmt[id], sizeof(uintptr_t), current_protection, NULL);
	return orginal;
} 