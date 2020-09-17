#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<process.h>

int map[5][10];
int px, py;
int ey1, ey2, ey3;
int key;
int dice;
int coin;
int store1;
int store2;
int store3;
int end;
int count;
int num;
int re;
int last;
int eat;
int percent;

HANDLE hMutex;
void check();

void com1()                              //컴퓨터1 을 움직이게 하는 함수
{
	coin = rand() % 2;

	if (coin == 0 && ey1 != 0)
	{
		if (store1 == 2)
		{
			store1 = 0;
		}
		map[0][ey1] = store1;
		store1 = map[0][ey1 - 1];
		ey1--;
		map[0][ey1] = 4;
	}
	else if (coin == 1 && ey1 != 9)
	{
		if (store1 == 2)
		{
			store1 = 0;
		}
		map[0][ey1] = store1;
		store1 = map[0][ey1 + 1];
		ey1++;
		map[0][ey1] = 4;
	}
}

void com2()            // 컴퓨터2 를 움직이게 하는 함수
{
	coin = rand() % 2;

	if (coin == 0 && ey2 != 0)
	{
		if (store2 == 2)
		{
			store2 = 0;
		}
		map[2][ey2] = store2;
		store2 = map[2][ey2 - 1];
		ey2--;
		map[2][ey2] = 4;
	}
	else if (coin == 1 && ey2 != 9)
	{
		if (store2 == 2)
		{
			store2 = 0;
		}
		map[2][ey2] = store2;
		store2 = map[2][ey2 + 1];
		ey2++;
		map[2][ey2] = 4;
	}
}

void com3()                 // 컴퓨터3 을 움직이게 하는 함수
{
	coin = rand() % 2;

	if (coin == 0 && ey3 != 0)
	{
		if (store3 == 2)
		{
			store3 = 0;
		}
		map[4][ey3] = store3;
		store3 = map[4][ey3 - 1];
		ey3--;
		map[4][ey3] = 4;
	}
	else if (coin == 1 && ey3 != 9)
	{
		if (store3 == 2)
		{
			store3 = 0;
		}
		map[4][ey3] = store3;
		store3 = map[4][ey3 + 1];
		ey3++;
		map[4][ey3] = 4;
	}

}

unsigned _stdcall user(void* arg)      // 플레이어가 키를 입력받아 움직이게 하는 첫번째 스레드 함수
{
	while (last < 10)
	{
		if (end == 0)
		{
			fflush(stdin);
			key = _getch();
			if (key == 72 && px != 0)
			{
				if (map[px - 1][py] != 1)
				{
					map[px][py] = 0;
					px--;
					map[px][py] = 2;
				}
			}

			else if (key == 75)
			{
				if (py == 0)
				{
					if (px != 1 && px != 3)
					{
						map[px][py] = 0;
						py += 9;
						map[px][py] = 2;
					}
				}
				else
				{
					if (map[px][py - 1] != 1)
					{
						map[px][py] = 0;
						py--;
						map[px][py] = 2;
					}
				}
			}

			else if (key == 77)
			{
				if (py == 9)
				{
					if (px != 1 && px != 3)
					{
						map[px][py] = 0;
						py -= 9;
						map[px][py] = 2;
					}
				}
				else
				{
					if (map[px][py + 1] != 1)
					{
						map[px][py] = 0;
						py++;
						map[px][py] = 2;
					}
				}
			}

			else if (key == 80 && px != 4)
			{
				if (map[px + 1][py] != 1)
				{
					map[px][py] = 0;
					px++;
					map[px][py] = 2;
				}
			}
			check();
		}
	}
	return 0;
}

void set()           // 컴퓨터 1,2,3 이 처음 움직일때 있었던 자리를 공터로 만드는 함수
{
	Sleep(2000);

	if (ey1 != 0)
	{
		store1 = map[0][ey1 - 1];
		map[0][ey1] = 0;
		ey1--;
		map[0][ey1] = 4;
	}
	else if (ey1 == 0)
	{
		store1 = map[0][ey1 + 1];
		map[0][ey1] = 0;
		ey1++;
		map[0][ey1] = 4;
	}

	if (ey2 != 0)
	{
		store2 = map[2][ey2 - 1];
		map[2][ey2] = 0;
		ey2--;
		map[2][ey2] = 4;
	}
	else if (ey2 == 0)
	{
		store2 = map[2][ey2 + 1];
		map[2][ey2] = 0;
		ey2++;
		map[2][ey2] = 4;
	}

	if (ey3 != 0)
	{
		store3 = map[4][ey3 - 1];
		map[4][ey3] = 0;
		ey3--;
		map[4][ey3] = 4;
	}
	else if (ey3 == 0)
	{
		store3 = map[4][ey3 + 1];
		map[4][ey3] = 0;
		ey3++;
		map[4][ey3] = 4;
	}
}

unsigned _stdcall computer1(void* arg)          // 컴퓨터들을 움직이게 하는 함수
{
	while (last < 10)
	{
		set();
		count = 0;
		while (count < 5 && end < 1)
		{
			Sleep(2000);
			com1();
			com2();
			com3();
			count++;
			check();
		}
		while (5 <= count && end < 1)      // 카운트가 5가 되었을 때 속도가 빨라짐
		{
			Sleep(1000);
			com1();
			com2();
			com3();
			check();
		}
	}
	return 0;
}

void print()                // 단순 맵 찍어내는 함수
{
	int i, j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == 0)
			{
				printf("□");
			}
			if (map[i][j] == 1)
			{
				printf("■");
			}
			if (map[i][j] == 2)
			{
				printf("㉯");
			}
			if (map[i][j] == 3)
			{
				printf("Ａ");
			}
			if (map[i][j] == 4)
			{
				printf("※");
			}
		}
		printf("\n");
	}
}

void apple()           // 랜덤확률로 사과를 만들어 내는 함수
{
	int i, j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] != 1 && map[i][j] != 2 && map[i][j] != 4)
			{
				dice = rand() % 100+1;      // dice 변수를 랜덤으로 활용하여 확률을 만들어 냄
				if (dice > percent)
				{
					map[i][j] = 0;
				}
				if (dice <= percent)
				{
					map[i][j] = 3;
				}
			}
		}
	}
}

void wall()            // 벽 위치 정하는 함수
{
	for (int i = 0; i < 4; i++)
	{
		map[1][1 + i] = 1;
	}
	for (int i = 0; i < 3; i++)
	{
		map[1][6 + i] = 1;
	}
	for (int i = 0; i < 4; i++)
	{
		map[3][1 + i] = 1;
	}
	for (int i = 0; i < 3; i++)
	{
		map[3][6 + i] = 1;
	}
}

void enemy()                 // 컴퓨터의 위치를 랜덤으로 설정하는 함수
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == 4)
			{
				map[i][j] = 0;
			}
		}
	}
	ey1 = rand() % 10;
	ey2 = rand() % 10;
	ey3 = rand() % 10;
	map[0][ey1] = 4;
	map[2][ey2] = 4;
	map[4][ey3] = 4;
}

void check()          // 컴퓨터와 플레이어가 만났을 때 게임을 중단하게 만드는 함수
{
	if (px == 0 && py == ey1)
	{
		end += 2;
	}
	if (px == 2 && py == ey2)
	{
		end += 2;
	}
	if (px == 4 && py == ey3)
	{
		end += 2;
	}
}

void player()             // 플레이어의 위치를 랜덤으로 받는 함수
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == 2)
			{
				map[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < 100; i++)
	{
		px = rand() % 5;
		py = rand() % 10;

		if (map[px][py] == 1 || map[px][py] == 4)
		{
			i--;
		}
		else
		{
			i = 100;
		}
	}
	map[px][py] = 2;
}

void success()         // 사과를 다 먹었는지 확인하고 종료하게 만드는 함수
{
	eat = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == 3)
			{
				eat++;
			}
		}
	}
	if (eat == 0)
	{
		if (store1 != 3 && store2 != 3 && store3 != 3)
			end += 3;
	}
}

void ctn()
{
	if (end == 2)                 // 게임이 성공하던지 실패 했을때 다시 할건지 질의
	{
		printf("미션 실패\n");
	}
	else if (end == 3)
	{
		printf("미션 성공\n");
	}
	printf("regame?\nyes = 1, no = 2\n");
	fflush(stdin);
	scanf("%d", &re);
	if (re == 1)
	{
		last = 5;
	}
	else
	{
		last = 10;
	}
}

void main()            // 메인함수 : 쓰레드함수와 반복문을 통해 게임구현
{
	srand(time(NULL));
	wall();

	unsigned Threadld1;
	unsigned Threadld2;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	unsigned long hTherad2 = _beginthreadex(NULL, 0, computer1, NULL, 0, &Threadld2);
	unsigned long hTherad1 = _beginthreadex(NULL, 0, user, NULL, 0, &Threadld1);

	while (last < 10)     // 게임을 계속해서 할 수 있게 해주는 반복문
	{
		percent = 0;
		printf("확률 입력 :");
		fflush(stdin);
		scanf("%d", &percent);
		re = 0;
		end = 0;
		enemy();
		player();
		apple();

		while (end < 1)       // 게임 한판을 이루는 반복문    
		{
			system("cls");
			print();
			success();
		}
		ctn();
	}
}

