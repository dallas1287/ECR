#pragma once
#include "PanelObject.h"



class ComicPanelHandler : public QObject
{
	Q_OBJECT
public:
	ComicPanelHandler(QWidget* owner);
	ComicPanelHandler(const ComicPanelHandler& other);
	~ComicPanelHandler();

	ComicPanelHandler& operator=(const ComicPanelHandler& other);

	QWidget* getOwner() { return m_owner; }
	PanelObjectPool& panelObjects() { return m_panelObjects; }

	void setSelected(PanelObject* panelObj);
	PanelObject* getSelected() const { return m_selected; }
	void createGrid(int numH, int width, int numV, int height, int padding);

public slots:
	void createPanelObject(DrawType mode, const QRect& rect);
	void createGraphicPanel(PanelObject* panelObj);

private:

	QWidget* m_owner = nullptr;
	PanelObjectPool m_panelObjects;
	PanelObject* m_selected = nullptr;


};

