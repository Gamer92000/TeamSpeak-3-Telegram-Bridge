#pragma comment (lib, "Qt5Core")

#include "WebComm.h"
#include <iostream>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QEventLoop>
#include <QtCore/QTextCodec>
#include "definitions.h"
#include "teamspeak/public_errors.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <Windows.h>

Communicator::Communicator(QObject* parent) :
    QObject(parent)
{
}

Communicator::~Communicator()
{
    delete messages;
}

void Communicator::setConfigPtr(config* con)
{
    conf = con;
}

void Communicator::setFunctionPtr(TS3Functions* functions)
{
    ts3Functions = functions;
}

void Communicator::sendGreetings()
{
    std::ostringstream greetings;
    greetings << "Telegram Bridge\nwas successfully initiated!";
    #if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
    TCHAR infoBuf[100];
    DWORD bufCharCount = 100;
    if (GetComputerName(infoBuf, &bufCharCount))
    {
        char szString[100];
        size_t nNumCharConverted;
        wcstombs_s(&nNumCharConverted, szString, 100, (wchar_t*)infoBuf, 100);
        greetings << "\n   Host System: " << szString;
    }
    #endif
    sendMessage(greetings.str().c_str(), "", 0, false);
}

void Communicator::sendMessage(const char* message, const char* uuid, uint64 serverConnectionHandlerID, bool save)
{
    if ((conf->getConfigOption("integratedBot").toBool() || !conf->getConfigOption("floodProtection").toBool()) && last != NULL && std::time(nullptr) - last < 3) return;
    std::ostringstream request;
    if (conf->getConfigOption("integratedBot").toBool()) request << "https://telegrambridgebot.julianimhof.de";
    else request << "https://api.telegram.org/bot" << qPrintable(conf->getConfigOption("botToken").toString().trimmed());

    request << "/sendMessage?parse_mode=HTML&chat_id=" << qPrintable(conf->getConfigOption("chatID").toString().trimmed()) << "&text=" << qPrintable(QUrl::toPercentEncoding(message, "", "lgtamp&"));
    this->startRequest(request.str().c_str(), std::string(uuid), serverConnectionHandlerID, true);
    last = std::time(nullptr);
}

void Communicator::readThread()
{
    running = true;
    int offset = 0;

    QNetworkAccessManager* qnam = new QNetworkAccessManager();
    QEventLoop* loop = new QEventLoop();

    while (running) {
        for (int i = 0; running && i < 1000; i++) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if(running && conf->getConfigOption("responses").toBool() && !conf->getConfigOption("integratedBot").toBool() && !conf->getConfigOption("botToken").toString().trimmed().isEmpty()){
            std::ostringstream updateStr;
            updateStr << "https://api.telegram.org/bot" << qPrintable(conf->getConfigOption("botToken").toString().trimmed()) << "/getUpdates?offset=" << offset + 1;
            if (!running) break;
            QNetworkReply* reply = qnam->get(QNetworkRequest(QUrl(updateStr.str().c_str())));
            QObject::connect(reply, SIGNAL(finished()), loop, SLOT(quit()));
            if (!running) break;
            loop->exec();
            while (!reply->isFinished() && running) {}
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject data = document.object();
            QJsonArray result = data["result"].toArray();
            if (result.isEmpty() || !running) continue;
            QJsonObject update = result.first().toObject();
            offset = update["update_id"].toInt();
            QJsonObject message = update["message"].toObject();
            if (message["reply_to_message"].isUndefined()) continue;
            QJsonObject resopnse = message["reply_to_message"].toObject();
            int msg_id = resopnse["message_id"].toInt();
            if (!messages->contains(msg_id)) continue;
            char* msg = (char*) message["text"].toString().toLocal8Bit().constData();
            if (!running) break;
            std::ostringstream text;
            text << "via TelegramBridge:\n" << msg;
            ID = -1;
            if (ts3Functions->requestClientIDs((*messages)[msg_id].second, (*messages)[msg_id].first.c_str(), NULL) == ERROR_ok) {
                std::unique_lock<std::mutex> lk(IDlockMutex);
                IDRequestCV.wait(lk);
                if (ID != -1) ts3Functions->requestSendPrivateTextMsg((*messages)[msg_id].second, text.str().c_str(), ID, NULL);
            }
        }
    }
    delete qnam;
    delete loop;
}

void Communicator::setAnyID(anyID id)
{
    ID = id;
}

void Communicator::checkForUpdate(update* upd)
{
    manager = new QNetworkAccessManager();
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(UPDATE_URL)));
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            ts3Functions->logMessage("TBridge: Unable to pull version information.", LogLevel_WARNING, "PLUGIN", NULL);
            return;
        }
        
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( reply->readAll(), &jsonError );
        if (jsonError.error != QJsonParseError::NoError)
        {
            ts3Functions->logMessage("TBridge: Unable to pull version information.", LogLevel_WARNING, "PLUGIN", NULL);
            return;
        }

        QJsonObject data = document.object();

        QString version = data["tag_name"].toString().trimmed();
        // remove v from version
        version.remove(0, 1);
        if (!QString::compare(version, PLUGIN_VERSION))
        {
            ts3Functions->logMessage("TBridge: No Update found!", LogLevel_INFO, "PLUGIN", NULL);
            return;
        }

        QString downloadLink = data["html_url"].toString().trimmed();

        ts3Functions->logMessage(("TBridge: New version found! Download at " + downloadLink.toStdString()).c_str(), LogLevel_INFO, "PLUGIN", NULL);
        upd->setText(PLUGIN_VERSION, version);
        upd->show();
    });
}

void Communicator::startRequest(const char* requestedUrl, std::string uuid, uint64 serverConnectionHandlerID, bool save)
{
    manager = new QNetworkAccessManager();
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(requestedUrl)));
    connect(reply, &QNetworkReply::finished, [=]()
    {
        std::ostringstream response;
        response << reply->readAll().constData();
        QJsonDocument document = QJsonDocument::fromJson(response.str().c_str());
        QJsonObject data = document.object();
        QJsonObject resp = data["result"].toObject();
        int id = resp["message_id"].toInt();
        if(strcmp(uuid.c_str(), "") != 0 && save && id != 0)
        {
            messages->insert(id, QPair<std::string, int>(uuid, serverConnectionHandlerID));
        }
        reply->deleteLater();
    });
}
