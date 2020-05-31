#include "ComicPanelHandler.h"

ComicPanelHandler::ComicPanelHandler(QWidget* owner): m_owner(owner)
{
}

ComicPanelHandler::ComicPanelHandler(const ComicPanelHandler& other): ComicPanelHandler(other.m_owner)
{
    //don't love this as a copy, but compiler will complain about unique_ptrs and deleted functions and what not
    for (const auto& p : other.m_panelObjects)
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(m_owner, p->getDrawMode(), p->getRect())));
}

ComicPanelHandler::~ComicPanelHandler()
{
}

ComicPanelHandler& ComicPanelHandler::operator=(const ComicPanelHandler& other)
{
	m_owner = other.m_owner;
    m_panelObjects.clear();
    for (const auto& p : other.m_panelObjects)
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(m_owner, p->getDrawMode(),  p->getRect())));

    return *this;
}

void ComicPanelHandler::createPanelObject(DrawType mode, const QRect& rect)
{
    if(std::find_if(m_panelObjects.begin(), m_panelObjects.end(), 
        [&](std::unique_ptr<PanelObject>& pObj) { return pObj->getRect() == rect && pObj->getDrawMode() == mode; }) == m_panelObjects.end())
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(m_owner, mode, rect)));
}

void ComicPanelHandler::createGraphicPanel(PanelObject* panelObj)
{
    if (panelObj && !panelObj->getGraphicPanel())
        panelObj->createGraphicPanel();
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

void ComicPanelHandler::createGrid(int numH, int width, int numV, int height, int padding)
{
    QPoint topleft(padding, padding);
    QSize pSize(width, height);

    for (int i = 0; i < numV; ++i)
    {
        for (int j = 0; j < numH; ++j)
        {
            createPanelObject(DrawType::Rectangle, QRect(topleft, pSize));
            topleft += QPoint(padding + width, 0);
        }
        topleft = QPoint(padding, topleft.y() + height + padding);
    }

}