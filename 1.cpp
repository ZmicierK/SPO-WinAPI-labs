#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include <fstream>
using namespace std;

struct TvInfo
{
	char BrandName[20]=" ";
	time_t TimeOfCommis= time(0);
	bool IsReady=0;
};

int menu()
{
	cout << "Main menu:\n";
	cout << "1. Show orders list\n";
	cout << "2. Add order\n";
	cout << "3. Confirm order readiness\n";
	cout << "4. Show not ready orders list\n";
	cout << "5. Exit\n";
	int ret= _getch() - 48;
	for (; ret < 1 || ret>5; ret = _getch() - 48) {};
	return ret;
}

int ReadList(HANDLE& handle)
{
	system("cls");
	DWORD BytesReaded;
	TvInfo InfoList;
	int RowIndex=1;
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
		return RowIndex;
	}
	else {cout << "Access denied"; return 0;}
}

int ReadListByFilter(HANDLE& handle)
{
	system("cls");
	LPCTSTR OutFileName = L"Output.txt";
	ofstream fout("outp.txt");
	DWORD BytesReaded, BytesWritten;
	TvInfo InfoList;
	int RowIndex = 1;
	if (LockFile(handle, 0, 0, GetFileSize(handle, NULL), 0))
	{
		SetFilePointer(handle, 0, 0, FILE_BEGIN);
		fout << left << setw(5) << "№" << left << setw(20) << "|TV model:" << setw(26) << " |Order date:" << setw(5) << "  |Status:" << "\n";
		cout << left << setw(5) << "№" << left << setw(20) << "|TV model:" << setw(26) << " |Order date:" << setw(5) << "  |Status:" << "\n";
		while (ReadFile(handle, &InfoList, sizeof(TvInfo), &BytesReaded, 0) && BytesReaded != 0)
		{
			if (!InfoList.IsReady)
			{
				char timeinfo[26];
				ctime_s(timeinfo, 26, &InfoList.TimeOfCommis);
				timeinfo[24] = '\0';
				fout << setw(5) << RowIndex << "|" << setw(5) << setw(20) << InfoList.BrandName << "|" << setw(26) << timeinfo << "|" << setw(5) << InfoList.IsReady << "\n";
				cout << setw(5) << RowIndex << "|" << setw(5) << setw(20) << InfoList.BrandName << "|" << setw(26) << timeinfo << "|" << setw(5) << InfoList.IsReady << "\n";
			}
			RowIndex++;
		}
		fout.close();
		UnlockFile(handle, 0, 0, GetFileSize(handle, NULL), 0);
	}
	else cout << "Access denied";
	system("pause");
	system("cls");
	return RowIndex;
}

void AddToList(HANDLE& handle)
{
	TvInfo InfoList;
	DWORD BytesWritten;
	DWORD FromPos=SetFilePointer(handle, 0, 0, FILE_END);
	cout << "Enter TV brand name: ";
	cin >> InfoList.BrandName;
	if (LockFile(handle, FromPos, 0, sizeof(TvInfo), 0))
	{
		WriteFile(handle, &InfoList, sizeof(InfoList), &BytesWritten, 0);
		UnlockFile(handle, FromPos, 0, sizeof(TvInfo), 0);
	}
	else { cout << "Access denied"; system("pause"); }
	system("cls");
}

void AssignOrderAsReady(HANDLE& handle)
{
	TvInfo InfoList;
	DWORD BytesWritten;
	int LastRowindex=ReadList(handle);
	int NumOfConfOrder;
	cout << "Enter the number of the completed order: ";
	while (!(cin >> NumOfConfOrder) || NumOfConfOrder <= 0 || NumOfConfOrder > LastRowindex - 1)
	{
		cin.clear(); cin.ignore(256, '\n'); cout << "Incorect input\n"; system("pause");
		system("cls"); ReadList(handle); cout << "Enter the number of the completed order: ";
	}
	if (LockFile(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, sizeof(TvInfo), 0))
	{
		SetFilePointer(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, FILE_BEGIN);
		if (ReadFile(handle, &InfoList, sizeof(TvInfo), &BytesWritten, 0)) 
		{
			InfoList.IsReady = 1;
			SetFilePointer(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, FILE_BEGIN);
			WriteFile(handle, &InfoList, sizeof(TvInfo), &BytesWritten, 0);
		}
		else { cout << "Read error"; system("pause"); }
		UnlockFile(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, sizeof(TvInfo), 0);
	}
	else {cout << "Access denied"; system("pause");}
	system("cls");
}

int main()
{
	setlocale(LC_ALL, "rus");
	LPCTSTR FileName = L"DB.dat";
	HANDLE handle = NULL;
	handle = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (handle == INVALID_HANDLE_VALUE) { cout << "Access denied\n"; return 0; };
	while (true) switch (menu())
	{
	case 1:ReadList(handle); system("pause"); system("cls"); break;
	case 2:AddToList(handle); break;
	case 3:AssignOrderAsReady(handle); break;
	case 4:ReadListByFilter(handle); break;
	case 5:CloseHandle(handle); return 0;
	}
}