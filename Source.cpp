#include "DxLib.h"
#include <math.h>
#include <iostream>
#define PI 3.14159	// 円周率

//#include<stdio.h>
//#include <stdlib.h>
//#include <iostream>
//#include <string>
//#include <cstdio>

const int SIZE_X = 800;//原点は左上
const int SIZE_Y = 700;//画面サイズ

int playergraph =  -1;
int HPgraph = -1;
int enemygraph[5];
int shotgraph[3];
int textgraph[6];
int bgm_pre;
int bgm_loop;
int shotSE[2];
int reloadSE;
int damageSE[2];
int cursorSE[3];//0=dicide,1=cancel,2=move

const int keytype = 7;//1frameボタンの種類
int keytime[keytype];
bool PushOneframe(int keyCode) {
	int id;
	switch (keyCode) {
	case KEY_INPUT_SPACE://攻撃
		id = 0;
		break;
	case KEY_INPUT_C://武器切り替え
		id = 1;
		break;
	case KEY_INPUT_BACK://一時停止
		id = 2;
		break;
	case KEY_INPUT_R://Reset
		id = 3;
		break;
	case KEY_INPUT_RETURN://デバッグ用
		id = 4;
		break;
	case KEY_INPUT_UP://選択用
		id = 5;
		break;
	case KEY_INPUT_DOWN://選択用
		id = 6;
		break;
	}
	if (CheckHitKey(keyCode) == 1) keytime[id]++;
	else keytime[id] = 0;

	return keytime[id] == 1;
}

class Unit {
public:
	int X, Y;//中心
	int sizeX, sizeY;//中央からの画像サイズ

	void Draw(int graph) {
		X = (X + SIZE_X) % SIZE_X;//画面端の反転
		if (graph != -1) {// プレイヤーを描画する
			DrawGraph(X - sizeX,
				Y - sizeY, graph, TRUE);//左上
		}
		else {
			Draw_GreenBox();
			if (0 >= X - sizeX) {//左端
				Draw_GreenBox(X + SIZE_X,  Y);
			}
			else if (X + sizeX >= SIZE_X) {//右端
				Draw_GreenBox(X - SIZE_X, Y);
			}
		}
	}
	void Draw_RedBox() {
		DrawBox(X - sizeX, Y - sizeY,//左上
			X + sizeX, Y + sizeY,//右下
			GetColor(255, 0, 0), TRUE);
	}
	void Draw_GreenBox() {
		DrawBox(X - sizeX, Y - sizeY,//左上
			X + sizeX, Y + sizeY,//右下
			GetColor(0, 255, 0), TRUE);
	}
	void Draw_BlueBox() {
		DrawBox(X - sizeX, Y - sizeY,//左上
			X + sizeX, Y + sizeY,//右下
			GetColor(0, 0, 255), TRUE);
	}
	void Draw_GreenBox(int x, int y) {
		DrawBox(x - sizeX, y - sizeY,//左上
			x + sizeX, y + sizeY,//右下
			GetColor(0, 255, 0), TRUE);
	}
	int GetX() {
		return X;
	}
	int GetY() {
		return Y;
	}
	int GetsizeX() {
		return sizeX;
	}
	int GetsizeY() {
		return sizeY;
	}
};

class Shot :public Unit{
private:
	bool shot = false;
	bool hit;
	int graphID;
	int type;
	int time = 0;
	int keepY = 0;
public:
	void Set(int graph, int x, int y,int t) {
		if (graph != -1) {
			graphID = graph;
			GetGraphSize(graph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
		}
		else {
			sizeX = sizeY = 20 / 2;
		}
		X = x;
		Y = y;
		type = t;
		shot = true;
		keepY = Y;
	}
	void Set(int graph, int x, int y, int ty, int ti) {
		if (graph != -1) {
			graphID = graph;
			GetGraphSize(graph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
		}
		else {
			sizeX = sizeY = 20 / 2;
		}
		X = x;
		Y = y;
		type = ty;
		shot = true;
		time = ti;
	}
	bool PlayerMove() {
		if (shot) {
			Draw(graphID);
			if (type == 0) {
				Y -= 15;
			}
			else if (type == 1){// || (type == 2) {
				time += 2;
				Y -= 10;
				X += (int)(cos(PI / 45 * time) * 5);
			}
			else if (type == 2) {
				DrawBox(X - 2, Y, X + 2, 0, GetColor(255, 0, 0), TRUE);
				Y -= (keepY - 1);
			}
		}
		shot = Y > 0;
		return shot;
	}
	bool EnemyMove(Unit p) {
		if (shot) {
			Draw(graphID);
			if (type == 1) {
				Y += 7;
			}
			else if (type == 2) {
				time += 2;
				Y += 4;
				X += (int)(cos(PI / 90 * time) * 4);
			}
			else if (type == 3) {
				Y += 11;
			}
			else if (type == 4) {
				time += 4;
				Y += 7;
				X += (int)(cos(PI / 45 * time) * 4);
			}
			else if (type == 5) {
				time += 1;
				time %= 2;//何フレームか
				Y += 3;
				if (time == 0) {
					int dx = 1;
					if (abs(p.GetX() - X) < SIZE_X / 2) {
						if (X - p.GetX() > 0) {
							dx = -dx;
						}
					}
					else {
						if (X - p.GetX() < 0) {
							dx = -dx;
						}
					}
					X += dx;
				}
			}
		}
		shot = Y < SIZE_Y;
		return shot;
	}
	bool Hit(Unit e, bool player) {
		if (player && type == 2) {
			if (e.GetX() - e.GetsizeX() <= X && X <= e.GetX() + e.GetsizeX()) {
				shot = false;
				return true;
			}
		}
		else{
			if (abs(Y - e.GetY()) <= sizeY + e.GetsizeY()) {
				if (abs(X - e.GetX()) <= sizeX + e.GetsizeX()) {
					shot = false;
					return true;
				}
			}
		}
		return false;
	}
	void Setshot(bool s) {
		shot = s;
	}
	bool Getshot() {
		return shot;
	}
};

class Character :public Unit {
public:
	int HP = 0;
	int attack = 0;
	int movespeed = 0;//動き
	int time = 0;
	void Damege(int i) {
		HP -= i;
		if (HP > 0) {
			PlaySoundMem(damageSE[0], DX_PLAYTYPE_BACK, TRUE);
		}
		else {
			PlaySoundMem(damageSE[1], DX_PLAYTYPE_BACK, TRUE);
		}
		Draw_RedBox();
	}
	void SetHP(int hp) {
		HP = hp;
	}
	int GetHP() {
		return HP;
	}
	int Getattack() {
		return attack;
	}
	int Gettime() {
		return time;
	}
};

const int shotlimit = 3;
int weaponkind = 3;
class Player:public Character {
private:
	int weapon;
	int inertiaX = 0, inertiaY = 0;
	int inertiaY_time = 0, inertiaX_time = 0;
	int shotcount = 0;
	Shot shots[shotlimit];
	bool laserwait = false;
public:
	Player(int graph) {
		if (graph != -1) {
			GetGraphSize(graph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
		}
		else {
			sizeX = sizeY = 40 / 2;
		}
		X = SIZE_X * 2 / 4;
		Y = SIZE_Y * 5 / 6;
		weapon = 0;
		movespeed = 5;
		HP = 5;
		attack = 1;
	}
	void Move() {
		if (!laserwait) {
			if (PushOneframe(KEY_INPUT_C)) {
				weapon++;
				weapon %= weaponkind;//武器の数
				PlaySoundMem(reloadSE, DX_PLAYTYPE_BACK, TRUE);
			}
			if (weapon == 2) {
				attack = 3;
			}
			else {
				attack = 1;
			}
			if (CheckHitKey(KEY_INPUT_UP) == 1) {
				inertiaY = -movespeed;
				Y -= movespeed;
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
				inertiaY = movespeed;
				Y += movespeed;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
				inertiaX = movespeed;
				X += movespeed;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
				inertiaX = -movespeed;
				X -= movespeed;
			}
		}
		Inertia(3);
		if (Y < 0 + sizeY) {//画面上
			Y = 0 + sizeY;
		}
		else if (SIZE_Y - sizeY < Y) {//画面下
			Y = SIZE_Y - sizeY;
		}
		Draw(playergraph);
	}
	void Inertia(int fpsspeed) {//何フレームで速度を落とすか
		if (CheckHitKey(KEY_INPUT_UP) == 0 && CheckHitKey(KEY_INPUT_DOWN) == 0 || laserwait ) {
			if (inertiaY != 0) {
				Y += inertiaY;
				inertiaY_time += 1;
				if (inertiaY_time >= fpsspeed) {
					inertiaY_time = 0;
					if (inertiaY < 0) {
						inertiaY += 1;
						if (inertiaY > 0) {
							inertiaY = 0;
						}
					}
					else {
						inertiaY -= 1;
						if (inertiaY < 0) {
							inertiaY = 0;
						}
					}
				}
			}
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) == 0 && CheckHitKey(KEY_INPUT_LEFT) == 0 ||  laserwait) {
			if (inertiaX != 0) {
				X += inertiaX;
				inertiaX_time += 1;
				if (inertiaX_time >= fpsspeed) {
					inertiaX_time = 0;
					if (inertiaX < 0) {
						inertiaX += 1;
						if (inertiaX > 0) {
							inertiaX = 0;
						}
					}
					else {
						inertiaX -= 1;
						if (inertiaX < 0) {
							inertiaX = 0;
						}
					}
				}
			}
		}
	}
	void Shoot() {
		if (PushOneframe(KEY_INPUT_SPACE)) {
			if (shotcount < shotlimit) {
				if (weapon == 0 || weapon == 1) {
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					shots[shotcount].Set(shotgraph[0], X, Y, weapon);
					shotcount++;
				}
				else if (weapon == 2) {
					if (shotcount == 0) {
						laserwait = true;
					}
				}
			}
		}
		if (laserwait) {
			time++;
			if (time >= 20) {
				laserwait = false;
				time = 0;
				PlaySoundMem(shotSE[1], DX_PLAYTYPE_BACK, TRUE);
				shots[shotcount].Set(shotgraph[0], X, Y, weapon);
				shotcount++;
			}
		}
		for (int i = 0; i < shotcount; i++) {
			if (shots[i].Getshot()) {
				if (!shots[i].PlayerMove()) {
					Compression(i);
					shotcount--;
				}
			}
		}
	}
	bool HitCheck(Unit e) {
		for (int i = 0; i < shotcount; i++) {
			if (shots[i].Getshot()) {
				if (shots[i].Hit(e, TRUE)) {
					Compression(i);
					shotcount--;
					return true;
				}
			}
		}
		return false;
	}
	void Compression(int i)
	{
		if (i + 1 < shotlimit)
		{
			if (shots[i + 1].Getshot()) {
				shots[i] = shots[i + 1];
				shots[i + 1].Setshot(FALSE);
				Compression(i + 1);
			}
		}
	}
	Shot* Getshots() {
		return shots;
	}
	int Getshotcount() {
		return shotcount;
	}
	int Getweapon() {
		return weapon;
	}
	void Setweapon(int type) {//0,1,2
		weapon = type;
	}
};

const int max = 10;
class Enemy:public Character{
private:
	int type;
	Shot shot[max];
	int maxshot, shotcount = 0;
public:
	void Set(int ty, int x, int y, int speed) {
		type = ty;
		if (enemygraph[type - 1] != -1) {
			GetGraphSize(enemygraph[type - 1], &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
		}
		else {
			sizeX = sizeY = 40 / 2;
		}
		HP = type;
		if (type == 1) {
			Setstatus(type, 1, 2);
		}
		else if (type == 2) {
			Setstatus(type, 1, 6);
		}
		else if (type == 3 ||type == 4) {
			Setstatus(type, 2, 5);
		}
		else if (type == 5) {
			Setstatus(type, 2, 10);
		}
		X = x;
		Y = y;
		movespeed = speed;
	}
	bool Move(Unit p) {
		if (time > 0) {
			time--;
		}
		if (HP > 0) {
			Draw(enemygraph[type - 1]);
			if (time <= 0 && shotcount < maxshot) {
				if (!shot[shotcount].Getshot()) {
					if (type == 1 || type == 3) {
						int dx = 20;
						if (X - sizeX - dx <= p.GetX() && p.GetX() <= X + sizeX + dx && p.GetY() > Y) {//どのタイミングで打つか
							shot[shotcount].Set(shotgraph[1], X, Y, type);
							shotcount++;
							time = 20 + (5 - type) * 10;
						}
					}
					else if (type == 4 || type == 5)
					{
						int dx = 100;
						if (X - sizeX - dx<= p.GetX() && p.GetX() <= X + sizeX + dx && p.GetY() > Y) {//どのタイミングで打つか
							shot[shotcount].Set(shotgraph[1], X, Y, type);
							shotcount++;
							time = 20 + (5 - type) * 10;
						}
					}
					else if (type == 2) {
						shot[shotcount].Set(shotgraph[2], X, Y, type);
						shotcount++;
						time = 40;
					}
				}
			}
			if (type == 4 || type == 5) {
				X += movespeed;
				X %= SIZE_X;
			}
			Y %= SIZE_Y;
		}
		bool hit[max] = { false };
		for (int i = 0; i < maxshot; i++) {
			if (shot[i].Getshot()) {
				if (!shot[i].EnemyMove(p)) {//弾の動きと速度
					shotcount--;
				}
				hit[i] = shot[i].Hit(p, FALSE);
			}
		}
		for (int i = 0; i < maxshot; i++) {
			if (hit[i]) {
				shotcount--;
				time += 20;
				return true;
			}
		}
		if (HP > 0) {
			return abs(Y - p.GetY()) <= sizeY + p.GetsizeY()
				&& abs(X - p.GetX()) <= sizeX + p.GetsizeX();//自機との接触
		}
		return false;
	}
	void Setstatus(int HP,int at, int shotcount) {//shotcount <= 10
		SetHP(HP);
		attack = at;
		maxshot = shotcount;
	}
	int Getshotcount() {
		return shotcount;
	}
	bool Getshot(int i) {
		return shot[i].Getshot();
	}
};

void Draw_String(int  x,int y,char str[]);
void Draw_String(int x, int y, char str[], int item);
void Draw_String(char str[]);
void Draw_Graph(int x, int y, int graph);

int saveHP = 5;
int saveweapon = 0;
int cleartime = 0;
bool loopbgm = false;
bool GameScene(int scene) {
	Player  player(playergraph);
	player.SetHP(saveHP);
	player.Setweapon(saveweapon);

	const int count = 10;
	Enemy enemys[count];
	int maxcount = 5 + scene;//敵の数
	for (int i = 0; i < maxcount; i++) {
		enemys[i].Set(scene, (i + 1) * (SIZE_X / (maxcount + 1)), 100, 2);//横一列
	}

	ClearDrawScreen();// 画面を初期化する
	player.Move();
	for (int id = 0; id < maxcount; id++) {//enemy[count]の描画
		enemys[id].Move(player);
	}
	Draw_String("Please push SPACE");
	ScreenFlip();// 裏画面の内容を表画面に反映させる 
	while(ProcessMessage() == 0 && !PushOneframe(KEY_INPUT_SPACE)) {}

	if (CheckSoundMem(bgm_pre) && ! loopbgm) {
		PlaySoundMem(bgm_pre, DX_PLAYTYPE_BACK);
	}

	bool start = true;//pause用
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		loopbgm = (CheckSoundMem(bgm_loop) == 1);
		if (CheckSoundMem(bgm_pre) == 0 && ! loopbgm) {
			PlaySoundMem(bgm_loop, DX_PLAYTYPE_BACK);
		}
		else if (! loopbgm) {
			PlaySoundMem(bgm_loop, DX_PLAYTYPE_BACK);
		}
		ClearDrawScreen();// 画面を初期化する

		player.Move();
		player.Shoot();

		int finish = maxcount;//敵の数
		for (int id = 0; id < maxcount; id++) {//enemy[count]の描画
			if (enemys[id].Move(player)) {//敵の描画と弾発射
				player.Damege(enemys[id].Getattack());
			}
			if (enemys[id].GetHP() > 0) {//生きていれば
				if (player.HitCheck(enemys[id])) {//自分の弾の確認
					enemys[id].Damege(player.Getattack());
				}
			}
			else {
				finish--;
			}
		}

		for (int i = 0; i < player.GetHP(); i++) {//HP表示
			DrawGraph(10 + i * 30, SIZE_Y - 50, HPgraph, TRUE);//左上
		}
		//Draw_String(10 + 5 * 30, SIZE_Y - 50, "weapon(%d)", player.Getweapon());
		DrawFormatString(10 + 5 * 30, SIZE_Y - 60,GetColor(255,255,255) ,"weapon(%d)", player.Getweapon() + 1);
		//デバッグ用
		/*clsDx();
		printfDx("HP=%d, (%d.%d, %d)\n", player.GetHP(), player.GetX(), player.GetY(), player.Getweapon());
		printfDx("敵(残り%d体)\n", finish);
		for (int id = 0; id < maxcount; id++) {//enemy[count]の描画
			printfDx("%d_%d(",id, enemys[id].GetHP());
			for (int i = 0; i < enemys[id].Getshotcount(); i++) {
				printfDx("%d,", enemys[id].Getshot(i));
			}
			printfDx(")\n");
		}*/

		if (PushOneframe(KEY_INPUT_BACK)) {
			//Draw_String(SIZE_X / 2, SIZE_Y * 3 / 5, "BACKSPACE：Continue\nSPACE：Go back Title");
			while (ProcessMessage() == 0) {
				if (PushOneframe(KEY_INPUT_UP) ||
					PushOneframe(KEY_INPUT_DOWN)) {
					PlaySoundMem(cursorSE[2], DX_PLAYTYPE_BACK);
					start = !start;
				}
				Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5, textgraph[0]);
				Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, textgraph[2]);
				if (start) Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5, textgraph[1]);
				else Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, textgraph[3]);
				Draw_String("Pause");

				if (PushOneframe(KEY_INPUT_RETURN)) {
					if (start) {
						PlaySoundMem(cursorSE[0], DX_PLAYTYPE_BACK);
						break;
					}
					else{
						PlaySoundMem(cursorSE[1], DX_PLAYTYPE_BACK);
						StopSoundMem(bgm_loop);
						return false;
					}
				}
			}
		}
		if (PushOneframe(KEY_INPUT_RETURN)) finish = 0;
		ScreenFlip();// 裏画面の内容を表画面に反映させる
		if (player.GetHP() <= 0 || finish <= 0) {
			if (player.GetHP() <= 0) {
				StopSoundMem(bgm_loop);
				Draw_String("Game Over");
			}
			else if (scene == 5) {
				StopSoundMem(bgm_loop);
				cleartime = GetNowCount() - cleartime;
				Draw_String(SIZE_X / 2, SIZE_Y / 2 + 50, "Time : %d.0s", (int)(cleartime / 1000));
				Draw_String("Finish");
			}
			else {
				saveHP = player.GetHP();
				saveweapon = player.Getweapon();
				Draw_String("Next Stage");
			}
			WaitTimer(1000);//1秒待機
			return player.GetHP() > 0;
		}
	}
	return false;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//windowモードに設定
	SetGraphMode(SIZE_X, SIZE_Y, 32);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//初期化処理後、画像の読み込み
	playergraph = LoadGraph("Picture\\Player.png");//相対パス
	HPgraph = LoadGraph("Picture\\HP.png");
	char enemyNames[5][140] = { "Picture\\Enemy_1.png","Picture\\Enemy_2.png","Picture\\Enemy_3.png"
		,"Picture\\Enemy_4.png","Picture\\Enemy_5.png" };
	for (int i = 0; i < 5; i++)  enemygraph[i] = LoadGraph(enemyNames[i]);
	char shotNames[3][140] = { "Picture\\Shot1.png", "Picture\\Shot2.png", "Picture\\Shot3.png" };
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
	SetFontSize(40);

	char textNames[6][140] = { "Picture\\start_1.png", "Picture\\Start_2.png", "Picture\\Finish_1.png","Picture\\Finish_2.png",
		"Picture\\About_1.png", "Picture\\About_2.png" };
	for (int i = 0; i < 6; i++) textgraph[i] = LoadGraph(textNames[i]);

	int select = 0;//startにカーソルがあるかどうか
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();// 画面を初期化する
		if (PushOneframe(KEY_INPUT_UP)) {
			PlaySoundMem(cursorSE[2], DX_PLAYTYPE_BACK);
			select -= 1;
			if (select < 0)select += 3;
		}
		if (PushOneframe(KEY_INPUT_DOWN)) {
			PlaySoundMem(cursorSE[2], DX_PLAYTYPE_BACK);
			select += 1;
			select %= 3;
		}

		Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10, textgraph[0]);
		Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10, textgraph[4]);
		Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, textgraph[2]);
		if (select == 0) Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10, textgraph[1]);
		else if (select == 1)Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10, textgraph[5]);
		else Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, textgraph[3]);

		Draw_String(SIZE_X / 2, SIZE_Y / 5, "Shooting");
		ScreenFlip();// 裏画面の内容を表画面に反映させる

		if (PushOneframe(KEY_INPUT_RETURN)) {
			if (select == 0) {
				PlaySoundMem(cursorSE[0], DX_PLAYTYPE_BACK);
				saveHP = 5;
				saveweapon = 0;
				cleartime = GetNowCount();
				loopbgm = false;
				for (int i = 1; i <= 5; i++) {
					if (!GameScene(i)) {
						break;
					}
				}
			}
			else if (select == 1) {
				PlaySoundMem(cursorSE[0], DX_PLAYTYPE_BACK);
				ScreenFlip();// 裏画面の内容を表画面に反映させる
				//while (0) {

				//}
			}
			else {
				PlaySoundMem(cursorSE[1], DX_PLAYTYPE_BACK);
				break;
			}
		}
	}
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}

void Draw_String(int x, int y, char str[]) {//flip無し
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(x - StrWidth / 2, y, str, GetColor(255, 255, 255));
}
void Draw_String(int x, int y, char str[], int item) {//flip無し
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawFormatString(x - StrWidth / 2, y, GetColor(255, 255, 255), str, item);
}
void Draw_String(char str[]) {
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(SIZE_X / 2 - StrWidth / 2, SIZE_Y / 2, str, GetColor(255, 255, 255));
	ScreenFlip();// 裏画面の内容を表画面に反映させる
}
void Draw_Graph(int x, int y, int graph) {
	int sizeX, sizeY;
	if (graph != -1) {
		GetGraphSize(graph, &sizeX, &sizeY);
	}
	else {
		sizeX = sizeY = 100;
	}
	DrawGraph(x - sizeX / 2, y - sizeY /2 , graph, TRUE);
}
