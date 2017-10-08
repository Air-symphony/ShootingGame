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

void Draw_String(int x, int y, char str[]) {//flip����
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(x - StrWidth / 2, y, str, GetColor(255, 255, 255));
}
void Draw_String(int x, int y, char str[], int item) {//flip����
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawFormatString(x - StrWidth / 2, y, GetColor(255, 255, 255), str, item);
}
void Draw_String(char str[]) {
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(SIZE_X / 2 - StrWidth / 2, SIZE_Y / 2, str, GetColor(255, 255, 255));
	ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
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
		int maxcount = 5 + scene;//�G�̐�
		for (int i = 0; i < maxcount; i++) {
			enemys[i].Set(material.enemygraph[scene], material.shotgraph, 3, scene, (i + 1) * (SIZE_X / (maxcount + 1)), 100, 2);//�����
			enemys[i].SetDamageSE(material.damageSE, 2);
		}

		ClearDrawScreen();// ��ʂ�����������
		player.Move();
		for (int id = 0; id < maxcount; id++) {//enemy[count]�̕`��
			enemys[id].Move(player);
		}
		Draw_String("Please push SPACE");
		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������ 
		while (ProcessMessage() == 0 && !input.PushOneframe_Decide()) {}

		if (CheckSoundMem(material.bgm_pre) && !loopbgm) {
			PlaySoundMem(material.bgm_pre, DX_PLAYTYPE_BACK);
		}

		bool start = true;//pause�p
		while (ProcessMessage() == 0 && input.ForcedTermination())
		{
			loopbgm = (CheckSoundMem(material.bgm_loop) == 1);
			if (CheckSoundMem(material.bgm_pre) == 0 && !loopbgm) {
				PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			}
			else if (!loopbgm) {
				PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			}
			ClearDrawScreen();// ��ʂ�����������

			DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", input.getJoypad());

			player.Move();
			player.Shoot();

			int finish = maxcount;//�G�̐�
			for (int id = 0; id < maxcount; id++) {//enemy[count]�̕`��
				if (enemys[id].Move(player)) {//�G�̕`��ƒe����
					player.Damege(enemys[id].Getattack());
				}
				if (enemys[id].GetHP() > 0) {//�����Ă����
					if (player.HitCheck(enemys[id])) {//�����̒e�̊m�F
						enemys[id].Damege(player.Getattack());
					}
				}
				else {
					finish--;
				}
			}

			for (int i = 0; i < player.GetHP(); i++) {//HP�\��
				DrawGraph(10 + i * 30, SIZE_Y - 50, material.HPgraph, TRUE);//����
			}
			//Draw_String(10 + 5 * 30, SIZE_Y - 50, "weapon(%d)", player.Getweapon());
			DrawFormatString(10 + 5 * 30, SIZE_Y - 60, GetColor(255, 255, 255), "weapon(%d)", player.Getweapon() + 1);
			//�f�o�b�O�p
			/*clsDx();
			printfDx("HP=%d, (%d.%d, %d)\n", player.GetHP(), player.GetX(), player.GetY(), player.Getweapon());
			printfDx("�G(�c��%d��)\n", finish);
			for (int id = 0; id < maxcount; id++) {//enemy[count]�̕`��
				printfDx("%d_%d(",id, enemys[id].GetHP());
				for (int i = 0; i < enemys[id].Getshotcount(); i++) {
					printfDx("%d,", enemys[id].Getshot(i));
				}
				printfDx(")\n");
			}*/

			if (input.PushOneframe_Stop()) {
				//Draw_String(SIZE_X / 2, SIZE_Y * 3 / 5, "BACKSPACE�FContinue\nSPACE�FGo back Title");
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
			ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
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
				WaitTimer(1000);//1�b�ҋ@
				return player.GetHP() > 0;
			}
		}
		return false;
	}
};

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	ChangeWindowMode(TRUE);//window���[�h�ɐݒ�
	SetGraphMode(SIZE_X, SIZE_Y, 32);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);

	material.SetMatrial();
	SetFontSize(40);

	Game game;
	int select = 0;//start�ɃJ�[�\�������邩�ǂ���
	while (ProcessMessage() == 0 && input.ForcedTermination()) {
		ClearDrawScreen();// ��ʂ�����������
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
		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������

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
				ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
				//while (0) {

				//}
			}
			else {
				PlaySoundMem(material.cursorSE[1], DX_PLAYTYPE_BACK);
				break;
			}
		}
	}
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}
