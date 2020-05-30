#include "ComicPageWidget.h"
#include "common.h"


ComicPageWidget::ComicPageWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    //setting a layout only works if there isn't already a layout so delete if necessary
    auto oldlayout = layout();
    if (oldlayout)
        delete oldlayout;
    setLayout(&m_layout);

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

void ComicPageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPen pen;
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QBrush brush(QColor(0, 0, 0, 0)); //fill transparent

    //draw selected
    auto selected = m_cpHandler.getSelected();
    if (selected)
    {
        pen.setColor(QColor(255, 255, 0));
        pen.setWidth(10);
        painter.setPen(pen);
        painter.fillRect(mapRectFromGlobal(this, selected->getRect()), brush);
        painter.drawRect(mapRectFromGlobal(this, selected->getRect()));
    }

    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(5);
    painter.setPen(pen);

    QRect rect(mapRectFromGlobal(this, getDrawnRect()));
    //QRect r(QPoint(0, 0), size());
    painter.fillRect(rect, brush);
    painter.drawRect(rect);
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

    m_curPos = event->globalPos();
    repaint();
}

void ComicPageWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;

    m_shapeStarted = true;
    m_rectStart = event->globalPos();
    m_curPos = QPoint(m_rectStart.x() + 10, m_rectStart.y() + 10); //assure a base shape of 10 pixels 
    m_cpHandler.setSelected(nullptr);
    repaint();
}

void ComicPageWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;
    m_shapeStarted = false;
    m_rectEnd = event->globalPos();
    emit signalPanelObjectCreation(getDrawnRect(m_rectEnd));
}

void ComicPageWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = m_cpHandler.getEnclosingShape(event->globalPos());
    if (!panelObj)
        return;

    m_cpHandler.setSelected(panelObj);
    emit signalGraphicPanelCreation(panelObj);
    repaint();
}

/**************************************************************************************************
**************************Keyboard Events**********************************************************
**************************************************************************************************/

void ComicPageWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
        m_drawing ^= true; //toggle drawMode
}