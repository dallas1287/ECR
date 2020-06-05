#include "DigitalComicReader.h"
#include "common.h"

DigitalComicReader::DigitalComicReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pageWidget.reset(new PageDisplayWidget(this));
    ui.scrollArea->setWidget(m_pageWidget.get());
    ui.scrollArea->setBackgroundRole(QPalette::Dark);
    m_pageEdit.reset(new PageEditForm(ui.scrollAreaPages));
    ui.scrollAreaPages->setWidget(m_pageEdit.get());
    ui.scrollAreaPages->setBackgroundRole(QPalette::Dark);
    ui.topToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.leftToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleLeftToolBarAction);
    connect(ui.topToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleTopToolBarAction);
    connect(ui.topToolBar, &QWidget::customContextMenuRequested, this, &DigitalComicReader::handleTopTBContextMenu);
    connect(m_pageWidget.get(), &PageDisplayWidget::toggleDrawing, this, &DigitalComicReader::toggleDrawingTBButton);
}

DigitalComicReader::~DigitalComicReader()
{
}

DrawType DigitalComicReader::getDrawType()
{
    for (DrawType type = DrawType::Polygon; type <= DrawType::Ellipse; type = ++type)
    {
        if (ui.leftToolBar->actions()[(unsigned int)type]->isChecked())
            return type;
    }
    return DrawType::OutOfRange;
}

/**************************************************************************************************
**************************Toolbar Events***********************************************************
**************************************************************************************************/

void DigitalComicReader::handleTopToolBarAction(QAction* action)
{
    auto selection = TopTBMap.find(action->text());
    switch (selection->second)
    {
    case TopToolBar::Drawing:
        emit m_pageWidget->toggleDrawing();
        break;
    case TopToolBar::CreateGrid:
        handleGridCreation();
        break;
    default:
        break;
    }
}

void DigitalComicReader::handleLeftToolBarAction(QAction* action)
{
    auto selection = LeftTBMap.find(action->text());
    switch (selection->second)
    {
        case LeftToolBar::Polygon:
        case LeftToolBar::Rectangle:
        case LeftToolBar::Circle:
        case LeftToolBar::Ellipse:
        {  //clear all actions checked first
            for (auto a : ui.leftToolBar->actions())
                a->setChecked(false);

            QAction* selected = ui.leftToolBar->actions()[(unsigned int)selection->second];
            if (selected)
                selected->setChecked(selected->isChecked() ^ true);
            break;
        }
        default:
            break;
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
    m_pageWidget->getComicPage().createGrid(numH, numV, hPadding, vPadding, hBorder, vBorder);
}

/**************************************************************************************************
**************************Slots********************************************************************
**************************************************************************************************/

void DigitalComicReader::toggleDrawingTBButton()
{
    QAction* a = ui.topToolBar->actions()[(int)TopToolBar::Drawing];
    if(a)
        a->setChecked(a->isChecked() ^ true);
}