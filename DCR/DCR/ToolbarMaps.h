#pragma once
#include <map>
#include <QString>

enum class TopToolBar
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
	Polygon,
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