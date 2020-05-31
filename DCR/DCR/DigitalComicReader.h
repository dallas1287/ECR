#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalComicReader.h"
#include "ComicPageWidget.h"
#include "GraphicPanel.h"
#include "PanelObject.h"
#include "DrawHandler.h"
#include "ComicPanelHandler.h"
#include "ComicPanelLayout.h"

class DigitalComicReader : public QMainWindow
{
    Q_OBJECT

public:
    DigitalComicReader(QWidget *parent = Q_NULLPTR);
	~DigitalComicReader();

public slots:
	void handleToolBarAction(QAction* action);

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;

private:
	void handleTopToolBarAction(TopToolBar selection);
	void handleLeftToolBarAction(LeftToolBar selection);
	void handleGridCreation();

    Ui::DigitalComicReaderClass ui;
	std::unique_ptr<ComicPageWidget> m_pageWidget;
};
