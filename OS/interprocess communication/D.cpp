#include<stdio.h>
#include<windows.h>

#define BUFFER_SIZE 50

int main()
{
	char buf[4096];
	int i = 0;
	double n = 0;
	double arr[3];
	double final;
	int line = 0;
	HANDLE ReadHandle;
	double buffer[1];
	FILE *fp;
	DWORD read;

	ReadHandle = GetStdHandle(STD_INPUT_HANDLE);


	    if (ReadFile(ReadHandle, buffer, 16, &read, NULL))   // 값을 전달 받는 버퍼! 
		{
			printf(">>D가 전달 받는 값 : %f\n", buffer[0]);
			printf("\n");
		}
		else
		{
			fprintf(stderr, "Child : Error reading from pipe\n");
		}

		fp = fopen("check.txt", "at");    // 전달 받을때 마다 파일에 저장
		fprintf(fp, "%f\n", buffer[0]);
		fclose(fp);

	// ----------------------------------------------

	fp = fopen("check.txt", "r");
	while (!feof(fp))   //파일의 끝을 만나지 않았다면 반복
	{
		line++;
		fgets(buf, sizeof(buf), fp);
	}
	fclose(fp);
	if (line == 4)       // 전달을 다 받으면 실행
	{
		fp = fopen("check.txt", "r");
		for (i = 0; i < 3; i++) {
			fscanf(fp, "%lf", &n);
			arr[i] = n;

		}
		printf("프로세스D 데이터 수신\n");
		printf("평균 : %f\n", arr[2]);       // 최종값 출력
		printf("표준편차 : %f\n", arr[1]);
		printf("중간값 : %f\n", arr[0]);
		final = (arr[2] - arr[0]) / arr[1];
		printf("평균과의 중간값의 차이를 표준편차로 나눈 값 : %f\n", final);


		int pid[1];
		pid[0] = GetCurrentProcessId();
		printf("프로세스D id : %d\n", pid[0]);   // PID 출력

		fclose(fp);
	}

	return 0;
}
