#include "DigitalComicReader.h"

DigitalComicReader::DigitalComicReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_gPanel.reset(new GraphicPanel(this));
    ui.gpLayout->layout()->addWidget(m_gPanel.get());

    m_gPanel->setFocus(Qt::FocusReason::OtherFocusReason);
}

