#include <iostream>
#include <iomanip>
#include <windows.h>
#include <time.h>
using namespace std;

DWORD WINAPI thread2_F(int** arg) 
{
	int* mass = arg[0];
	int masSize = *arg[1];
	HANDLE thread_mutex = OpenMutex(SYNCHRONIZE, FALSE, L"Mutex");
	WaitForSingleObject(thread_mutex, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		if ((mass[i] > -100 && mass[i] < -10) || (mass[i] < 100 && mass[i] > 10)) mass[i]=0;
	}
	cout << "Eddited mass:\n";

	for (int i = 0; i < 10; i++)
	{
		cout <<setw(5)<< mass[i];
	}
	ReleaseMutex(thread_mutex);
	return 0;
}


DWORD WINAPI thread1_F(int** arg) 
{
	int* mass = arg[0];
	int masSize = *arg[1];
	HANDLE thread_mutex = OpenMutex(SYNCHRONIZE, FALSE, L"Mutex");
	WaitForSingleObject(thread_mutex, INFINITE);
	srand(time(0)); LONG random;
	
	for (int i = 0; i < 10; i++)
	{
		random = rand() % 300 - 150;
		mass[i] = random;
	}

	cout << "Input mass:\n";

	for (int i = 0; i < 10; i++)
	{
		cout << setw(5) << mass[i];
	}
	cout << endl;
	ReleaseMutex(thread_mutex);
	return 0;
}



int main()
{
	const int MASSIZE = 10;
	int mass[MASSIZE], masSize = MASSIZE;
	int* argument[2] = { mass, &masSize };
	HANDLE Mutex = CreateMutex(NULL, FALSE, L"Mutex");
	HANDLE thread1 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)thread1_F, argument, NULL, 0);
	Sleep(100);
	HANDLE thread2 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)thread2_F, argument, NULL, 0);
	HANDLE Threads[2] = { thread1, thread2 };
	WaitForMultipleObjects(2, Threads, TRUE, INFINITE);
	CloseHandle(Mutex);
	CloseHandle(thread1);
	CloseHandle(thread2);
	return 0;
}