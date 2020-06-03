#include "GraphicPanel.h"
#include <QPainter>
#include "MediaConverter.h"
#include "PanelObject.h"
#include "common.h"


GraphicPanel::GraphicPanel(QWidget* parent, PanelObject* controller, const QString& filePath, Qt::WindowFlags flags) : QOpenGLWidget(parent), m_controller(controller), m_filePath(filePath)
{
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	setFocus(Qt::FocusReason::OtherFocusReason);
}

GraphicPanel::GraphicPanel(const GraphicPanel& other): GraphicPanel(other.parentWidget(), other.m_controller, other.m_filePath, other.windowFlags())
{
	m_gObj.reset(new GraphicsObject(*(other.m_gObj.get())));
}

GraphicPanel::~GraphicPanel()
{
}

GraphicPanel& GraphicPanel::operator=(const GraphicPanel& other)
{
	m_controller = other.m_controller;
	setParent(other.parentWidget());
	m_gObj.reset(new GraphicsObject());
	return *this;
}

void GraphicPanel::initializeGL()
{
	initializeOpenGLFunctions();
	setBackground(QVector4D()); //black
	m_gObj.reset(new GraphicsObject());
	if (m_gObj.get())
		m_gObj->initialize();
	loadFrame(m_filePath);
}

void GraphicPanel::loadFrame(const QString& path)
{
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

	painter.end();
}