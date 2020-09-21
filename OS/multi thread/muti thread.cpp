#include<stdio.h>
#include<windows.h>
#include<time.h>

int min, max;
int num1, num2, num3, num4;   // 각각의 스레드마다 하나의 전역변수를 주었음
                                       // num은 각각의 스레드가 판별한 소수의 갯수를 저장함

int comp(int start, int end)      // 소수를 판별하고 갯수를 리턴하는 함수
{
	int i, j;
	int temp = 0;
	int count = 0;

	for (i = start; i < end; i++)
	{
		for (j = 1; j < i; j++)
		{
			if (i % j == 0)
			{
				temp++;
			}
		}
		if (temp == 1)
		{
			count++;
		}
		temp = 0;
	}
	return count;
}


DWORD WINAPI Worker1(LPVOID Param)      // 1번 스레드
{
	DWORD end = *(DWORD *)Param;
	num1 = comp(min,end);

	return 0;
}

DWORD WINAPI Worker2(LPVOID Param)       // 2번 스레드
{
	DWORD start = *(DWORD *)Param;
	DWORD end = *(DWORD *)Param * 2;

	num2 = comp(start, end);

	return 0;
}

DWORD WINAPI Worker3(LPVOID Param)     // 3번 스레드
{
	DWORD start = *(DWORD *)Param * 2;
	DWORD end = *(DWORD *)Param * 3;

	num3 = comp(start, end);

	return 0;
}

DWORD WINAPI Worker4(LPVOID Param)     // 4번 스레드
{
	DWORD start = *(DWORD *)Param * 3;

	num4 = comp(start, max);

	return 0;
}

int main()
{
	clock_t start1, end1;
	float res1;
	
	int count;

	DWORD ThreadId1, ThreadId2, ThreadId3, ThreadId4;
	HANDLE ThreadHandle1, ThreadHandle2, ThreadHandle3, ThreadHandle4;

	printf("min\n");
	scanf("%d", &min);
	printf("max\n");
	scanf("%d", &max);

	start1 = clock();             // 소수 판별 연산 시작 : 타이머 시작

	int Param = (max - min) / 4;     // 범위를 4분할하여 각각의 스레드에 파라미터로 전달

	ThreadHandle1 = CreateThread(NULL, 0, Worker1, &Param, 0, &ThreadId1);
	ThreadHandle2 = CreateThread(NULL, 0, Worker2, &Param, 0, &ThreadId2);
	ThreadHandle3 = CreateThread(NULL, 0, Worker3, &Param, 0, &ThreadId3);
	ThreadHandle4 = CreateThread(NULL, 0, Worker4, &Param, 0, &ThreadId4);

	WaitForSingleObject(ThreadHandle1, INFINITE);    // 스레드가 종료하길 기다린다
	WaitForSingleObject(ThreadHandle2, INFINITE);
	WaitForSingleObject(ThreadHandle3, INFINITE);
	WaitForSingleObject(ThreadHandle4, INFINITE);

	count = num1 + num2 + num3 + num4;      // 결과물 총합

	end1 = clock();                  // 연산 끝 : 타이머 끝
	res1 = (float)(end1 - start1) / CLOCKS_PER_SEC;

	printf(" 첫번째 스레드id : %lu\n 두번째 스레드id : %lu\n", ThreadId1, ThreadId2);
	printf(" 세번째 스레드id : %lu\n 네번째 스레드id : %lu\n", ThreadId3, ThreadId4);

	CloseHandle(ThreadHandle1);
	CloseHandle(ThreadHandle2);
	CloseHandle(ThreadHandle3);
	CloseHandle(ThreadHandle4);

	printf("소수 갯수 : %d\n", count);     // 결과물 출력
	printf("수행시간 : %.3f", res1);

	return 0;
}