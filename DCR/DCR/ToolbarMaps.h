#pragma once
#include <map>
#include <QString>
#include "common.h"

enum class TopToolBar : unsigned int
{
	Drawing
};

const std::map<QString, TopToolBar> TopTBMap =
{
	{"Draw", TopToolBar::Drawing}
};

const std::map<TopToolBar, QString> TopTBRevMap =
{
	{TopToolBar::Drawing, "Draw"}
};

enum class LeftToolBar
{
	Polygon = (int) DrawType::Polygon,
	Rectangle,
	Circle,
	Ellipse
};

const std::map<QString, LeftToolBar> LeftTBMap =
{
	{"DrawPolygon", LeftToolBar::Polygon },
	{"DrawRect", LeftToolBar::Rectangle},
	{"DrawCircle", LeftToolBar::Circle},
	{"DrawEllipse", LeftToolBar::Ellipse}
};


