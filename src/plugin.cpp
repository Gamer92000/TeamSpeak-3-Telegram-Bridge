#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#pragma warning(disable : 4100) /* Disable Unreferenced parameter warning */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <thread>
#include <future>
#include <iostream>
#include <sstream>

#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"

#include "plugin.h"
#include "definitions.h"
#include "helper.hpp"
#include "config.h"
#include "update.h"
#include "WebComm.h"

static struct TS3Functions ts3Functions;
static char* pluginID = NULL;
config* configObject;
update* upd;
Communicator* comm;
std::thread run;

/*-------------------------- Configure Here --------------------------*/
/*
 * The following functions should be configured to your needs.
 */
int ts3plugin_init()
{
	char configPath[PATH_BUFSIZE];
	ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	configObject = new config(QString::fromUtf8(configPath) + CONFIG_FILE);

	comm = new Communicator();
	comm->setConfigPtr(configObject);
	comm->setFunctionPtr(&ts3Functions);

	run = std::thread(&Communicator::readThread, comm);

	if(configObject->getConfigOption("greetings").toBool()) comm->sendGreetings();

	upd = new update(configObject);
	if (configObject->getConfigOption("updateCheck").toBool()) comm->checkForUpdate(upd);

	return 0;
}

void ts3plugin_shutdown()
{
	if (comm)
	{
		comm->running = false;
		run.join();
		delete comm;
	}
	if (configObject)
	{
		configObject->close();
		delete configObject;
		configObject = NULL;
	}
	if(pluginID)
	{
		free(pluginID);
		pluginID = NULL;
	}
	if (upd)
	{
		upd->close();
		delete upd;
		upd = NULL;
	}
}

enum
{
	MENU_ID_GLOBAL_SETTINGS = 1,
	MENU_ID_GLOBAL_UPDATE
};

void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon)
{
	BEGIN_CREATE_MENUS(2);  /* IMPORTANT: Number of menu items must be correct! */
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_SETTINGS, "Settings", "Settings.svg");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_UPDATE, "Check for Updates", "Update.svg");
	END_CREATE_MENUS;  /* Includes an assert checking if the number of menu items matched */

	*menuIcon = (char*)malloc(PLUGIN_MENU_BUFSZ * sizeof(char));
	_strcpy(*menuIcon, PLUGIN_MENU_BUFSZ, "Telegram.svg");

}

void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID)
{
	switch (type)
	{
	case PLUGIN_MENU_TYPE_GLOBAL:
		switch (menuItemID)
		{
		case MENU_ID_GLOBAL_SETTINGS:
			if (configObject->isVisible())
			{
				configObject->raise();
				configObject->activateWindow();
			}
			else configObject->show();
			break;
		case MENU_ID_GLOBAL_UPDATE:
			comm->checkForUpdate(upd);
			break;
		}
		break;
	}
}

/*-------------------------- DON'T TOUCH --------------------------*/
/*
 * Those functions are setup nicely and
 * should be configured using the definitions.hpp file.
 */
const char *ts3plugin_name()
{
	return PLUGIN_NAME;
}

const char *ts3plugin_version()
{
	return PLUGIN_VERSION;
}

int ts3plugin_apiVersion()
{
	return PLUGIN_API_VERSION;
}

const char *ts3plugin_author()
{
	return PLUGIN_AUTHOR;
}

const char *ts3plugin_description()
{
	return PLUGIN_DESCRIPTION;
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs)
{
	ts3Functions = funcs;
}

int ts3plugin_offersConfigure()
{
	return PLUGIN_OFFERS_CONFIGURE_QT_THREAD;
}

void ts3plugin_configure(void* handle, void* qParentWidget)
{
	if (configObject->isVisible())
	{
		configObject->raise();
		configObject->activateWindow();
	}
	else
	{
		configObject->show();
	}
}

void ts3plugin_registerPluginID(const char* id)
{
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	_strcpy(pluginID, sz, id);
}

void ts3plugin_freeMemory(void* data)
{
	free(data);
}

/*-------------------------- OTHER STUFF --------------------------*/

void ts3plugin_onClientIDsEvent(uint64 serverConnectionHandlerID, const char* uniqueClientIdentifier, anyID clientID, const char* clientName)
{
	std::unique_lock<std::mutex> lck(comm->IDlockMutex);
	comm->setAnyID(clientID);
}

void ts3plugin_onClientIDsFinishedEvent(uint64 serverConnectionHandlerID)
{
	printf("?\n");
	comm->IDRequestCV.notify_one();
}

int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored)
{
	anyID me;
	ts3Functions.getClientID(serverConnectionHandlerID, &me);
	if (me == fromID && configObject->getConfigOption("ignoreSelf").toBool())
	{
		return 0;
	}

	bool send = 0;
	int muted;
	int muted2;
	int sound;
	int afk;
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_INPUT_MUTED, &muted);
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_INPUT_DEACTIVATED, &muted2);
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_OUTPUT_MUTED, &sound);
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_AWAY, &afk);

	if ((targetMode == TextMessageTarget_CLIENT &&
		(configObject->getConfigOption("privateMute").toBool() && (muted || muted2) ||
		configObject->getConfigOption("privateSound").toBool() && sound ||
		configObject->getConfigOption("privateAFK").toBool() && afk ||
		configObject->getConfigOption("privateAlways").toBool()) &&
		configObject->getConfigOption("privateEnabled").toBool()) ||
		(targetMode == TextMessageTarget_CHANNEL &&
		(configObject->getConfigOption("channelMute").toBool() && (muted || muted2) ||
		configObject->getConfigOption("channelSound").toBool() && sound ||
		configObject->getConfigOption("channelAFK").toBool() && afk ||
		configObject->getConfigOption("channelAlways").toBool()) &&
		configObject->getConfigOption("channelEnabled").toBool()) ||
		(targetMode == TextMessageTarget_SERVER &&
		(configObject->getConfigOption("serverMute").toBool() && (muted || muted2) ||
		configObject->getConfigOption("serverSound").toBool() && sound ||
		configObject->getConfigOption("serverAFK").toBool() && afk ||
		configObject->getConfigOption("serverAlways").toBool()) &&
		configObject->getConfigOption("serverEnabled").toBool()))
	{
		MessageType type = MessageType::MESSAGE_SERVER;
		if (targetMode == TextMessageTarget_CLIENT)
			type = MessageType::MESSAGE_PRIVATE;
		else if (targetMode == TextMessageTarget_CHANNEL)
			type = MessageType::MESSAGE_CHANNEL;

		auto msg = prepareMessage(type, fromName, message);

		comm->sendMessage(msg.c_str(), fromUniqueIdentifier, serverConnectionHandlerID, true);
	}

	return 0;
}

int ts3plugin_onClientPokeEvent(
	uint64 serverConnectionHandlerID,
	anyID fromClientID,
	const char* pokerName,
	const char* pokerUniqueIdentity,
	const char* message,
	int ffIgnored)
{
	anyID me;
	ts3Functions.getClientID(serverConnectionHandlerID, &me);
	if (me == fromClientID && configObject->getConfigOption("ignoreSelf").toBool())
	{
		return 0;
	}
	if (!configObject->getConfigOption("pokeEnabled").toBool())
	{
		return 0;
	}

	bool send = 0;
	int muted;
	int muted2;
	int sound;
	int afk;
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_INPUT_MUTED, &muted);
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_INPUT_DEACTIVATED, &muted2);
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_OUTPUT_MUTED, &sound);
	ts3Functions.getClientSelfVariableAsInt(serverConnectionHandlerID, CLIENT_AWAY, &afk);

	if (configObject->getConfigOption("pokeMute").toBool() && (muted || muted2) ||
		configObject->getConfigOption("pokeSound").toBool() && sound ||
		configObject->getConfigOption("pokeAFK").toBool() && afk ||
		configObject->getConfigOption("pokeAlways").toBool())
	{

		auto msg = prepareMessage(MessageType::MESSAGE_POKE, pokerName, message);
		comm->sendMessage(msg.c_str(), pokerUniqueIdentity, serverConnectionHandlerID, true);
	}
	
	return 0; 
}