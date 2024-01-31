#pragma once

#define PLUGIN_API_VERSION 26

// this needs to be unique for each plugin
#define CONFIG_FILE "TelegramBridge.ini"

#define PLUGIN_NAME "Telegram Bridge"
#define PLUGIN_VERSION "<version>"
#define PLUGIN_AUTHOR "JUL14N"
#define PLUGIN_DESCRIPTION "This plugin adds Telegram - TeamSpeak cross communication.";

#define UPDATE_URL "https://api.github.com/repos/Gamer92000/TeamSpeak-3-Telegram-Bridge/releases/latest"

/*-------------------------- INTERNAL DEFINITIONS --------------------------*/
#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128
