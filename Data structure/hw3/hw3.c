#include<stdio.h>
#include<malloc.h>

#define RANGE 0.001   //오차범위

typedef struct element { int numerator, denominator; };
typedef struct rat_node {	// 단순 연결리스트 노드 구조
	element ratio;		// 모두 양수 또는 음수일 수 있음
	struct rat_node *link;
} rat_node;

typedef struct rational {	// 다항식 표현의 구조체
	int length;
	rat_node *head;
} rational;

rational* create()         // 단순 리스트구조체 초기화
{
	rational *plist = (rational*)malloc(sizeof(rational));
	plist->length = 0;
	plist->head = NULL;
	return plist;
}

double makedouble(element value)         // 단순 실수값 계산기
{
	 return ((double)value.numerator) / ((double)value.denominator);
}

rat_node* get_node_at(rational *plist, element value)  // 노드 위치 찾아주는 함수
{
	rat_node *p = plist->head;
	rat_node *pre = plist->head;
	
	while (p->link != NULL && makedouble(value) >= makedouble(p->ratio))
	{
		pre = p;
		p = p->link;
	}
	if (makedouble(value) > makedouble(p->ratio))  // 가장 큰 값일때는 마지막 인덱스를 줌
	{
		return p;
	}
	else return pre;   // 그 외에는 그 전 인덱스를 줘서 사이에 들어가게 한다.
	
}

void insert(rational *plist ,rat_node *pre ,element value)  // 노드 추가 함수
{
	rat_node *p = (rat_node*)malloc(sizeof(rat_node));

	p->ratio = value;
	p->link = pre->link;    // pre 의 뒤에 추가된다.
	pre->link = p;
	plist->length += 1;
}

void insert_first(rational *plist, element value)  // 노드 추가 함수
{
	rat_node *p = (rat_node*)malloc(sizeof(rat_node));

	p->ratio = value;
	p->link = plist->head;  // 헤드앞에 생기게 된다
	plist->head = p;
	plist->length += 1;
}

void list_insert(rational *plist, element value)
{
	if(plist->head == NULL) 	insert_first(plist, value);  // 헤드가 널값일 경우
	else if (makedouble(value) < makedouble(plist->head->ratio))
	{
		insert_first(plist, value);   // 새로운 값이 헤드의 값보다 작을경우
	}
	else           // 새로운 값이 리스트 사이에 껴야 할 경우
	{
		rat_node *p = get_node_at(plist, value);
		insert(plist, p, value);
	}
}

rat_node* get_delete_at(rational *plist, double num)   // 노드 삭제 위치 반환
{
	rat_node *temp = plist->head;
	rat_node *pre = NULL;
	double positive;
	                               // 절대값 처리
	if (makedouble(temp->ratio) - num < 0) positive = num - makedouble(temp->ratio);
	else positive = makedouble(temp->ratio) - num;   

	while (positive > RANGE)   // 실수는  == 연산 안되므로 오차범위로 비교
	{
		pre = temp;
		temp = temp->link;
		                                   //절대값 처리
		if (makedouble(temp->ratio) - num < 0) positive = num - makedouble(temp->ratio);
		else positive = makedouble(temp->ratio) - num;

	}

	if (positive < RANGE)   // 값을 찾았으면 그 노드의 전노드를 알려준다
	{
		return pre;
	}
	else return NULL;  // 값이 없을경우 널값 반환
}

void Delte_first(rational *plist)   // 헤드포인터 노드 삭제
{
	if (plist->head != NULL)
	{
		rat_node *remove;
		remove = plist->head;
		plist->head = remove->link;
		free(remove);
	}
}

void Delete(rational *plist)   // 삭제 함수
{
	rat_node *pre = NULL;
	rat_node *temp = plist->head;

	int count = 0;
	double positive;
	double num[100];
	double input = 0;
	int i = 0;
	printf("삭제할 실수 목록 입력 '999' 입력시 입력 종료\n");
	while (i < 100 && input != 999)  // 삭제 값 입력받는다 100개 이하 or 999입력시 종료
	{
		scanf("%lf", &input);
		if(input != 999)	num[i++] = input;
	}

	if (i == 1)   // case 1 : 입력이 한개 그 숫자만 제거
	{
		if (makedouble(plist->head->ratio) - num[0] < 0)  // 절대값 처리
			positive = num[0] - makedouble(plist->head->ratio);
		else positive = makedouble(plist->head->ratio) - num[0];

		if (positive < RANGE)         // 헤드포인터가 찾는값일 경우 바로 삭제
		{
			Delte_first(plist);
			printf("한개 삭제\n");
		}
		else               // 찾는값이 뒤에 있을경우 위치 찾는 함수 호출
		{
			pre = get_delete_at(plist, num[0]);
			if (pre == NULL)      // 널 값을 반환받음
			{
				printf("삭제할 값이 존재하지 않습니다.\n");
			}
			else            // 값을 찾았을 경우 삭제
			{
				rat_node *remove = pre->link;
				pre->link = remove->link;
				free(remove);
				printf("한개 삭제\n");
			}
		}
	}
	////////////////////   

	else    // case 2 : 입력이 두개 이상 -> 최소, 최대값
	{
		int length = i;
		double min = num[0];
		double max = num[0];
		for (i = 0; i < length; i++)
		{
			if (num[i] < min)
			{
				min = num[i];
			}
			if (num[i] > max)
			{
				max = num[i];
			}
		}      // 최대 최소값 추출

		if (makedouble(plist->head->ratio) >= min)  // 헤드가 범위안에 포함될때
		{
			while (makedouble(plist->head->ratio) >= min && makedouble(plist->head->ratio) <= max)
			{
				Delte_first(plist);               // 헤드부터 쭈욱 지운다
				count++;
				if (plist->head == NULL)          // 마지막값 삭제 에러 처리
				{
					break;
				}
			}
			printf("%d개 삭제\n", count);
		}         
		else    // 헤드가 범위안에 속하지 않을때
		{
			count = 0;
			while (makedouble(temp->ratio) < min) // 삭제 시작부분 찾기
			{
				pre = temp;
				temp = temp->link;
			}

			while (makedouble(pre->link->ratio) <= max)  // 실제 삭제
			{
				rat_node *remove = pre->link;
				pre->link = remove->link;
				free(remove);
				count++;
				if (pre->link == NULL)
				{
					break;
				}
			}

			printf("%d개 삭제\n", count);
		}
	}

}      /// 완료

rational* reverse(rational *plist , rational *r_list)  // 리스트 뒤집기
{
	rat_node *temp = plist->head;

	while (temp != NULL)        // 기존 리스트의 헤드부터 새로운 리스트에 헤드에만 추가하면
	{                            // 스택처럼 알아서 뒤집혀준다
		insert_first(r_list, temp->ratio);
		temp = temp->link;
	}

	return r_list;  // 새로운 리스트 반환
}


void display(rational *plist)
{
	for (rat_node *p = plist->head; p != NULL; p = p->link)
	{
		printf("%d/%d= %.3f-> ", p->ratio.numerator, p->ratio.denominator, (float)p->ratio.numerator/ (float)p->ratio.denominator);
	}
	printf("NULL\n");
}

int main()
{
	rational *list;
	rational *r_list;
	element input;     // 입력받을 구조체 변수

	list = create();       // 리스트 초기화
	r_list = create();

	input.denominator = 1;
	while (input.denominator != 0)       // 입력, 추가
	{
		printf("분자 분모를 입력하세요 : ");
		scanf("%d %d", &input.numerator, &input.denominator);
		if(input.denominator != 0) list_insert(list, input);  // 0으로 나누기 연산 못하게 방지

		display(list);
	}

	Delete(list);     // 삭제 기능
	display(list);     

	printf("리스트 역순으로 돌리기\n");
	r_list = reverse(list , r_list);
	display(list);    // 기존 리스트
	display(r_list);   // 뒤집힌 리스트


	return 0;
}
