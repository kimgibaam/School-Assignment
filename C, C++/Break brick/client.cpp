#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define PORT	5500
#define BUFFERSIZE 100

class game {   // 맵 생성 부모 클래스
protected:		  // 자식 클래스에 상속 가능
	int imap[10][6];  // 전체 맵 변수
public:
	void Draw_Map();
};

class Ball : public game {  // 공 조정 자식 클래스 (부모 클래스 상속)
private:
	int dr;
	int number;
	int localtime;
	int map[10][6];
	int ballx, bally;   // 볼의 좌표
public:
	int Soc_Cli();
	Ball();
	~Ball();
	void Init_G();		// 초기화 및 전체 프로그램 조정
	void Move_Ball();	// 공 쏘기
	void Get_Map();		// 부모 클래스에서 생성된 맵을 받음
	int Get_Val(int i, int j);
	void Key_In();		// 공 쏘기 위한 키 선택
	void check();

	void Move_Up();  // 수직으로 상승
	void Move_Right();  // 오른쪽으로 상승
	void Move_Left();  // 왼쪽으로 상승

	void Move_Down(); // 하강 조정
	void Get_Down(); // 수직으로 하강
	void Right_Down();//오른쪽으로 하강
	void Left_Down(); // 왼쪽으로 하강 

	void Print_Ball(); // 맵출력
};

Ball::Ball()
{
	localtime = 0;
}  // 기본 생성자

Ball::~Ball()   // 소켓 종료 후 객체가 소멸될때 "연결 종료" 출력
{
	cout << "연결 종료" << endl;

}

void game::Draw_Map()
{
	int a, b, c;
	int rd;

	srand(time(NULL));
	c = 0;
	rd = rand() % 3; // 랜덤으로 벽돌 모양 지정

	for (a = 0; a < 10; a++) {
		for (b = 0; b < 6; b++) {
			if (a <= 2) {   // 벽돌 모양 지정
				if (rd == 0) imap[a][b] = 1;
				if (rd == 1) imap[a][b] = 2;
				if (rd == 2) imap[a][b] = 3;
				++c;
			}
			else imap[a][b] = 0;  // 나머지는 빈 공간
			if (c == 3) {
				c = 0;   // 벽돌 크기 지정
				rd = rand() % 3; // 랜덤으로 벽돌 모양 지정
			}
		}
	}
}

void Ball::Init_G()
{
	Draw_Map();  // 화면 출력

	Move_Ball(); // 공 조정
}

void Ball::Move_Ball()
{
	int a;
	Get_Map();   // 맵 획득
	bally = 9;
	dr = rand() % 6;  // 최초 x 좌표 설정
	ballx = dr;
	map[bally][ballx] = 4;  // 최초 공 설정

	Print_Ball();  // 처음 화면
	Key_In();      // 게임 시작
}

void Ball::Get_Map() // 부모클래스의 맵 정보 받음
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 6; j++) {
			map[i][j] = Get_Val(i, j);
		}
	}
}

int Ball::Get_Val(int i, int j)  // 부모클래스의 맵 정보 받음
{
	return imap[i][j]; // 부모클래스의 배열 imap의 정보를 받음
}


void Ball::Key_In()  // 키 입력
{
	number = 0;
	while (number != 1) {  // 무한 루프

		dr = rand() % 3;
		if (dr == 0) Move_Up();
		if (dr == 1) Move_Right();
		if (dr == 2) Move_Left();
		localtime++;
		check();    // 체크 함수 넣어줘서 벽돌다 깼으면 반복문 종료
	}

	cout << "로컬타임 : " << localtime << endl;   // 게임종료후 로컬타임 출력
	cout << endl;
}

void Ball::check()    // 공 움직임 끝난후 벽돌개수 체크
{
	int a, b;
	number = 0;
	for (a = 0; a < 10; a++)
	{
		for (b = 0; b < 6; b++)
		{
			if (map[a][b] != 0)
			{
				number++;
			}
		}
	}
}

void Ball::Move_Up()  // 수직 상승
{
	int n, m;  // 공의 좌표 임시저장 공간
	int cnt;

	cnt = bally;  // 높이 y 좌표 설정
	while (cnt > 0) {

		if (map[bally - 1][ballx] == 0)
		{
			system("cls"); // 화면 갱신

			m = ballx;  // 이전 좌표 확보
			n = bally;
			map[n][m] = 0;  // 이전 좌표 공을 무효화
			--bally;    // 공의 수직 상승
			map[bally][ballx] = 4;

			Print_Ball();  // 공표시
			Sleep(30);
			--cnt;
		}
		else if (map[bally - 1][ballx] != 0)   
		{
			system("cls"); // 화면 갱신
			map[bally - 1][ballx] = 0;
			Print_Ball();  // 공표시
			Sleep(30);
			cnt = 0;
		}
	}
	Move_Down();
}

void Ball::Move_Right()  // 우측 상승
{
	int n, m;
	int rd, cnt;
	int tg = 0;

	cnt = bally;    // y 좌표 설정
	while (cnt > 0) {

		if (map[bally - 1][ballx] == 0 && map[bally - 1][ballx + 1] == 0)
		{
			system("cls");  // 화면 갱신

			m = ballx;   // 이전 좌표 확보
			n = bally;
			map[n][m] = 0;  // 이전 공 삭제
			--bally;    // 상승

			if (ballx < 5) ballx++;  // x 좌표 범위 안에서 증가
			if (ballx == 5) cnt = 0;  // x 좌표 초과 시 증가 멈춤

			map[bally][ballx] = 4;  // 공표시

			Print_Ball();  // 출력
			Sleep(30);
			--cnt;
		}
		else if (map[bally - 1][ballx] != 0 || map[bally - 1][ballx + 1] != 0)
		{
			system("cls");
			map[bally - 1][ballx] = 0;
			Print_Ball();  // 출력
			Sleep(30);
			cnt = 0;
			tg++;
		}
	}

	if (bally > 0)
	{
		if (tg == 0)
		{
			Move_Left();
		}
	}  // y 좌표가 아직 남아있을 때, (오른쪽) x 좌표가 벽에 부딪히면 (좌측으로) 공방향 전환
	Move_Down();  // 하강
}

void Ball::Move_Left()
{
	int n, m;
	int rd, cnt;
	int tg = 0;

	cnt = bally;
	while (cnt > 0) {

		if (map[bally - 1][ballx] == 0 && map[bally - 1][ballx - 1] == 0)
		{
			system("cls");

			m = ballx;
			n = bally;
			map[n][m] = 0;
			--bally;    // 상승

			if (ballx > 0) ballx--;
			if (ballx == 0) cnt = 0;

			map[bally][ballx] = 4;

			Print_Ball();
			Sleep(30);
			--cnt;
		}
		else if (map[bally - 1][ballx] != 0 || map[bally - 1][ballx - 1] != 0)
		{
			system("cls");
			map[bally - 1][ballx] = 0;
			Print_Ball();
			Sleep(30);
			cnt = 0;
			tg++;
		}
	}

	if (bally > 0)
	{
		if (tg == 0)
		{
			Move_Right(); // y 좌표가 아직 남아있을 때, (왼쪽) x 좌표가 벽에 부딪히면 (우측으로) 공방향 전환
		}
	}
	Move_Down();
}

void Ball::Move_Down()    // 벽돌이 어디 있느냐에 따라 내려오는 방향이 결정됨
{
	int nm;

	if (map[bally][ballx + 1] == 0 && map[bally][ballx - 1] == 0)
	{
		nm = rand() % 3;

		if (nm == 0) Get_Down();  // 수짓 하강
		if (nm == 1) Right_Down(); // 우측 하강
		if (nm == 2) Left_Down();  // 좌측 하강
	}
	else if (map[bally][ballx + 1] != 0 && map[bally][ballx - 1] == 0)
	{
		nm = rand() % 2;

		if (nm == 0) Get_Down();  // 수짓 하강
		if (nm == 1) Left_Down(); // 우측 하강

	}
	else if (map[bally][ballx + 1] == 0 && map[bally][ballx - 1] != 0)
	{
		nm = rand() % 2;

		if (nm == 0) Get_Down();  // 수짓 하강
		if (nm == 1) Right_Down(); // 우측 하강
	}
	else if (map[bally][ballx + 1] != 0 && map[bally][ballx - 1] != 0)
	{
		Get_Down();
	}
}

void Ball::Get_Down()  // 수직 하강
{
	int n, m;
	int cnt;

	cnt = bally;
	while (cnt <= 8) {
		system("cls");

		m = ballx;
		n = bally;
		map[n][m] = 0;
		++bally;   // 수직 하강
		map[bally][ballx] = 4;

		Print_Ball();
		Sleep(30);
		++cnt;
	}
}

void Ball::Right_Down()
{
	int n, m;
	int rd, cnt;
	int tg;

	cnt = bally;
	while (cnt <= 8) {

		if (map[bally + 1][ballx + 1] == 0)
		{
			system("cls");

			m = ballx;
			n = bally;
			map[n][m] = 0;
			++bally;    // 하강

			if (ballx < 5) ballx++;  // x 좌표 우측조정, 우측으로 하강
			if (ballx == 5) cnt = 9; // 벽에 부딪혔는지 확인

			map[bally][ballx] = 4;

			Print_Ball();
			Sleep(30);
			++cnt;
		}
		else if (map[bally + 1][ballx + 1] != 0)
		{
			cnt = 9;
		}
	}

	if (bally < 8) Left_Down(); // y 좌표가 아직 남아있을 때, (오른쪽) x 좌표가 벽에 부딪히면 (좌측으로) 공방향 전환
}

void Ball::Left_Down()
{
	int n, m;
	int rd, cnt;
	int tg;

	cnt = bally;
	while (cnt <= 8) {

		if (map[bally + 1][ballx - 1] == 0)
		{
			system("cls");

			m = ballx;
			n = bally;
			map[n][m] = 0;
			++bally;    // 하강

			if (ballx > 0) ballx--;  // x 좌표 좌측조정,좌측으로 하강
			if (ballx == 0) cnt = 9;

			map[bally][ballx] = 4;

			Print_Ball();
			Sleep(30);
			++cnt;
		}
		else if (map[bally + 1][ballx - 1] != 0)
		{
			cnt = 9;
		}
	}

	if (bally < 8) Right_Down(); // y 좌표가 아직 남아있을 때, (왼쪽) x 좌표가 벽에 부딪히면 (우측으로) 공방향 전환
}

void Ball::Print_Ball() // 화면 출력
{
	int a, b;

	for (a = 0; a < 10; a++) {
		for (b = 0; b < 6; b++) {
			if (map[a][b] == 0) cout << "  ";  //두 칸
			if (map[a][b] == 1) cout << "■";  //■
			if (map[a][b] == 2) cout << "◆";  //◆
			if (map[a][b] == 3) cout << "☆";  //☆
			if (map[a][b] == 4) cout << "●";  //●
		}
		cout << endl;;
	}
}


int Ball::Soc_Cli()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	/*서버에 접속해서 메시지를 주고 받는 클라이언트 소켓생성*/
	int mySocket = socket(PF_INET, SOCK_STREAM, 0);

	/*클라이언트 소켓생성에 실패하면 에러메시지를 출력하고 함수 종료*/
	if (mySocket == -1) {
		cout << "클라이언트 소켓을 생성하는데 실패 했습니다" << endl;
		return 0;
	}

	/*서버의 주소 정보를 저장할 구조체 serverAddress 선언*/
	sockaddr_in serverAddress;

	/*구조체를 0으로 초기화함*/
	memset(&serverAddress, 0, sizeof(serverAddress));
	/*서버의 IP 대입*/
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*주소의 종류 대입*/
	serverAddress.sin_family = AF_INET;
	/*서버의 포트번호 대입*/
	serverAddress.sin_port = htons(PORT);

	/*서버에 접속*/
	if (connect(mySocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		cout << "서버와 연결하는데 실패했습니다" << endl;
		return 0;
	}
	char greetMessage[BUFFERSIZE];  //서버에서 전송할 환영 메시지를 저장할 배열

									/*서버에서 전송한 환영 메시지를 읽어 들임*/
	recv(mySocket, greetMessage, sizeof(greetMessage), 0);
	cout << greetMessage << endl;


	int key;
	char localsend[BUFFERSIZE];
	while (1)
	{
		cout << "시작은 스페이스바" << endl;
		cout << "종료는 이스케이프" << endl;

		key = getch();         // 키 입력 받음
		if (key == 32)     // 스페이스바 누르면 게임 시작, 종료 후 로컬타임 서버에 전송
		{
			Init_G();         
			sprintf(localsend, "로컬타임 : %d\n", localtime);
			send(mySocket, localsend, sizeof(localsend), 0);
		}
		else if (key == 27)    // 이스케이프 누르면 소켓 연결 죵료
		{
			closesocket(mySocket);
			break;
		}
	}
}

int main()
{
	Ball cli;

	cli.Soc_Cli();
}