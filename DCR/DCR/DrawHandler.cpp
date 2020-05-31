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
		case DrawType::Polygon:
			drawPolygon(painter);
			break;
		case DrawType::Rectangle:
			drawRectangle(painter, rect);
			break;
		case DrawType::Circle:
			drawCircle(painter, rect);
			break;
		case DrawType::Ellipse:
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

void DrawHandler::drawBackground(QPainter& painter, const QSize& pageSize)
{
	QPen pen;
	pen.setColor(QColor(0, 0, 0));
    pen.setWidth(3);
    painter.setPen(pen);
    QRect r(QPoint(0, 0), pageSize);
    QBrush brush(QColor(255, 255, 255)); //fill white background
    painter.fillRect(r, brush);
    painter.drawRect(r);
}

void DrawHandler::drawSelected(QPainter& painter, PanelObject* selected)
{
	if (selected)
	{
		QPen pen;
		pen.setColor(QColor(255, 255, 0));
		pen.setWidth(10);
		painter.setPen(pen);
		painter.fillRect(selected->getRect(), TransparentBrush);
		painter.drawRect(selected->getRect());
	}
}