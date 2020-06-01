#include "DigitalComicReader.h"
#include "common.h"

DigitalComicReader::DigitalComicReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pageWidget.reset(new ComicPageWidget(this));
    ui.scrollArea->setWidget(m_pageWidget.get());
    ui.scrollArea->setBackgroundRole(QPalette::Dark);

    ui.topToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.leftToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleToolBarAction);
    connect(ui.topToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleToolBarAction);
    connect(ui.topToolBar, &QWidget::customContextMenuRequested, this, &DigitalComicReader::handleTopTBContextMenu);

}

DigitalComicReader::~DigitalComicReader()
{
}

/**************************************************************************************************
**************************Toolbar Events***********************************************************
**************************************************************************************************/

void DigitalComicReader::handleToolBarAction(QAction* action)
{
    if (!action)
        return;

    auto topTB = TopTBMap.find(action->text());
    if (topTB != TopTBMap.end())
    {
        handleTopToolBarAction(topTB->second);
        return;
    }

    auto leftTB = LeftTBMap.find(action->text());
    if (leftTB != LeftTBMap.end())
    {
        handleLeftToolBarAction(leftTB->second);
        return;
    }
}

void DigitalComicReader::handleTopToolBarAction(TopToolBar selection)
{
    switch (selection)
    {
        case TopToolBar::Drawing:
            m_pageWidget->setDrawing(m_pageWidget->isDrawing() ^ true);
            break;
        case TopToolBar::CreateGrid:
            handleGridCreation();
            break;
        default:
            return;
    }
}

void DigitalComicReader::handleLeftToolBarAction(LeftToolBar selection)
{
    if (selection >= LeftToolBar::Polygon && selection <= LeftToolBar::Ellipse)
    {
        //clear all actions checked first
        for (auto a : ui.leftToolBar->actions())
            a->setChecked(false);

        QAction* selected = ui.leftToolBar->actions()[(int)selection];
        if (selected)
        {
            selected->setChecked(selected->isChecked() ^ true);
            m_pageWidget->setDrawMode(selection);
        }
    }
}

void DigitalComicReader::handleTopTBContextMenu(const QPoint& pos)
{
    QAction* a = ui.topToolBar->actionAt(pos);
    if (a == ui.topToolBar->actions()[(unsigned int)TopToolBar::CreateGrid])
    {
        QMenu* gridCtxMenu = new QMenu(this);
        QAction grid(tr("Make Grid"));
        grid.setIcon(a->icon());
        QAction grid2(tr("Make another Grid"));
        grid2.setIcon(a->icon());
        gridCtxMenu->addAction(&grid);
        gridCtxMenu->addAction(&grid2);

        //this is lame but it seems to put the ctx menus first item's center-ish at the pt given 
        //so the below pt puts it in a better position
        auto wSize = ui.topToolBar->widgetForAction(a)->sizeHint();
        QPoint pt(wSize.width() * 2, wSize.height() + wSize.height() / 2);

        connect(&grid, &QAction::triggered, this, &DigitalComicReader::onCreateGridCtxMenu);
        gridCtxMenu->exec(mapToGlobal(pt));

        delete gridCtxMenu;
    }
}

void DigitalComicReader::onCreateGridCtxMenu()
{
    handleGridCreation();
}

void DigitalComicReader::handleGridCreation()
{
    int numH = ui.lineEditNumPanelsH->text().toInt();
    int numV = ui.lineEditNumPanelsV->text().toInt();
    int hPadding = ui.lineEditPPH->text().toInt();
    int vPadding = ui.lineEditPPV->text().toInt();
    int hBorder = ui.lineEditBorderH->text().toInt();
    int vBorder = ui.lineEditBorderV->text().toInt();
    m_pageWidget->createGrid(numH, numV, hPadding, vPadding, hBorder, vBorder);
}

/**************************************************************************************************
**************************Keyboard Events**********************************************************
**************************************************************************************************/

void DigitalComicReader::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        m_pageWidget->setDrawing(m_pageWidget->isDrawing() ^ true);
        QAction* a = ui.topToolBar->actions()[(int)TopToolBar::Drawing];
        if(a)
            a->setChecked(a->isChecked() ^ true);
    }
}
