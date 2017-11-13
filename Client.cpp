#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>

#include "DxLib.h"

#define PORT 9876 //サーバープログラムとポート番号を合わせてください

class ClientSocket{
private:
	char IP_address[20];
	const int RANKING = 10;

	// IP アドレス，ポート番号，ソケット，sockaddr_in 構造体
	char destination[32];
	int dstSocket;
	struct sockaddr_in dstAddr;
	// 各種パラメータ
	char buffer[1024];

	// Windows の場合
	WSADATA data;

	char filepath[256] = "IP.txt";
	int FileHandle;
	char *next;
	char *ctx;//内部利用
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

			// sockaddr_in 構造体のセット
			memset(&dstAddr, 0, sizeof(dstAddr));
			dstAddr.sin_port = htons(PORT);
			dstAddr.sin_family = AF_INET;
			dstAddr.sin_addr.s_addr = inet_addr(IP_address);

			// ソケットの生成
			dstSocket = socket(AF_INET, SOCK_STREAM, 0);

			//接続
			if (connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))) {
				DrawString(0, 0, "サーバに接続できませんでした", GetColor(255, 255, 255));
				ScreenFlip();
				throw "Error";
			}
			else {
				DrawString(0, 0, "サーバに接続しました", GetColor(255, 255, 255));
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
				//パケットの受信
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