#pragma once
#include <QPainter>
#include "ToolbarMaps.h"
#include "ComicPage.h"

const QBrush TransparentBrush = QBrush(QColor(0, 0, 0, 0));

class PenSettings
{
public:
	PenSettings() {}
	~PenSettings(){}

	void setupPen(QPen& pen)
	{
		pen.setStyle(m_style);
		pen.setColor(m_color);
		pen.setWidth(m_width);
	}

	void setStyle(Qt::PenStyle style) { m_style = style; }
	Qt::PenStyle style() { return m_style; }
	void setcolor(const QColor& color) { m_color = color; }
	QColor color() { return m_color; }
	void setWidth(int width) { m_width = width; }
	int width() { return m_width; }

private:
	Qt::PenStyle m_style = Qt::PenStyle::SolidLine;
	QColor m_color = QColor(Qt::black);
	int m_width = 5;
};

class DrawHandler
{
public:
	DrawHandler();
	DrawHandler(QWidget* owner);
	DrawHandler(const DrawHandler& other);

	~DrawHandler();

	DrawHandler& operator=(const DrawHandler& other);

	void drawPage(ComicPage& page);

	void addPoint(const QPoint& point) { m_points.push_back(point); }

private:
	void draw(DrawType mode, const QRect& rect = QRect());
	void draw(PanelObjectPool& pObjs);
	void drawPolygon();
	void drawRectangle(const QRect& rect);
	void drawCircle(const QRect& rect);
	void drawEllipse(const QRect& rect);
	void drawBackground(const QSize& pageSize);
	void drawSelected(PanelObject* selected);

	QWidget* m_owner = nullptr;
	QPainter m_painter;
	QPen m_pen;
	QBrush m_bgBrush = QBrush(Qt::white);
	PenSettings m_penSettings;
	PenSettings m_bgSettings;
	std::vector<QPoint> m_points;
};
