#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

typedef struct element {      // 데이터 기록할 구조체
	char name[20];
	int key;             //무게는 키로 설정
	int volume;
}element;

typedef struct Node {
	element* data;     // 여기로 접근하려면 나중에 메모리할당 해주어야됨
	struct Node *link;
}Node;

int hash(int x)
{
	return x % 31;
}

Node* insert(Node* head, element* value)
{
	Node* p = (Node*)malloc(sizeof(Node));
	element* dp = (element*)malloc(sizeof(element));  

	p->data = dp;

	p->data->key = value->key;
	p->data->volume = value->volume;
	strcpy(p->data->name,value->name);


	p->link = NULL;

	Node* temp = head;

	if (head == NULL)
	{
		head = p;
		return head;
	}

	printf("test");

	while (temp->link != NULL)
	{
		temp = temp->link;
	}

	temp->link = p;

	return head;
}

element* search(Node * head, int input)
{
	Node* temp = head;

	while (temp != NULL)
	{
		if (temp->data->key == input)
		{
			return temp->data;
		}
		else
		{
			temp = temp->link;
		}
	}

	return NULL;
}

void init(Node* table[])
{
	for (int i = 0; i < 31; i++)
	{
		table[i] = NULL;
	}
}

int main()
{

	Node* table[31];    // 주소 초기화 필요
	init(table);

	element* temp = (element*)malloc(sizeof(element));


	FILE *fp = fopen("input.txt", "r");    //읽기 모드로 입력파일 오픈

	for (int i = 0; i < 20; i++)
	{
		fscanf(fp, "%s %d %d ", &(temp->name), &(temp->key), &(temp->volume));  //파일 내용 읽기
		
		table[hash(temp->key)] = insert(table[hash(temp->key)], temp);         //테이블에 넣기

		printf("%s %d %d 테이블에 추가 완료\n", temp->name, temp->key, temp->volume);
	}
	fclose(fp);

	int input;
	int index;

	printf("찾는 물건의 무게를 입력 : ");
	scanf_s("%d", &input);
	index = hash(input);

	element* result = search(table[index], input);

	if (result != NULL)
		printf("찾는 물건 : %s %d %d\n", result->name, result->key, result->volume);
	else
		printf("없습니다");



	return 0;
}

