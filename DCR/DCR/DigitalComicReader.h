#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalComicReader.h"
#include "GraphicPanel.h"
#include "ComicPanelLayout.h"
#include "PanelObject.h"

class DigitalComicReader : public QMainWindow
{
    Q_OBJECT

public:
    DigitalComicReader(QWidget *parent = Q_NULLPTR);
	~DigitalComicReader();

	bool IsDrawing() const { return m_drawMode; }

public slots:
	void HandleToolBarAction(QAction* action);

protected:
	virtual void paintEvent(QPaintEvent* event) override;

	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;

private:
	void createPanel(PanelObject* pObj);
	PanelObject* createPanelObject(const QRect& size);

	PanelObject* getEnclosingShape(const QPoint& cursor);
	
	QRect getDrawnRect(const QPoint& start, const QPoint& cur) const;
	QRect getDrawnRect(const QPoint& input) const;
	QRect getDrawnRect() const;

	void SetSelected(PanelObject* selected);

    Ui::DigitalComicReaderClass ui;
	std::unique_ptr<ComicPanelLayout> m_layout;
	std::vector<std::unique_ptr<PanelObject>> m_panelObjects;

	bool m_drawMode = false;
	bool m_shapeStarted = false;
	QPoint m_rectStart, m_rectEnd, m_curPos;
	PanelObject* m_selected = nullptr;
};
