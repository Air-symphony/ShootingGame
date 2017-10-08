#include "InputKey.cpp"
#include "Material.cpp"
#include "PlayerUnit.cpp"
#include "EnemyUnit.cpp"
#include <iostream>

InputKey input;
Material material;

int saveHP = 5;
int saveweapon = 0;
int cleartime = 0;
bool loopbgm = false;

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
	DrawGraph(x - sizeX / 2, y - sizeY / 2, graph, TRUE);
}

class Game {
public:
	Game(){
	}
	bool GameScene(int scene) {
		Player player(material.playergraph, material.shotgraph, 3, material.reloadSE, material.shotSE, 2);
		player.SetHP(saveHP);
		player.Setweapon(saveweapon);
		player.SetDamageSE(material.damageSE, 2);

		const int count = 10;
		Enemy enemys[count];
		int maxcount = 5 + scene;//敵の数
		for (int i = 0; i < maxcount; i++) {
			enemys[i].Set(material.enemygraph[scene], material.shotgraph, 3, scene, (i + 1) * (SIZE_X / (maxcount + 1)), 100, 2);//横一列
			enemys[i].SetDamageSE(material.damageSE, 2);
		}

		ClearDrawScreen();// 画面を初期化する
		player.Move();
		for (int id = 0; id < maxcount; id++) {//enemy[count]の描画
			enemys[id].Move(player);
		}
		Draw_String("Please push SPACE");
		ScreenFlip();// 裏画面の内容を表画面に反映させる 
		while (ProcessMessage() == 0 && !input.PushOneframe_Decide()) {}

		if (CheckSoundMem(material.bgm_pre) && !loopbgm) {
			PlaySoundMem(material.bgm_pre, DX_PLAYTYPE_BACK);
		}

		bool start = true;//pause用
		while (ProcessMessage() == 0 && input.ForcedTermination())
		{
			loopbgm = (CheckSoundMem(material.bgm_loop) == 1);
			if (CheckSoundMem(material.bgm_pre) == 0 && !loopbgm) {
				PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			}
			else if (!loopbgm) {
				PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			}
			ClearDrawScreen();// 画面を初期化する

			DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", input.getJoypad());

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
				DrawGraph(10 + i * 30, SIZE_Y - 50, material.HPgraph, TRUE);//左上
			}
			//Draw_String(10 + 5 * 30, SIZE_Y - 50, "weapon(%d)", player.Getweapon());
			DrawFormatString(10 + 5 * 30, SIZE_Y - 60, GetColor(255, 255, 255), "weapon(%d)", player.Getweapon() + 1);
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

			if (input.PushOneframe_Stop()) {
				//Draw_String(SIZE_X / 2, SIZE_Y * 3 / 5, "BACKSPACE：Continue\nSPACE：Go back Title");
				while (ProcessMessage() == 0) {
					if (input.PushOneframe_KeyUP() ||
						input.PushOneframe_KeyDOWN()) {
						PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
						start = !start;
					}
					Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5, material.textgraph[0]);
					Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[2]);
					if (start) Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5, material.textgraph[1]);
					else Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[3]);
					Draw_String("Pause");

					if (input.PushOneframe_RETURN()) {
						if (start) {
							PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
							break;
						}
						else {
							PlaySoundMem(material.cursorSE[1], DX_PLAYTYPE_BACK);
							StopSoundMem(material.bgm_loop);
							return false;
						}
					}
				}
			}
			if (input.PushOneframe_RETURN()) finish = 0;
			ScreenFlip();// 裏画面の内容を表画面に反映させる
			if (player.GetHP() <= 0 || finish <= 0) {
				if (player.GetHP() <= 0) {
					StopSoundMem(material.bgm_loop);
					Draw_String("Game Over");
				}
				else if (scene == 5) {
					StopSoundMem(material.bgm_loop);
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
};

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

	material.SetMatrial();
	SetFontSize(40);

	Game game;
	int select = 0;//startにカーソルがあるかどうか
	while (ProcessMessage() == 0 && input.ForcedTermination()) {
		ClearDrawScreen();// 画面を初期化する
		if (input.PushOneframe_KeyUP()) {
			PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
			select -= 1;
			if (select < 0)select += 3;
		}
		if (input.PushOneframe_KeyDOWN()) {
			PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
			select += 1;
			select %= 3;
		}

		clsDx();

		if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) {
			printfDx("A");
		}
		if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP) != 0) {
			printfDx("UP");
		}
		if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN) != 0) {
			printfDx("DOWN");
		}

		DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", input.getJoypad());

		Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10, material.textgraph[0]);
		Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10, material.textgraph[4]);
		Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[2]);
		if (select == 0) Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10, material.textgraph[1]);
		else if (select == 1)Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10, material.textgraph[5]);
		else Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[3]);

		Draw_String(SIZE_X / 2, SIZE_Y / 5, "Shooting");
		ScreenFlip();// 裏画面の内容を表画面に反映させる

		if (input.PushOneframe_Decide()) {
			if (select == 0) {
				PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
				saveHP = 5;
				saveweapon = 0;
				cleartime = GetNowCount();
				loopbgm = false;
				for (int i = 1; i <= 5; i++) {
					if (!game.GameScene(i)) {
						break;
					}
				}
			}
			else if (select == 1) {
				PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
				ScreenFlip();// 裏画面の内容を表画面に反映させる
				//while (0) {

				//}
			}
			else {
				PlaySoundMem(material.cursorSE[1], DX_PLAYTYPE_BACK);
				break;
			}
		}
	}
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}
