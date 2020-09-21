#include<stdio.h>
#include<malloc.h>

#define MAX_SIZE 100   // 스택사이즈

typedef struct element { 
	int numerator, denominator; 
};

typedef struct TreeNode {	
	element key;		
	struct TreeNode *left,*right;
} TreeNode;

int top = -1;
TreeNode *stack[MAX_SIZE];

void push(TreeNode *p)   // 스택 푸시연산
{
	if (top < MAX_SIZE - 1)
		stack[++top] = p;
}

TreeNode *pop()   // 스택 팝 연산
{
	TreeNode *p = NULL;
	if (top >= 0)
		p = stack[top--];
	return p;
}

double makeR(element value)    // element 분수값을 실수로 만들어냄
{
	return ((double)value.numerator / (double)value.denominator);  // 양쪽다 더블처리 해줘야됨
}

TreeNode* search(TreeNode*node, element key)   // 이진탐색트리 탐색연산
{
	if (node == NULL) return NULL;
	if (makeR(key) > makeR(node->key))   // 실수비교하면 연산오류난다
		return search(node->right, key);
	else if (makeR(key) < makeR(node->key))
		return search(node->left, key);
	else
		return node;
}

TreeNode *new_node(element item)   // 새노드 삽입 함수
{
	TreeNode *temp = (TreeNode*)malloc(sizeof(TreeNode));
	temp->key = item;
	temp->left = temp->right = NULL;
	return temp;
}

TreeNode *insert_node(TreeNode *node, element key)  // 노드 삽입 장소 찾아 삽입
{
	if (node == NULL) return new_node(key);
	if (makeR(key) <= makeR(node->key))    // 같은값은 왼쪽으로 가게 처리
	{
		node->left = insert_node(node->left, key);
	}
	else if (makeR(key) > makeR(node->key))
	{
		node->right = insert_node(node->right, key);
	}

	return node;
}

TreeNode *min_value_node(TreeNode *node)  // 파라미터 노드를 기준 가장 작은값의 노드 찾음
{
	TreeNode *current = node;

	while (current->left != NULL)
	{
		current = current->left;
	}
	return current;
}

TreeNode *delete_node(TreeNode* root, element key)   // 기본삭제 함수
{

	if (root == NULL) return NULL;
	if (makeR(key) < makeR(root->key))
		root->left = delete_node(root->left, key);   // 삭제된 노드의 자식이랑 이어준다
	else if (makeR(key) > makeR(root->key))
		root->right = delete_node(root->right, key);

	else
	{
		if (root->left == NULL)
		{
			TreeNode *temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			TreeNode *temp = root->left;
			free(root);
			return temp;
		}
		TreeNode *temp = min_value_node(root->right);
		root->key = temp->key;
		root->right = delete_node(root->right, temp->key);
	}
	return root;
}


void inorder(TreeNode *root)   // 기본 중위순회 함수
{
	if (root)
	{
		inorder(root->left);
		printf("[%lf] ", makeR(root->key));
		inorder(root->right);
	}
}

void preorder_traverse(TreeNode *root)  // 전위순회 함수
{
	if (root)
	{
		printf("[%lf] ", makeR(root->key));
		preorder_traverse(root->left);
		preorder_traverse(root->right);   // 복붙도 잘해야됨
	}
}

double find_max(TreeNode *root)   // 가장 큰 값 찾는 함수
{
	while (root->right != NULL)
	{
		root = root->right;
	}
	return makeR(root->key);
}

int get_level(TreeNode *root, TreeNode *node, int *num) // 현재 노드의 레벨을 알려주는 연산
{
	if (root == NULL) return NULL;

	if (makeR(root->key) > makeR(node->key))   // 키값이 작거나 크면 레벨을 하나 올리고 밑의 노드를 방문
	{
		*num += 1;
		return get_level(root->left, node, num);  // 리턴해줘야 함수가 끝나고 넘어간다
	}

	if (makeR(root->key) < makeR(node->key))
	{
		*num += 1;
		return get_level(root->right, node , num);
	}
	else                 // 키값을 찾았을때
	{
		if (root != node)   // 값은값이 여러가지 있을경우 키값은 같으나 주소값이 다를수 있다 
		{                  //  이때는 삽입할때 장치해둔 대로 왼쪽으로 넘어간다
			*num += 1;
			return get_level(root->left, node , num);
		}
		else              // 노드를 제대로 찾았을 경우,
		{
			int temp = *num;  // 미리 레벨값 옮겨두고
			*num = 1;       // 레벨 값을 다시 초기화 해주고
			return temp;   // 옮겨둔 값을 반환한다
		}
	}

}

double level_sum(TreeNode *root, int level , int *num)  // 레벨합 연산 함수
{
	TreeNode *temp = root;
	double sum = 0.0;

	while (1)         // 반복 중위순회 기반으로 모든 노드의 레벨 검사
	{
		for (; root; root = root->left)
			push(root);
		root = pop();
		if (!root) break;

		if (level == get_level(temp,root, num))  // 현재 노드가 입력된 레벨과 같다면
			sum += makeR(root->key);             // 더해준다
		root = root->right;
	}

	return sum;   
}

void children(TreeNode *root, int num , int *child_num)  // 자식노드수 연산
{
	if (root)         // 중위 순위를 기반으로 모든 노드의 자식수를 검사한다
	{
		children(root->left,num , child_num);

		switch (num)           
		{
		case 0 :   // 입력이 0 : 자식이 없는 노드의 수 구하기
			if (root->left == NULL && root->right == NULL)
				*child_num += 1;
			break;
		case 1:    // 입력이 1 : 자식이 한개인 노드의 수 구하기
			if ((root->left == NULL && root->right != NULL) || (root->left != NULL && root->right == NULL))
				*child_num += 1;
			break;
		case 2:    // 입력이 2 : 자식이 두개인 노드의 수 구하기
			if (root->left != NULL && root->right != NULL)
				*child_num += 1;
			break;

		}

		children(root->right,num , child_num);
	}
}

void inorder_search(TreeNode *root, int num , int *i)  // k번째 수 찾기 함수
{
	if (root)   // 중위순회 기반으로 k번째수 간단히 알수 있다
	{
		inorder_search(root->left,num,i);
		if (*i == num)
		{
			printf("%d번째 수 : [%lf] ", num ,makeR(root->key));
		}
		*i += 1;             // 포인터 변수에 역참조연산할때 증감연산자 사용불가
		inorder_search(root->right,num,i);
	}
}

int main()
{
	TreeNode *root = NULL;    // 루트노드 포인터 변수
	element input;          // 입력받을 구조체 변수
	int count_delete = 0;    // 삭제된 노드 갯수 변수
	int num;                // 단순 입력 변수
	int count_level = 1;   // 해당 노드의 레벨을 알아내기 위한 포인터변수
	int child_num = 0;     // 해당 자식노드 수를 가진 노드 카운팅 변수
	int i = 1;           // k번째 수를 찾기위한 인덱스 변수 
	
	////////////////// 입력기능
	input.denominator = 1;
	while (input.denominator != 0)      
	{
		printf("삽입할 분자 분모를 입력하세요 : ");   // 값 입력
		scanf("%d %d", &input.numerator, &input.denominator);
		if (input.denominator != 0)    // 삽입
		{
			root = insert_node(root, input);
			inorder(root);  // 중위 순회로 삽입된 값 확인
			printf("\n");
		}
	}
	printf("----삽입 완료 ----\n");

	//////////////////// 삭제기능
	printf("삭제할 분자 분모를 입력하세요 : "); 
	scanf("%d %d", &input.numerator, &input.denominator);

	while (search(root, input) != NULL)   // NULL 조건 정확히 명시해준다
	{                                     // 삭제할 값이 노드에 있다면 계속 반복
		root = delete_node(root, input);
		count_delete++;                  // 삭제될 때마다 카운트 증가
	}
	inorder(root);  // 중위 순회로 삭제된 후의 노드들 확인
	printf("\n삭제된 노드 개수 : %d\n", count_delete);

	//////////////// 가장 큰값 찾기 기능
	printf("\n가장 큰 값 : %lf\n", find_max(root));

	////////////// 단순 전위순회
	printf("\n전위순회\n");
	preorder_traverse(root);
	printf("\n");

	/////////////// k번째 수 찾기 기능
	printf("\nk번째 수 입력 : ");
	scanf("%d", &num);
	inorder_search(root,num,&i);
	printf("\n");

	//////////////// 자식노드 기능
	printf("\n0~2의 숫자입력 : ");
	scanf("%d", &num);
	children(root, num, &child_num);
	printf("%d개의 자식노드를 가지고 노드 : %d개\n", num ,child_num);

	/////////////////// 레벨합 기능
	printf("\n합 연산할 레벨입력 : ");
	scanf("%d", &num);
	printf("레벨합 : %lf\n", level_sum(root, num , &count_level));

	return 0;
}