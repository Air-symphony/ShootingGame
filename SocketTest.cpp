#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "DxLib.h"

#define PORT 9876 //�T�[�o�[�v���O�����ƃ|�[�g�ԍ������킹�Ă�������

// IP �A�h���X�C�|�[�g�ԍ��C�\�P�b�g�Csockaddr_in �\����
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

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(TRUE);//window���[�h�ɐݒ�
	SetGraphMode(800, 400, 32);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);
	//SetHookWinProc(WndProc);

	// �e��p�����[�^
	char buffer[1024];

	// Windows �̏ꍇ
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	// sockaddr_in �\���̂̃Z�b�g
	memset(&dstAddr, 0, sizeof(dstAddr));
	dstAddr.sin_port = htons(PORT);
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_addr.s_addr = inet_addr("172.16.126.1");

	// �\�P�b�g�̐���
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	ClearDrawScreen();
	//�ڑ�
	if (connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))) {
		//printf("%s�@�ɐڑ��ł��܂���ł���\n", destination);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Can't connect to [%s]", destination);
		DrawFormatString(0, 30, GetColor(255, 255, 255), "%s does not connect\n", destination);
		ScreenFlip();
		WaitKey();
		return(-1);
	}
	//printf("%s �ɐڑ����܂���\n", destination);
	//printf("�K���ȃA���t�@�x�b�g����͂��Ă�������\n");
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%s �ɐڑ����܂���\n", destination);
	ScreenFlip();
	WaitKey();

	ClearDrawScreen();
	char text[256];
	float rank[10] = { 50.000000, 60.000000, 70.000000, 80.000000, 90.000000, 
		99.000000, 100.000000, 101.000000, 102.000000, 103.000000, };
	while (1) {
		//�p�P�b�g�̑��M
		send(dstSocket, "SendData", 1024, 0);
		for (int i = 0; i < 10; i++) {
			sprintf_s(text, 256, "%f", rank[i]);
			send(dstSocket, text, 1024, 0);
		}
		//�p�P�b�g�̎�M
		recv(dstSocket, buffer, 1024, 0);
		//printf("�� %s\n\n", buffer);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "�� %s\n\n", buffer);
		ScreenFlip();
		WaitKey();
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}
	}

	// Windows �ł̃\�P�b�g�̏I��
	closesocket(dstSocket);
	WSACleanup();
	DxLib_End();
	return(0);
}