#pragma once
#include <QtWidgets/QDialog>
#pragma comment (lib, "Qt5Widgets")
#pragma comment (lib, "Qt5Core")
#include <QtCore/QSettings>
#include <memory>
#include "config.h"

namespace Ui {
	class updateui;
}

class update : public QDialog
{
	Q_OBJECT
public:
	update(config* , QWidget* = nullptr);
	~update();
	update(const update&) = delete;
	update& operator=(const update&) = delete;
	void setText(QString, QString, QString);

protected:
	void showEvent(QShowEvent* e) override;
	void changeEvent(QEvent* e) override;

private:
	void setConfigPtr(config*);
	std::unique_ptr<Ui::updateui> m_ui;
	config* conf;
};