#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalComicReader.h"
#include "PageDisplayWidget.h"
#include "PageEditForm.h"

class DigitalComicReader : public QMainWindow
{
    Q_OBJECT

public:
    DigitalComicReader(QWidget *parent = Q_NULLPTR);
	~DigitalComicReader();
	DrawType getDrawType();

public slots:
	void handleTopToolBarAction(QAction* action);
	void handleLeftToolBarAction(QAction* action);
	void handleTopTBContextMenu(const QPoint& pos);
	void onCreateGridCtxMenu();
	void toggleDrawingTBButton();
	void createNewPage();

private:
	void handleGridCreation();

    Ui::DigitalComicReaderClass ui;
	std::unique_ptr<PageDisplayWidget> m_pageWidget;
	std::unique_ptr<PageEditForm> m_pageEdit;
};
