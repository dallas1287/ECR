#include "ComicPanelLayout.h"

ComicPanelLayout::ComicPanelLayout() : QLayout()
{
}

ComicPanelLayout::~ComicPanelLayout()
{
	QLayoutItem* item;
	while (item = takeAt(0))
		delete item;
}

void ComicPanelLayout::addItem(QLayoutItem* item)
{
	if (item)
		m_items.append(item);
}

QLayoutItem* ComicPanelLayout::itemAt(int index) const
{
	if(index < 0 || index >= m_items.size())
		return nullptr;

	return m_items.value(index);
}

QLayoutItem* ComicPanelLayout::takeAt(int index)
{
	if (index < 0 || index >= m_items.size())
		return nullptr;

	return m_items.takeAt(index);
}

//copied code from LayoutManagement example https://doc.qt.io/qt-5/layout.html
QSize ComicPanelLayout::sizeHint() const
{
	QSize s(0, 0);
	int n = m_items.count();
	if (n > 0)
		s = QSize(100, 70); //start with a nice default size
	int i = 0;
	while (i < n) {
		QLayoutItem* o = m_items.at(i);
		s = s.expandedTo(o->sizeHint());
		++i;
	}
	return s + n * QSize(spacing(), spacing());
}