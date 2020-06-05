#include "PageEditForm.h"

PageEditForm::PageEditForm(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	//create header and layout
	m_button.reset(new QPushButton("v"));
	m_button->setCheckable(true);
	m_button->setChecked(true);
	m_button->setFixedWidth(25);
	m_button->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
	m_headerLabel.reset(new QLabel("Page 0"));
	m_headerLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_headerLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed));
	m_headerLayout = new QHBoxLayout();
	m_headerLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
	m_headerLayout->addWidget(m_button.get());
	m_headerLayout->addWidget(m_headerLabel.get());

	//create Widget properties form and layout
	m_formWidget = new QWidget(this);
	m_formLayout = new QFormLayout();
	m_formLayout->setLabelAlignment(Qt::AlignRight);
	m_formLayout->setVerticalSpacing(1);
	m_formLayout->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::ExpandingFieldsGrow);
	addWidgets();
	m_formWidget->setLayout(m_formLayout);

	//create vertical layout for full widget
	m_vLayout = new QVBoxLayout();
	m_vLayout->addLayout(m_headerLayout);
	m_vLayout->addWidget(m_formWidget);
	m_vLayout->setAlignment(m_headerLayout, Qt::AlignTop);
	m_vLayout->setAlignment(m_formLayout, Qt::AlignTop);
	m_vLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
	setLayout(m_vLayout);

	connect(m_button.get(), &QAbstractButton::clicked, this, &PageEditForm::onCollapseClicked);
}

PageEditForm::~PageEditForm()
{
}

void PageEditForm::addWidgets()
{
	for (int i = 0; i <= (unsigned int)Widgets::VPadding; ++i)
	{
		labels.emplace_back(std::unique_ptr<QLabel>(new QLabel(labelText[i])));
		labels.back()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

		lineEdits.emplace_back(std::unique_ptr<QLineEdit>(new QLineEdit(this)));
		lineEdits.back()->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred));

		m_formLayout->addRow(labels.back().get(), lineEdits.back().get());
	}
}

/**************************************************************************************************
***********************************Slots***********************************************************
**************************************************************************************************/

void PageEditForm::onCollapseClicked(bool checked)
{
	m_formWidget->setVisible(checked);
	m_button->setText(checked ? "v" : ">");
}