#include "ComicPage.h"

ComicPage::ComicPage(QWidget* surface, const QSize& size) : m_drawSurface(surface), m_pageSize(size)
{
}

ComicPage::~ComicPage()
{
}

void ComicPage::createGrid(int numH, int numV, int hPadding, int vPadding, int hBorder, int vBorder)
{
    int panelWidth = (m_pageSize.width() - ((hBorder * 2) + ((numH - 1) * hPadding))) / numH;
    int panelHeight = (m_pageSize.height() - ((vBorder * 2) + ((numV - 1) * vPadding))) / numV;
    QSize panelSize(panelWidth, panelHeight);
    m_cpHandler.createGrid(numH, panelWidth, numV, panelHeight, hPadding, vPadding, hBorder, vBorder);
    m_drawSurface->repaint();
}