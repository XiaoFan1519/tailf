#include <conio.h>
#include <iostream>
#include <csignal>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

// 是否结束线程
bool isClose = false;

void sleep (long millisecond) {
	chrono::milliseconds dura (millisecond);
	this_thread::sleep_for (dura);
}

// 监视文件
void watch (const char *name) {
	cout << name << endl;
	wchar_t content[256];
	wmemset (content, '\0', sizeof(content) / sizeof(wchar_t));
	wifstream in{ name, wifstream::in };

	if (in.bad ()) {
		wcout << "读取文件发生错误" << endl;
		isClose = true;
		return;
	}

	while (in.read (content, 256) || in.eof()) {
		int len = wcsnlen (content, 256);
		if (0 != len) {
			wcout << content;
			wmemset (content, '\0', sizeof (content) / sizeof (wchar_t));
		}

		if (in.eof ()) {
			in.clear ();
			sleep (1000);
		}

		// 主线程要求停止
		if (isClose) {
			in.close ();
			return;
		}
	}

	if (in.bad ()) {
		wcout << "读取文件发生错误" << endl;
		isClose = true;
	}
	
	in.close ();
}

int main (int argc, char* argv[]) {
	
	if (2 != argc) {
		wcout << "请输入文件名" << endl;
		wcout << "tailf filename" << endl;
		return -1;
	}

	thread thread{ watch, argv[1] };

	int c;
	do {
		c = _getch ();
		if ((10 == c) || (13 == c)) {
			wcout << endl;
			continue;
		}

		wcout << (char)c;
	} while (c != '.');

	// 通知线程停止
	isClose = true;
	thread.join ();
	return 0;
}