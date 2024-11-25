#pragma once
#include <QPainter>

class DrawCommands
{
public:
	void Draw(Qpainter* p);
	void Draw()
	bool Synchronization3DWithModel();//сделать возврат кода ошибки и напихать проверок
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


