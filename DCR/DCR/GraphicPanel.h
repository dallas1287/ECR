#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QResizeEvent>
#include "shader_defs.h"
#include "GraphicsObject.h"

class GraphicPanelBase : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT
public:
	GraphicPanelBase(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	virtual ~GraphicPanelBase();
	QWidget* getParent() { return m_parent; }

protected:
	//virtual void wheelEvent(QWheelEvent* event);
	//virtual void mouseMoveEvent(QMouseEvent* event);
	//virtual void mousePressEvent(QMouseEvent* event);
	//virtual void mouseReleaseEvent(QMouseEvent* event);
	//virtual void keyPressEvent(QKeyEvent* event);
	//virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void panelPaint() = 0;
	void setBackground(QVector4D bg) { glClearColor(bg.x(), bg.y(), bg.z(), bg.w()); }
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;

private:
	QWidget* m_parent = nullptr;

};

class GraphicPanel : public GraphicPanelBase
{
public:
	GraphicPanel(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	virtual ~GraphicPanel();

protected:
	virtual void initializeGL() override;
	virtual void panelPaint() override;

	std::unique_ptr<GraphicsObject> m_gObj;

};

