#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DigitalComicReader.h"
#include "GraphicPanel.h"

class DigitalComicReader : public QMainWindow
{
    Q_OBJECT

public:
    DigitalComicReader(QWidget *parent = Q_NULLPTR);

private:
    Ui::DigitalComicReaderClass ui;
    std::unique_ptr<GraphicPanel> m_gPanel;

};
