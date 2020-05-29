#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalComicReader.h"
#include "GraphicPanel.h"
#include "ComicPanelLayout.h"
#include "PanelObject.h"
#include "DrawHandler.h"
#include "ComicPanelHandler.h"

class DigitalComicReader : public QMainWindow
{
    Q_OBJECT

public:
    DigitalComicReader(QWidget *parent = Q_NULLPTR);
	~DigitalComicReader();

	bool IsDrawing() const { return m_drawMode; }
	void addPanelWidget(PanelObject* pObj);

public slots:
	void handleToolBarAction(QAction* action);

signals:
	void signalPanelObjectCreation(const QRect& rect);
	void signalGraphciPanelCreation(PanelObject* pObj);

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

    Ui::DigitalComicReaderClass ui;
	std::unique_ptr<ComicPanelLayout> m_layout;

	DrawHandler m_drawHandler;
	ComicPanelHandler m_cpHandler = ComicPanelHandler(this);

	bool m_drawMode = false;
	bool m_shapeStarted = false;
	QPoint m_rectStart, m_rectEnd, m_curPos;
};
