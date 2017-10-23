#pragma once
#include "Dxlib.h"
/*
�摜�\���֌W
int graphID;
int sizeX, sizeY;
*/
class Graph {
public:
	/*�摜�n���h��*/
	int graphID;
	/*���S����̉摜�T�C�Y*/
	int sizeX, sizeY;
	Graph() {
		graphID = NULL;
	}
	/*
	Graph(LoadGraph("\\00.png"))
	*/
	Graph(int _graphID) {
		graphID = _graphID;
		GetGraphSize(graphID, &sizeX, &sizeY);
		sizeX = sizeX / 2;
		sizeY = sizeY / 2;
	}
	/*
	SetGraph(LoadGraph("\\00.png"))
	*/
	void SetGraph(int graph) {
		graphID = graph;
		GetGraphSize(graphID, &sizeX, &sizeY);
		sizeX = sizeX / 2;
		sizeY = sizeY / 2;
	}
	/*
	x, y = �摜��\�������������W
	number = ����̈ʒu�𒆐S�Ƃ��ĕ\��(�e���L�[)
	*/
	void Draw_Graph(int x, int y, int number) {
		if (number == 1)
			DrawGraph(x, y, graphID, TRUE);
		else if (number == 2)
			DrawGraph(x - sizeX, y, graphID, TRUE);
		else if (number == 3)
			DrawGraph(x + sizeX , y, graphID, TRUE);
		else if (number == 4)
			DrawGraph(x, y - sizeY, graphID, TRUE);
		else if (number == 5)
			DrawGraph(x - sizeX, y - sizeY, graphID, TRUE);
		else if (number == 6)
			DrawGraph(x + sizeX, y - sizeY, graphID, TRUE);
		else if (number == 7)
			DrawGraph(x, y + sizeY, graphID, TRUE);
		else if (number == 8)
			DrawGraph(x - sizeX, y + sizeY, graphID, TRUE);
		else if (number == 9)
			DrawGraph(x + sizeX, y + sizeY, graphID, TRUE);
	}
	int GetsizeX() {
		return sizeX;
	}
	int GetsizeY() {
		return sizeY;
	}
};