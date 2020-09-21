#include<stdio.h>
#include<time.h>
#include<malloc.h>

void left_up(int arr[], int step, int* num , int *top);  // 왼발이 움직일때의 함수
void right_up(int arr[], int step, int* num , int *top); // 오른발이 움직일때의 함수
void print(int arr[], int *top);   // 단순 프린트함수

int main()
{
	clock_t start, finish;  // 타이머 변수
	double duration;        // 시간 기록할 변수

	int topnum = -1;             // 배열 인덱스로 쓸 변수
	int *top;                      // 인덱스 수정을 위해 포인터로 사용함
	top = &topnum;

	int step_m;               // 올라가야 할 계단 수
	int result = 0;             // 총 경우의 수
	int* num_m = &result;        // 경우의 수를 받기위한 포인터
       
	int *ptr;    // 걸음 진행 상황을 담아 놓을 포인터

	printf("0이 아닌 계단수 입력 : ");
	scanf("%d", &step_m);                         // 계단 수 입력
	printf("\n");

	ptr = (int*)malloc(step_m * sizeof(int));   // 계단 수에 맞춰서 동적 할당 

	start = clock();           // 걸음수 입력부터 타이머시작

	if (step_m > 0)             // 계단수가 0보다 클때만 수행
	{
		printf("왼발이 먼저 시작하는 경우\n");
		left_up(ptr, step_m, num_m, top);            // 왼발먼저 출발할 때의 경우의 수

		printf("오른발이 먼저 시작하는 경우\n");
		right_up(ptr, step_m, num_m , top);        // 오른발 먼저 출발할 때의 경우의 수
	}
	free(ptr);

	printf("모든 경우의 수 : %d\n", result);

	finish = clock();          // 타이머 종료
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("수행시간 : %f\n", duration);

	return 0;

}

void print(int arr[] , int *top)
{
	for (int i = 0; i <= *top; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void left_up(int arr[], int step, int* num , int *top)
{
	if (step == 0)         // 더이상 오를 계단이 없을때 프린트하고 경우의 수 한개 올림
	{
		print(arr, top);
		*num += 1;
		return;
	}

	for (int a = 3; a > 0; a--)    // 왼발이 1~3발일때의 모든경우의 수를 세기위한 반복문
	{
		if (step < a) a = step;      // 남은 계단 수가 보폭보다 작을경우 보폭을 낮춤

		*top += 1;                 // 인덱스를 하나 올림
		arr[*top] = a;            // 보폭수를 배열 끝에 넣고
		right_up(arr, step - a, num , top);   // 남은 계단수를 빼고 오른발 함수를 호출
		*top -= 1;        // 오른발 함수를 호출하여 모든 경우의 수를 세었기 때문에 
		                 // 인덱스 하나를 줄이고 보폭수를 낮춰서 다음 경우의 수를 계산하게함
	}
}

void right_up(int arr[], int step, int* num , int *top)   // 왼발과 보폭수만 다르고 기능이 같음
{ 
	if (step == 0)
	{
		print(arr , top);
		*num += 1;
		return;
	}

	for (int a = 1; a >= 0; a--)
	{
		*top += 1;
		arr[*top] = a;
		left_up(arr, step - a, num , top);
		*top -= 1;
	}
}