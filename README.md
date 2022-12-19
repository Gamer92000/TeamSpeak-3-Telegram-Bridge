# 🔗 Telegram Bridge

<div align="center">

  [![GitHub license](https://img.shields.io/github/license/Gamer92000/TeamSpeak-3-Telegram-Bridge.svg)](https://github.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/blob/main/LICENSE)
  [![GitHub commits](https://badgen.net/github/commits/Gamer92000/TeamSpeak-3-Telegram-Bridge/main)](https://GitHub.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/commit/)
  [![Github stars](https://img.shields.io/github/stars/Gamer92000/TeamSpeak-3-Telegram-Bridge.svg)](https://GitHub.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/stargazers/)
  [![Renovate](https://img.shields.io/badge/maintained%20with-renovate-brightgreen)](https://github.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/issues/4)
  <!-- [![Deploy](https://img.shields.io/github/actions/workflow/status/Gamer92000/TeamSpeak-3-Telegram-Bridge/deploy.yml?branch=*)](https://github.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/actions/workflows/deploy.yml) -->
  <!-- https://github.com/badges/shields/issues/8736 -->
  <br>
  <h3>If you like this project, please consider giving it a star ⭐️!</h3>
</div>

## 📖 Description

The Telegram Bridge for TeamSpeak 3 redirects messages from the TeamSpeak 3 client to a Telegram chat and allows you to send answers back. Supported message types are server messages, channel messages, private messages and pokes.

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## 🚀 Usage

### 📦 Requirements

To use this plugin you need the following:

* [A TeamSpeak 3 client](https://teamspeak.com/en/downloads/)
* [A Telegram Account](https://telegram.org/)
* ([A Telegram Bot](https://core.telegram.org/bots#6-botfather)) for now this is optional, but not using your own bot will result in some limitations
### 📥 Installation

Go to [releases](https://github.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/releases) and download the latest version of the plugin. Then simply execute the `.ts3_plugin` file by double-clicking it. The plugin will automatically be installed and activated. If this doesn't work, you can also install the plugin manually by renaming the `.ts3_plugin` file to a `.zip` file and extracting it to the `AppData` folder of your TeamSpeak 3 client (usually `%AppData%\TS3Client\`).
Before you can use the plugin, you need to configure it. See the [Configuration](#-configuration) section for more information.
### 📝 Configuration

After installing the plugin, you can configure it by opening the plugin settings in the TeamSpeak 3 client. Do so by clicking on `Plugins` in the Menubar, then `Telegram Bridge` and finally `Settings`. You can also open the configuration by clicking on `Settings` in the plugin list in the Client Options.

You will need to enter your Telegram Bot Token and your Telegram Chat ID. You can get your Telegram Bot Token by talking to the [BotFather](https://t.me/BotFather) on Telegram. You can get your Telegram Chat ID by talking to the [ChatIDBot](https://t.me/chatid_echo_bot) on Telegram. You can also use the [Telegram API](https://core.telegram.org/bots/api#getting-updates) to get your Chat ID. For more information on how to get the Chat ID of a group, see [this](https://stackoverflow.com/a/32572159/1320237) StackOverflow answer.

For now you can also use the default Bot Token and Chat ID, but this will result in some limitations. See the [Limitations](#-limitations) section for more information. In the future, this will not be possible anymore.

It is necessary to give the bot access to your chat, before either of the two options will work. You can do this by sending a message to the bot in the chat you want to use. You can also use the `/start` command to start the bot.

### 🚧 Limitations

When using the default Bot Token and Chat ID, you will be limited to the following:
* **As the author of this plugin I am technically able to read all messages sent to the bot. I will not do this, but I cannot guarantee that I will not do this in the future. If you want to be sure that I will not read your messages, you should use your own bot.**
* I can not guarantee that the bot will be online 24/7
* You will not be able to respond to messages from Telegram
* You will not be able to disable the rate limit

## ⚠️ Disclaimer

This plugin is not supported by TeamSpeak Systems GmbH. It is provided as-is and without any warranty. You are free to use it as a starting point for your own plugins, but you are responsible for any issues that may arise from using it.

<!-- Bugs and Feature Requests -->
## 🐛 Bugs and Feature Requests

If you encounter any bugs or have a feature request, feel free to open an [issue](https://github.com/Gamer92000/TeamSpeak-3-Telegram-Bridge/issues/new). Please make sure to include as much information as possible. This includes the version of the plugin you are using, the version of the TeamSpeak 3 client you are using and the operating system you are using.

## 📝 Contributing

If you want to contribute to this project, feel free to open a pull request. I will try to review it as soon as possible.

## 📜 Changelog

See the [CHANGELOG](CHANGELOG.md) file for more information.