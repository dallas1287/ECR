#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QResizeEvent>
#include "shader_defs.h"
#include "GraphicsObject.h"

class GraphicPanel : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT
public:
	GraphicPanel(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	GraphicPanel(const GraphicPanel& other);
	virtual ~GraphicPanel();

	GraphicPanel& operator=(const GraphicPanel& other);
	QWidget* getParent() { return m_parent; }

protected:
	//virtual void wheelEvent(QWheelEvent* event);
	//virtual void mousePressEvent(QMouseEvent* event);
	//virtual void mouseReleaseEvent(QMouseEvent* event);
	//virtual void keyPressEvent(QKeyEvent* event);
	//virtual void keyReleaseEvent(QKeyEvent* event);
	void setBackground(QVector4D bg) { glClearColor(bg.x(), bg.y(), bg.z(), bg.w()); }
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void panelPaint();

	std::unique_ptr<GraphicsObject> m_gObj;
	QWidget* m_parent = nullptr;

};
