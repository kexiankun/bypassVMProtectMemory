#pragma once

#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _PEB_LDR_DATA
{
	ULONG Length; // +0x00
	BOOLEAN Initialized; // +0x04
	PVOID SsHandle; // +0x08
	LIST_ENTRY InLoadOrderModuleList; // +0x0c
	LIST_ENTRY InMemoryOrderModuleList; // +0x14
	LIST_ENTRY InInitializationOrderModuleList;// +0x1c
} PEB_LDR_DATA, * PPEB_LDR_DATA; // +0x24

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		LIST_ENTRY HashLinks;
		struct
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports;
	};
	_ACTIVATION_CONTEXT* EntryPointActivationContext;
	PVOID PatchInformation;
	LIST_ENTRY ForwarderLinks;
	LIST_ENTRY ServiceTagLinks;
	LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

void* NtCurrentPeb()
{
#ifdef _WIN64
	return (void*)__readgsqword(0x30);
#else
	__asm {
		mov eax, fs: [0x30] ;
	}
#endif
}

PEB_LDR_DATA* NtGetPebLdr(void* peb)
{
#ifdef _WIN64
	return (PEB_LDR_DATA*)(*(ULONGLONG*)((BYTE*)peb + 0x18));
#else
	__asm {
		mov eax, peb;
		mov eax, [eax + 0xc];
	}
#endif
}
