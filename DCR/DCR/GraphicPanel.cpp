#include "GraphicPanel.h"
#include <QPainter>
#include "MediaConverter.h"
#include "PanelObject.h"
#include "common.h"


GraphicPanel::GraphicPanel(QWidget* parent, PanelObject* controller, Qt::WindowFlags flags) : QOpenGLWidget(parent), m_controller(controller)
{
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	setFocus(Qt::FocusReason::OtherFocusReason);
}

GraphicPanel::GraphicPanel(const GraphicPanel& other): GraphicPanel(other.parentWidget(), other.m_controller, other.windowFlags())
{
	m_gObj.reset(other.m_gObj.get());
}

GraphicPanel::~GraphicPanel()
{
}

GraphicPanel& GraphicPanel::operator=(const GraphicPanel& other)
{
	m_controller = other.m_controller;
	setParent(other.parentWidget());
	m_gObj.reset(other.m_gObj.get());
	return *this;
}

void GraphicPanel::initializeGL()
{
	initializeOpenGLFunctions();
	setBackground(QVector4D()); //black
	m_gObj.reset(new GraphicsObject());
	if (m_gObj)
		m_gObj->initialize();

	CMediaConverter mc;
	int width = 0;
	int height = 0;
	unsigned char* data = nullptr;
	ErrorCode ret = mc.loadFrame("../EeveeRender1.mp4", width, height, &data);

	if (ret != ErrorCode::SUCCESS || width == 0 || height == 0)
	{
		if (data)
			delete[] data;
		return;
	}

	if (!m_gObj->resetTexture(width, height, (void*)data))
		qDebug() << "Texture data not set";

	delete[] data;
}
void GraphicPanel::paintGL()
{
	panelPaint();
}

void GraphicPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}


void GraphicPanel::panelPaint()
{
	QPainter painter;
	painter.begin(this);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_gObj)
		m_gObj->Draw();
	
	QPen pen(QColor(255, 0, 255));
	pen.setWidth(10);
	painter.setPen(pen);
	pen.setStyle(Qt::PenStyle::SolidLine);

	QBrush brush(QColor(0, 0, 0, 0)); //fill transparent

	painter.beginNativePainting();

	QRect geo(QPoint(0,0), QSize(50, 50));
	painter.fillRect(geo, brush);
	painter.drawRect(geo);
	painter.endNativePainting();

	painter.end();
}