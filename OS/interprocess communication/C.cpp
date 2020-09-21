#include<stdio.h>
#include<windows.h>
#include <math.h>
#include <tchar.h> 

#define BUFFER_SIZE 500

double stdev(int data[], double mid)          // 표준편차 구하는 함수
{
	int i;
	double sum = 0.0;
	double mean = mid;

	for (i = 0; i < 100; ++i)
	{
		sum += (data[i] - mean) * (data[i] - mean);
	}

	sum /= (double)(100 - 1);

	return (sqrt(sum));
}

int main()
{
	int n = 0;

	printf("프로세스C 데이터 수신\n");

	HANDLE ReadHandle , WriteHandle;
	int buffer[BUFFER_SIZE];
	double buf2[1];
	DWORD read , written;

	ReadHandle = GetStdHandle(STD_INPUT_HANDLE);
	
	if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))   // A 에게 숫자 받음
	{
		for (int i = 0;i < 100;i++)
		{
			printf("%d ", buffer[i]);
		}
	}
	else
	{
		fprintf(stderr, "Child : Error reading from pipe\n");
	}
	printf("\n");

	if (ReadFile(ReadHandle, buf2, 8, &read, NULL))    // A 에게 평균 받음
	{
		printf("평균값 : %f ", buf2[0]);
	}
	double temp = buf2[0];
	double stddev[1];
	stddev[0] = stdev(buffer, temp);
	
	printf("표준편차 : %f\n", stddev[0]);
	n++;
	int pid[1];
	pid[0] = GetCurrentProcessId();
	printf("프로세스C id : %d\n", pid[0]);
	printf("\n");

	// -------------------------

	HANDLE ReadHandle2, WriteHandle2;   // D 에게 표준편차 전달하는 파이프
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

	TCHAR command2[] = _T("check.exe");
	CreateProcess(NULL, command2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2);

	CloseHandle(ReadHandle2);

	if (!WriteFile(WriteHandle2, stddev, 8, &written2, NULL))//num buffer size
		fprintf(stderr, "Error writing to pipe\n");


	CloseHandle(WriteHandle2);

	WaitForSingleObject(pi2.hProcess, INFINITE);

	CloseHandle(pi2.hProcess);
	CloseHandle(pi2.hThread);

	return 0;

}
