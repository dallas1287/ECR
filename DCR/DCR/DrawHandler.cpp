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

void DrawHandler::draw(QPainter& painter, const QRect& rect)
{
	switch (m_mode)
	{
		case LeftToolBar::Polygon:
			drawPolygon(painter);
			break;
		case LeftToolBar::Rectangle:
			drawRectangle(painter, rect);
			break;
		case LeftToolBar::Circle:
			drawCircle(painter, rect);
			break;
		case LeftToolBar::Ellipse:
			drawEllipse(painter, rect);
			break;
		default:
			return;
	}
}

void DrawHandler::drawPolygon(QPainter& painter)
{
	if (m_points.empty())
		return;

	QPen pen;
	pen.setStyle(Qt::PenStyle::SolidLine);
	painter.setRenderHint(QPainter::Antialiasing, true);
	pen.setColor(QColor(255, 0, 0));
	pen.setWidth(5);
	painter.setPen(pen);

	painter.drawPolyline(m_points.data(), m_points.size());
}

void DrawHandler::drawRectangle(QPainter& painter, const QRect& rect)
{
	QPen pen;
	pen.setStyle(Qt::PenStyle::SolidLine);
	painter.setRenderHint(QPainter::Antialiasing, true);
	pen.setColor(QColor(255, 0, 0));
	pen.setWidth(5);
	painter.setPen(pen);

	QBrush brush(QColor(0, 0, 0, 0)); //fill transparent

	painter.fillRect(rect, brush);
	painter.drawRect(rect);
}

void DrawHandler::drawCircle(QPainter& painter, const QRect& rect)
{
	int max = std::max(rect.width(), rect.height());
	QRect circle(rect.topLeft(), QSize(max, max));

	QPen pen;
	pen.setStyle(Qt::PenStyle::SolidLine);
	painter.setRenderHint(QPainter::Antialiasing, true);
	pen.setColor(QColor(255, 0, 0));
	pen.setWidth(5);
	painter.setPen(pen);

	QBrush brush(QColor(0, 0, 0, 0)); //fill transparent

	painter.fillRect(circle, brush);
	painter.drawEllipse(circle);
}

void DrawHandler::drawEllipse(QPainter& painter, const QRect& rect)
{
	QPen pen;
	pen.setStyle(Qt::PenStyle::SolidLine);
	painter.setRenderHint(QPainter::Antialiasing, true);
	pen.setColor(QColor(255, 0, 0));
	pen.setWidth(5);
	painter.setPen(pen);

	QBrush brush(QColor(0, 0, 0, 0)); //fill transparent

	painter.fillRect(rect, brush);
	painter.drawEllipse(rect);
}