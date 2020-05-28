#include "PanelObject.h"

PanelObject::PanelObject(QWidget* parent, const QRect& rect): m_parent(parent), m_rect(rect)
{
}

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
	m_gPanel.reset(new GraphicPanel(m_parent));
	m_gPanel->setGeometry(m_rect);
}