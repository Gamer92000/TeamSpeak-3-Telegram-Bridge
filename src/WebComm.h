#pragma once

#pragma comment (lib, "Qt5Core")
#pragma comment (lib, "Qt5Network")
#include <string>
#include <ctime>
#include <QtCore/QUrl>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <mutex>
#include <condition_variable>
#include "config.h"
#include "update.h"
#include "ts3_functions.h"

class Communicator : public QObject {
	Q_OBJECT
public:
	explicit Communicator(QObject* parent = 0);
	~Communicator();
	void setConfigPtr(config* conf);
	void setFunctionPtr(TS3Functions* functions);
	void sendGreetings();
	void sendMessage(const char* message, const char* uuid, uint64 serverConnectionHandlerID, bool save);
	void readThread();
	void setAnyID(anyID id);
	void checkForUpdate(update*);
	bool running;
	std::mutex IDlockMutex;
	std::condition_variable IDRequestCV;

private:
	int offset;
	QMap<int, QPair<std::string, int>>* messages = new QMap<int, QPair<std::string, int>>();
	std::time_t last = NULL;
	config* conf = NULL;
	TS3Functions* ts3Functions = NULL;
	QNetworkAccessManager* manager;
	anyID ID;
	void startRequest(const char* requestedUrl, std::string uuid, uint64 serverConnectionHandlerID, bool save);
};