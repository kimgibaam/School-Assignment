#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define  PORT 5500
#define BUFFERSIZE 100

HANDLE  mutex;
int clientNumber = 0;
unsigned  threadID;
/*현재 접속중인 모든 클라이언트의 클라이언트 소켓을 저장할 변수*/
int allClientSocket[100];

class THD {
public:
	int Soc_Ser();
	static unsigned __stdcall broadcastAllClient(void *arg);  // 로컬타임 전송받는 쓰레드 함수
};

int THD::Soc_Ser()
{
	mutex = CreateMutex(NULL, FALSE, NULL);
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	/*클라이언트의 접속을 기다리는 서버 소켓 생성*/
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	/*서버의 주소를 저장하는 구조체 serverAddress 생성*/
	sockaddr_in serverAddress;

	/*serverAddress 구조체를 0으로 초기화*/
	memset(&serverAddress, 0, sizeof(serverAddress));

	/*serverAddress 에 현재 IP 대입 INADDR_ANY->자동으로 IP 를 리턴하는 상수*/
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	/*주소 타입 설정*/
	serverAddress.sin_family = AF_INET;
	/*클라이언트와 메시지를 주고 받을 포트 번호 설정*/
	serverAddress.sin_port = htons(PORT);

	/*서버 소켓에 주소정보 설정하합니다.에러가 발생하면 에러 메시지를 출력하고 프로그램을 종료합니다. */
	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		cout << "서버 소켓을 바인드 하는데 실패했습니다." << endl;
		return 0;
	}
	/*클라이언트의 접속을 기다리는 대기모드로 전환*/
	if (listen(serverSocket, 5) == -1) {
		cout << "서버 소켓을 listen 모드로 설정하는데 실패했습니다" << endl;
		return 0;
	}
	cout << "서버가 실행 되었습니다." << endl;
	cout << "**********로컬 타임 내역***********" << endl;


	while (1) {
		/*클라이언트의 주소 정보를 저장할 구조체 선언*/
		struct sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);

		/*클라이언트의 접속을 허락하고 클라이언트와 데이터를    주고받는 클라이언트 소켓생성
		합니다. 클라이언트의 주소 정보를 clientAddress 에 대입합니다.*/
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
			&clientAddressSize);

		/*뮤텍스를 이용해서 공유 자원을 잠급니다. 공유 자원을 열때 까지 다른 쓰레드는 공유
		자원을 사용할 수 없고 대기 상태에서 기다립니다.*/
		WaitForSingleObject(mutex, INFINITE);
		/*현재 접속중인 클라이언트의 수를 1증가 시킵니다.*/
		clientNumber++;

		/*현재 접속중인 모든 클라이언트의 클라이언트 소켓을 저장하는 배열 allClientSocket 에
		클라이언트소켓을 대입합니다.*/
		allClientSocket[clientNumber - 1] = clientSocket;

		/*공유 변수의 잠금을 해제합니다. 공유 자원을 사용할 수 없어서 대기 중이던 다른쓰레드
		의 대기 상태가 풀립니다.*/
		ReleaseMutex(mutex);

		/*클라이언트에게 전달할 환영 메시지를 저장할 배열을 선언했습니다.*/
		char greetMessage[BUFFERSIZE];

		/*환영메시지를 greetMessage 에 대입했습니다.*/
		sprintf(greetMessage, "[서버]환영합니다.\n");

		/*환영메시지를 서버로 전송합니다.*/
		send(clientSocket, greetMessage, sizeof(greetMessage), 0);

		/*클라이언트에서 전송한 메시지를 읽어 들여서 모든 클라이언트로 전송하는 broadcastAllClient 함수를 호출합니다.*/
		unsigned long thread;
		thread = _beginthreadex(NULL, 0, broadcastAllClient, (void*)clientSocket, 0, &threadID);
	}
	cout << "서버를 종료 합니다" << endl;
	return 0;
}

unsigned __stdcall THD::broadcastAllClient(void *arg)
{
	/*클라이언트가 전송한 메시지를 읽어 들여서 저장할 배열*/
	char fromClient[BUFFERSIZE];

	/*클라이언트가 접속하면 main 함수에서 클라이언트와의 접속을 허락하고 클라이언트와 데이터를 주고
	받을수 있는 클라이언트 소켓을 생성합니다. 멀티 쓰레드로 broadcastAllClient 함수를 시작하는데 이때
	인자로 클라이언트 소켓을 전달합니다. main 함수에서 전달한 클라이언트 소켓을 myClientsocket 변수에 대입했습니다.*/

	int myClientSocket = (int)arg;
	int i = 0;

	/*myClientSocket으로 연결된 현재 클라이언트가 전송한 메시지를 읽어 들이는 부분입니다.
	현재 클라이언트가 접속을 종료 할때 까지 계속 반복합니다.*/
	while (1)
	{
		/*현재 클라이언트가 전송한 메시지를 읽어 들여서 fromClient 배열에 대입
		합니다.*/
		int readlen = recv(myClientSocket, fromClient, sizeof(fromClient), 0);

		/*myClientSocket 으로 연결된 현재 클라이언트가 접속을 종료 하면 readlen 변수에는
		0이 대입됩니다. readlen 변수에 대입된 값이 0이하면 반복문을 종료합니다.*/
		if (readlen <= 0) break;

		/*클라이언트에서 전달한 메시지를 화면에 출력합니다.*/
		cout << fromClient << endl;

	} //while      

	  // 종료한 클라이언트 소켓 제거 부분 
	WaitForSingleObject(mutex, INFINITE);//뮤텍스 동기화 시작
	for (i = 0; i<clientNumber; i++)
	{
		/*allClientSocket 의 i번째 소켓이 현재 클라이언트 소켓인 myClientSocket 과 같으면*/
		if (allClientSocket[i] == myClientSocket)
		{
			/*현재 클라이언트 소켓의 아래쪽에 저장된 클라이언트 소켓이 하나씩 위로 당겨져서
			현재 클라이언트 소켓의 자리를 덮어씀*/
			for (; i<clientNumber - 1; i++) {
				allClientSocket[i] = allClientSocket[i + 1];
			} // for
			break;
		} // if
			
		clientNumber--;//전체 클라이언트의 수를 1감소 시킴
		ReleaseMutex(mutex);//뮤텍스 동기화 종료
		closesocket(myClientSocket);//현재 클라이언트와 연결된 소켓을 종료함
	}  // for
	return 0;
} 

int main()
{
	THD soc;

	soc.Soc_Ser();
}