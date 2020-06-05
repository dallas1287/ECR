#include "PanelObject.h"
#include "DigitalComicReader.h"
#include "common.h"

PanelObject::PanelObject(DrawType mode, const QRect& rect): m_drawMode(mode), m_rect(rect)
{
}

//TODO: fix the copy constructor and copy assignment for the unique_ptr
PanelObject::PanelObject(const PanelObject& other): PanelObject(other.m_drawMode, other.m_rect)
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

GraphicPanel* PanelObject::createGraphicPanel(const QString& path)
{
	m_gPanel.reset(new GraphicPanel(this, path));
	m_gPanel->setGeometry(m_rect);
	return m_gPanel.get();
}