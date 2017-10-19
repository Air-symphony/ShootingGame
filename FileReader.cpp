#pragma once
#include "Dxlib.h"

class FileReader {
private:
	const int RANKING = 10;
	char filepath[256] = "Ranking.txt";
	int FileHandle;
	char *next;
	char *ctx;//“à•”—˜—p
public:
	float rank[10];

	FileReader() {
		ReadFile();
	}

	//ŠÂ‹«İ’èABPM“™
	bool ReadFile() {
		int count = 2;
		char string[256];
		try {
			FileHandle = FileRead_open(filepath);
			//while (FileRead_eof(FileHandle) == 0)

			if (FileHandle == -1) {
				throw "Not Ranking File";
			}
			for (int i = 0; i < RANKING; i++) {
				// ˆês“Ç‚İ‚İ
				FileRead_gets(string, 256, FileHandle);
				next = strtok_s(string, "", &ctx);
				char char_bpm[256];
				sprintf_s(char_bpm, 256, "%s", next);
				rank[i] = (float)atof(char_bpm);
			}
			FileRead_close(FileHandle);
		}
		catch (char* text) {
			printfDx(text);
			FileRead_close(FileHandle);
			return false;
		}
		return true;
	}
	/*-1, 0~9*/
	int CheckInRanking(double time) {
		for (int i = 0; i < RANKING; i++) {
			if (time <= (double)rank[i]) {
				for (int j = RANKING - 1; j > i; j--) {
					rank[j] = rank[j - 1];
				}
				rank[i] = time;
				return i;
			}
		}
		return -1;
	}
	bool UpdateFile() {
		FILE *fp;
		int err_no = fopen_s(&fp, filepath, "w");
		if (err_no != 0) return false;

		for (int i = 0; i < RANKING; i++) {
			fprintf(fp, "%f\n", rank[i]);
		}
		fclose(fp);
		return true;
	}

	int getRANKING() {
		return RANKING;
	}
};