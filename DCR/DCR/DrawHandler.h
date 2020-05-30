#pragma once
#include <QPainter>
#include "ToolbarMaps.h"

const QBrush TransparentBrush = QBrush(QColor(0, 0, 0, 0));

class DrawHandler
{
public:
	DrawHandler();
	DrawHandler(const DrawHandler& other);

	~DrawHandler();

	DrawHandler& operator=(const DrawHandler& other);

	LeftToolBar getMode() { return m_mode; }
	void setMode(LeftToolBar mode) { m_mode = mode; }

	void draw(QPainter& painter, const QRect& rect);
	void drawPolygon(QPainter& painter);
	void drawRectangle(QPainter& painter, const QRect& rect);
	void drawCircle(QPainter& painter, const QRect& rect);
	void drawEllipse(QPainter& painter, const QRect& rect);

	void addPoint(const QPoint& point) { m_points.push_back(point);}

private:

	QPen m_pen;
	LeftToolBar m_mode = LeftToolBar::Rectangle;
	std::vector<QPoint> m_points;
};
