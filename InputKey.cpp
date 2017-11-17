#pragma once
#include "DxLib.h"

/*
PAD_INPUT_1 A
PAD_INPUT_2 B
PAD_INPUT_3 X
PAD_INPUT_4 Y
PAD_INPUT_5 L
PAD_INPUT_6 R
PAD_INPUT_7 SELECT
PAD_INPUT_8 START
*/
class InputKey {
private:
	int id;
	int joypad;
	static const int keytype = 9;//1frameÉ{É^ÉìÇÃéÌóﬁ
	int keytime[keytype];
	bool PushOneframe(int keyCode) {
		if (CheckHitKey(keyCode) == 1) keytime[id]++;
		else keytime[id] = 0;
		return keytime[id] == 1;
	}
	bool PushOneframe(int keyCode, int PADID, int PADkeyCode) {
		if (joypad > 0) {
			if ((GetJoypadInputState(PADID) & PADkeyCode) != 0) keytime[id]++;
			else keytime[id] = 0;
		}
		else {
			if (CheckHitKey(keyCode) == 1) keytime[id]++;
			else keytime[id] = 0;
		}
		return keytime[id] == 1;
	}
	bool PushLong(int keyCode, int PADID, int PADkeyCode) {
		if (joypad > 0) return (GetJoypadInputState(PADID) & PADkeyCode) != 0;
		return CheckHitKey(keyCode) == 1;	
	}
public:
	InputKey() {
		joypad = GetJoypadNum();
	}
	void setJoypad() {
		joypad = GetJoypadNum();
	}
	int getJoypad() {
		return joypad;
	}
	/*0, KEY_INPUT_SPACE*/
	bool PushOneframe_Decide() {
		id = 0;
		return PushOneframe(KEY_INPUT_SPACE, DX_INPUT_PAD1, PAD_INPUT_1);
	}
	/*0, KEY_INPUT_SPACE*/
	bool PushOneframe_Attack() {
		id = 0;
		return PushOneframe(KEY_INPUT_SPACE, DX_INPUT_PAD1, PAD_INPUT_1);
	}
	/*1, KEY_INPUT_C*/
	bool PushOneframe_WeaponChange() {
		id = 1;
		return PushOneframe(KEY_INPUT_C, DX_INPUT_PAD1, PAD_INPUT_5);
	}
	/*2, KEY_INPUT_BACK*/
	bool PushOneframe_Stop() {
		id = 2;
		return PushOneframe(KEY_INPUT_BACK, DX_INPUT_PAD1, PAD_INPUT_8);
	}
	/*3, KEY_INPUT_R*/
	bool PushOneframe_Reset() {
		id = 3;
		//if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_7);
		return PushOneframe(KEY_INPUT_R);
	}
	/*4, KEY_INPUT_RETURN*/
	bool PushOneframe_RETURN() {
		id = 4;
		return PushOneframe(KEY_INPUT_RETURN, DX_INPUT_PAD1, PAD_INPUT_7);
	}
	/*5, KEY_INPUT_UP*/
	bool PushOneframe_KeyUP() {
		id = 5;
		return PushOneframe(KEY_INPUT_UP, DX_INPUT_PAD1, PAD_INPUT_UP);
	}
	/*6, KEY_INPUT_DOWN*/
	bool PushOneframe_KeyDOWN() {
		id = 6;
		return PushOneframe(KEY_INPUT_DOWN, DX_INPUT_PAD1, PAD_INPUT_DOWN);
	}
	/*7, KEY_INPUT_UP*/
	bool PushOneframe_KeyLEFT() {
		id = 7;
		return PushOneframe(KEY_INPUT_LEFT, DX_INPUT_PAD1, PAD_INPUT_LEFT);
	}
	/*8, KEY_INPUT_DOWN*/
	bool PushOneframe_KeyRIGHT() {
		id = 8;
		return PushOneframe(KEY_INPUT_RIGHT, DX_INPUT_PAD1, PAD_INPUT_RIGHT);
	}


	/*CheckHitKey(KEY_INPUT_UP) == 1*/
	bool Push_KeyUP() {
		return PushLong(KEY_INPUT_UP, DX_INPUT_PAD1, PAD_INPUT_UP);
	}
	/*CheckHitKey(KEY_INPUT_DOWN) == 1*/
	bool Push_KeyDOWN() {
		return PushLong(KEY_INPUT_DOWN, DX_INPUT_PAD1, PAD_INPUT_DOWN);
	}
	/*CheckHitKey(KEY_INPUT_LEFT) == 1*/
	bool Push_KeyLEFT() {
		return PushLong(KEY_INPUT_LEFT, DX_INPUT_PAD1, PAD_INPUT_LEFT);
	}
	/*CheckHitKey(KEY_INPUT_RIGHT) == 1*/
	bool Push_KeyRIGHT() {
		return PushLong(KEY_INPUT_RIGHT, DX_INPUT_PAD1, PAD_INPUT_RIGHT);
	}
	/*ã≠êßèIóπ âüÇ≥ÇÍÇƒÇ»ÇØÇÍÇŒ
	CheckHitKey(KEY_INPUT_ESCAPE) == 0*/
	bool ForcedTermination() {
		return CheckHitKey(KEY_INPUT_ESCAPE) == 0;
	}
};