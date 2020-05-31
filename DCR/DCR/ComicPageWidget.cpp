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

void ComicPageWidget::setDrawMode(DrawType selection)
{
    if (selection >= DrawType::Polygon && selection <= DrawType::Ellipse) 
        m_drawHandler.setMode(selection); 
}

//this is wild, but to convert the toolbar enum to the drawtype enum, it converts it to the underlying type of int,
//then to the DrawType enum class
void ComicPageWidget::setDrawMode(LeftToolBar selection)
{
    if (selection >= LeftToolBar::Polygon && selection <= LeftToolBar::Ellipse)
        m_drawHandler.setMode((DrawType)(unsigned int)selection);
}

void ComicPageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPen pen;
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //draw background
    m_drawHandler.drawBackground(painter, m_size);

    //draw selected
    m_drawHandler.drawSelected(painter, m_cpHandler.getSelected());

    //draw current
    if (m_movingPanel)
        m_drawHandler.draw(painter, m_editingRect);
    else if(m_drawing)
        m_drawHandler.draw(painter, getDrawnRect());

    //m_drawHandler.draw(painter, m_cpHandler.panelObjects());
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

void ComicPageWidget::movePanel(const QPoint& curPos)
{
    int vmovement = curPos.y() - m_movingStartPos.y();
    int hmovement = curPos.x() - m_movingStartPos.x();
    m_movingStartPos = curPos;

    QPoint delta(hmovement, vmovement);
    QPoint moved_tl = m_editingRect.topLeft();
    m_editingRect.moveTopLeft(moved_tl + delta);
}

/**************************************************************************************************
**************************Mouse Events*************************************************************
**************************************************************************************************/

void ComicPageWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;

    if (m_movingPanel)
        movePanel(event->pos());
    else if(m_shapeStarted)
        m_curPos = event->pos();
    
    repaint();
}

void ComicPageWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;

    //handle moving
    if (m_editingRect.isValid() && !m_editingRect.isNull() && m_editingRect.contains(event->pos()))
    {
        m_movingPanel = true;
        m_movingStartPos = event->pos();
        return;
    }

    //handle drawing
    m_shapeStarted = true;
    m_editingRect.setRect(0, 0, 0, 0); //invalidated

    if (getDrawMode() == DrawType::Polygon)
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

    if (m_movingPanel)
    {
        m_movingPanel = false;
        return;
    }
    
    m_shapeStarted = false;
    m_rectEnd = event->pos();
    m_editingRect = getDrawnRect(m_rectEnd);
    emit signalPanelObjectCreation(getDrawMode(), getDrawnRect(m_rectEnd));
}

void ComicPageWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = m_cpHandler.getEnclosingShape(event->pos());
    if (!panelObj)
        return;

    m_cpHandler.setSelected(panelObj);
    emit signalGraphicPanelCreation(panelObj);
}