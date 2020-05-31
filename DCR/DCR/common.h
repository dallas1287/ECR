#pragma once
#include <QWidget>
#include <QRect>

enum class DrawType : unsigned int
{
	Polygon,
	Rectangle,
	Circle,
	Ellipse
};

static QRect mapRectFromGlobal(QWidget* obj, const QRect& globalRect)
{
	if (globalRect.isNull() || !globalRect.isValid())
		return QRect();

	auto topleft = obj->mapFromGlobal(globalRect.topLeft());

	return QRect(topleft, QSize(globalRect.width(), globalRect.height()));
}

static QRect mapRectToGlobal(QWidget* obj, const QRect& localRect)
{
	if (localRect.isNull() || !localRect.isValid())
		return QRect();

	auto topleft = obj->mapFromGlobal(localRect.topLeft());

	return QRect(topleft, QSize(localRect.width(), localRect.height()));
}