#include<stdio.h>
#include<windows.h>
#include <tchar.h>

#define BUFFER_SIZE 500

int count = 0;
int temp = 0;

typedef struct NodeStruct           // 이진탐색트리 구조체
{
	int value;
	struct NodeStruct* leftChild;
	struct NodeStruct* rightChild;
}Node;

Node* root = NULL;

Node* BST_insert(Node* root, int value)         // 삽입함수
{
	if (root == NULL)
	{
		root = (Node*)malloc(sizeof(Node));
		root->leftChild = root->rightChild = NULL;
		root->value = value;
		return root;
	}
	else
	{
		if (root->value > value)
			root->leftChild = BST_insert(root->leftChild, value);
		else
			root->rightChild = BST_insert(root->rightChild, value);
	}
	return root;
}

void BST_print(Node* root)        // 출력 함수
{
	if (root == NULL)
		return;

	printf("%d ", root->value);
	BST_print(root->leftChild);
	BST_print(root->rightChild);
}

void InOrderPrintTree(Node* root)         // 중위순회 함수
{
	if (root == NULL)
		return;

	// 왼쪽 하위 트리 출력
	InOrderPrintTree(root->leftChild);

	// 부모 노드 출력
	printf(" %d", root->value);

	count++;
	if (count == 50)          // 중간값 적출 위한 카운트
	{
		temp = root->value;
	}

	// 오른쪽 하위 트리 출력
	InOrderPrintTree(root->rightChild);
}

int main()
{
	printf("프로세스B 데이터 수신\n");

	double a[1];
	HANDLE ReadHandle;
	int buffer[BUFFER_SIZE];    
	DWORD read;

	ReadHandle = GetStdHandle(STD_INPUT_HANDLE);

	if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))      // a에게 숫자 전달받음
	{
		for (int i = 0;i < 100;i++)
		{
			printf("%d ", buffer[i]);
		}
	}
	else
	{
		fprintf(stderr, "Child : Error reading from pipe\n");
	}
	printf("\n");

	// ----------------------------------------------
	
	for (int i = 0; i < 100;i++)
	{
		root = BST_insert(root, buffer[i]);   // 받은 숫자들로 이진탐색트리 구성
	}
	printf("이진탐색트리 출력\n");
	BST_print(root);                 // 출력
	printf("\n");

	printf("중위순회 실행\n");
	InOrderPrintTree(root);       // 중위순회
	printf("\n");

	a[0] = temp;
	printf("중간값 : %f\n", a[0]);     // 중간값 출력

	int pid[1];
	pid[0] = GetCurrentProcessId();
	printf("프로세스B id : %d\n", pid[0]);   // PID 출력
	printf("\n");

	// --------------------------------------------------

	HANDLE ReadHandle1, WriteHandle1;        // D 에게 중간값 전달할 파이프
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	DWORD written;

	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

	ZeroMemory(&pi, sizeof(pi));

	if (!CreatePipe(&ReadHandle1, &WriteHandle1, &sa, 0))
	{
		fprintf(stderr, "Create Pipe Failed\n");
		return 1;
	}

	GetStartupInfo(&si);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	si.hStdInput = ReadHandle1;
	si.dwFlags = STARTF_USESTDHANDLES;

	SetHandleInformation(WriteHandle1, HANDLE_FLAG_INHERIT, 0);

	TCHAR command[] = _T("check.exe");
	CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	CloseHandle(ReadHandle1);

	if (!WriteFile(WriteHandle1, a, 8, &written, NULL))//num buffersize 4는 기록할 데이터의 길이
		fprintf(stderr, "Error writing to pipe\n");

	CloseHandle(WriteHandle1);

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// -----------------------------------------------
	
	return 0;
}
