#include "DxLib.h" // �v���O������WinMain����n�܂�܂�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//window���[�h�ɐݒ�
	SetGraphMode(500, 500, 32);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{
		return -1; // �G���[���N�����璼���ɏI��
	}

	int x = 0;

	int id = LoadGraph("C:\\Users\\admin\\Desktop\\img\\photo.jpg");
	int size_x, size_y;
	GetGraphSize(id, &size_x, &size_y);
	DrawGraph(x - size_x / 2, 200 - size_y / 2, id, true);

	bool check = false;
	int text[] = { LoadGraph("C:\\Users\\admin\\Desktop\\img\\Start_1.png"), LoadGraph("C:\\Users\\admin\\Desktop\\img\\Start_2.png"),
		LoadGraph("C:\\Users\\admin\\Desktop\\img\\Finish_1.png") ,LoadGraph("C:\\Users\\admin\\Desktop\\img\\Finish_2.png") };
	int updown[] = { 0,0 };
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();// ��ʂ�����������

		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			x = x + 1;
		}
		else if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			x = x - 1;
		}
		DrawGraph(x - size_x / 2, 200, id, true);

		if (CheckHitKey(KEY_INPUT_UP) == 1) {
			updown[0] = updown[0] + 1;
		}
		else {
			updown[0] = 0;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
			updown[1] = updown[1] + 1;
		}
		else {
			updown[1] = 0;
		}
		if (updown[0] == 1 || updown[1] == 1) {
			check = !check;
		}
		if (check) {
			DrawGraph(200, 300, text[1], true);
			DrawGraph(200, 350, text[2], true);
		}
		else {
			DrawGraph(200, 300, text[0], true);
			DrawGraph(200, 350, text[3], true);
		}

		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
	}

	DxLib_End(); // �c�w���C�u�����g�p�̏I������
	return 0; // �\�t�g�̏I��
}