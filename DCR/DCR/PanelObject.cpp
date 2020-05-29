#include "PanelObject.h"
#include "DigitalComicReader.h"
#include "common.h"

PanelObject::PanelObject(QWidget* parent, const QRect& rect): m_parent(parent), m_rect(rect)
{
}

//TODO: fix the copy constructor and copy assignment for the unique_ptr
PanelObject::PanelObject(const PanelObject& other): PanelObject(other.m_parent, other.m_rect)
{
	m_gPanel.reset(other.m_gPanel.get());
}

PanelObject::~PanelObject()
{
}

PanelObject& PanelObject::operator=(const PanelObject& other)
{
	return *this;
}

void PanelObject::createGraphicPanel()
{
	m_gPanel.reset(new GraphicPanel(this, m_parent));
	m_gPanel->setGeometry(mapRectFromGlobal(m_gPanel.get(), m_rect));
	((DigitalComicReader*)m_parent)->addPanelWidget(this);
}