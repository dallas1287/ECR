#pragma once
#include <QWidget>
#include "ComicPanelLayout.h"
#include "ComicPanelHandler.h"
#include "PanelObject.h"
#include "DrawHandler.h"

class ComicPageWidget : public QWidget
{
	Q_OBJECT
public:
	ComicPageWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	ComicPageWidget(const ComicPageWidget& other);
	virtual ~ComicPageWidget();

	bool isDrawing() { return m_drawing; }
	void setDrawing(bool state) { m_drawing = state; }
	LeftToolBar getDrawMode() { return m_drawHandler.getMode(); }
	void setDrawMode(LeftToolBar selection) { if (selection >= LeftToolBar::Polygon && selection <= LeftToolBar::Ellipse) m_drawHandler.setMode(selection); }
	void addPanelWidget(PanelObject* panelObj);

signals:
	void signalPanelObjectCreation(const QRect& rect);
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

	ComicPanelLayout m_layout;
	ComicPanelHandler m_cpHandler = ComicPanelHandler(this);
	DrawHandler m_drawHandler;

	bool m_drawing = false;
	bool m_shapeStarted = false;
	QPoint m_rectStart, m_rectEnd, m_curPos;
};

