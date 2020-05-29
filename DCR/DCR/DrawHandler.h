#pragma once
#include <QPainter>

enum class DrawMode
{
	Line,
	Rectangle,
	Circle,
	Ellipse
};

const std::map<QString, DrawMode> drawModeMap =
{
	{"DrawLines", DrawMode::Line },
	{"DrawRect", DrawMode::Rectangle},
	{"DrawCircle", DrawMode::Circle},
	{"DrawEllipse", DrawMode::Ellipse}
};

const QBrush TransparentBrush = QBrush(QColor(0, 0, 0, 0));

class DrawHandler
{
public:
	DrawHandler();
	DrawHandler(const DrawHandler& other);

	~DrawHandler();

	DrawHandler& operator=(const DrawHandler& other);

	QPainter& Painter() { return m_painter; }

	DrawMode getMode() { return m_mode; }
	void setMode(DrawMode mode) { m_mode = mode; }
	void setMode(const QString& modeStr);
	void Draw();
	
private:
	void DrawLine();
	void DrawRectangle();
	void DrawCircle();
	void DrawEllipse();

	QPainter m_painter;
	QPen m_pen;
	DrawMode m_mode = DrawMode::Rectangle;
};

