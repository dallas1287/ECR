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

	DrawType getDrawMode() { return m_mode; }
	void setDrawMode(DrawType mode) { m_mode = mode; }

	void draw(QPainter& painter, const QRect& rect);
	void draw(QPainter& painter, DrawType mode, const QRect& rect);
	void draw(QPainter& painter, PanelObjectPool& pObjs);
	void drawPolygon(QPainter& painter);
	void drawRectangle(QPainter& painter, const QRect& rect);
	void drawCircle(QPainter& painter, const QRect& rect);
	void drawEllipse(QPainter& painter, const QRect& rect);
	void drawBackground(QPainter& painter, const QSize& pageSize);
	void drawSelected(QPainter& painter, PanelObject* selected);

	void addPoint(const QPoint& point) { m_points.push_back(point); }

private:
	DrawType m_mode = DrawType::Rectangle;
	QPen m_pen;
	std::vector<QPoint> m_points;
};
