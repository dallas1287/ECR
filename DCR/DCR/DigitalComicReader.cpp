#include "DigitalComicReader.h"
#include "common.h"

DigitalComicReader::DigitalComicReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pageWidget.reset(new ComicPageWidget(this));
    m_pageWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    ui.gridLayout->addWidget(m_pageWidget.get());
    ui.gridLayout->setContentsMargins(ui.centralWidget->contentsMargins());

    connect(ui.leftToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleToolBarAction);
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

    m_drawHandler.setMode(action->text());

    //TODO: handle icon enabled/disabled settings
}
