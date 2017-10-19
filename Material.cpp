#pragma once
#include "Dxlib.h"

/*
int playergraph;
int HPgraph;
int enemygraph[5];
int shotgraph[3];
int bgm_pre;
int bgm_loop;
int shotSE[2];
int reloadSE;
int damageSE[2];
int cursorSE[3];
*/
class Material {
public:
	int playergraph;
	int HPgraph;
	int enemygraph[5];
	char enemyNames[5][140] = { "Picture\\Enemy_1.png","Picture\\Enemy_2.png","Picture\\Enemy_3.png"
		,"Picture\\Enemy_4.png","Picture\\Enemy_5.png" };

	char shotNames[3][140] = { "Picture\\Shot1.png", "Picture\\Shot2.png", "Picture\\Shot3.png" };
	int shotgraph[3];
	
	int bgm_pre;
	int bgm_loop;
	int shotSE[2];
	int reloadSE;
	int damageSE[2];
	int cursorSE[3];
	char textNames[6][140] = { "Picture\\start_1.png", "Picture\\Start_2.png", "Picture\\Finish_1.png","Picture\\Finish_2.png",
		"Picture\\About_1.png", "Picture\\About_2.png" };
	int textgraph[6];

	//‘Š‘ÎƒpƒX
	Material() {
		
	}

	/*int playergraph;
int HPgraph;
int enemygraph[5];
int shotgraph[3];
int bgm_pre;
int bgm_loop;
int shotSE[2];
int reloadSE;
int damageSE[2];
int cursorSE[3];*/
	void SetMatrial() {
		playergraph = LoadGraph("Picture\\Player.png");
		HPgraph = LoadGraph("Picture\\HP.png");
		for (int i = 0; i < 5; i++) enemygraph[i] = LoadGraph(enemyNames[i]);
		for (int i = 0; i < 3; i++) shotgraph[i] = LoadGraph(shotNames[i]);
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
		for (int i = 0; i < 6; i++) textgraph[i] = LoadGraph(textNames[i]);
	}
	void DeleteMaterial() {
		DeleteGraph(playergraph);
		DeleteGraph(HPgraph);
		for (int i = 0; i < 5; i++) DeleteGraph(enemygraph[i]);
		for (int i = 0; i < 3; i++) DeleteGraph(shotgraph[i]);

		DeleteSoundMem(bgm_pre);
		DeleteSoundMem(bgm_loop);
		for (int i = 0; i < 2; i++) {
			DeleteSoundMem(shotSE[i]);
			DeleteSoundMem(damageSE[i]);
			DeleteSoundMem(cursorSE[i]);
		}
		DeleteSoundMem(cursorSE[2]);
		for (int i = 0; i < 6; i++) DeleteGraph(textgraph[i]);
	}
};