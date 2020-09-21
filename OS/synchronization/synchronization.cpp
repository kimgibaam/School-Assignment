#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

int min, max;                  // 입력값
int q[2024];              // 큐
int rear, front = 0;        // 큐의 머리와 꼬리
int count = 0;             // 카운트 = 생산 - 소비
int pro = 0;             // 생산 갯수
int con = 0;                // 소비 갯수
int LSD_freq[10];
int MSD_freq[10];
HANDLE hMutex;              // 뮤텍스 핸들

int put(int val)              // 큐에 값 넣는 함수
{
	WaitForSingleObject(hMutex, 10);       // 뮤텍스로 동기화

	q[rear] = val;                         // rear가 큐의 끝 다음의 빈공간이므로 바로 저장
	rear = ++rear % 2024;             // rear를 다음 빈공간으로 변경
	pro++;
	count++;               // 생산후 카운트 증가

	ReleaseMutex(hMutex);

	return 0;
}

int get(void)         // 큐에 있는 값을 반환하는 함수
{
	int i;

	i = q[front];    // front의 값을 가져옴
	front = ++front % 2024;   // front를 다음 데이터 요소로
	con++;
	count--;                 // 소비후 카운트 감소

	return i;

}

int comp(int a)      // 소수판별후 반환
{
	int i = a;
	int j;
	int temp = 0;

	for (j = 1; j < i; j++)
	{
		if (i % j == 0)
		{
			temp++;
		}
	}
	if (temp == 1)
	{
		return i;
	}
	else
	{
		return 0;
	}
}

int freq(int a)             // LSD와 MSD의 빈도수를 올리는 함수
{
	int num = a;
	int temp;
	temp = a % 10;
	LSD_freq[temp] += 1;

	while (num != 0)
	{
		temp = num % 10;
		num /= 10;
	}
	MSD_freq[temp] += 1;

	return 0;
}

DWORD WINAPI Worker1(LPVOID Param)      // 1번 스레드 생산
{
	DWORD mid = *(DWORD *)Param;
	int temp = 0;

	for (int i = min; i < mid; i++)          // min 부터 중간값까지 소수 판별후 큐에 넣음
	{                                                
		if (pro < max)
		{
			if (count < 2024)     // 카운트가 큐의 크기와 같아지면 기다려야함
			{
				temp = comp(i);
				if (temp != 0) put(temp);  // 소수라면 큐에 들어감
			}
			else if (count >= 2024)
			{
				i--;  
				         // 계속 의미없는 루프를 돌게됨 -> 기다림
			}
		}
	}
	return 0;
}

DWORD WINAPI Worker2(LPVOID Param)      // 1번 스레드 생산
{
	DWORD mid = *(DWORD *)Param;
	int temp = 0;

	for (int i = mid; i <= max; i++)   // 중간값부터 max 까지 소수 판별후 큐에 넣음
	{
		if (pro < max)
		{
			if (count < 2024)     // 카운트가 큐의 크기와 같아지면 기다려야함
			{
				temp = comp(i);
				if (temp != 0) put(temp);  // 소수라면 큐에 들어감
			}
			else if (count >= 2024)
			{
				i--;
				   // 계속 의미없는 루프를 돌게됨 -> 기다림
			}
		}
	}
	return 0;
}

DWORD WINAPI Worker3(LPVOID Param)     // 3번 스레드 소비
{
	int temp = 0;

	while (count > 0)                // 카운트 0 -> 큐가 비어있지 않을때 실행
	{
		WaitForSingleObject(hMutex, 10);         // 뮤텍스로 동기화
		
		temp = get();
		freq(temp);

		ReleaseMutex(hMutex);

		if (count == 0)    // 카운트 0 -> 비어있음 -> 기다린다!
		{
			for (int i = 0;i < 100; i++)
			{
				Sleep(1);
				if (count > 0)
				{
					i += 100;
				}
			}
		}
	}
	
	if (con == pro)
	{
		return 0;
	}
}

DWORD WINAPI Worker4(LPVOID Param)     // 4번 스레드 소비
{
	int temp = 0;
	Sleep(10);

	while (count > 2)          // 3번스레드와 동기화를 위해 카운트 범위를 다르게 설정
	{
		WaitForSingleObject(hMutex, 10);

		temp = get();
		freq(temp);

		ReleaseMutex(hMutex);

		if (count == 2)    // 카운트 0 -> 비어있음 -> 기다린다!
		{
			for (int i = 0;i < 100; i++)
			{
				Sleep(1);
				if (count > 2)
				{
					i += 100;
				}
			}
		}
	}

	if (con == pro)
	{
		return 0;
	}
}

void output()                // 단순 결과출력 함수
{
	printf("\n\n");
	
	printf("생산 갯수 = %d / ", pro);
	printf("소비 갯수 = %d / ", con);
	printf("생산 갯수 - 소비 갯수 = %d\n", count);

	printf("\n");
	for (int i = 0; i < 10;i++)
	{
		printf("LSD 빈도수 %d = %d , 확률 = %.2f\n", i, LSD_freq[i], (double)LSD_freq[i] /(double) pro * 100.0);
	}

	printf("\n");
	for (int i = 0; i < 10;i++)
	{
		printf("MSD 빈도수 %d = %d , 확률 = %.2f\n", i, MSD_freq[i], (double)MSD_freq[i] / (double)pro * 100.0);
	}
}


int main()
{
	DWORD ThreadId1, ThreadId2, ThreadId3, ThreadId4;
	HANDLE ThreadHandle1, ThreadHandle2, ThreadHandle3, ThreadHandle4;
	hMutex = CreateMutex(NULL, FALSE, NULL);

	printf("min\n");
	scanf("%d", &min);
	printf("max\n");
	scanf("%d", &max);

	int Param = (max - min) / 2;

	ThreadHandle1 = CreateThread(NULL, 0, Worker1, &Param, 0, &ThreadId1);
	ThreadHandle2 = CreateThread(NULL, 0, Worker2, &Param, 0, &ThreadId2);
	ThreadHandle3 = CreateThread(NULL, 0, Worker3, NULL, 0, &ThreadId3);
	ThreadHandle4 = CreateThread(NULL, 0, Worker4, NULL, 0, &ThreadId4);

	WaitForSingleObject(ThreadHandle1, INFINITE);    // 스레드가 종료하길 기다린다
	WaitForSingleObject(ThreadHandle2, INFINITE);
	WaitForSingleObject(ThreadHandle3, INFINITE);
	WaitForSingleObject(ThreadHandle4, INFINITE);

	CloseHandle(ThreadHandle1);
	CloseHandle(ThreadHandle2);
	CloseHandle(ThreadHandle3);
	CloseHandle(ThreadHandle4);
	CloseHandle(hMutex);
	
	output();

	return 0;
}


