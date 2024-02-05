#pragma once

#include <QUrl>
#include <iostream>
#include "definitions.h"
#include "bbcode.hpp"

// -------------------------- TeamSpeak Internal --------------------------

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src)  \
	{                                   \
		strncpy(dest, src, destSize - 1); \
		(dest)[destSize - 1] = '\0';      \
	}
#endif

/* Helper function to create a menu item */
static struct PluginMenuItem *createMenuItem(enum PluginMenuType type, int id, const char *text, const char *icon)
{
	struct PluginMenuItem *menuItem = (struct PluginMenuItem *)malloc(sizeof(struct PluginMenuItem));
	menuItem->type = type;
	menuItem->id = id;
	_strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
	_strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
	return menuItem;
}

/* Some makros to make the code to create menu items a bit more readable */
#define BEGIN_CREATE_MENUS(x) \
	const size_t sz = x + 1;    \
	size_t n = 0;               \
	*menuItems = (struct PluginMenuItem **)malloc(sizeof(struct PluginMenuItem *) * sz);
#define CREATE_MENU_ITEM(a, b, c, d) (*menuItems)[n++] = createMenuItem(a, b, c, d);
#define END_CREATE_MENUS    \
	(*menuItems)[n++] = NULL; \
	assert(n == sz);

// -------------------------- Custom --------------------------

enum MessageType
{
	MESSAGE_POKE = 0,
	MESSAGE_PRIVATE = 1,
	MESSAGE_CHANNEL = 2,
	MESSAGE_SERVER = 3
};

static std::string prepareMessage(MessageType type, const char *source, const char *message)
{
	std::string msg = "`";

	switch (type)
	{
	case MESSAGE_POKE:
		msg += "You were poked";
		break;
	case MESSAGE_PRIVATE:
		msg += "You received a private message";
		break;
	case MESSAGE_CHANNEL:
		msg += "You received a channel message";
		break;
	case MESSAGE_SERVER:
		msg += "You received a server message";
		break;
	default:
		// Should never happen
		msg += "You received a message";
		break;
	}

	msg += "`\n*_" + std::string(telegramEscape(source)) + "_*\n" + parseBbcode(message);

	QByteArray encoded = QUrl::toPercentEncoding(QString::fromStdString( msg ), "", "");
	return encoded.toStdString();
}