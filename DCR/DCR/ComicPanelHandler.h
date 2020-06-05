#pragma once
#include "PanelObject.h"

class ComicPage;

struct NewShape
{
	bool shapeStarted = false;
	QPoint startPt;
	QPoint curPt;
	std::vector<QPoint> m_points;

	void addPoint(const QPoint& pt)
	{
		m_points.push_back(pt);
	}

	void reset()
	{
		shapeStarted = false;
		startPt = QPoint();
		curPt = startPt;
		m_points.clear();
	}
};

struct EditShape
{
	PanelObject* pObj = nullptr;
	QPoint startPt;
	DrawType type = DrawType::OutOfRange;

	void reset()
	{
		pObj = nullptr;
		startPt = QPoint();
		type = DrawType::OutOfRange;
	}
};

class ComicPanelHandler : public QObject
{
	Q_OBJECT
public:
	ComicPanelHandler(ComicPage* owner);
	ComicPanelHandler(const ComicPanelHandler& other);
	~ComicPanelHandler();

	ComicPanelHandler& operator=(const ComicPanelHandler& other);

	ComicPage* getOwner() { return m_owner; }
	PanelObjectPool& panelObjects() { return m_panelObjects; }
	bool isDrawing() { return m_drawing; }
	void setSelected(PanelObject* panelObj);
	PanelObject* getSelected() const { return m_selected; }
	QRect getCurrentShape();
	DrawType getCurrentShapeType();
	PanelObject* getEnclosingShape(const QPoint& cursor);
	void createGrid(int numH, int width, int numV, int height, int hPadding, int vPadding, int hBorder, int vBorder);
	void createGPWithPath(PanelObject* panelObj, const QString& path);

	void mouseMoved(const QPoint& pos);
	void mouseClicked(const QPoint& pos);
	void mouseReleased(const QPoint& pos);

	static bool isRectMinSize(const QRect& rect) { return rect.width() >= MinRectWidth && rect.height() >= MinRectHeight; }
	static DrawType getDrawType();

public slots:
	void createPanelObject(DrawType mode, const QRect& rect);
	void createGraphicPanel(PanelObject* panelObj);
	void clearPage();
	void clearEditingShapes();
	void toggleDrawing();
	void setDrawing(bool state);

signals:
	void addGraphicPanelToLayout(GraphicPanel* gp);

private:
	QRect getDrawnRect(const QPoint& start, const QPoint& cur) const;
	QRect getDrawnRect(const QPoint& input) const;
	QRect getDrawnRect() const;
	void movePanel(const QPoint& curPos);

	ComicPage* m_owner = nullptr;
	PanelObjectPool m_panelObjects;
	PanelObject* m_selected = nullptr;

	NewShape m_newShape;
	EditShape m_editShape;
	bool m_drawing = false;
	bool m_movingPanel = false;
};

