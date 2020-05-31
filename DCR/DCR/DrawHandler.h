#pragma once
#include <QPainter>
#include "ToolbarMaps.h"
#include "PanelObject.h"

const QBrush TransparentBrush = QBrush(QColor(0, 0, 0, 0));

class DrawHandler
{
public:
	DrawHandler();
	DrawHandler(const DrawHandler& other);

	~DrawHandler();

	DrawHandler& operator=(const DrawHandler& other);

	DrawType getMode() { return m_mode; }
	void setMode(DrawType mode) { m_mode = mode; }

	void draw(QPainter& painter, const QRect& rect);
	void drawPolygon(QPainter& painter);
	void drawRectangle(QPainter& painter, const QRect& rect);
	void drawCircle(QPainter& painter, const QRect& rect);
	void drawEllipse(QPainter& painter, const QRect& rect);
	void drawBackground(QPainter& painter, const QSize& pageSize);
	void drawSelected(QPainter& painter, PanelObject* selected);

	void addPoint(const QPoint& point) { m_points.push_back(point);}

private:

	QPen m_pen;
	DrawType m_mode = DrawType::Rectangle;
	std::vector<QPoint> m_points;
};
