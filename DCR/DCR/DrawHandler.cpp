#include "DrawHandler.h"

DrawHandler::DrawHandler()
{
}

DrawHandler::DrawHandler(const DrawHandler& other)
{
}

DrawHandler::~DrawHandler()
{
}

DrawHandler& DrawHandler::operator=(const DrawHandler& other)
{
	return *this;
}

void DrawHandler::setMode(const QString& modeStr)
{
	auto iter = drawModeMap.find(modeStr);

	if (iter != drawModeMap.end())
		m_mode = iter->second;
}

void DrawHandler::Draw()
{
	switch (m_mode)
	{
		case DrawMode::Line:
			DrawLine();
		case DrawMode::Rectangle:
			DrawRectangle();
		case DrawMode::Circle:
			DrawCircle();
		case DrawMode::Ellipse:
			DrawEllipse();
		default:
			return;
	}
}

void DrawHandler::DrawLine()
{

}

void DrawHandler::DrawRectangle()
{

}

void DrawHandler::DrawCircle()
{

}

void DrawHandler::DrawEllipse()
{

}