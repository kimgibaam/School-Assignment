#include<stdio.h>
#include<malloc.h>

#define MAX_SIZE 100

int make_necklace(int arr[], int stack[], int top, int i);  // 목걸이를 실질적으로 만드는 함수
int necklace(int arr[], int size);                             // 목걸이를 만들수 있는지를 검사하는 함수

int necklace(int arr[], int size)
{
	int *stack = (int *)malloc(size * sizeof(int));        // 동적 할당된 스택
	int top = -1;                                               // 스택의 인덱스
	int index;
	int max = 0;
	int num = 0;

	for (int i = 0; i < size; i++)
	{
		stack[++top] = arr[i];           // 스택에 하나씩 값을 넣는다.

		if (arr[i] == arr[i + 1])                // 목걸이를 만들수 있다면
		{
			num = make_necklace(arr, stack, top, i);     // 목걸이를 만든다.
			if (num > max)                // 구슬 수가 기존의 목걸이보다 많으면
			{
				max = num;        // 스왑한다.
				index = i;
			}
		}
	}
	index = index - (max / 2) + 1;           // 단순 자릿수 계산
	printf("\n%d번째 수 %d부터 구슬이 %d개인 목걸이\n", (index + 1), arr[index], max);

	return 0;
}

int make_necklace(int arr[], int stack[], int top, int i)
{
	int num = 0;

	while (stack[top] == arr[i + 1] && top >= 0)        // 구슬수를 센다.
	{
		num += 2;
		top--;
		i += 1;
	}
	return num;
}

int main()
{
	int *temp = (int *)malloc(MAX_SIZE * sizeof(int));     // 임시로 숫자를 받을 그릇 100개까지 숫자를 받을수 있다.
	int num;
	int count = 0;

	printf("1~10까지 숫자 한개씩 입력\n0 입력시 입력 종료\n");

	int i = 0;
	while (i < MAX_SIZE)
	{
		scanf("%d", &num);      // 숫자를 한개씩 입력, 0입력시 입력 끝

		if (num != 0)
		{
			temp[i] = num;
			i++;
		}
		else
		{
			count = i;           // 0이 눌렸을때의 숫자 갯수를 저장하기 위함
			i = MAX_SIZE;     // 루프 탈출
		}
	}

	int *p = (int *)malloc(count * sizeof(int));     // 입력받은 숫자만큼만 동적할당
	for (i = 0; i < count; i++)         // 값 옮김
	{
		p[i] = temp[i];
	}
	free(temp);           // 임시 메모리공간은 반납한다.

	for (i = 0; i < count; i++)    // 입력받은값 출력
	{
		printf("%d ", p[i]);
	}

	necklace(p, count);

	return 0;
}