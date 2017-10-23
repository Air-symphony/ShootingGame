#pragma once
#include "Graphics.cpp"

/*
int bgm_pre, bgm_loop;
int shotSE[2];
int reloadSE;
int damageSE[2];
int cursorSE[3];

Graph player, HP, weapon[3];
Graph enemy[5];
Graph shot[3];
Graph menu_Start[2], menu_Ranking[2], menu_Finish[2];
*/
class Material {
private:
	char enemyNames[5][30] = { "Picture\\Enemy_1.png","Picture\\Enemy_2.png","Picture\\Enemy_3.png"
		,"Picture\\Enemy_4.png","Picture\\Enemy_5.png" };

	char shotNames[3][30] = { "Picture\\Shot1.png", "Picture\\Shot2.png", "Picture\\Shot3.png" };
	char textNames[6][30] = { "Picture\\start_1.png", "Picture\\Start_2.png", "Picture\\Finish_1.png","Picture\\Finish_2.png",
		"Picture\\Ranking_1.png", "Picture\\Ranking_2.png" };
	char weaponNames[3][30] = { "Picture\\Weapon_1.png", "Picture\\Weapon_2.png", "Picture\\Weapon_3.png" };

public:
	int bgm_pre, bgm_loop;
	int shotSE[2];
	int reloadSE;
	int damageSE[2];
	int cursorSE[3];
	
	Graph player, HP, weapon[3];
	Graph enemy[5];
	Graph shot[3];
	Graph menu_Start[2], menu_Ranking[2], menu_Finish[2];

	//‘Š‘ÎƒpƒX
	Material() {
		
	}

	/*
	int bgm_pre, bgm_loop;
	int shotSE[2];
	int reloadSE;
	int damageSE[2];
	int cursorSE[3];
	
	Graph player, HP, weapon[3];
	Graph enemy[5];
	Graph shot[3];
	Graph menu_Start[2], menu_Ranking[2], menu_Finish[2];
	*/
	void SetMatrial() {
		player.SetGraph(LoadGraph("Picture\\Player.png"));
		HP.SetGraph(LoadGraph("Picture\\HP.png"));
		for (int i = 0; i < 5; i++) enemy[i].SetGraph(LoadGraph(enemyNames[i]));
		for (int i = 0; i < 3; i++) shot[i].SetGraph(LoadGraph(shotNames[i]));
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
		for (int i = 0; i < 2; i++) menu_Start[i % 2].SetGraph(LoadGraph(textNames[i]));
		for (int i = 2; i < 4; i++) menu_Finish[i % 2].SetGraph(LoadGraph(textNames[i]));
		for (int i = 4; i < 6; i++) menu_Ranking[i % 2].SetGraph(LoadGraph(textNames[i]));

		for (int i = 0; i < 3; i++) weapon[i].SetGraph(LoadGraph(weaponNames[i]));
	}
	void DeleteMaterial() {
		player.Delete();
		HP.Delete();
		for (int i = 0; i < 5; i++) enemy[i].Delete();
		for (int i = 0; i < 3; i++) shot[i].Delete();

		DeleteSoundMem(bgm_pre);
		DeleteSoundMem(bgm_loop);
		for (int i = 0; i < 2; i++) {
			DeleteSoundMem(shotSE[i]);
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