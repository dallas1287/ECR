#pragma once
#include <QWidget>
#include "ComicPanelLayout.h"
#include "ComicPanelHandler.h"
#include "PanelObject.h"

class ComicPageWidget : public QWidget
{
	Q_OBJECT
public:
	ComicPageWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	ComicPageWidget(const ComicPageWidget& other);
	virtual ~ComicPageWidget();

	bool isDrawing() { return m_drawing; }
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
	virtual void keyPressEvent(QKeyEvent* event) override;

private:
	QRect getDrawnRect(const QPoint& start, const QPoint& cur) const;
	QRect getDrawnRect(const QPoint& input) const;
	QRect getDrawnRect() const;

	ComicPanelLayout m_layout;
	ComicPanelHandler m_cpHandler = ComicPanelHandler(this);

	bool m_drawing = false;
	bool m_shapeStarted = false;
	QPoint m_rectStart, m_rectEnd, m_curPos;
};

