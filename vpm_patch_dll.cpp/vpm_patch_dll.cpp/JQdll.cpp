#include"JQdll.hpp"

/*
dllname:		���ٳ�dll��ԭʼ����
OrigDllPath:	���ٳ�dll�����������·��
*/
VOID SuperDllHijack(LPCWSTR dllname, LPWSTR OrigDllPath)
{
			WCHAR wszDllName[100] = { 0 };
			void* peb = NtCurrentPeb();
			PEB_LDR_DATA* ldr = NtGetPebLdr(peb);

	for (LIST_ENTRY* entry = ldr->InLoadOrderModuleList.Blink;
					entry != (LIST_ENTRY*)(&ldr->InLoadOrderModuleList);
				   	entry = entry->Blink) 
	{
			PLDR_DATA_TABLE_ENTRY data = (PLDR_DATA_TABLE_ENTRY)entry;

			memset(wszDllName, 0, 100 * 2);
			memcpy(wszDllName, data->BaseDllName.Buffer, data->BaseDllName.Length);

		if (!_wcsicmp(wszDllName, dllname)) 
		{

			HMODULE hMod = LoadLibrary(OrigDllPath);
			data->DllBase = hMod;
			break;
		}
	}
}
VOID DllHi(HMODULE hMod)
{
	TCHAR tszDllPath[MAX_PATH] = { 0 };

	GetModuleFileName(hMod, tszDllPath, MAX_PATH);
	PathRemoveFileSpec(tszDllPath);
	PathAppend(tszDllPath, L"_X.dll");//ԭʼDLL

	SuperDllHijack(L"X.dll", tszDllPath);//�ҽ�DLL
	CloseHandle(hMod);
}