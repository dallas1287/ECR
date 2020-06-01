#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QResizeEvent>
#include "shader_defs.h"
#include "GraphicsObject.h"

class PanelObject;

class GraphicPanel : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT
public:
	GraphicPanel(QWidget* parent, PanelObject* controller, const QString& filePath = QString(), Qt::WindowFlags flags = Qt::WindowFlags());
	GraphicPanel(const GraphicPanel& other);
	virtual ~GraphicPanel();

	GraphicPanel& operator=(const GraphicPanel& other);

	void loadFrame(const QString& path);

protected:
	//virtual void wheelEvent(QWheelEvent* event);
	//virtual void mousePressEvent(QMouseEvent* event);
	//virtual void mouseReleaseEvent(QMouseEvent* event);
	//virtual void keyPressEvent(QKeyEvent* event);
	//virtual void keyReleaseEvent(QKeyEvent* event);

	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void panelPaint();

	void setBackground(QVector4D bg) { glClearColor(bg.x(), bg.y(), bg.z(), bg.w()); }

	std::unique_ptr<GraphicsObject> m_gObj;
	PanelObject* m_controller = nullptr;
	QString m_filePath;

};
