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
        m_drawHandler.setDrawMode(selection); 
}

//this is wild, but to convert the toolbar enum to the drawtype enum, it converts it to the underlying type of int,
//then to the DrawType enum class
void ComicPageWidget::setDrawMode(LeftToolBar selection)
{
    if (selection >= LeftToolBar::Polygon && selection <= LeftToolBar::Ellipse)
        m_drawHandler.setDrawMode((DrawType)(unsigned int)selection);
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
        m_drawHandler.draw(painter, m_newShape.editingRect);
    else if(m_drawing)
        m_drawHandler.draw(painter, getDrawnRect());

    m_drawHandler.draw(painter, m_cpHandler.panelObjects());
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
    return getDrawnRect(m_newShape.startPt, cur);
}

QRect ComicPageWidget::getDrawnRect() const
{
    return getDrawnRect(m_newShape.startPt, m_newShape.curPt);
}

void ComicPageWidget::addPanelWidget(PanelObject* panelObj)
{
    layout()->addWidget(panelObj->getGraphicPanel());
}

void ComicPageWidget::movePanel(const QPoint& curPos)
{
    int vmovement = curPos.y() - m_newShape.movingStartPt.y();
    int hmovement = curPos.x() - m_newShape.movingStartPt.x();
    m_newShape.movingStartPt = curPos;

    QPoint delta(hmovement, vmovement);
    QPoint moved_tl = m_newShape.editingRect.topLeft();
    m_newShape.editingRect.moveTopLeft(moved_tl + delta);
}

PanelObject* ComicPageWidget::getEnclosingShape(const QPoint& cursor)
{
    auto iter = std::find_if(m_cpHandler.panelObjects().begin(), m_cpHandler.panelObjects().end(), [&](std::unique_ptr<PanelObject>& p) { return p->getRect().contains(cursor); });
    if (iter != m_cpHandler.panelObjects().end())
        return (*iter).get();
    return nullptr;
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
        m_newShape.curPt = event->pos();
    
    repaint();
}

void ComicPageWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;

    //handle moving
    if (m_newShape.editingRect.isValid() && !m_newShape.editingRect.isNull() && m_newShape.editingRect.contains(event->pos()))
    {
        m_movingPanel = true;
        m_newShape.movingStartPt = event->pos();
        return;
    }

    if (getEnclosingShape(event->pos()))
    {

    }

    //handle drawing
    m_shapeStarted = true;
    m_newShape.editingRect.setRect(0, 0, 0, 0); //invalidated

    if (getDrawMode() == DrawType::Polygon)
    {
        m_drawHandler.addPoint(event->pos());
    }
    else
    {
        m_newShape.startPt = event->pos();
        m_newShape.curPt = QPoint(m_newShape.startPt.x() + 10, m_newShape.startPt.y() + 10); //assure a base shape of 10 pixels
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
    m_newShape.endPt = event->pos();
    m_newShape.editingRect = getDrawnRect(m_newShape.endPt);
    emit signalPanelObjectCreation(getDrawMode(), getDrawnRect(m_newShape.endPt));
}

void ComicPageWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = getEnclosingShape(event->pos());
    if (!panelObj)
        return;

    m_cpHandler.setSelected(panelObj);
    emit signalGraphicPanelCreation(panelObj);
}