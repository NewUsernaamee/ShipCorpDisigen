#pragma once
#include <QPainter>

class DrawCommands
{
public:
	void Draw(Qpainter* p);
	void Draw()
	bool Synchronization3DWithModel();//������� ������� ���� ������ � �������� ��������
private:
	DrawCommands();
	~DrawCommands();
};

DrawCommands::DrawCommands()
{
}

DrawCommands::~DrawCommands()
{
}


