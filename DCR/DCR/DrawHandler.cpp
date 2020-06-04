#include "DrawHandler.h"

DrawHandler::DrawHandler()
{
}

DrawHandler::DrawHandler(QPaintDevice* device) : m_device(device)
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

void DrawHandler::draw(DrawType mode, const QRect& rect)
{
	if (!m_painter.begin(m_device))
	{
		qDebug() << "Painter failed to begin";
		return;
	}

	m_painter.setRenderHint(QPainter::Antialiasing, true);
	m_penSettings.setupPen(m_pen);
	m_painter.setPen(m_pen);
	m_painter.setBrush(TransparentBrush);

	switch (mode)
	{
		case DrawType::Polygon:
			drawPolygon();
			break;
		case DrawType::Rectangle:
			drawRectangle(rect);
			break;
		case DrawType::Circle:
			drawCircle(rect);
			break;
		case DrawType::Ellipse:
			drawEllipse(rect);
			break;
		default:
			break;
	}

	m_painter.end();
}

void DrawHandler::draw(const QRect& rect)
{
	draw(m_mode, rect);
}

void DrawHandler::draw(PanelObjectPool& pObjs)
{
	for (auto& p : pObjs)
		draw(p->getDrawMode(), p->getRect());
}

void DrawHandler::drawPolygon()
{
	if (m_points.empty())
		return;

	m_painter.drawPolyline(m_points.data(), m_points.size());
}

void DrawHandler::drawRectangle(const QRect& rect)
{
	m_painter.fillRect(rect, m_painter.brush());
	m_painter.drawRect(rect);
}

void DrawHandler::drawCircle(const QRect& rect)
{
	int max = std::max(rect.width(), rect.height());
	QRect circle(rect.topLeft(), QSize(max, max));

	m_painter.fillRect(circle, m_painter.brush());
	m_painter.drawEllipse(circle);
}

void DrawHandler::drawEllipse(const QRect& rect)
{
	m_painter.fillRect(rect, m_painter.brush());
	m_painter.drawEllipse(rect);
}

void DrawHandler::drawBackground(const QSize& pageSize)
{
	m_painter.begin(m_device);
	QPen pen;
	pen.setColor(QColor(0, 0, 0));
	pen.setWidth(3);
	m_painter.setPen(pen);
    QRect r(QPoint(0, 0), pageSize);
    QBrush brush(QColor(255, 255, 255)); //fill white background
    m_painter.fillRect(r, brush);
    m_painter.drawRect(r);
	m_painter.end();
}

void DrawHandler::drawSelected(PanelObject* selected)
{
	if (selected)
	{
		QPen pen;
		pen.setColor(QColor(255, 255, 0));
		pen.setWidth(10);
		m_painter.setPen(pen);
		m_painter.fillRect(selected->getRect(), TransparentBrush);
		m_painter.drawRect(selected->getRect());
	}
}