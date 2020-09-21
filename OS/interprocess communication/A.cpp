#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <tchar.h> 

#define BUFFER_SIZE 500

double average(int arr[]);    // 평균 만드는 함수

int main()
{
	printf("프로세스 A 시작!!\n");

	FILE* fp = NULL;  // 파일입력부분
	int temp;
	int num[BUFFER_SIZE];
	int n = 0;

	fp = fopen("num.txt", "r");

	while (feof(fp) == 0)
	{
		fscanf(fp, "%d", &temp);
		num[n] = temp;
		printf("%d ", num[n]);
		n++;
	}
	fclose(fp);
	printf("\n");

	double aver = average(num);
	printf("평균값 : %f\n", aver);

	double avg[1];
	avg[0] = aver;
	int pid[1];
	pid[0] = GetCurrentProcessId();
	printf("프로세스A id : %d\n", pid[0]);
	printf("\n");

	//------------------------------------------------------

	HANDLE ReadHandle, WriteHandle;    // 첫번째 파이프 B 와 통신
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	DWORD written;

	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

	ZeroMemory(&pi, sizeof(pi));

	if (!CreatePipe(&ReadHandle, &WriteHandle, &sa, 0))
	{
		fprintf(stderr, "Create Pipe Failed\n");
		return 1;
	}

	GetStartupInfo(&si);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	si.hStdInput = ReadHandle;
	si.dwFlags = STARTF_USESTDHANDLES;

	SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);

	TCHAR command[] = _T("bst_median.exe");
	CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	CloseHandle(ReadHandle);

	if (!WriteFile(WriteHandle, num, BUFFER_SIZE, &written, NULL))   //  100개 숫자 전달
		fprintf(stderr, "Error writing to pipe\n");

	CloseHandle(WriteHandle);

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//-----------------------------------------------------------


	HANDLE ReadHandle2, WriteHandle2;   // 두번째 파이프 C 와 통신
	STARTUPINFO si2;
	PROCESS_INFORMATION pi2;

	DWORD written2;

	SECURITY_ATTRIBUTES sa2 = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

	ZeroMemory(&pi2, sizeof(pi2));

	if (!CreatePipe(&ReadHandle2, &WriteHandle2, &sa2, 0))
	{
		fprintf(stderr, "Create Pipe Failed\n");
		return 1;
	}

	GetStartupInfo(&si2);
	si2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	si2.hStdInput = ReadHandle2;
	si2.dwFlags = STARTF_USESTDHANDLES;

	SetHandleInformation(WriteHandle2, HANDLE_FLAG_INHERIT, 0);

	TCHAR command2[] = _T("std_dev.exe");
	CreateProcess(NULL, command2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2);

	CloseHandle(ReadHandle2);

	if (!WriteFile(WriteHandle2, num, BUFFER_SIZE, &written2, NULL))   // 숫자 100개 전달
		fprintf(stderr, "Error writing to pipe\n");
	if (!WriteFile(WriteHandle2, avg, 8, &written2, NULL))   // 평균 전달
		fprintf(stderr, "Error writing to pipe\n");

	CloseHandle(WriteHandle2);

	WaitForSingleObject(pi2.hProcess, INFINITE);

	CloseHandle(pi2.hProcess);
	CloseHandle(pi2.hThread);

	//------------------------------------------------------------

	HANDLE ReadHandle3, WriteHandle3;    // 세번째 파이프 D 와 통신
	STARTUPINFO si3;
	PROCESS_INFORMATION pi3;

	DWORD written3;

	SECURITY_ATTRIBUTES sa3 = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

	ZeroMemory(&pi3, sizeof(pi3));

	if (!CreatePipe(&ReadHandle3, &WriteHandle3, &sa3, 0))
	{
		fprintf(stderr, "Create Pipe Failed\n");
		return 1;
	}

	GetStartupInfo(&si3);
	si3.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	si3.hStdInput = ReadHandle3;
	si3.dwFlags = STARTF_USESTDHANDLES;

	SetHandleInformation(WriteHandle3, HANDLE_FLAG_INHERIT, 0);

	TCHAR command3[] = _T("check.exe");

	CreateProcess(NULL, command3, NULL, NULL, TRUE, 0, NULL, NULL, &si3, &pi3);

	CloseHandle(ReadHandle3);

	if (!WriteFile(WriteHandle3, avg, 8, &written3, NULL))    // 평균 전달
		fprintf(stderr, "Error writing to pipe\n");

	CloseHandle(WriteHandle3);

	WaitForSingleObject(pi3.hProcess, INFINITE);

	CloseHandle(pi3.hProcess);
	CloseHandle(pi3.hThread);


	return 0;
}


double average(int arr[])
{
	double store = 0;
	for (int i = 0;i < 100;i++)
	{
		store += arr[i];
	}
	store = store / 100;

	return store;
}

