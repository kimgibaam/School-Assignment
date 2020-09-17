#include<iostream>
#include<conio.h>
#include<cstdlib>
#include<windows.h>
#include<ctime>
using namespace std;

class park {
private:
	int dice;
	int tall;
	int i, j, k;
	int px, py;
	int st , ln;
	int tx, ty, tz;
	int map[10][10][10];
	int p[10][10];
	int s[10][10];
public:
	int time;
	void init();
	void set();
	void s_move();
	void f_move();
	void print();
	void sky();
	void tree_m();
	void tree_p();
	void turn();
	void snow();
	void inform();
};

void park::init()         // 프린트 할때 쓰일 배열 초기화
{
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			p[i][j] = 0;
		}
	}
}

void park::set()    // 배열들 초기화 & 플레이어 위치 설정 & 나무크기를 처음에 랜덤으로 정함
{
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			for (k = 0; k < 10; k++)
			{
				map[i][j][k] = 0;
			}
		}
	}
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			s[i][j] = 0;
		}
	}

	px = 5;
	py = 5;
	p[py][px] = 1;
	tall = rand() % 3 + 2;
	if (tall == 2)
	{
		st = 1, ln = 8;
	}
	if (tall == 3)
	{
		st = 2, ln = 7;
	}
	if (tall == 4)
	{
		st = 3, ln = 6;
	}
}

void park::s_move()     // 키를 입력받아 상단면 좌표 설정하게함
{                           // (처음 정해진 트리크기 만큼만 움직일수 있음)
	int key;
    print();
	cout << "상단, 움직이고 스페이스바" << endl;
	key = _getch();
	cout << endl;
	while (key != 32)
	{
		key = _getch();
		if (key == 72 && py > st)
		{
			p[py][px] = 0;
			py--;
			p[py][px] = 1;
		}
		else if (key == 75 && px > st)
		{
			p[py][px] = 0;
			px--;
			p[py][px] = 1;
		}
		else if (key == 77 && px < ln)
		{
			p[py][px] = 0;
			px++;
			p[py][px] = 1;
		}
		else if (key == 80 && py < ln)
		{
			p[py][px] = 0;
			py++;
			p[py][px] = 1;
		}
		system("cls");
		print();
		cout << "상단, 움직이고 스페이스바" << endl;
		cout << endl;
	}
	tx = px;       // 설정한 좌표 삼차원배열에 좌표설정
	ty = py;
	system("cls");
	print();
	cout << "정면, 움직이고 스페이스바" << endl;
}

void park::f_move()      // 정면 좌표 설정
{
	int key;
	key = _getch();
	cout << endl;
	while (key != 32)
	{
		key = _getch();
		if (key == 72 && py > st)
		{
			p[py][px] = 0;
			py--;
			p[py][px] = 1;
		}
		else if (key == 80 && py != 9)
		{
			p[py][px] = 0;
			py++;
			p[py][px] = 1;
		}
		system("cls");
		print();
		cout << "정면, 움직이고 스페이스바" << endl;
		cout << endl;
	}
	tz = py;
}

void park::print()      // 단순 프린트 함수 
{
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (p[i][j] == 0)
			{
				cout <<"□";
			}
			if (p[i][j] == 1)
			{
				cout << "Ｐ";
			}
			if (p[i][j] == 2)
			{
				cout << "＠";
			}
			if (p[i][j] == 3)
			{
				cout << "＃";
			}
			if (p[i][j] == 4)
			{
				cout << "＆";
			}
			if (p[i][j] == 5)
			{
				cout << "●";
			}
			if (p[i][j] == 6)
			{
				cout << "□";
			}
		}
		cout << endl;
	}
}

void park::sky()    // 단순 프린트함수 (상단면 고정)
{
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (s[i][j] == 0)
			{
				cout << "□";
			}
			if (s[i][j] == 1)
			{
				cout << "Ｐ";
			}
			if (s[i][j] == 2)
			{
				cout << "＠";
			}
			if (s[i][j] == 3)
			{
				cout << "＃";
			}
			if (s[i][j] == 4)
			{
				cout << "＆";
			}
			if (s[i][j] == 5)
			{
				cout << "●";
			}
			if (s[i][j] == 6)
			{
				cout << "□";
			}
		}
		cout << endl;
	}
}

void park::tree_m()   // 삼차원 배열에 트리를 만드는 함수
{
	init();
	int x = 0;
	int y = 0;
	int z = 0;
	if (tall == 2)
	{
		y = 0;
		z = 1;
	}
	else if (tall == 3)
	{
		y = 1;
		z = 2;
	}
	else if (tall == 4)
	{
		y = 2;
		z = 3;
	}
	for (i = tz; i > tz - 2 - y; i--)
	{
		for (j = ty - z + x; j <= ty + z - x; j++)
		{
			for (k = tx - z + x; k <= tx + z - x; k++)
			{
				dice = rand() % 3 + 2;
				map[i][j][k] = dice;
			}
		}
		x++;
	}
}

void park::tree_p()     // 만들어진 트리를 프린트
{
	for (k = 9; k >= 0; k--)        // 위쪽 방향 출력
	{
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 10; j++)
			{
				if (map[k][i][j] != 0)
				{
					s[i][j] = map[k][i][j];
				}
			}
		}
	}
	system("cls");
	sky();
	init();

	for (k = 0; k < 10; k++)    // 정면 방향 출력
	{
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 10; j++)
			{
				if (map[i][k][j] != 0)
				{
					p[i][j] = map[i][k][j];
				}
			}
		}
	}
	cout << endl;
	print();
	inform();
}

void park::turn()    // 키를 입력받아서 방면을 움직이게 해줌
{
	int key;
	key = _getch();
	while (key != 32)
	{
		key = _getch();
		if (key == 72)
		{
			init();
			for (i = 0; i < 10; i++)    // 뒷면 방향 출력
			{
				for (k = 9; k >= 0; k--)
				{
					for (j = 0; j < 10; j++)
					{
						if (map[i][k][j] != 0)
						{
							p[i][9 - j] = map[i][k][j];
						}
					}
				}
			}
			system("cls");
			sky();
			cout << endl;
			print();
			inform();
		}
		if (key == 75)
		{
			init();

			for (i = 0; i < 10; i++)    // 좌 방향 출력
			{
				for (j = 0; j < 10; j++)
				{
					for (k = 9; k >= 0; k--)
					{
						if (map[i][j][k] != 0)
						{
							p[i][j] = map[i][j][k];
						}
					}
				}
			}
			system("cls");
			sky();
			cout << endl;
			print();
			inform();
		}
		if (key == 77)
		{
			init();

			for (i = 9; i >= 0; i--)    // 우 방향 출력
			{
				for (j = 0; j < 10; j++)
				{
					for (k = 0; k < 10; k++)
					{
						if (map[i][j][k] != 0)
						{
							p[i][9 - j] = map[i][j][k];
						}
					}
				}
			}
			system("cls");
			sky();
			cout << endl;
			print();
			inform();
		}
		if (key == 80)
		{
			init();
			for (i = 0; i < 10; i++)    // 정면 방향 출력
			{
				for (k = 0; k < 10; k++)
				{
					for (j = 0; j < 10; j++)
					{
						if (map[i][k][j] != 0)
						{
							p[i][j] = map[i][k][j];
						}
					}
				}
			}
			system("cls");
			sky();
			cout << endl;
			print();
			inform();
		}
	}
}

void park::inform()   // 게임할때 필요한 정보 찍는 함수

{
	cout << "좌키 : 왼쪽면" << endl;
	cout << "우키 : 오른쪽면" << endl;
	cout << "상키 : 뒷면" << endl;
	cout << "하키 : 정면" << endl;
	cout << "스페이스바 누르면 눈옴" << endl;
}  
void park::snow()   // 눈 찍어내고 , 내리게 하는 함수 (1초에 한칸)
{
	int count = 0;
	int sx, sy;
	int as;
	while (count < time)
	{
		sx = rand() % 10;
		sy = rand() % 10;
		for (i = 0; i < 100; i++)
		{
			if (map[0][sy][sx] != 0)
			{
				sx = rand() % 10;
				sy = rand() % 10;
			}
			else if (map[0][sy][sx] == 0)
			{
				i = 100;
			}
		}
		map[0][sy][sx] = 5;
		tree_p();

		for (k = 9; k >= 0; k--)
		{
			for (i = 0; i < 10; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if (map[9][i][j] == 5)
					{
						map[9][i][j] = 6;
					}
					if (map[k][i][j] == 5)
					{
						if (map[k + 1][i][j] != 0)
						{
							map[k][i][j] = 0;
						}
						else
						{
							map[k][i][j] = 0;
							map[k + 1][i][j] = 5;
						}
					}
				}
			}
		}
		Sleep(1000);
		tree_p();
		system("cls");
		count++;
	}
	tree_p();
}

int main()  // 메인함수
{
	srand(time(NULL));
	park one;
	cout << "초단위 시간 입력 : ";
	cin >> one.time;
	system("cls");
	one.init();
	one.set();
	one.s_move();
	one.f_move();
	one.tree_m();
	one.tree_p();
	one.turn();
	one.snow();
	one.turn();
	
	return 0;
}

