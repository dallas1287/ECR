#include "DigitalComicReader.h"
#include "common.h"

DigitalComicReader::DigitalComicReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_layout.reset(new ComicPanelLayout());

    //setting a layout only works if there isn't already a layout so delete if necessary
    auto oldlayout = ui.centralWidget->layout();
    if(oldlayout)
        delete oldlayout;
    ui.centralWidget->setLayout(m_layout.get());

    connect(ui.leftToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::handleToolBarAction);
    connect(this, &DigitalComicReader::signalPanelObjectCreation, &m_cpHandler, &ComicPanelHandler::createPanelObject);
    connect(this, &DigitalComicReader::signalGraphciPanelCreation, &m_cpHandler, &ComicPanelHandler::createGraphicPanel);
}

DigitalComicReader::~DigitalComicReader()
{
}

void DigitalComicReader::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPen pen;
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QBrush brush(QColor(0, 0, 0, 0)); //fill transparent

    //draw selected
    auto selected = m_cpHandler.getSelected();
    if (selected)
    {
        pen.setColor(QColor(255, 255, 0));
        pen.setWidth(10);
        painter.setPen(pen);
        painter.fillRect(mapRectFromGlobal(this, selected->getRect()), brush);
        painter.drawRect(mapRectFromGlobal(this, selected->getRect()));
    }

    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(5);
    painter.setPen(pen);

    QRect rect(mapRectFromGlobal(this, getDrawnRect()));

    painter.fillRect(rect, brush);
    painter.drawRect(rect);
}

void DigitalComicReader::addPanelWidget(PanelObject* pObj)
{
    ui.centralWidget->layout()->addWidget(pObj->getGraphicPanel());
}

QRect DigitalComicReader::getDrawnRect(const QPoint& start, const QPoint& cur) const
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

    return QRect(QPoint(left, top), QSize(right - left, bottom - top));
}

QRect DigitalComicReader::getDrawnRect(const QPoint& cur) const
{
    return getDrawnRect(m_rectStart, cur);
}

QRect DigitalComicReader::getDrawnRect() const
{
    return getDrawnRect(m_rectStart, m_curPos);
}

/**************************************************************************************************
**************************Mouse Events*************************************************************
**************************************************************************************************/

void DigitalComicReader::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_drawMode || !m_shapeStarted)
        return;

    m_curPos = event->globalPos();
    repaint();
}

void DigitalComicReader::mousePressEvent(QMouseEvent* event)
{
     if (!m_drawMode)
        return;

    m_shapeStarted = true;
    m_rectStart = event->globalPos();
    m_curPos = QPoint(m_rectStart.x() + 10, m_rectStart.y() + 10); //assure a base shape of 10 pixels 
    m_cpHandler.setSelected(nullptr);
    repaint();
}

void DigitalComicReader::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_drawMode)
        return;
    m_shapeStarted = false;
    m_rectEnd = event->globalPos();
    emit signalPanelObjectCreation(getDrawnRect(m_rectEnd));
}

void DigitalComicReader::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = m_cpHandler.getEnclosingShape(event->globalPos());
    if (!panelObj)
        return;

    m_cpHandler.setSelected(panelObj);
     emit signalGraphciPanelCreation(panelObj);
    repaint();
}

/**************************************************************************************************
**************************Keyboard Events**********************************************************
**************************************************************************************************/

void DigitalComicReader::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
        m_drawMode ^= true; //toggle drawMode
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
