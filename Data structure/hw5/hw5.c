#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define size 7   // 인접행렬 사이즈

typedef struct GrapeType {    
	int n;
	int adj_mat[size][size];
}GrapeType;

void init_grape(GrapeType* g)   // 인접행렬 초기화
{
	g->n = size;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			g->adj_mat[i][j] = rand() % size;  // 숫자 랜덤설정
		}
	}
}

void print(GrapeType* g)         // 인접행렬 출력
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ",g->adj_mat[i][j]);
		}
		printf("\n");
	}
}

int make_max(GrapeType* g, int num, int temp, int i, int j)  // 연결된 개수 구하는 함수
{
	g->adj_mat[i][j] = 0;   // 먼저 방문한 곳은 다시 못들어가게 0으로 처리해줌
	int this_num = num;     // 총 개수 꼬이지 않게 변수 한개 따로 선언해서 씀

	if (j + 1 < size && g->adj_mat[i][j + 1] == temp)    // 분할 정복 활용
		num += make_max(g, this_num, temp, i, j + 1);    

	if (j - 1 >= 0 && g->adj_mat[i][j - 1] == temp)
		num += make_max(g, this_num, temp, i, j - 1);

	if (i - 1 >= 0 && g->adj_mat[i - 1][j] == temp)
		num += make_max(g, this_num, temp, i - 1, j);

	if (i + 1 < size && g->adj_mat[i + 1][j] == temp)
		num += make_max(g, this_num, temp, i + 1, j);

	
	num++;        
	return num;

}

int main()
{
	srand(time(NULL));    // 랜덤으로 계속해서 섞을 수 있음

	GrapeType* g = (GrapeType*)malloc(sizeof(GrapeType));  // 그래프 동적할당
	init_grape(g);   // 초기화
	print(g);     // 인접행렬 디스플레이

	int max_num[size] = { 0 };   // 각 숫자마다 연결된 가장 큰 갯수 저장
	int temp1 = 0;              // 임시 변수
	int temp2 = 0;             // 임시 변수

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (g->adj_mat[i][j] != 0)  // 모든 간선을 검사하되, 0 이 아닌 곳에서만 갯수 셈
			{
				temp1 = g->adj_mat[i][j];
				temp2 = make_max(g, 0, g->adj_mat[i][j], i, j);
				if (temp2 > max_num[temp1])  
					max_num[temp1] = temp2;
			}
		}
	}


	printf("\n");

	for (int i = 1; i < size; i++)  // 결과물 출력
	{
		printf("%d %d, ", i, max_num[i]);
	}


	return 0;
}
