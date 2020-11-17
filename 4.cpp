#include <iostream>
#include <iomanip>
#include <windows.h>
#include <time.h>
#include <conio.h>

using namespace std;

DWORD WINAPI thread1_F(int** arg)
{
	int* mass = arg[0];
	int masSize = *arg[1];
	srand(time(NULL));
	LONG random;
	while (true)
	{
		for (int i = 0; i < 10; i++)
		{
			random = rand() % 300 - 150;
			InterlockedExchange((LONG*)mass + i, random);
		}
		Sleep(10);
		for (int i = 0; i < 10; i++)
		{
			cout<<setw(5) << mass[i] << "  ";
		}
		cout << endl;
	}
	return 1;
}

DWORD WINAPI thread2_F(int** arg)
{
	int* mass = arg[0];
	int masSize = *arg[1];
	while (true)
	{
		for (int i = 0; i < masSize; i++)
		{
			if ((mass[i] > -100 && mass[i] < -10) || (mass[i] < 100 && mass[i] > 10)) InterlockedExchange((LONG *)mass + i, 0);
		}
	}
	return 1;
}

int main()
{
	const int MASSIZE = 10;
	int mass[MASSIZE],	masSize= MASSIZE;
	int* argument[2] = { mass, &masSize };
	HANDLE thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread1_F, argument, NULL, 0);
	HANDLE thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread2_F, argument, NULL, 0);
	while (!_kbhit()) {}
	if (!thread1) CloseHandle(thread1);
	if (!thread2) CloseHandle(thread2);
	return 1;
}