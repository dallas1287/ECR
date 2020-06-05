#pragma once
#include <QtWidgets>
#include <QRect>

const int MinRectWidth = 10;
const int MinRectHeight = 10;
const QSize StandardPageSize = QSize(1080, 1920);

enum class DrawType : unsigned int
{
	Polygon,
	Rectangle,
	Circle,
	Ellipse,
	OutOfRange
};

static DrawType operator++(DrawType& index)
{
	auto inc = static_cast<DrawType>(static_cast<unsigned int>(index) + 1);
	return inc;
}


static QMainWindow* getMainWindow()
{
	foreach(QWidget* widget, qApp->topLevelWidgets())
	{
		if (QMainWindow* mainWindow = qobject_cast<QMainWindow*>(widget))
			return mainWindow;
	}

	return nullptr;
}

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