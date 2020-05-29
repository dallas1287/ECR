#pragma once
#include <QWidget>
#include <QRect>

static QRect mapRectFromGlobal(QWidget* obj, const QRect& globalRect)
{
	if (globalRect.isNull() || !globalRect.isValid())
		return QRect();

	auto topleft = obj->mapFromGlobal(globalRect.topLeft());

	return QRect(topleft, QSize(globalRect.width(), globalRect.height()));
}