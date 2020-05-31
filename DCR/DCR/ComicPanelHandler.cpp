#include "ComicPanelHandler.h"

ComicPanelHandler::ComicPanelHandler(QWidget* owner): m_owner(owner)
{
}

ComicPanelHandler::ComicPanelHandler(const ComicPanelHandler& other): ComicPanelHandler(other.m_owner)
{
    //don't love this as a copy, but compiler will complain about unique_ptrs and deleted functions and what not
    for (const auto& p : other.m_panelObjects)
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(m_owner, p->getType(), p->getRect())));
}

ComicPanelHandler::~ComicPanelHandler()
{
}

ComicPanelHandler& ComicPanelHandler::operator=(const ComicPanelHandler& other)
{
	m_owner = other.m_owner;
    m_panelObjects.clear();
    for (const auto& p : other.m_panelObjects)
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(m_owner, p->getType(),  p->getRect())));

    return *this;
}

void ComicPanelHandler::createPanelObject(DrawType type, const QRect& rect)
{
    if(std::find_if(m_panelObjects.begin(), m_panelObjects.end(), [&](std::unique_ptr<PanelObject>& pObj) { return pObj->getRect() == rect; }) == m_panelObjects.end())
        m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(m_owner, type, rect)));
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

PanelObject* ComicPanelHandler::getEnclosingShape(const QPoint& cursor)
{
    auto iter = std::find_if(m_panelObjects.begin(), m_panelObjects.end(), [&](std::unique_ptr<PanelObject>& p) { return p->getRect().contains(cursor); });
    if (iter != m_panelObjects.end())
        return (*iter).get();
    return nullptr;
}