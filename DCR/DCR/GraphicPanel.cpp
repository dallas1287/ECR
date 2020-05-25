#include "GraphicPanel.h"
#include <QPainter>
#include "MediaConverter.h"

GraphicPanelBase::GraphicPanelBase(QWidget* parent, Qt::WindowFlags flags): m_parent(parent)
{
}

GraphicPanelBase::~GraphicPanelBase()
{

}

void GraphicPanelBase::initializeGL()
{

}

void GraphicPanelBase::paintGL()
{
	panelPaint();
}

void GraphicPanelBase::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

GraphicPanel::GraphicPanel(QWidget* parent, Qt::WindowFlags flags) : GraphicPanelBase(parent, flags)
{
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	setFocus(Qt::FocusReason::OtherFocusReason);
}

GraphicPanel::~GraphicPanel()
{
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

	bool failed = false;
	if (ret != ErrorCode::SUCCESS || width == 0 || height == 0)
		return;

	if (!m_gObj->resetTexture(width, height, (void*)data))
		qDebug() << "Texture data not set";

	delete[] data;
}

void GraphicPanel::panelPaint()
{
	QPainter painter;
	painter.begin(this);
	painter.beginNativePainting();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_gObj)
		m_gObj->Draw();

	painter.end();
	update();
}