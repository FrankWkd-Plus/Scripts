#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;
string s;

bool IsUserAdmin() {
	BOOL b;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	b = AllocateAndInitializeSid(
	        &NtAuthority,
	        2,
	        SECURITY_BUILTIN_DOMAIN_RID,
	        DOMAIN_ALIAS_RID_ADMINS,
	        0, 0, 0, 0, 0, 0,
	        &AdministratorsGroup);
	if (b) {
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &b)) {
			b = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}
	return b == TRUE;
}

// 封装自动申请管理员权限及后续操作的函数
void RequestAdminAndExecute() {
	if (!IsUserAdmin()) {
		// 获取当前可执行文件的完整路径
		wchar_t szPath[MAX_PATH];
		GetModuleFileNameW(NULL, szPath, MAX_PATH);

		// 重新以管理员权限启动程序
		SHELLEXECUTEINFOW sei = { sizeof(sei) };
		sei.lpVerb = L"runas";
		sei.lpFile = szPath;
		sei.nShow = SW_SHOWNORMAL;

		if (!ShellExecuteExW(&sei)) {
			DWORD dwError = GetLastError();
			if (dwError == ERROR_CANCELLED) {
				std::wcout << L"用户取消了 UAC 提示。" << std::endl;
			} else {
				std::wcout << L"以管理员权限启动程序时出错，错误代码: " << dwError << std::endl;
			}
			exit(0);
		}
		exit(0);
	}

	// 这里可以添加需要管理员权限执行的代码
	std::wcout << L"已获取管理员权限，可执行需要管理员权限的操作。" << std::endl;
}

std::string read(const std::string& prompt = "Verify Password: ", bool showAsterisk = true) {
	std::string password;
	char ch;
	std::cout << prompt;

	while ((ch = _getch()) != '\r') {  // 按Enter键结束输入
		if (ch == '\b') {  // 处理退格键
			if (!password.empty()) {
				std::cout << "\b \b";  // 退格并清除显示的星号
				password.pop_back();
			}
		} else {
			password += ch;
			if (showAsterisk) {
				std::cout << '*';  // 显示星号代替实际字符
			}
		}
	}
	std::cout << std::endl;  // 输入完成后换行
	return password;
}

int main() {
	RequestAdminAndExecute();
	s = read();
	if (s != "Frank1202" and s != "Start1" and s != "Start2" and s != "Start") {
		cout << "WA.\n";
		Sleep(5000);//防止编写恶意代码暴力解密
		return 0;
	}
	if (s == "Start1") {
		system("start %CD%\\_default");
		exit(0);
	}
	if (s == "Start2") {
		system("start %CD%\\LessonVideos");
		exit(0);
	}
	if (s == "Start") {
		system("start %CD%\\_default\\Releases\\_QuickStart");
		exit(0);
	}
start:
	;
	
	system("cls");
	cout << "快速加密操作面板 v1.0 By FrankWkd\n";
	cout << "Updated At 2025.05.17\n";

	cout << "[N] 全部隐藏\n[Y] 全部显示\n----\n[A] 跳转到主目录\n[B] 跳转到课程目录\n[C] 跳转到快捷栏\n----\n";
	cout << "[a] 加密S.team\n[b] 解密S.team\n----\n";
	cout << "[1] 加密主文件\n[2] 解密主文件\n[3] 加密课程文件\n[4] 解密课程文件\n";
	cout << "[0] 退出\n";
	char k;
	k = getch();
	if (k == 'N') {
		system("attrib +s +h %CD%\\_default");
		system("attrib +s +h %CD%\\LessonVideos");
		goto endd;
	} else if (k == 'Y') {
		system("attrib -s -h %CD%\\_default");
		system("attrib -s -h %CD%\\LessonVideos");
		goto endd;
	} else if (k == 'a') {
		system("attrib +s +h %CD%\\_default\\_Documents!\\_Doc\\teams");
		goto endd;
	} else if (k == 'b') {
		system("attrib -s -h %CD%\\_default\\_Documents!\\_Doc\\teams");
		goto endd;
	} else if (k == '1') {
		system("attrib +s +h %CD%\\_default");
		goto endd;
	} else if (k == '2') {
		system("attrib -s -h %CD%\\_default");
		goto endd;
	} else if (k == '3') {
		system("attrib +s +h %CD%\\LessonVideos");
		goto endd;
	} else if (k == '4') {
		system("attrib -s -h %CD%\\LessonVideos");
		goto endd;
	} else if (k == 'A') {
		system("start %CD%\\_default");
		exit(0);
	} else if (k == 'B') {
		system("start %CD%\\LessonVideos");
		exit(0);
	} else if (k == 'C') {
		system("start %CD%\\_default\\Releases\\_QuickStart");
		exit(0);
	}
	exit(0);
endd:
	;
	cout << "操作完成。\n";
	Sleep(1000);
	goto start;
}
/*
-2025.05.05 v0.1 Log：创建基本功能
-2025.05.10 v0.2 Log：修改测试信息没有删除的Bug
-2025.05.10 v0.3 Log：添加一键免显示目录就能够到达内部目录的快捷动作
-2025.05.10 v0.4 Log：删除每次输入还要按回车的老套输入，顺便防止利用输入作弊卡Bug
-2025.05.10 v0.5 Log：密码功能添加隐私功能，输入的密码即刻隐藏变成"*"
-2025.05.10 v0.6 Log：优化"---"快捷指令，变成打开目录而不是显示目录
-2025.05.10 v0.7 Log：添加等待程序，防止攻击者遍历密码
-2025.05.16 v0.8 Log：简化冗余代码
-2025.05.16 v0.9 Log：添加一键转到快捷方式启动栏的功能
-2025.05.16 v1.0 Log：优化代码结构，删除没用的，简化UI
-2025.05.25 v1.1 Log：
*/
