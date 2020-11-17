#include <iostream>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <conio.h>
using namespace std;
HANDLE hSemaphore;

void thread01(int** arg){
	int* mass = arg[0];
	int length = *arg[1];
	WaitForSingleObject(hSemaphore, INFINITE);
	for (int i = 0; i < length; i++) 
	{		
		Sleep(100);
		mass[i] = rand() % 501;
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
}

void thread02(int** arg){
	int* mass = arg[0];
	int length = *arg[1];
	int min = 500;
	WaitForSingleObject(hSemaphore, INFINITE);
	for (int i = 0; i < length; i++) 
	{
		cout << "mas[" << i << "]= " << mass[i] << endl;
		if (mass[i] < min) min = mass[i];
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
	cout << "Min element: " << min << endl;
}

int main()
{
	int n = 0;
	cout << "array size: "; cin >> n; cout << endl;
	int* mass = new int[n];
	int* argument[2] = { mass, &n };
	hSemaphore = CreateSemaphore(NULL, 1, 1, L"MySemaphore");
	if (hSemaphore == NULL) cout << "CREATE SEMAPHORE: failure"<< GetLastError() << endl;
	HANDLE hTh01 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread01, argument, 0, NULL);
	if (hTh01 == NULL) return GetLastError();
	Sleep(100);
	HANDLE hTh02 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread02, argument, 0, NULL);
	if (hTh02 == NULL) return GetLastError();
	while (!_kbhit()) {}
	CloseHandle(hSemaphore);
	CloseHandle(hTh01);
	CloseHandle(hTh02);
	return 0;
}