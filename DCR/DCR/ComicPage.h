#pragma once
#include <QtWidgets>
#include "ComicPanelHandler.h"
#include "DrawHandler.h"

class ComicPage
{
public:
	ComicPage() {}
	ComicPage(QWidget* surface, const QSize& size);
	~ComicPage();
	QSize& pageSize() { return m_pageSize; }
	void setPageSize(const QSize& size) { m_pageSize = size; }
	ComicPanelHandler& getPanelHandler() { return m_cpHandler; }
	void createGrid(int numH, int numV, int hPadding, int vPadding, int hBorder, int vBorder);

private:
	QWidget* m_drawSurface = nullptr;
	ComicPanelHandler m_cpHandler = ComicPanelHandler(this);
	QSize m_pageSize = StandardPageSize;
};

