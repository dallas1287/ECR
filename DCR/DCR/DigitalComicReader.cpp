#include "DigitalComicReader.h"
#include <QPainter>

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

    connect(ui.mainToolBar, &QToolBar::actionTriggered, this, &DigitalComicReader::HandleToolBarAction);
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
    if (m_selected)
    {
        m_selected->getRect();
        pen.setColor(QColor(255, 255, 0));
        pen.setWidth(10);
        painter.setPen(pen);
        painter.fillRect(m_selected->getRect(), brush);
        painter.drawRect(m_selected->getRect());
    }

    pen.setColor(QColor(0, 0, 0));
    pen.setWidth(5);
    painter.setPen(pen);

    QRect rect(getDrawnRect());

    painter.fillRect(rect, brush);
    painter.drawRect(rect);
}

void DigitalComicReader::createPanel(PanelObject* pObj)
{
    pObj->createGraphicPanel();
    ui.centralWidget->layout()->addWidget(pObj->getGraphicPanel());
}

PanelObject* DigitalComicReader::createPanelObject(const QRect& size)
{
    m_panelObjects.emplace_back(std::unique_ptr<PanelObject>(new PanelObject(this, size)));
    return m_panelObjects.back().get();
}

PanelObject* DigitalComicReader::getEnclosingShape(const QPoint& cursor)
{
    auto iter = std::find_if(m_panelObjects.begin(), m_panelObjects.end(), [&](std::unique_ptr<PanelObject>& p) { return p->getRect().contains(cursor); });
    if (iter != m_panelObjects.end())
        return (*iter).get();
    return nullptr;
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

    return QRect(QPoint(left, top), QPoint(right, bottom));
}

QRect DigitalComicReader::getDrawnRect(const QPoint& cur) const
{
    return getDrawnRect(m_rectStart, cur);
}

QRect DigitalComicReader::getDrawnRect() const
{
    return getDrawnRect(m_rectStart, m_curPos);
}

void DigitalComicReader::SetSelected(PanelObject* selected)
{
    for (std::unique_ptr<PanelObject>& p : m_panelObjects)
    {
        if (!p.get())
            continue;

        if (p.get() != selected)
        {
            p->setSelected(false);
        }
        else
        {
            p->setSelected();
        }
    }
    m_selected = selected;
}

/**************************************************************************************************
**************************Mouse Events*************************************************************
**************************************************************************************************/

void DigitalComicReader::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_drawMode || !m_shapeStarted)
        return;

    m_curPos = event->pos();
    repaint();
}

void DigitalComicReader::mousePressEvent(QMouseEvent* event)
{
    if (!m_drawMode)
        return;
    m_shapeStarted = true;
    m_rectStart = event->pos();
    m_curPos = QPoint(m_rectStart.x() + 10, m_rectStart.y() + 10); //assure a base shape of 10 pixels 
    repaint();
}

void DigitalComicReader::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_drawMode)
        return;
    m_shapeStarted = false;
    m_rectEnd = event->pos();
    createPanel(createPanelObject(getDrawnRect()));
    repaint();
}

void DigitalComicReader::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto panelObj = getEnclosingShape(event->pos());
    if (!panelObj)
        return;

    SetSelected(panelObj);
    //panelObj->createGraphicPanel();
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

void DigitalComicReader::HandleToolBarAction(QAction* action)
{
    qDebug() << "handled toolbar action";
    if (action)
        qDebug() << " got action: " << action->text();
}
