
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

// 计算实际的运行时地址
DWORD CalculateRuntimeAddress(HANDLE hProcess, HMODULE hModule, DWORD originalRVA) {
	MODULEINFO mi;
	BOOL result = GetModuleInformation(hProcess, hModule, &mi, sizeof(mi));
	if (result) {
		// 输出模块信息
		printf("模块基址: %p\n", mi.lpBaseOfDll);
		printf("模块大小: %lu 字节\n", mi.SizeOfImage);
		printf("模块入口点: %p\n", mi.EntryPoint);
		// 计算实际的运行时地址
		DWORD runtimeAddress = (DWORD)mi.lpBaseOfDll + originalRVA;
		printf("计算后的运行时地址: %08X\n", runtimeAddress);
		return runtimeAddress;
	}
	else {
		printf("获取模块信息失败。\n");
		return 0;
	}
}

// 获取指定进程名的进程ID
DWORD GetProcessIdByName(const char* processName) {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf("创建进程快照失败。\n");
		return 0;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		printf("获取第一个进程失败。\n");
		return 0;
	}

	do {
		if (strcmp(pe32.szExeFile, processName) == 0) {
			CloseHandle(hProcessSnap);
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	printf("未找到进程 '%s'。\n", processName);
	return 0;
}

// 应用补丁的函数
BOOL ApplyPatch(HANDLE hProcess, LPVOID runtimeAddress, BYTE* patch, DWORD patchSize) {

	BOOL result = WriteProcessMemory(hProcess, runtimeAddress, patch, patchSize, NULL);
	if (result) {
		printf("成功应用补丁到地址: %08X\n", runtimeAddress);
	}
	else {
		printf("在地址 %08X 处应用补丁失败。\n", runtimeAddress);
	}
	return result;
}

int main() {
	const char* processName = "cmpvpm.exe"; // 你想要查找的进程名
	DWORD processId = GetProcessIdByName(processName);

	if (processId == 0) {
		printf("未找到进程。\n");
		return 1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (hProcess == NULL) {
		printf("打开进程失败。\n");
		return 1;
	}

	// 假设我们要修改的原始地址是相对于模块基址的偏移量
	DWORD FirstRVA = 0x1C19; // 示例地址，实际使用时需要替换为正确的地址
	DWORD SecondRVA = 0x1C2e; // 示例地址，实际使用时需要替换为正确的地址
	HMODULE hModuleArray[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(hProcess, hModuleArray, sizeof(hModuleArray), &cbNeeded)) {
		HMODULE hModule = hModuleArray[0]; // 获取第一个模块
		DWORD FirstAddress = CalculateRuntimeAddress(hProcess, hModule, FirstRVA);
		// 假设我们要修改的原始地址是相对于模块基址的偏移量
		DWORD SecondAddress = CalculateRuntimeAddress(hProcess, hModule, SecondRVA);

		BYTE PatchData[] = { 0x75 }; // NOP指令

		ApplyPatch(hProcess, (LPVOID)FirstAddress, PatchData, sizeof(PatchData));
		ApplyPatch(hProcess, (LPVOID)SecondAddress, PatchData, sizeof(PatchData));
	
		}
	
	// 关闭进程句柄
	CloseHandle(hProcess);

	getchar();
	return 0;
}
