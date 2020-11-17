#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;

struct TvInfo
{
	char BrandName[20] = " ";
	time_t TimeOfCommis = time(0);
	bool IsReady = 0;
};

int main(int argc, char** argv)
{
	TvInfo InfoList;
	DWORD BytesWritten;
	LPCTSTR FileName = L"D:\\Db\\DB.dat";
	cout << "NumOfArg: " << argc-1 << " FirstArg: " << argv[1]<< " SecondArg: " << argv[2];
	int LastRowindex = atoi(argv[2]);
	int NumOfConfOrder = atoi(argv[1]);
	HANDLE handle = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (LockFile(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, sizeof(TvInfo), 0))
	{
		SetFilePointer(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, FILE_BEGIN);
		if(ReadFile(handle, &InfoList, sizeof(TvInfo), &BytesWritten, 0))
		{
			InfoList.IsReady = 1;
			SetFilePointer(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, FILE_BEGIN);
			WriteFile(handle, &InfoList, sizeof(TvInfo), &BytesWritten, 0);
		}
		else { cout << "Read error"; system("pause"); }
		UnlockFile(handle, sizeof(TvInfo) * (NumOfConfOrder - 1), 0, sizeof(TvInfo), 0);
	}
	else { cout << "Access denied"; system("pause"); }
	CloseHandle(handle);
}