#include "PageDisplayWidget.h"
#include "DigitalComicReader.h"
#include "common.h"

PageDisplayWidget::PageDisplayWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), m_comicPage(ComicPage(this, StandardPageSize)), m_drawHandler(this)
{
    //setting a layout only works if there isn't already a layout so delete if necessary
    auto oldlayout = layout();
    if (oldlayout)
        delete oldlayout;
    setLayout(&m_layout);

    setFixedSize(m_comicPage.pageSize());
    setFocusPolicy(Qt::StrongFocus);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAttribute(Qt::WA_Hover);

    connect(this, &QWidget::customContextMenuRequested, this, &PageDisplayWidget::handleCtxMenuRequested);
    connect(this, &PageDisplayWidget::signalGraphicPanelCreation, &m_comicPage.getPanelHandler(), &ComicPanelHandler::createGraphicPanel);
    connect(&m_comicPage.getPanelHandler(), &ComicPanelHandler::addGraphicPanelToLayout, this, &PageDisplayWidget::addGraphicPanelToLayout);
    connect(this, &PageDisplayWidget::toggleDrawing, &m_comicPage.getPanelHandler(), &ComicPanelHandler::toggleDrawing);
    connect(this, &PageDisplayWidget::clearEditing, &m_comicPage.getPanelHandler(), &ComicPanelHandler::clearEditingShapes);
}

PageDisplayWidget::PageDisplayWidget(const PageDisplayWidget& other)
{
}

PageDisplayWidget::~PageDisplayWidget()
{
}

void PageDisplayWidget::paintEvent(QPaintEvent* event)
{
    m_drawHandler.drawPage(m_comicPage);
}



/**************************************************************************************************
**************************Slots********************************************************************
**************************************************************************************************/

void PageDisplayWidget::addGraphicPanelToLayout(GraphicPanel* gp)
{
    layout()->addWidget(gp);
}

void PageDisplayWidget::handleCtxMenuRequested(const QPoint& pos)
{
    QMenu* ctxMenu = new QMenu(this);
    QAction clearPage(tr("Clear Page"));
    QAction setIV(tr("Set Image/Video"));
    QAction settings(tr("Settings"));
    ctxMenu->addAction(&clearPage);

    PanelObject* selected = m_comicPage.getPanelHandler().getEnclosingShape(pos);
    if (selected)
    {
        ctxMenu->addAction(&setIV);
        m_comicPage.getPanelHandler().setSelected(selected);
    }

    ctxMenu->addAction(&settings);

    connect(&clearPage, &QAction::triggered, &m_comicPage.getPanelHandler(), &ComicPanelHandler::clearPage);
    connect(&setIV, &QAction::triggered, this, &PageDisplayWidget::launchImageSelectDialog);
    connect(&settings, &QAction::triggered, this, &PageDisplayWidget::launchSettingsDialog);
    ctxMenu->exec(mapToGlobal(pos));
    delete ctxMenu;
}

void PageDisplayWidget::launchImageSelectDialog()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image or Video"), QString(), tr("Images (*.png *.jpg *.bmp *.mp4)"));
    if(!path.isEmpty())
        m_comicPage.getPanelHandler().createGPWithPath(m_comicPage.getPanelHandler().getSelected(), path);
}

void PageDisplayWidget::launchSettingsDialog()
{

}

/**************************************************************************************************
**************************Mouse Events*************************************************************
**************************************************************************************************/

void PageDisplayWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_comicPage.getPanelHandler().mouseMoved(event->pos());
    repaint();
}

void PageDisplayWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        return;
    m_comicPage.getPanelHandler().mouseClicked(event->pos());
    repaint();
}

void PageDisplayWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        return;

    m_comicPage.getPanelHandler().mouseReleased(event->pos());
    repaint();
}

void PageDisplayWidget::enterEvent(QEvent* event)
{
    setFocus(Qt::FocusReason::OtherFocusReason);
}

void PageDisplayWidget::leaveEvent(QEvent* event)
{
    emit clearEditing();
}
/**************************************************************************************************
**************************Keyboard Events**********************************************************
**************************************************************************************************/

void PageDisplayWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        emit toggleDrawing();
    }
}