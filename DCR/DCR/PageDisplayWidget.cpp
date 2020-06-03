#include "PageDisplayWidget.h"
#include "common.h"

PageDisplayWidget::PageDisplayWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    //setting a layout only works if there isn't already a layout so delete if necessary
    auto oldlayout = layout();
    if (oldlayout)
        delete oldlayout;
    setLayout(&m_layout);

    setFixedSize(m_size);
    setFocusPolicy(Qt::StrongFocus);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QWidget::customContextMenuRequested, this, &PageDisplayWidget::handleCtxMenuRequested);
    connect(this, &PageDisplayWidget::signalPanelObjectCreation, &m_cpHandler, &ComicPanelHandler::createPanelObject);
    connect(this, &PageDisplayWidget::signalGraphicPanelCreation, &m_cpHandler, &ComicPanelHandler::createGraphicPanel);
}

PageDisplayWidget::PageDisplayWidget(const PageDisplayWidget& other)
{
}

PageDisplayWidget::~PageDisplayWidget()
{
}

void PageDisplayWidget::setDrawMode(DrawType selection)
{
    if (selection >= DrawType::Polygon && selection <= DrawType::Ellipse) 
        m_drawHandler.setDrawMode(selection); 
}

//this is wild, but to convert the toolbar enum to the drawtype enum, it converts it to the underlying type of int,
//then to the DrawType enum class
void PageDisplayWidget::setDrawMode(LeftToolBar selection)
{
    if (selection >= LeftToolBar::Polygon && selection <= LeftToolBar::Ellipse)
        m_drawHandler.setDrawMode((DrawType)(unsigned int)selection);
}

void PageDisplayWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPen pen;
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //draw background
    m_drawHandler.drawBackground(painter, m_size);

    //draw selected
    m_drawHandler.drawSelected(painter, m_cpHandler.getSelected());

    //draw current
    if (m_movingPanel)
        m_drawHandler.draw(painter, m_editShape.pObj->getRect());
    else if(m_drawing)
        m_drawHandler.draw(painter, getDrawnRect());

    m_drawHandler.draw(painter, m_cpHandler.panelObjects());
}

QRect PageDisplayWidget::getDrawnRect(const QPoint& start, const QPoint& cur) const
{
    int top, bottom, left, right;
    if (start.x() < cur.x())
    {
        left = start.x();
        right = cur.x();
    }
    else
    {
        left = cur.x();
        right = start.x();
    }

    if (start.y() < cur.y())
    {
        top = start.y();
        bottom = cur.y();
    }
    else
    {
        top = cur.y();
        bottom = start.y();
    }

    if (top < 0)
        top = 0;

    if (left < 0)
        left = 0;

    if (bottom > height())
        bottom = height();

    if (right > width())
        right = width();

    return QRect(QPoint(left, top), QSize(right - left, bottom - top));
}

QRect PageDisplayWidget::getDrawnRect(const QPoint& cur) const
{
    return getDrawnRect(m_newShape.startPt, cur);
}

QRect PageDisplayWidget::getDrawnRect() const
{
    return getDrawnRect(m_newShape.startPt, m_newShape.curPt);
}

void PageDisplayWidget::addPanelWidget(PanelObject* panelObj)
{
    layout()->addWidget(panelObj->getGraphicPanel());
}

PanelObject* PageDisplayWidget::getEnclosingShape(const QPoint& cursor)
{
    auto iter = std::find_if(m_cpHandler.panelObjects().begin(), m_cpHandler.panelObjects().end(), [&](std::unique_ptr<PanelObject>& p) { return p->getRect().contains(cursor); });
    if (iter != m_cpHandler.panelObjects().end())
        return (*iter).get();
    return nullptr;
}

void PageDisplayWidget::movePanel(const QPoint& curPos)
{

    int vmovement = curPos.y() - m_editShape.startPt.y();
    int hmovement = curPos.x() - m_editShape.startPt.x();
    m_editShape.startPt = curPos;

    QPoint delta(hmovement, vmovement);
    QPoint moved_tl = m_editShape.pObj->getRect().topLeft();

    m_editShape.pObj->getRectToEdit().moveTopLeft(moved_tl + delta);
    if(m_editShape.pObj->getGraphicPanel())
        m_editShape.pObj->getGraphicPanel()->setGeometry(m_editShape.pObj->getRect());
}

void PageDisplayWidget::createGrid(int numH, int numV, int hPadding, int vPadding, int hBorder, int vBorder)
{
    int panelWidth = (m_size.width() - ((hBorder * 2) + ((numH - 1) * hPadding))) / numH;
    int panelHeight = (m_size.height() - ((vBorder * 2) + ((numV - 1) * vPadding))) / numV;
    QSize panelSize(panelWidth, panelHeight);
    m_cpHandler.createGrid(numH, panelWidth, numV, panelHeight, hPadding, vPadding, hBorder, vBorder);
    repaint();
}

/**************************************************************************************************
**************************Slots********************************************************************
**************************************************************************************************/

void PageDisplayWidget::handleCtxMenuRequested(const QPoint& pos)
{
    QMenu* ctxMenu = new QMenu(this);
    QAction clearPage(tr("Clear Page"));
    QAction setIV(tr("Set Image/Video"));
    QAction settings(tr("Settings"));
    ctxMenu->addAction(&clearPage);
    
    PanelObject* selected = getEnclosingShape(pos);
    if (selected)
    {
        ctxMenu->addAction(&setIV);
        m_cpHandler.setSelected(selected);
    }
    ctxMenu->addAction(&settings);

    connect(&clearPage, &QAction::triggered, &m_cpHandler, &ComicPanelHandler::clearPage);
    connect(&setIV, &QAction::triggered, this, &PageDisplayWidget::launchImageSelectDialog);
    connect(&settings, &QAction::triggered, this, &PageDisplayWidget::launchSettingsDialog);
    ctxMenu->exec(mapToGlobal(pos));
    delete ctxMenu;
}

void PageDisplayWidget::launchImageSelectDialog()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image or Video"), QString(), tr("Images (*.png *.jpg *.bmp *.mp4)"));
    if(!path.isEmpty())
        m_cpHandler.createGPWithPath(m_cpHandler.getSelected(), path);
}

void PageDisplayWidget::launchSettingsDialog()
{

}

/**************************************************************************************************
**************************Mouse Events*************************************************************
**************************************************************************************************/

void PageDisplayWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_drawing)
        return;

    if (m_movingPanel)
        movePanel(event->pos());
    else if(m_newShape.shapeStarted)
        m_newShape.curPt = event->pos();
    
    repaint();
}

void PageDisplayWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_drawing || event->button() == Qt::RightButton)
        return;

    //handle editing
    PanelObject* movingShape = getEnclosingShape(event->pos());
    if (movingShape)
    {
        m_movingPanel = true;
        m_editShape.startPt = event->pos();
        m_editShape.pObj = movingShape;
        return;
    }

    //handle drawing
    m_editShape.reset(); //invalidates the editShape object, if we've made it this far then we're drawing a new shape
    m_newShape.shapeStarted = true;

    if (getDrawMode() == DrawType::Polygon)
    {
        m_drawHandler.addPoint(event->pos());
    }
    else
    {
        m_newShape.startPt = event->pos();
        m_newShape.curPt = QPoint(m_newShape.startPt.x() + 10, m_newShape.startPt.y() + 10); //assure a base shape of 10 pixels
    }

    m_cpHandler.setSelected(nullptr);
    repaint();
}

void PageDisplayWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_drawing || event->button() == Qt::RightButton)
        return;

    if (m_movingPanel)
    {
        m_movingPanel = false;
        m_editShape.reset();
        return;
    }
    
    m_newShape.shapeStarted = false;
    emit signalPanelObjectCreation(getDrawMode(), getDrawnRect(event->pos()));
}

void PageDisplayWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = getEnclosingShape(event->pos());
    if (!panelObj)
        return;

    m_cpHandler.setSelected(panelObj);
    emit signalGraphicPanelCreation(panelObj);
}