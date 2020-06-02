#include "PanelObject.h"
#include "DigitalComicReader.h"
#include "common.h"

PanelObject::PanelObject(QWidget* owner, DrawType mode, const QRect& rect): m_owner(owner), m_drawMode(mode), m_rect(rect)
{
}

//TODO: fix the copy constructor and copy assignment for the unique_ptr
PanelObject::PanelObject(const PanelObject& other): PanelObject(other.m_owner, other.m_drawMode, other.m_rect)
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

void PanelObject::createGraphicPanel(const QString& path)
{
	m_gPanel.reset(new GraphicPanel(m_owner, this, path));
	m_gPanel->setGeometry(m_rect);
	((PageDisplayWidget*)m_owner)->addPanelWidget(this);
}