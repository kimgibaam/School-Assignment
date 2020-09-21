#include<stdio.h>
#include<windows.h>
#include<time.h>

int main()
{
	clock_t start1, end1;
	float res1;
	
	int i,j;
	int count = 0;
	int temp = 0;
	int min, max = 0;


	printf("min\n");             // 숫자 두개 입력
	scanf("%d", &min);
	printf("max\n");
	scanf("%d", &max);

	start1 = clock();             // 타이머 시작

	for (i = min; i <= max; i++)       // 소수판별
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

	end1 = clock();                  // 타이머 끝
	res1 = (float)(end1 - start1) / CLOCKS_PER_SEC;

	printf("소수 갯수 : %d\n", count);         // 결과 출력
	printf("수행시간 : %.3f", res1);

}