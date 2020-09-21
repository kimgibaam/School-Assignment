#include<stdio.h>
#include<stdlib.h>

#define SWAP(x,y,t) ((t)=(x),(x)=(y),(y)=(t))
#define MAX_SIZE 8

int partition(int list[], int left, int right)  
{
	// 책의 내용 그대로의 파티션 함수

	int pivot, temp;
	int low, high;

	low = left;
	high = right + 1;
	pivot = list[left];

	do {
		do
			low++;
		while (list[low] < pivot);

		do
			high--;
		while (list[high] > pivot);

		if (low < high) SWAP(list[low], list[high], temp);
	} while (low < high);

	SWAP(list[left], list[high], temp);

	return high;
}

int selet(int list[], int left, int right, int i)
{
	// 파티션 함수를 이용한 선택함수

	if (left == right)
		return list[left];

	int q = partition(list, left, right); // 파티션

	int k = q - left + 1;

	if (i < k)
		return selet(list, left, q - 1, i);
	else if (i == k)  
		return list[q];
	else
		return selet(list, q + 1, right, i - k);
}

void main()
{
	// 리스트 초기화
	int list[8] = { 6 , 9 , 11 , 3 , 8 , 2 , 7 , 10 };  

	int k;

	while (1)
	{
		while (1)
		{
			printf("k값은 ? ");
			scanf_s("%d", &k);

			if (k == 0)
				break;
			else if (k < 1 || k > 8)
			{
				printf("1~8 의 값만 입력\n");
			}
			else
				break;
		}

		if (k == 0)
		{
			printf("종료합니다");
			break;
		}

		int result = selet(list, 0, 7, k);

		printf("%d 입니다\n", result);

	}
}