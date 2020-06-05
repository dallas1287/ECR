#include "ComicPanelHandler.h"
#include "ComicPage.h"
#include "DigitalComicReader.h"

ComicPanelHandler::ComicPanelHandler(ComicPage* owner): m_owner(owner)
{
}

ComicPanelHandler::ComicPanelHandler(const ComicPanelHandler& other): ComicPanelHandler(other.m_owner)
{
    //don't love this as a copy, but compiler will complain about unique_ptrs and deleted functions and what not
    for (const auto& p : other.m_panelObjects)
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(p->getDrawMode(), p->getRect())));
}

ComicPanelHandler::~ComicPanelHandler()
{
}

ComicPanelHandler& ComicPanelHandler::operator=(const ComicPanelHandler& other)
{
	m_owner = other.m_owner;
    m_panelObjects.clear();
    for (const auto& p : other.m_panelObjects)
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(p->getDrawMode(),  p->getRect())));

    return *this;
}

void ComicPanelHandler::setSelected(PanelObject* selected)
{
    for (std::unique_ptr<PanelObject>& p : m_panelObjects)
    {
        if (!p.get())
            continue;
        p.get() != selected ? p->setSelected(false) : p->setSelected();
    }
    m_selected = selected;
}

void ComicPanelHandler::createGrid(int numH, int width, int numV, int height, int hPadding, int vPadding, int hBorder, int vBorder)
{
    QPoint topleft(hBorder, vBorder);
    QSize pSize(width, height);

    for (int i = 0; i < numV; ++i)
    {
        for (int j = 0; j < numH; ++j)
        {
            createPanelObject(DrawType::Rectangle, QRect(topleft, pSize));
            topleft += QPoint(hPadding + width, 0);
        }
        topleft = QPoint(hBorder, topleft.y() + height + vPadding);
    }
}

PanelObject* ComicPanelHandler::getEnclosingShape(const QPoint& cursor)
{
    auto iter = std::find_if(panelObjects().begin(), panelObjects().end(), [&](std::unique_ptr<PanelObject>& p) { return p->getRect().contains(cursor); });
    if (iter != panelObjects().end())
        return (*iter).get();
    return nullptr;
}

QRect ComicPanelHandler::getDrawnRect(const QPoint& start, const QPoint& cur) const
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

    if (bottom > m_owner->pageSize().height())
        bottom = m_owner->pageSize().height();

    if (right > m_owner->pageSize().width())
        right = m_owner->pageSize().width();

    return QRect(QPoint(left, top), QSize(right - left, bottom - top));
}

QRect ComicPanelHandler::getDrawnRect(const QPoint& cur) const
{
    return getDrawnRect(m_newShape.startPt, cur);
}

QRect ComicPanelHandler::getDrawnRect() const
{
    return getDrawnRect(m_newShape.startPt, m_newShape.curPt);
}

void ComicPanelHandler::movePanel(const QPoint& curPos)
{
    int vmovement = curPos.y() - m_editShape.startPt.y();
    int hmovement = curPos.x() - m_editShape.startPt.x();
    m_editShape.startPt = curPos;

    QPoint delta(hmovement, vmovement);
    QPoint moved_tl = m_editShape.pObj->getRect().topLeft();

    m_editShape.pObj->getRectToEdit().moveTopLeft(moved_tl + delta);
    if (m_editShape.pObj->getGraphicPanel())
        m_editShape.pObj->getGraphicPanel()->setGeometry(m_editShape.pObj->getRect());
}

QRect ComicPanelHandler::getCurrentShape()
{
    if (m_movingPanel)
        return m_editShape.pObj->getRect();
    if (m_drawing)
        return getDrawnRect();

    return QRect();
}

DrawType ComicPanelHandler::getCurrentShapeType()
{
    if (m_movingPanel)
        return m_editShape.type;
    if (m_drawing)
        return getDrawType();

    return DrawType::OutOfRange;
}

void ComicPanelHandler::mouseMoved(const QPoint& pos)
{
    if (!m_drawing)
        return;

    if (m_movingPanel)
        movePanel(pos);
    else if (m_newShape.shapeStarted)
        m_newShape.curPt = pos;
}

void ComicPanelHandler::mouseClicked(const QPoint& pos)
{
    if (!m_drawing)
        return;

    //handle editing
    PanelObject* movingShape = getEnclosingShape(pos);
    if (movingShape)
    {
        m_movingPanel = true;
        m_editShape.startPt = pos;
        m_editShape.pObj = movingShape;
        return;
    }

    //handle drawing
    m_editShape.reset(); //invalidates the editShape object, if we've made it this far then we're drawing a new shape
    m_newShape.shapeStarted = true;

    if (getDrawType() == DrawType::Polygon)
    {
        m_newShape.addPoint(pos);
    }
    else
    {
        m_newShape.startPt = pos;
        m_newShape.curPt = QPoint(m_newShape.startPt.x() + 10, m_newShape.startPt.y() + 10); //assure a base shape of 10 pixels
    }

    setSelected(nullptr);
}

void ComicPanelHandler::mouseReleased(const QPoint& pos)
{
    if (!m_drawing)
        return;

    if (m_movingPanel)
    {
        m_movingPanel = false;
        m_editShape.reset();
        return;
    }

    m_newShape.shapeStarted = false;
    createPanelObject(getDrawType(), getDrawnRect(pos));
}

/**************************************************************************************************
**************************Slots********************************************************************
**************************************************************************************************/

void ComicPanelHandler::toggleDrawing()
{
    m_drawing ^= true;
}

void ComicPanelHandler::setDrawing(bool state)
{
    m_drawing = state;
}

void ComicPanelHandler::createPanelObject(DrawType mode, const QRect& rect)
{
    if (ComicPanelHandler::isRectMinSize(rect) && 
        std::find_if(m_panelObjects.begin(), m_panelObjects.end(),
            [&](std::unique_ptr<PanelObject>& pObj) { return pObj->getRect() == rect && pObj->getDrawMode() == mode; }) == m_panelObjects.end())
    {
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(mode, rect)));
    }
}

void ComicPanelHandler::createGraphicPanel(PanelObject* panelObj)
{
    createGPWithPath(panelObj, QString());
}

void ComicPanelHandler::createGPWithPath(PanelObject* panelObj, const QString& path)
{
    if (panelObj && !panelObj->getGraphicPanel())
    {
        GraphicPanel* gp = panelObj->createGraphicPanel(path);
        if(gp)
            emit addGraphicPanelToLayout(gp);
    }
}

void ComicPanelHandler::clearPage()
{
    m_panelObjects.clear();
    m_newShape.reset();
    m_editShape.reset();
}

void ComicPanelHandler::clearEditingShapes()
{
    m_newShape.reset();
    m_editShape.reset();
    m_movingPanel = false;
}

/**************************************************************************************************
**************************Static Functions*********************************************************
**************************************************************************************************/

DrawType ComicPanelHandler::getDrawType()
{
    if (!getMainWindow())
        return DrawType::OutOfRange; 
    return ((DigitalComicReader*)getMainWindow())->getDrawType();
}