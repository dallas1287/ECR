#pragma once
#include <QWidget>
#include "ComicPanelLayout.h"
#include "ComicPanelHandler.h"
#include "PanelObject.h"
#include "DrawHandler.h"

struct NewShape
{
	bool shapeStarted = false;
	QPoint startPt;
	QPoint curPt;

	void reset()
	{
		shapeStarted = false;
		startPt = QPoint();
		curPt = startPt;
	}
};

struct EditShape
{
	PanelObject* pObj = nullptr;
	QPoint startPt;

	void reset()
	{
		pObj = nullptr;
		startPt = QPoint();
	}
};

class ComicPageWidget : public QWidget
{
	Q_OBJECT
public:
	ComicPageWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	ComicPageWidget(const ComicPageWidget& other);
	virtual ~ComicPageWidget();

	bool isDrawing() { return m_drawing; }
	void setDrawing(bool state) { m_drawing = state; }
	DrawType getDrawMode() { return m_drawHandler.getDrawMode(); }
	void setDrawMode(DrawType selection); 
	void setDrawMode(LeftToolBar selection);
	void addPanelWidget(PanelObject* panelObj);
	PanelObject* getEnclosingShape(const QPoint& cursor);
	void createGrid(int numH, int numV, int hPadding, int vPadding, int hBorder, int vBorder);

signals:
	void signalPanelObjectCreation(DrawType type, const QRect& rect);
	void signalGraphicPanelCreation(PanelObject* pObj);

protected:
	virtual void paintEvent(QPaintEvent* event) override;

	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QRect getDrawnRect(const QPoint& start, const QPoint& cur) const;
	QRect getDrawnRect(const QPoint& input) const;
	QRect getDrawnRect() const;

	void movePanel(const QPoint& curPos);
	void movePanel(const QPoint& curPos, PanelObject* pObj);

	ComicPanelLayout m_layout;
	ComicPanelHandler m_cpHandler = ComicPanelHandler(this);
	DrawHandler m_drawHandler;

	QSize m_size = QSize(1080, 1920);

	bool m_drawing = false;
	bool m_movingPanel = false;
	NewShape m_newShape;
	EditShape m_editShape;
};

