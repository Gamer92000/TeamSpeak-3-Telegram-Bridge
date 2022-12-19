#include "update.h"
#include "ui_update.h"

update::update(config* cfg, QWidget* parent) : QDialog(parent),
	m_ui(std::make_unique<Ui::updateui>()) 
{
	setConfigPtr(cfg);
	m_ui->setupUi(this);

	setWindowTitle("Telegram Bridge :: Update");

	setWindowFlags(Qt::SubWindow | Qt::WindowStaysOnTopHint);

	connect(m_ui->ok, &QPushButton::clicked, this, [&]() {
		if(conf) conf->setConfigOption("updateCheck", !m_ui->checkBox->isChecked());
		close();
		});

	adjustSize();
}

update::~update() {
}

void update::setText(QString curr, QString latest) {
	QString str = m_ui->label->text();
	str.replace("%curr%", curr);
	str.replace("%lat%", latest);
	m_ui->label->setText(str);
}

void update::setConfigPtr(config* cfg) {
	conf = cfg;
}

void update::showEvent(QShowEvent* /* e */) {
	adjustSize();
}

void update::changeEvent(QEvent* e) {
	if (e->type() == QEvent::StyleChange && isVisible()) {
		m_ui->verticalLayoutWidget->adjustSize();
		adjustSize();
	}
}