#include <windows.h>
#include <iostream>
#include <string>
#include "VMProtectSDK.h"

// �����û���������
const std::string correctUsername = "admin";
const std::string correctPassword = "password";

// ��¼����
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
	dummy = 0; // ʹ�� dummy �����������������ȫ�Ż�����ָ���
	VMProtectEnd();
	return (username == correctUsername && password == correctPassword);
}

int main() {

	

	std::string username, password;

	// �����û���������
	std::cout << "�������û���: ";
	std::cin >> username;

	std::cout << "����������: ";
	std::cin >> password;

	// ���е�¼��֤
	if (login(username, password)) {
		//std::cout << "��¼�ɹ�����ӭ������" << username << "��\n";
		MessageBoxA(NULL, "��¼�ɹ�����ӭ����", "username", MB_OK);
	}
	else {
		//std::cout << "��¼ʧ�ܣ��û������������\n";
		MessageBoxA(NULL, "��¼ʧ�ܣ��û������������", "username", MB_OK);
	}
	
	return 0;
}
