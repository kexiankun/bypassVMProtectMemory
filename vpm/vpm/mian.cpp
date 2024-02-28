#include <windows.h>
#include <iostream>
#include <string>
#include "VMProtectSDK.h"

// 定义用户名和密码
const std::string correctUsername = "admin";
const std::string correctPassword = "password";

// 登录函数
bool login(const std::string& username, const std::string& password) {
	VMProtectBegin((char *)"Test marker");
	
	volatile int dummy;
	_asm {

nop
nop
nop
nop
nop
nop

	}
	dummy = 0; // 使用 dummy 变量来避免编译器完全优化掉空指令块
	VMProtectEnd();
	return (username == correctUsername && password == correctPassword);
}

int main() {

	

	std::string username, password;

	// 输入用户名和密码
	std::cout << "请输入用户名: ";
	std::cin >> username;

	std::cout << "请输入密码: ";
	std::cin >> password;

	// 进行登录验证
	if (login(username, password)) {
		//std::cout << "登录成功！欢迎回来，" << username << "。\n";
		MessageBoxA(NULL, "登录成功！欢迎回来", "username", MB_OK);
	}
	else {
		//std::cout << "登录失败！用户名或密码错误。\n";
		MessageBoxA(NULL, "登录失败！用户名或密码错误。", "username", MB_OK);
	}
	
	return 0;
}
