#pragma once
#include <QtWidgets>

class PageEditForm : public QWidget
{
	enum class Widgets : unsigned int
	{
		Name,
		Index,
		File,
		HPadding,
		VPadding
	};

	const std::vector<QString> labelText =
	{
		"Name",
		"Index",
		"Source File",
		"Horizontal Padding",
		"Vertical Padding"
	};

public:
	PageEditForm();
	~PageEditForm();

public slots:
	void onCollapseClicked(bool checked);

private:
	void addWidgets();

	QHBoxLayout* m_headerLayout = nullptr;
	QVBoxLayout* m_vLayout = nullptr;
	QFormLayout* m_formLayout = nullptr;
	QWidget* m_formWidget = nullptr;
	std::unique_ptr<QPushButton> m_button;
	std::unique_ptr<QLabel> m_headerLabel;
	std::vector<std::unique_ptr<QLabel>> labels;
	std::vector<std::unique_ptr<QLineEdit>> lineEdits;
};

