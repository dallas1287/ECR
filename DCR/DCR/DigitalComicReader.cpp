#include "DigitalComicReader.h"
#include "common.h"

DigitalComicReader::DigitalComicReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pageWidget.reset(new ComicPageWidget(this));
    ui.scrollArea->setWidget(m_pageWidget.get());
    ui.scrollArea->setBackgroundRole(QPalette::Dark);


    connect(ui.leftToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleToolBarAction);
    connect(ui.topToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleToolBarAction);
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
