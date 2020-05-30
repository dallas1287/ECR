#pragma once
#include <QRect>
#include "GraphicPanel.h"

class PanelObject
{
public:
	PanelObject(QWidget* owner, const QRect& rect);
	PanelObject(const PanelObject& other);

	~PanelObject();

	PanelObject& operator=(const PanelObject& other);

	QWidget* getOwner() { return m_owner; }
	const QRect& getRect() { return m_rect; }
	GraphicPanel* getGraphicPanel() { return m_gPanel.get(); }

	void setIndex(int index) { m_index = index; }
	int getIndex() { return m_index; }
	void createGraphicPanel();

	bool isSelected() const { return m_isSelected; }
	void setSelected(bool state = true) { m_isSelected = state; }

private:
	QWidget* m_owner;
	QRect m_rect;
	std::unique_ptr<GraphicPanel> m_gPanel;
	int m_index = -1;
	bool m_isSelected = false;
};

