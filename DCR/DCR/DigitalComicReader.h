#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalComicReader.h"
#include "ComicPageWidget.h"
#include "GraphicPanel.h"
#include "PanelObject.h"
#include "DrawHandler.h"
#include "ComicPanelHandler.h"

class DigitalComicReader : public QMainWindow
{
    Q_OBJECT

public:
    DigitalComicReader(QWidget *parent = Q_NULLPTR);
	~DigitalComicReader();

public slots:
	void handleToolBarAction(QAction* action);

private:
    Ui::DigitalComicReaderClass ui;

	std::unique_ptr<ComicPageWidget> m_pageWidget;

	DrawHandler m_drawHandler;
	ComicPanelHandler m_cpHandler = ComicPanelHandler(this);
};
