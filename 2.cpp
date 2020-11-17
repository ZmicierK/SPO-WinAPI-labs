#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include <fstream>
using namespace std;

struct TvInfo
{
	char BrandName[20] = " ";
	time_t TimeOfCommis = time(0);
	bool IsReady = 0;
};

int ReadList(HANDLE& handle)
{
	system("cls");
	DWORD BytesReaded;
	TvInfo InfoList;
	int RowIndex = 1;
	if (LockFile(handle, 0, 0, GetFileSize(handle, NULL), 0))
	{
		SetFilePointer(handle, 0, 0, FILE_BEGIN);
		cout << left << setw(5) << "№" << left << setw(20) << "|TV model:" << setw(26) << " |Order date:" << setw(5) << "  |Status:" << "\n";
		while (ReadFile(handle, &InfoList, sizeof(TvInfo), &BytesReaded, 0) && BytesReaded != 0)
		{
			char timeinfo[26];
			ctime_s(timeinfo, 26, &InfoList.TimeOfCommis);
			timeinfo[24] = '\0';
			cout << setw(5) << RowIndex << "|" << setw(5) << setw(20) << InfoList.BrandName << "|" << setw(26) << timeinfo << "|" << setw(5) << InfoList.IsReady << "\n";
			RowIndex++;
		}
		UnlockFile(handle, 0, 0, GetFileSize(handle, NULL), 0);
		return RowIndex - 1;
	}
	else { cout << "Access denied"; system("pause"); return 0; }
}


int main()
{
	TvInfo InfoList;
	LPCTSTR FileName = "D:\\DB\\DB.dat";
	HANDLE handle = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	int RowIndex = 1, LastRowindex = 0, NumOfStructToChange = 0;
	while (!LastRowindex) LastRowindex = ReadList(handle);
	cout << "Enter number of struct to change: ";
	while (!(cin >> NumOfStructToChange) || NumOfStructToChange <= 0 || NumOfStructToChange > LastRowindex - 1)
	{
		cin.clear(); cin.ignore(256, '\n'); cout << "Incorect input\n"; system("pause");
		system("cls"); LastRowindex=ReadList(handle); cout << "Enter the number of the completed order: ";
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	char appName[50] = "D:\\DB\\Daughter.exe ";
	char arg1[3];
	char arg2[3];
	sprintf_s(arg1, "%d", NumOfStructToChange);
	sprintf_s(arg2, "%d", LastRowindex);
	strcat_s(appName, arg1);
	strcat_s(appName, " ");
	strcat_s(appName, arg2);
	cout << appName << endl;
	if (!CreateProcess(NULL,appName,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) cout << "Error during starting process\n" << endl;
	WaitForSingleObject(pi.hProcess, INFINITE);
	ReadList(handle);
	CloseHandle(handle);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}