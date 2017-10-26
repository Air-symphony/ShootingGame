#pragma once
#include "InputKey.cpp"
#include "Material.cpp"
#include "PlayerUnit.cpp"
#include "EnemyUnit.cpp"
#include "FileReader.cpp"
#include <iostream>

InputKey input;
Material material;

/*�����̍����̎擾*/
int fontsize = 40;

/*
fontsize = size;
SetFontSize(fontsize);
*/
void FontSize(int size) {
	fontsize = size;
	SetFontSize(fontsize);
}
/*
x, y = �摜��\�������������W
str = ������
number = ����̈ʒu�𒆐S�Ƃ��ĕ\��(�e���L�[)
*/
void Draw_String(int x, int y, char str[], int number = 5) {//flip����
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	if (number == 1)
		DrawString(x, y, str, GetColor(255, 255, 255));
	else if (number == 2)
		DrawString(x - StrWidth / 2, y, str, GetColor(255, 255, 255));
	else if (number == 3)
		DrawString(x - StrWidth, y, str, GetColor(255, 255, 255));
	else if (number == 4)
		DrawString(x, y - fontsize / 2, str, GetColor(255, 255, 255));
	else if (number == 5)
		DrawString(x - StrWidth / 2, y - fontsize / 2, str, GetColor(255, 255, 255));
	else if (number == 6)
		DrawString(x - StrWidth, y - fontsize / 2, str, GetColor(255, 255, 255));
	else if (number == 7)
		DrawString(x, y - fontsize, str, GetColor(255, 255, 255));
	else if (number == 8)
		DrawString(x - StrWidth / 2, y - fontsize, str, GetColor(255, 255, 255));
	else if (number == 9)
		DrawString(x - StrWidth, y - fontsize, str, GetColor(255, 255, 255));
}
/*
DrawFormatString(x - StrWidth / 2, y - fontsize / 2, GetColor(255, 255, 255), str, item);
*/
void Draw_String(int x, int y, char str[], double item) {
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawFormatString(x - StrWidth / 2, y - fontsize / 2, GetColor(255, 255, 255), str, item);
}
/*
DrawString(SIZE_X / 2 - StrWidth / 2, SIZE_Y / 2 - fontsize / 2, str, GetColor(255, 255, 255));
ScreenFlip();
*/
void Draw_String(char str[]) {
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(SIZE_X / 2 - StrWidth / 2, SIZE_Y / 2 - fontsize / 2, str, GetColor(255, 255, 255));
	ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
}

void PrintRanking(FileReader fileReader, int rank);

int GameSceneCount = 5;
class Game {
private:
	Player player;
	int enemyCount;

	bool loopbgm;
	bool isClear;
	int clearTime;//�N���A���Ԃ̕ۑ�
public:
	Game(){
	}
	/*�Q�[���J�n���̏����ݒ�*/
	void init() {
		loopbgm = false;
		isClear = false;
		player.Set(material.player, material.shot, 3, material.reloadSE, material.shotSE, 2);
		player.SetDamageSE(material.damageSE, 2);
		clearTime = GetNowCount();
	}

	/*�Q�[�����e*/
	bool PlayGame(int scene) {
		int _stoptime = GetNowCount();
		player.SetInit();
		
		Enemy enemys[10];
		enemyCount = 5 + scene;//�G�̐�
		for (int i = 0; i < enemyCount; i++) {
			enemys[i].Set(material.enemy[scene - 1], material.shot, 3, scene, (i + 1) * (SIZE_X / (enemyCount + 1)), 100, 2);//�����
			enemys[i].SetDamageSE(material.damageSE, 2);
		}

		ClearDrawScreen();// ��ʂ�����������
		/*�����`����e*/
		player.Draw();
		for (int id = 0; id < enemyCount; id++) {//enemy[count]�̕`��
			enemys[id].Draw();
		}
		for (int i = 0; i < player.GetHP(); i++) {//HP�\��
			material.HP.Draw_Graph(10 + i * 30, SIZE_Y - 50, 1);
		}
		/*���͑҂�*/
		Draw_String(SIZE_X / 2, SIZE_Y / 2, "Please Push", 8);
		if (input.getJoypad() > 0) Draw_String(SIZE_X/ 2, SIZE_Y / 2, "A", 2);
		else Draw_String(SIZE_X / 2, SIZE_Y / 2, "SPACE", 2);

		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������ 

		while (ProcessMessage() == 0 && input.ForcedTermination() && !input.PushOneframe_Decide()) {}
		_stoptime = GetNowCount() - _stoptime;
		clearTime += _stoptime;

		if (CheckSoundMem(material.bgm_pre) == 0 && !loopbgm) {
			PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			loopbgm = (CheckSoundMem(material.bgm_loop) == 1);
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

			/*�v���C���[�̓���*/
			player.Move();
			player.Shoot();

			/*����^�C�v�̕\��*/
			material.weapon[player.Getweapon()].Draw_Graph(SIZE_X, SIZE_Y, 9);
			
			/*�G�̐�*/
			int finish = enemyCount;
			/*�G�̓���*/
			for (int id = 0; id < enemyCount; id++) {//enemy[count]�̕`��
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
				material.HP.Draw_Graph(10 + i * 30, SIZE_Y - 50, 1);
			}
			
			//�f�o�b�O�p
			/*input.setJoypad();
			DrawFormatString(0, 0, GetColor(255, 255, 255), "joypad=%d", input.getJoypad());
			clsDx();
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
				while (ProcessMessage() == 0 && input.ForcedTermination()) {
					int stoptime = GetNowCount();
					if (input.PushOneframe_KeyUP() ||
						input.PushOneframe_KeyDOWN()) {
						PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
						start = !start;
					}
					material.menu_Start[0].Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5);
					material.menu_Finish[0].Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10);
					if (start) material.menu_Start[1].Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5);
					else material.menu_Finish[1].Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10);
					Draw_String("Pause");

					if (input.PushOneframe_Decide()) {
						if (start) {
							stoptime = GetNowCount() - stoptime;
							clearTime = clearTime + stoptime;
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

			/*���̃V�[�����I�������*/
			if (player.GetHP() <= 0 || finish <= 0) {
				/*�������ꍇ*/
				if (player.GetHP() <= 0) {
					StopSoundMem(material.bgm_loop);
					Draw_String("Game Over");
				}
				/*�S�ẴV�[�����I�������*/
				else if (scene >= GameSceneCount) {
					clearTime = GetNowCount() - clearTime;
					double time = (((double)clearTime) / 1000.0);
					Draw_String(SIZE_X / 2, SIZE_Y / 2 + 50, "Time : %lf s", time);
					Draw_String("Finish");
					isClear = true;
					StopSoundMem(material.bgm_loop);
				}
				/*�r���V�[�����I�������*/
				else {
					Draw_String("Next Stage");
				}
				int waittime = 1000;
				WaitTimer(waittime);//1�b�ҋ@
				if (!isClear) clearTime += waittime;

				return player.GetHP() > 0;
			}
		}
		return false;
	}

	int getCleartime() {
		return clearTime;
	}
	bool getClear() {
		return isClear;
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
	FontSize(40);

	FileReader fileReader;
	Game game;
	int select = 0;//�J�[�\���̈ʒu
	while (ProcessMessage() == 0 && input.ForcedTermination()) {
		ClearDrawScreen();// ��ʂ�����������
		clsDx();
		input.setJoypad();

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
	
		FontSize(60);
		Draw_String(SIZE_X / 2, SIZE_Y / 4, "Shooting Star");

		material.menu_Start[0].Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10);
		material.menu_Ranking[0].Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10);
		material.menu_Finish[0].Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10);

		if (select == 0) material.menu_Start[1].Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10);
		else if (select == 1)material.menu_Ranking[1].Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10);
		else material.menu_Finish[1].Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10);

		FontSize(20);
		if (input.getJoypad() > 0) Draw_String(SIZE_X, SIZE_Y, "JoyPad connected.", 9);
		else Draw_String(SIZE_X, SIZE_Y, "JoyPad didn't connect.", 9);
		FontSize(40);
		
		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������

		if (input.PushOneframe_Decide()) {
			/*Start*/
			if (select == 0) {
				PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
				game.init();
				/*�Q�[�����e*/
				for (int i = 1; i <= GameSceneCount; i++) {
					if (!game.PlayGame(i)) {
						break;
					}
				}
				/*�����I���̏ꍇ�͖���*/
				if (game.getClear()) {
					int rank = fileReader.CheckInRanking((double)game.getCleartime() / 1000.0);
					PrintRanking(fileReader, rank);
				}
			}
			/*Ranking*/
			else if (select == 1) {
				PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
				PrintRanking(fileReader, -1);
			}
			/*Finish*/
			else {
				PlaySoundMem(material.cursorSE[1], DX_PLAYTYPE_BACK);
				break;
			}
		}
	}
	material.DeleteMaterial();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}

/*�����L���O�̕\��
0 <= rank <= 9 �����L���O����*/
void PrintRanking(FileReader fileReader, int rank) {
	float rankData[10];
	for (int i = 0; i < fileReader.getRANKING(); i++) {
		rankData[i] = fileReader.rank[i];
	}

	ClearDrawScreen();// ��ʂ�����������
	for (int i = 0; i < fileReader.getRANKING(); i++) {
		DrawFormatString(SIZE_X / 3 - 20, 100 + 40 * i, GetColor(255, 255, 255), "%d��:", i + 1);
		DrawFormatString(SIZE_X / 3 + 80, 100 + 40 * i, GetColor(255, 255, 255), "%f", rankData[i]);
	}
	if (0 <= rank) {
		DrawFormatString(SIZE_X / 3 + 80, 100 + 40 * rank, GetColor(0, 255, 255), "%f", rankData[rank]);
		fileReader.UpdateFile();
	}
	ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
	while (ProcessMessage() == 0 && input.ForcedTermination()) {
		if (input.PushOneframe_Decide()) {
			PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
			break;
		}
	}
}