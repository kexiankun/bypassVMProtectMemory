加密解密对抗VMProtectMemory，

远程进程补丁vmp_patch_exe,

通过进程名获取进程ID的函数 GetProcessIdByName。
计算模块的运行时地址的函数 CalculateRuntimeAddress。
应用补丁到目标进程的函数 ApplyPatch。
在 main 函数中，获取指定进程名的进程ID，打开进程句柄，获取第一个模块的句柄，并计算两个地址的运行时地址，然后应用补丁到这两个地址。
最后关闭进程句柄。
需要注意的是，在使用 EnumProcessModules 函数时，如果要获取进程中所有模块的句柄，需要确保提供足够大的缓冲区来存储模块句柄。
此外，这个示例只获取了第一个模块的句柄，如果目标进程包含多个模块，可能需要根据实际情况修改代码以处理更多的模块。

DLL劫持 vpm_patch_dll
