#include <iostream>
#include <windows.h>
#include "conio.h"

using namespace std;

int counter1 = 0;
int counter2 = 0;
int counter3 = 0;

DWORD WINAPI thread1_F(LPVOID p)
{
	while (true)
	{
		counter1++;
		cout << "Thread 1: " << counter1 << endl;
		Sleep(100);
	}
	return 1;
}


DWORD WINAPI thread2_F(LPVOID p)
{
	while (true)
	{
		if (counter1 * counter3 >= 5000) SuspendThread(GetCurrentThread());
		counter2++;
		cout << "Thread 2: " << counter2 << endl;
		Sleep(300);
	}
	return 1;
}


DWORD WINAPI thread3_F(LPVOID p)
{
	while (true)
	{
		counter3++;
		cout << "Thread 3: " << counter3 << endl;
		Sleep(200);
	}
	return 1;
}


int main()
{
	HANDLE thread1 = CreateThread(NULL, 0, thread1_F, NULL, NULL, 0);
	HANDLE thread2 = CreateThread(NULL, 0, thread2_F, NULL, NULL, 0);
	HANDLE thread3 = CreateThread(NULL, 0, thread3_F, NULL, NULL, 0);
	char ch;
	while ((ch = _getche()) != 'q'){}
	if (!thread1) CloseHandle(thread1);
	if (!thread2) CloseHandle(thread2);
	if (!thread3) CloseHandle(thread3);
}