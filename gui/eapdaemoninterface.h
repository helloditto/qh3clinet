/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -v -c EAPDaemonInterface -p eapdaemoninterface.h:eapdaemoninterface.cpp ../auth/com.qh3client.EAPDaemon.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef EAPDAEMONINTERFACE_H_1368845047
#define EAPDAEMONINTERFACE_H_1368845047

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.qh3client.EAPDaemon
 */
class EAPDaemonInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.qh3client.EAPDaemon"; }

public:
    EAPDaemonInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~EAPDaemonInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<bool> HasLogin()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("HasLogin"), argumentList);
    }

    inline QDBusPendingReply<> Login(const QVariantMap &userinfo)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(userinfo);
        return asyncCallWithArgumentList(QLatin1String("Login"), argumentList);
    }

    inline QDBusPendingReply<QString> LoginUser()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("LoginUser"), argumentList);
    }

    inline QDBusPendingReply<> Logoff()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Logoff"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void Message(const QString &msg);
    void Status(int statno);
};

namespace com {
  namespace qh3client {
    typedef ::EAPDaemonInterface EAPDaemon;
  }
}
#endif