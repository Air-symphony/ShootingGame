#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>

#include "DxLib.h"

#define PORT 9876 //�T�[�o�[�v���O�����ƃ|�[�g�ԍ������킹�Ă�������

class ClientSocket{
private:
	char IP_address[20];
	const int RANKING = 10;

	// IP �A�h���X�C�|�[�g�ԍ��C�\�P�b�g�Csockaddr_in �\����
	char destination[32];
	int dstSocket;
	struct sockaddr_in dstAddr;
	// �e��p�����[�^
	char buffer[1024];

	// Windows �̏ꍇ
	WSADATA data;

	char filepath[256] = "IP.txt";
	int FileHandle;
	char *next;
	char *ctx;//�������p
public:
	float RankingData[10];

	ClientSocket() {
		int count = 2;
		char string[256];
		try {
			FileHandle = FileRead_open(filepath);
			if (FileHandle == -1) {
				throw "Not Ranking File";
			}
			FileRead_gets(IP_address, 256, FileHandle);
			FileRead_close(FileHandle);
		}
		catch (char* text) {
			printfDx(text);
			FileRead_close(FileHandle);
		}
		for (int i = 0; i < 10; i++) {
			RankingData[i] = 50.0 + i * 5.0;
		}
		GetData();
	}

	bool StartSocket() {
		try {
			WSAStartup(MAKEWORD(2, 0), &data);

			// sockaddr_in �\���̂̃Z�b�g
			memset(&dstAddr, 0, sizeof(dstAddr));
			dstAddr.sin_port = htons(PORT);
			dstAddr.sin_family = AF_INET;
			dstAddr.sin_addr.s_addr = inet_addr(IP_address);

			// �\�P�b�g�̐���
			dstSocket = socket(AF_INET, SOCK_STREAM, 0);

			//�ڑ�
			if (connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))) {
				DrawString(0, 0, "�T�[�o�ɐڑ��ł��܂���ł���", GetColor(255, 255, 255));
				ScreenFlip();
				throw "Error";
			}
			else {
				DrawString(0, 0, "�T�[�o�ɐڑ����܂���", GetColor(255, 255, 255));
				ScreenFlip();
				return true;
			}
		}
		catch(char* text) {
			ScreenFlip();
			WaitKey();
			return false;
		}
	}

	bool GetData() {
		try {
			if (StartSocket() == false) {
				throw "Error";
			}
			else {
				send(dstSocket, "GetData", 1024, 0);
				for (int i = 0; i < RANKING; i++) {
					recv(dstSocket, buffer, 1024, 0);
					RankingData[i] = (float)atof(buffer);
				}
				send(dstSocket, "Complete", 1024, 0);

				closesocket(dstSocket);
				WSACleanup();
				return true;
			}
		}
		catch (char* text) {
			closesocket(dstSocket);
			WSACleanup();
			return false;
		}
	}

	bool UpdateData() {
		try {
			if (StartSocket() == false) {
				throw "Error";
			}
			else {
				send(dstSocket, "SendData", 1024, 0);

				char text[256];
				for (int i = 0; i < RANKING; i++) {
					sprintf_s(text, 256, "%f", RankingData[i]);
					send(dstSocket, text, 1024, 0);
				}
				//�p�P�b�g�̎�M
				recv(dstSocket, buffer, 1024, 0);

				closesocket(dstSocket);
				WSACleanup();
				return true;
			}
		}
		catch (char* text) {
			closesocket(dstSocket);
			WSACleanup();
			return false;
		}
	}

	int CheckInRanking(double time) {
		for (int i = 0; i < RANKING; i++) {
			if (time <= (double)RankingData[i]) {
				for (int j = RANKING - 1; j > i; j--) {
					RankingData[j] = RankingData[j - 1];
				}
				RankingData[i] = time;
				return i;
			}
		}
		return -1;
	}

	int getRANKING() {
		return RANKING;
	}
};