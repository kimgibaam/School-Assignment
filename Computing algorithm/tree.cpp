// 컴퓨터정보통신공학 172847 김기범

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAX 100

typedef struct data {      // 데이터 기록할 구조체
	char name[20];
	int key;             //무게는 키로 설정
	int volume;
}DATA;

typedef struct node{          // 노드
	struct data stuff;        // 데이터 저장할 구조체
    struct node *left;          // 왼쪽 자식
    struct node *right;         // 오른쪽 자식
    struct node *p;               //부모노드
}NODE;

NODE* search_node( NODE *, int);
void insert_node(NODE **, char name[] ,int , int);

int main(void)
{
	DATA one;
	NODE *root = NULL;

	FILE *fp = fopen("input.txt", "r");    //읽기 모드로 입력파일 오픈

	for (int i = 0; i < 16; i++)       
	{
		fscanf(fp, "%s %d %d ", &one.name , &one.key, &one.volume);  //파일 내용 읽기
		insert_node(&root, one.name, one.key, one.volume);         //트리에 넣기
		printf("%s %d %d 트리에 추가 완료\n", one.name, one.key, one.volume);
	}
	fclose(fp);
	printf("\n");

	int a;

	printf("찾을 무게값 입력 : ");
	scanf("%d", &a);           // scanf 안에 \n 넣으면 버퍼 문제생긴다.

	NODE *b = search_node(root, a);         // 입력한 값 트리에서 찾기
	if (b == NULL) printf("트리에 없는 값입니다\n");       // 없는 값 입력
	else printf("%s %d %d\n", b->stuff.name, b->stuff.key, b->stuff.volume);  // 찾고 출력

	return 0;
}

// 노드 탐색 함수
NODE* search_node(NODE *node, int key)
{
	while (node != NULL && node->stuff.key != key)  // 노드가 비었거나 노드키가 찾는 값이 아닐때 반복
	{
		if (node->stuff.key > key)     // 노드의키가 찾는값보다 크면
		{
			node = node->left;          // 왼쪽 자식노드로 이동
		}
		else
		{
			node = node->right;
		}
	}

	return node;
}

// 노드 추가 함수
void insert_node(NODE **node, char name[], int key ,int volume)
{
	NODE *n = (NODE *)malloc(sizeof(NODE));  // 메모리 할당
	NODE *temp = *node; // 임시 노드

	n->stuff.key = key;
	n->stuff.volume = volume;
	strcpy(n->stuff.name, name);    // 문자열 복사

	n->p = NULL;   // 부모노드
	n->left = NULL;
	n->right = NULL;

	// 노드가 없을 경우
	if (temp == NULL)
	{
		*node = n;        // 노드 추가

		return;
	}

	// 삽입될 위치를 찾는다
	while (temp != NULL)    // 현재노드가 널값이 아니라면
	{
		n->p = temp;   // 현재노드가 부모노드가 된다

		if (temp->stuff.key > key)
		{
			temp = temp->left;    // 현재노드가 키값보다 크다면 왼쪽 노드로 이동
		}
		else
		{
			temp = temp->right;
		}
	}

	// 트리 노드 추가
	if ((n->p)->stuff.key > key)  // 부모노드의 키값이 키값보다 크면
	{
		(n->p)->left = n;     // 새로 할당된 노드가 왼쪽자식노드가 된다.
	}
	else
	{
		(n->p)->right = n;
	}
}