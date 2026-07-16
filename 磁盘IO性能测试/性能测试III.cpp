#pragma GCC target("avx")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")
#include <bits/stdc++.h>
#include <conio.h>
#include <chrono>
#include <windows.h>
using namespace std;
#include <filesystem>
namespace fs = std::filesystem;
string s;
bool is_live = 0,is_running = 0;
inline void inject_data(int id){
	ofstream inject(s+"\\"+(to_string(id)+"txt.txt"));
	while(is_live){
		inject << 1;
	}
}

long long get_folder_size(const fs::path& folder_path) {
	if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) {
		throw std::invalid_argument("Invalid folder path");
	}
	
	long long size = 0;
	for (const auto& entry : fs::recursive_directory_iterator(folder_path)) {
		if (fs::is_regular_file(entry.status())) {
			size += entry.file_size();
		}
	}
	return size;
}


int main(){
	int n; 
	printf("RAM Load Track VII\n");
	printf("定址传输性能测试 V9 <Threads>\n");
	printf("32线程10秒传输>1GB<为正常性能\n----\n");
	printf("测试文件夹名称（随便起，别重复就行）: ");
	
	cin>>s;
	
	fs::create_directory(".\\"+s+"\\");
	printf("线程数和测试时间(秒): ");
	scanf("%d",&n);
	int k;
	scanf("%d",&k);
	is_live = 1;
	for (int i = 0; i < n; ++i) {
		thread threads(inject_data,i+1);
		threads.detach();
		printf("\r>>Thread %d is deteched.",i);
	}
	for(int i = 1;i <= 9;i++){
		this_thread::sleep_for(chrono::milliseconds(k*100));
		long long siz = get_folder_size(s+"\\");
		printf("\r写入总数(±2%): %lld Byte | ",siz);
		cout<<fixed<<setprecision(4)<<siz*1.0/1024<<" KB | ";
		cout<<fixed<<setprecision(4)<<siz*1.0/1024/1024<<" MB | ";
		cout<<fixed<<setprecision(4)<<siz*1.0/1024/1024/1024<<" GB";
	}
	is_live = 0;
	this_thread::sleep_for(chrono::milliseconds(k*100));
	long long siz = get_folder_size(s+"\\");
	printf("\r写入总数(±2%): %lld Byte | ",siz);
	cout<<fixed<<setprecision(4)<<siz*1.0/1024<<" KB | ";
	cout<<fixed<<setprecision(4)<<siz*1.0/1024/1024<<" MB | ";
	cout<<fixed<<setprecision(4)<<siz*1.0/1024/1024/1024<<" GB\n";
	printf("写入完毕.数据见上");
	getch();
}
