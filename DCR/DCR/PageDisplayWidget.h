#pragma once
#include "ComicPage.h"
#include "ComicPanelLayout.h"
#include "PanelObject.h"
#include "DrawHandler.h"

class PageDisplayWidget : public QWidget
{
	Q_OBJECT
public:
	PageDisplayWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	PageDisplayWidget(const PageDisplayWidget& other);
	virtual ~PageDisplayWidget();

	ComicPage& getComicPage() { return m_comicPage; }
	void setComicPage(const ComicPage& page) { m_comicPage = page; }

signals:
	void signalGraphicPanelCreation(PanelObject* pObj);
	void toggleDrawing();
	void clearEditing();

public slots:
	void handleCtxMenuRequested(const QPoint& pos);
	void launchImageSelectDialog();
	void launchSettingsDialog();
	void addGraphicPanelToLayout(GraphicPanel* gp);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void enterEvent(QEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;

private:
	ComicPanelLayout m_layout;
	ComicPage m_comicPage;
	DrawHandler m_drawHandler;
};

