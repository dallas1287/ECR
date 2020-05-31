#include "ComicPageWidget.h"
#include "common.h"


ComicPageWidget::ComicPageWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    //setting a layout only works if there isn't already a layout so delete if necessary
    auto oldlayout = layout();
    if (oldlayout)
        delete oldlayout;
    setLayout(&m_layout);

    setFixedSize(m_size);

    setFocusPolicy(Qt::StrongFocus);

    connect(this, &ComicPageWidget::signalPanelObjectCreation, &m_cpHandler, &ComicPanelHandler::createPanelObject);
    connect(this, &ComicPageWidget::signalGraphicPanelCreation, &m_cpHandler, &ComicPanelHandler::createGraphicPanel);
}

ComicPageWidget::ComicPageWidget(const ComicPageWidget& other)
{
}

ComicPageWidget::~ComicPageWidget()
{
}

void ComicPageWidget::resizeEvent(QResizeEvent* event)
{
    auto a = event->oldSize();
    auto b = event->size();
    bool stophere = true;
}

void ComicPageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPen pen;
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);

    ////draw selected
    //auto selected = m_cpHandler.getSelected();
    //if (selected)
    //{
    //    pen.setColor(QColor(255, 255, 0));
    //    pen.setWidth(10);
    //    painter.setPen(pen);
    //    painter.fillRect(selected->getRect(), brush);
    //    painter.drawRect(selected->getRect());
    //}

    pen.setColor(QColor(0, 0, 0));
    pen.setWidth(3);
    painter.setPen(pen);
    QRect r(QPoint(0, 0), size());
    QBrush brush(QColor(255, 255, 255)); //fill white background
    painter.fillRect(r, brush);
    painter.drawRect(r);

    m_drawHandler.draw(painter, getDrawnRect());
}

QRect ComicPageWidget::getDrawnRect(const QPoint& start, const QPoint& cur) const
{
    int top, bottom, left, right;
    if (start.x() < cur.x())
    {
        left = start.x();
        right = cur.x();
    }
    else
    {
        left = cur.x();
        right = start.x();
    }

    if (start.y() < cur.y())
    {
        top = start.y();
        bottom = cur.y();
    }
    else
    {
        top = cur.y();
        bottom = start.y();
    }

    if (top < 0)
        top = 0;

    if (left < 0)
        left = 0;

    if (bottom > height())
        bottom = height();

    if (right > width())
        right = width();

    return QRect(QPoint(left, top), QSize(right - left, bottom - top));
}

QRect ComicPageWidget::getDrawnRect(const QPoint& cur) const
{
    return getDrawnRect(m_rectStart, cur);
}

QRect ComicPageWidget::getDrawnRect() const
{
    return getDrawnRect(m_rectStart, m_curPos);
}

void ComicPageWidget::addPanelWidget(PanelObject* panelObj)
{
    layout()->addWidget(panelObj->getGraphicPanel());
}

/**************************************************************************************************
**************************Mouse Events*************************************************************
**************************************************************************************************/

void ComicPageWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_drawing || !m_shapeStarted)
        return;

    m_curPos = event->pos();
    repaint();
}

void ComicPageWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;

    m_shapeStarted = true;

    if (getDrawMode() == LeftToolBar::Polygon)
    {
        m_drawHandler.addPoint(event->pos());
    }
    else
    {
        m_rectStart = event->pos();
        m_curPos = QPoint(m_rectStart.x() + 10, m_rectStart.y() + 10); //assure a base shape of 10 pixels
    }

    m_cpHandler.setSelected(nullptr);
    repaint();
}

void ComicPageWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;
    m_shapeStarted = false;
    m_rectEnd = event->pos();
    emit signalPanelObjectCreation(getDrawnRect(m_rectEnd));
}

void ComicPageWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = m_cpHandler.getEnclosingShape(event->pos());
    if (!panelObj)
        return;

    m_cpHandler.setSelected(panelObj);
    emit signalGraphicPanelCreation(panelObj);
}