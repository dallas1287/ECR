#pragma once
#include "PanelObject.h"

typedef std::vector<std::unique_ptr<PanelObject>> PanelObjectPool;

class ComicPanelHandler : public QObject
{
	Q_OBJECT
public:
	ComicPanelHandler(QWidget* parent);
	ComicPanelHandler(const ComicPanelHandler& other);
	~ComicPanelHandler();

	ComicPanelHandler& operator=(const ComicPanelHandler& other);

	PanelObjectPool& panelObjects() { return m_panelObjects; }

	void setSelected(PanelObject* panelObj);
	PanelObject* getSelected() const { return m_selected; }
	PanelObject* getEnclosingShape(const QPoint& cursor);

public slots:
	void createPanelObject(const QRect& rect);
	void createGraphicPanel(PanelObject* panelObj);

private:

	QWidget* m_parent = nullptr;
	PanelObjectPool m_panelObjects;
	PanelObject* m_selected = nullptr;


};

