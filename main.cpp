#include <conio.h>
#include <iostream>
#include <csignal>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

// �Ƿ�����߳�
bool isClose = false;

void sleep (long millisecond) {
	chrono::milliseconds dura (millisecond);
	this_thread::sleep_for (dura);
}

// �����ļ�
void watch (const char *name) {
	cout << name << endl;
	wchar_t content[256];
	wmemset (content, '\0', sizeof(content) / sizeof(wchar_t));
	wifstream in{ name, wifstream::in };

	if (in.bad ()) {
		wcout << "��ȡ�ļ���������" << endl;
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

		// ���߳�Ҫ��ֹͣ
		if (isClose) {
			in.close ();
			return;
		}
	}

	if (in.bad ()) {
		wcout << "��ȡ�ļ���������" << endl;
		isClose = true;
	}
	
	in.close ();
}

int main (int argc, char* argv[]) {
	
	if (2 != argc) {
		wcout << "�������ļ���" << endl;
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

	// ֪ͨ�߳�ֹͣ
	isClose = true;
	thread.join ();
	return 0;
}