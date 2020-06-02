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

public slots:
	void handleToolBarAction(QAction* action);
	void handleTopTBContextMenu(const QPoint& pos);
	void onCreateGridCtxMenu();

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;

private:
	void handleTopToolBarAction(TopToolBar selection);
	void handleLeftToolBarAction(LeftToolBar selection);
	void handleGridCreation();

    Ui::DigitalComicReaderClass ui;
	std::unique_ptr<PageDisplayWidget> m_pageWidget;
	std::unique_ptr<PageEditForm> m_pageEdit;
};
