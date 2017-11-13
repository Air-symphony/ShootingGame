#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "DxLib.h"

#define PORT 9876 //サーバープログラムとポート番号を合わせてください

// IP アドレス，ポート番号，ソケット，sockaddr_in 構造体
char destination[32];
int dstSocket;
struct sockaddr_in dstAddr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		closesocket(dstSocket);
		WSACleanup();
		DxLib_End();
		break;
	case WM_QUIT:
		closesocket(dstSocket);
		WSACleanup();
		DxLib_End();
		break;
	case WM_DESTROY:
		closesocket(dstSocket);
		WSACleanup();
		DxLib_End();
		PostQuitMessage(0);
		return 0;
	}

	return 0;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(TRUE);//windowモードに設定
	SetGraphMode(800, 400, 32);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	//SetHookWinProc(WndProc);

	// 各種パラメータ
	char buffer[1024];

	// Windows の場合
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	// sockaddr_in 構造体のセット
	memset(&dstAddr, 0, sizeof(dstAddr));
	dstAddr.sin_port = htons(PORT);
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_addr.s_addr = inet_addr("172.16.126.1");

	// ソケットの生成
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	ClearDrawScreen();
	//接続
	if (connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))) {
		//printf("%s　に接続できませんでした\n", destination);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Can't connect to [%s]", destination);
		DrawFormatString(0, 30, GetColor(255, 255, 255), "%s does not connect\n", destination);
		ScreenFlip();
		WaitKey();
		return(-1);
	}
	//printf("%s に接続しました\n", destination);
	//printf("適当なアルファベットを入力してください\n");
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%s に接続しました\n", destination);
	ScreenFlip();
	WaitKey();

	ClearDrawScreen();
	char text[256];
	float rank[10] = { 50.000000, 60.000000, 70.000000, 80.000000, 90.000000, 
		99.000000, 100.000000, 101.000000, 102.000000, 103.000000, };
	while (1) {
		//パケットの送信
		send(dstSocket, "SendData", 1024, 0);
		for (int i = 0; i < 10; i++) {
			sprintf_s(text, 256, "%f", rank[i]);
			send(dstSocket, text, 1024, 0);
		}
		//パケットの受信
		recv(dstSocket, buffer, 1024, 0);
		//printf("→ %s\n\n", buffer);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "→ %s\n\n", buffer);
		ScreenFlip();
		WaitKey();
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}
	}

	// Windows でのソケットの終了
	closesocket(dstSocket);
	WSACleanup();
	DxLib_End();
	return(0);
}