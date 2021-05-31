// TestServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment(lib, "ws2_32")

using namespace std;

#define PORT        4578
#define PACKET_SIZE 1024

SOCKET skt, client_sock;

void proc_recvs()
{
    char buffer[PACKET_SIZE] = { 0 };

    while (!WSAGetLastError())
    {
        ZeroMemory(&buffer, PACKET_SIZE);
        recv(client_sock, buffer, PACKET_SIZE, 0);
        cout << "받은 메세지 : " << buffer << endl;
    }
}

void ByteArrayToStructure()
{

}

int main()
{
    std::cout << "Hello World!\n";

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(skt, (SOCKADDR*)&addr, sizeof(addr));
    listen(skt, SOMAXCONN);

    SOCKADDR_IN client = {};
    int iClntSize = sizeof(client);
    ZeroMemory(&client, iClntSize);
    client_sock = accept(skt, (SOCKADDR*)&client, &iClntSize);

    char buffer[PACKET_SIZE] = { 0 };
    thread proc2(proc_recvs);

    while (!WSAGetLastError()) 
    {
        cin >> buffer;
        send(client_sock, buffer, strlen(buffer), 0);
    }
    proc2.join();
    closesocket(client_sock);
    closesocket(skt);

    WSACleanup();
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
