#pragma once
#include "Graphics.cpp"

/*
Graph menu_Start[2], menu_Ranking[2], menu_Finish[2];

Graph HP;
Graph player, enemy[5], shot[3], background, weapon[3];
Graph player_Toho, enemy_Toho[5], shot_Toho[7], background_Toho;

int bgm_pre, bgm_loop;
int shotSE[2];
int enemyshotSE_Toho[2];
int reloadSE;
int damageSE[2];
int cursorSE[3];*/
class Material {
private:
	char Picture_path[9] = "Picture\\";
	char Normal_path[8] = "Normal\\";
	char Toho_path[6] = "Toho\\";
	char Text_path[6] = "Text\\";
	char Enemy_path[7] = "Enemy\\";
	char Background_path[12] = "Background\\";
	char Shot_path[6] = "Shot\\";
	char Weapon_path[8] = "Weapon\\";
	char Sound_path[7] = "Sound\\";
	char Pictrue_extension[5] = ".png";
public:
	Graph menu_Start[2], menu_Ranking[2], menu_Finish[2];

	Graph HP;
	Graph player, enemy[5], shot[3], background, weapon[3];
	Graph player_Toho, enemy_Toho[5], shot_Toho[7], background_Toho;

	int bgm_pre, bgm_loop;
	int shotSE[2];
	int enemyshotSE_Toho[2];
	int reloadSE;
	int damageSE[2];
	int cursorSE[3];

	//‘Š‘ÎƒpƒX
	Material() {
		
	}

	/*
	Graph menu_Start[2], menu_Ranking[2], menu_Finish[2];

	Graph HP;
	Graph player, enemy[5], shot[3], background, weapon[3];
	Graph player_Toho, enemy_Toho[5], shot_Toho[7], background_Toho;

	int bgm_pre, bgm_loop;
	int shotSE[2];
	int enemyshotSE_Toho[2];
	int reloadSE;
	int damageSE[2];
	int cursorSE[3];
	*/
	void SetMatrial() {
		char _filepath[256];
		char text[10];
		//player.SetGraph(LoadGraph("Picture\\Normal\\Player.png"));
		strcpy_s(_filepath, Picture_path);
		strcat_s(_filepath, Normal_path);
		strcat_s(_filepath, "Player");
		strcat_s(_filepath, Pictrue_extension);
		player.SetGraph(LoadGraph(_filepath));

		//HP.SetGraph(LoadGraph("Picture\\Normal\\HP.png"));
		strcpy_s(_filepath, Picture_path);
		strcat_s(_filepath, Normal_path);
		strcat_s(_filepath, "HP");
		strcat_s(_filepath, Pictrue_extension);
		HP.SetGraph(LoadGraph(_filepath));

		for (int i = 0; i < 5; i++) {
			//strcpy_s(_filepath, "Picture\\Normal\\Enemy\\Enemy_");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Normal_path);
			strcat_s(_filepath, Enemy_path);
			strcat_s(_filepath, "Enemy_");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			enemy[i].SetGraph(LoadGraph(_filepath));
		}
		for (int i = 0; i < 3; i++) {
			//strcpy_s(_filepath, "Picture\\Normal\\Shot\\Shot");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Normal_path);
			strcat_s(_filepath, Shot_path);
			strcat_s(_filepath, "Shot");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			shot[i].SetGraph(LoadGraph(_filepath));
		}
		//background.SetGraph(LoadGraph("Picture\\Normal\\Background\\back_01.png"));
		strcpy_s(_filepath, Picture_path);
		strcat_s(_filepath, Normal_path);
		strcat_s(_filepath, Background_path);
		strcat_s(_filepath, "back_01");
		strcat_s(_filepath, Pictrue_extension);
		background.SetGraph(LoadGraph(_filepath));

		for (int i = 0; i < 2; i++) {
			//strcpy_s(_filepath, "Picture\\Text\\Start_");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Text_path);
			strcat_s(_filepath, "Start_");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			menu_Start[i].SetGraph(LoadGraph(_filepath));

			//strcpy_s(_filepath, "Picture\\Text\\Finish_");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Text_path);
			strcat_s(_filepath, "Finish_");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			menu_Finish[i].SetGraph(LoadGraph(_filepath));

			//strcpy_s(_filepath, "Picture\\Text\\Ranking_");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Text_path);
			strcat_s(_filepath, "Ranking_");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			menu_Ranking[i].SetGraph(LoadGraph(_filepath));
		}
		for (int i = 0; i < 3; i++) {
			//strcpy_s(_filepath, "Picture\\Normal\\Weapon\\Weapon_");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Normal_path);
			strcat_s(_filepath, Weapon_path);
			strcat_s(_filepath, "Weapon_");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			weapon[i].SetGraph(LoadGraph(_filepath));
		}

		//player_Toho.SetGraph(LoadGraph("Picture\\Toho\\Toho_Player.png"));
		strcpy_s(_filepath, Picture_path);
		strcat_s(_filepath, Toho_path);
		strcat_s(_filepath, "Toho_Player");
		strcat_s(_filepath, Pictrue_extension);
		player_Toho.SetGraph(LoadGraph(_filepath));
		for (int i = 0; i < 7; i++) {
			//strcpy_s(_filepath, "Picture\\Toho\\Shot\\Toho_shot");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Toho_path);
			strcat_s(_filepath, Shot_path);
			strcat_s(_filepath, "Toho_shot");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			shot_Toho[i].SetGraph(LoadGraph(_filepath));
		}
		for (int i = 0; i < 5; i++) {
			//strcpy_s(_filepath, "Picture\\Toho\\Enemy\\Toho_Enemy");
			strcpy_s(_filepath, Picture_path);
			strcat_s(_filepath, Toho_path);
			strcat_s(_filepath, Enemy_path);
			strcat_s(_filepath, "Toho_Enemy");
			sprintf_s(text, 10, "%d", i + 1);
			strcat_s(_filepath, text);
			strcat_s(_filepath, Pictrue_extension);
			enemy_Toho[i].SetGraph(LoadGraph(_filepath));
		}
		//background_Toho.SetGraph(LoadGraph("Picture\\Toho\\Background\\back_05.png"));
		strcpy_s(_filepath, Picture_path);
		strcat_s(_filepath, Toho_path);
		strcat_s(_filepath, Background_path);
		strcat_s(_filepath, "back_05");
		strcat_s(_filepath, Pictrue_extension);
		background_Toho.SetGraph(LoadGraph(_filepath));

		bgm_pre = LoadSoundMem("Sound\\silent\\pre.mp3");
		bgm_loop = LoadSoundMem("Sound\\silent\\loop.mp3");
		shotSE[0] = LoadSoundMem("Sound\\shot3.mp3");
		shotSE[1] = LoadSoundMem("Sound\\shot5.mp3");
		reloadSE = LoadSoundMem("Sound\\reload.mp3");
		damageSE[0] = LoadSoundMem("Sound\\damage.mp3");
		damageSE[1] = LoadSoundMem("Sound\\bomb.mp3");
		cursorSE[0] = LoadSoundMem("Sound\\decision23.mp3");
		cursorSE[1] = LoadSoundMem("Sound\\cancel1.mp3");
		cursorSE[2] = LoadSoundMem("Sound\\cursor4.mp3");
		enemyshotSE_Toho[0] = LoadSoundMem("Sound\\Toho\\shot1.mp3");
		enemyshotSE_Toho[1] = LoadSoundMem("Sound\\Toho\\shot1.mp3");
	}


	void DeleteMaterial() {
		player.Delete();
		HP.Delete();
		for (int i = 0; i < 5; i++) enemy[i].Delete();
		for (int i = 0; i < 3; i++) {
			shot[i].Delete();
			weapon[i].Delete();
		}
		background.Delete();

		for (int i = 0; i < 5; i++) enemy_Toho[i].Delete();
		for (int i = 0; i < 7; i++) shot_Toho[i].Delete();
		background_Toho.Delete();

		DeleteSoundMem(bgm_pre);
		DeleteSoundMem(bgm_loop);
		for (int i = 0; i < 2; i++) {
			DeleteSoundMem(shotSE[i]);
			DeleteSoundMem(enemyshotSE_Toho[i]);
			DeleteSoundMem(damageSE[i]);
			DeleteSoundMem(cursorSE[i]);
		}
		DeleteSoundMem(cursorSE[2]);
		for (int i = 0; i < 2; i++) menu_Start[i].Delete();
		for (int i = 0; i < 2; i++) menu_Ranking[i].Delete();
		for (int i = 0; i < 2; i++) menu_Finish[i].Delete();
		for (int i = 0; i < 3; i++) weapon[i].Delete();
	}
};