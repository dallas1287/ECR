#pragma once

#include <QtWidgets>
#include <QVector>

class ComicPanelLayout : public QLayout
{
public:
	ComicPanelLayout();
	ComicPanelLayout(QWidget* parent);
	~ComicPanelLayout();

protected:
	virtual void addItem(QLayoutItem* item) override;
	virtual int count() const override { return m_items.size(); };
	virtual QLayoutItem* itemAt(int index) const override;
	virtual QLayoutItem* takeAt(int index) override;
	virtual QSize sizeHint() const override;

private:
	QVector<QLayoutItem*> m_items;
};