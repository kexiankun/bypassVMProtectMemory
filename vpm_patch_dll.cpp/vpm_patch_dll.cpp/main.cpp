#include "main.hpp"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		/*DllHi(hModule);*/
		Jmp_Call();
	
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		
		break;
	}
	return TRUE;
}


VOID Jmp_Call()
{
	HMODULE hModule = NULL;
	LPVOID Pointer_GetVaService = NULL, Jmp_Pointer = NULL, Jmp_Pointer1 = NULL, Jmp_Pointer2 = NULL, Jmp_Pointer3 = NULL, Jmp_Pointer4 = NULL;
	DWORD dwOldProtect = NULL;
	byte DataJudgment = NULL;


	hModule = GetModuleHandle(TEXT("xxxx.exe"));

	if (hModule !=NULL)
	{
		Jmp_Pointer = (byte*)hModule + 0x26AE;// 26AD

		Jmp_Pointer1 = (byte*)hModule + 0xAFA2C;//     C7DE3

		Jmp_Pointer2 = (byte*)hModule + 0xC7DE6;//     C7DE3
	}
	if (Jmp_Pointer!=NULL)
	{

		do
		{
			VirtualProtect((LPVOID)Jmp_Pointer, sizeof(&Jmp_Pointer), PAGE_EXECUTE_READWRITE, &dwOldProtect);

			if (*(byte*)Jmp_Pointer = 0x85)
			{

				RtlCopyMemory(&DataJudgment, LPVOID(Jmp_Pointer), sizeof(DataJudgment));//复制0x84进行判断
			}

		} while (DataJudgment == 0x84);

		*(DWORD*)Jmp_Pointer1 = 0x4B434E4A;   /* ~0x4A4E434B;*/

		*(DWORD*)Jmp_Pointer2 = 0x4B434E4A;

		VirtualProtect((PVOID)Jmp_Pointer, sizeof(&Jmp_Pointer), dwOldProtect, &dwOldProtect);
	}
	else
	{
		MessageBox(NULL, TEXT("补丁失败"), TEXT("提示："), MB_OK);
	}

}



