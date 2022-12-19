#include "config.h"
#include "ui_config.h"

config::config(const QString& configLocation, QWidget* parent /* = nullptr */) : QDialog(parent),
	m_ui(std::make_unique<Ui::configui>()),
	m_settings(std::make_unique<QSettings>(configLocation, QSettings::IniFormat, this))
{
	m_ui->setupUi(this);

	setWindowTitle("Telegram Bridge :: Config");

	// Connect UI Elements.
	connect(m_ui->pbOk, &QPushButton::clicked, this, &config::saveSettings);
	connect(m_ui->pbCancel, &QPushButton::clicked, this, &QWidget::close);
	// Show off some Lambda connect

	connect(m_ui->pokeEnable, &QCheckBox::toggled, this, [&](bool checked) {
		m_ui->poke1->setEnabled(checked);
		m_ui->poke2->setEnabled(checked);
		m_ui->poke3->setEnabled(checked);
		m_ui->poke4->setEnabled(checked);
	});
	connect(m_ui->privateEnable, &QCheckBox::toggled, this, [&](bool checked) {
		m_ui->priv1->setEnabled(checked);
		m_ui->priv2->setEnabled(checked);
		m_ui->priv3->setEnabled(checked);
		m_ui->priv4->setEnabled(checked);
		});
	connect(m_ui->channelEnable, &QCheckBox::toggled, this, [&](bool checked) {
		m_ui->channel1->setEnabled(checked);
		m_ui->channel2->setEnabled(checked);
		m_ui->channel3->setEnabled(checked);
		m_ui->channel4->setEnabled(checked);
		});
	connect(m_ui->serverEnable, &QCheckBox::toggled, this, [&](bool checked) {
		m_ui->server1->setEnabled(checked);
		m_ui->server2->setEnabled(checked);
		m_ui->server3->setEnabled(checked);
		m_ui->server4->setEnabled(checked);
		});
	connect(m_ui->integratedBot, &QCheckBox::toggled, this, [&](bool checked) {
		m_ui->BotTokenInput->setEnabled(!checked);
		m_ui->responses->setEnabled(!checked);
		m_ui->floodProtection->setEnabled(!checked);
		});
	adjustSize();
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

config::~config() {
	m_settings->sync();
}


void config::setConfigOption(const QString& option, const QVariant& value) {
	m_settings->setValue(option, value);
}

QVariant config::getConfigOption(const QString& option) const {
	return m_settings->value(option);
}

void config::showEvent(QShowEvent* /* e */) {
	adjustSize();
	loadSettings();
}

void config::changeEvent(QEvent* e) {
	if (e->type() == QEvent::StyleChange && isVisible()) {
		m_ui->verticalLayoutWidget_2->adjustSize();
		adjustSize();
	}
}

void config::saveSettings() {
	setConfigOption("botToken", m_ui->BotTokenInput->text());
	setConfigOption("chatID", m_ui->ChatIDInput->text());
	setConfigOption("greetings", m_ui->greetings->isChecked());
	setConfigOption("ignoreSelf", m_ui->ignoreSelf->isChecked());
	setConfigOption("responses", m_ui->responses->isChecked());
	setConfigOption("integratedBot", m_ui->integratedBot->isChecked());

	setConfigOption("pokeEnabled", m_ui->pokeEnable->isChecked());
	setConfigOption("pokeAlways", m_ui->poke1->isChecked());
	setConfigOption("pokeMute", m_ui->poke2->isChecked());
	setConfigOption("pokeSound", m_ui->poke3->isChecked());
	setConfigOption("pokeAFK", m_ui->poke4->isChecked());

	setConfigOption("privateEnabled", m_ui->privateEnable->isChecked());
	setConfigOption("privateAlways", m_ui->priv1->isChecked());
	setConfigOption("privateMute", m_ui->priv2->isChecked());
	setConfigOption("privateSound", m_ui->priv3->isChecked());
	setConfigOption("privateAFK", m_ui->priv4->isChecked());

	setConfigOption("channelEnabled", m_ui->channelEnable->isChecked());
	setConfigOption("channelAlways", m_ui->channel1->isChecked());
	setConfigOption("channelMute", m_ui->channel2->isChecked());
	setConfigOption("channelSound", m_ui->channel3->isChecked());
	setConfigOption("channelAFK", m_ui->channel4->isChecked());

	setConfigOption("serverEnabled", m_ui->serverEnable->isChecked());
	setConfigOption("serverAlways", m_ui->server1->isChecked());
	setConfigOption("serverMute", m_ui->server2->isChecked());
	setConfigOption("serverSound", m_ui->server3->isChecked());
	setConfigOption("serverAFK", m_ui->server4->isChecked());

	setConfigOption("updateCheck", m_ui->updateCheck->isChecked());
	setConfigOption("floodProtection", m_ui->floodProtection->isChecked());

	close();
}

void config::loadSettings() {
	m_ui->BotTokenInput->setText(getConfigOption("botToken").toString());
	m_ui->ChatIDInput->setText(getConfigOption("chatID").toString());
	m_ui->greetings->setChecked(getConfigOption("greetings").toBool());
	m_ui->ignoreSelf->setChecked(getConfigOption("ignoreSelf").toBool());
	m_ui->responses->setChecked(getConfigOption("responses").toBool());
	m_ui->integratedBot->setChecked(getConfigOption("integratedBot").toBool());
	
	m_ui->pokeEnable->setChecked(getConfigOption("pokeEnabled").toBool());
	m_ui->poke1->setChecked(getConfigOption("pokeAlways").toBool());
	m_ui->poke2->setChecked(getConfigOption("pokeMute").toBool());
	m_ui->poke3->setChecked(getConfigOption("pokeSound").toBool());
	m_ui->poke4->setChecked(getConfigOption("pokeAFK").toBool());

	m_ui->privateEnable->setChecked(getConfigOption("privateEnabled").toBool());
	m_ui->priv1->setChecked(getConfigOption("privateAlways").toBool());
	m_ui->priv2->setChecked(getConfigOption("privateMute").toBool());
	m_ui->priv3->setChecked(getConfigOption("privateSound").toBool());
	m_ui->priv4->setChecked(getConfigOption("privateAFK").toBool());

	m_ui->channelEnable->setChecked(getConfigOption("channelEnabled").toBool());
	m_ui->channel1->setChecked(getConfigOption("channelAlways").toBool());
	m_ui->channel2->setChecked(getConfigOption("channelMute").toBool());
	m_ui->channel3->setChecked(getConfigOption("channelSound").toBool());
	m_ui->channel4->setChecked(getConfigOption("channelAFK").toBool());

	m_ui->serverEnable->setChecked(getConfigOption("serverEnabled").toBool());
	m_ui->server1->setChecked(getConfigOption("serverAlways").toBool());
	m_ui->server2->setChecked(getConfigOption("serverMute").toBool());
	m_ui->server3->setChecked(getConfigOption("serverSound").toBool());
	m_ui->server4->setChecked(getConfigOption("serverAFK").toBool());

	m_ui->updateCheck->setChecked(getConfigOption("updateCheck").toBool());
	m_ui->floodProtection->setChecked(getConfigOption("floodProtection").toBool());

	// Manually set enabled state of elements, as the event doesn't fire if the state is the same as it was already (ie. unchecked).
	m_ui->poke1->setEnabled(m_ui->pokeEnable->isChecked());
	m_ui->poke2->setEnabled(m_ui->pokeEnable->isChecked());
	m_ui->poke3->setEnabled(m_ui->pokeEnable->isChecked());
	m_ui->poke4->setEnabled(m_ui->pokeEnable->isChecked());

	m_ui->priv1->setEnabled(m_ui->privateEnable->isChecked());
	m_ui->priv2->setEnabled(m_ui->privateEnable->isChecked());
	m_ui->priv3->setEnabled(m_ui->privateEnable->isChecked());
	m_ui->priv4->setEnabled(m_ui->privateEnable->isChecked());

	m_ui->channel1->setEnabled(m_ui->channelEnable->isChecked());
	m_ui->channel2->setEnabled(m_ui->channelEnable->isChecked());
	m_ui->channel3->setEnabled(m_ui->channelEnable->isChecked());
	m_ui->channel4->setEnabled(m_ui->channelEnable->isChecked());

	m_ui->server1->setEnabled(m_ui->serverEnable->isChecked());
	m_ui->server2->setEnabled(m_ui->serverEnable->isChecked());
	m_ui->server3->setEnabled(m_ui->serverEnable->isChecked());
	m_ui->server4->setEnabled(m_ui->serverEnable->isChecked());

	m_ui->BotTokenInput->setEnabled(!m_ui->integratedBot->isChecked());
	m_ui->responses->setEnabled(!m_ui->integratedBot->isChecked());
	m_ui->floodProtection->setEnabled(!m_ui->integratedBot->isChecked());
}