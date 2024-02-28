#include <windows.h>
#include <ntstatus.h>
#include <tlhelp32.h>
#include <stdio.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

// ����ZwProtectVirtualMemory������ԭ��
typedef NTSTATUS(WINAPI *PFZWPROTECTVIRTUALMEMORY)(
	HANDLE ProcessHandle,
	PVOID *BaseAddress,
	PSIZE_T NumberOfBytesToProtect,
	ULONG NewAccessProtection,
	PULONG OldAccessProtection
	);

// ����ZwWriteVirtualMemory������ԭ��
typedef NTSTATUS(WINAPI *PFZWWRITEVIRTUALMEMORY)(
	HANDLE ProcessHandle,
	PVOID BaseAddress,
	PVOID Buffer,
	SIZE_T BufferSize,
	PSIZE_T NumberOfBytesWritten
	);

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

// ����ʵ�ʵ�����ʱ��ַ
DWORD CalculateRuntimeAddress(HANDLE hProcess, HMODULE hModule, DWORD originalRVA) {
	MODULEINFO mi;
	BOOL result = GetModuleInformation(hProcess, hModule, &mi, sizeof(mi));
	if (result) {
		// ���ģ����Ϣ
		printf("ģ���ַ: %p\n", mi.lpBaseOfDll);
		printf("ģ���С: %lu �ֽ�\n", mi.SizeOfImage);
		printf("ģ����ڵ�: %p\n", mi.EntryPoint);
		// ����ʵ�ʵ�����ʱ��ַ
		DWORD runtimeAddress = (DWORD)mi.lpBaseOfDll + originalRVA;
		printf("����������ʱ��ַ: %08X\n", runtimeAddress);
		return runtimeAddress;
	}
	else {
		printf("��ȡģ����Ϣʧ�ܡ�\n");
		return 0;
	}
}

// ��ȡָ���������Ľ���ID
DWORD GetProcessIdByName(const char* processName) {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf("�������̿���ʧ�ܡ�\n");
		return 0;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		printf("��ȡ��һ������ʧ�ܡ�\n");
		return 0;
	}

	do {
		if (strcmp(pe32.szExeFile, processName) == 0) {
			CloseHandle(hProcessSnap);
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	printf("δ�ҵ����� '%s'��\n", processName);
	return 0;
}

// Ӧ�ò����ĺ���
BOOL ApplyPatch(HANDLE hProcess, LPVOID runtimeAddress, BYTE* patch, DWORD patchSize) {

	BOOL result = WriteProcessMemory(hProcess, runtimeAddress, patch, patchSize, NULL);
	if (result) {
		printf("�ɹ�Ӧ�ò�������ַ: %08X\n", runtimeAddress);
	}
	else {
		printf("�ڵ�ַ %08X ��Ӧ�ò���ʧ�ܡ�\n", runtimeAddress);
	}
	return result;
}

int main() {
	const char* processName = "cmpvpm.exe"; // ����Ҫ���ҵĽ�����
	DWORD processId = GetProcessIdByName(processName);

	if (processId == 0) {
		printf("δ�ҵ����̡�\n");
		return 1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (hProcess == NULL) {
		printf("�򿪽���ʧ�ܡ�\n");
		return 1;
	}

	// ��������Ҫ�޸ĵ�ԭʼ��ַ�������ģ���ַ��ƫ����
	DWORD FirstRVA = 0x1C19; // ʾ����ַ��ʵ��ʹ��ʱ��Ҫ�滻Ϊ��ȷ�ĵ�ַ
	DWORD SecondRVA = 0x1C2e; // ʾ����ַ��ʵ��ʹ��ʱ��Ҫ�滻Ϊ��ȷ�ĵ�ַ
	HMODULE hModuleArray[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(hProcess, hModuleArray, sizeof(hModuleArray), &cbNeeded)) {
		HMODULE hModule = hModuleArray[0]; // ��ȡ��һ��ģ��
		DWORD FirstAddress = CalculateRuntimeAddress(hProcess, hModule, FirstRVA);
		// ��������Ҫ�޸ĵ�ԭʼ��ַ�������ģ���ַ��ƫ����
		DWORD SecondAddress = CalculateRuntimeAddress(hProcess, hModule, SecondRVA);

		BYTE PatchData[] = { 0x75 }; // NOPָ��

		ApplyPatch(hProcess, (LPVOID)FirstAddress, PatchData, sizeof(PatchData));
		ApplyPatch(hProcess, (LPVOID)SecondAddress, PatchData, sizeof(PatchData));
	
		}
	
	// �رս��̾��
	CloseHandle(hProcess);

	getchar();
	return 0;
}