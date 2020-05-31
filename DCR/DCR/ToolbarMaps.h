#pragma once
#include <map>
#include <QString>
#include "common.h"

enum class TopToolBar : unsigned int
{
	Drawing,
	CreateGrid
};

const std::map<QString, TopToolBar> TopTBMap =
{
	{"Draw", TopToolBar::Drawing},
	{"CreateGrid", TopToolBar::CreateGrid}
};

const std::map<TopToolBar, QString> TopTBRevMap =
{
	{TopToolBar::Drawing, "Draw"},
	{TopToolBar::CreateGrid, "CreateGrid"}
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


