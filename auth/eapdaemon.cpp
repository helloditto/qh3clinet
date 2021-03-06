#include "eapdaemon.h"
#include <QDebug>
#include <QtDBus/QtDBus>
#include <thread>

EAPDaemon::EAPDaemon(QObject *parent) :
    QObject(parent), hasLogin(false)
{
    QDBusConnection connection = QDBusConnection::systemBus();
    connection.registerService("com.qh3client.EAPDaemon");
    connection.registerObject("/", this);
}

void EAPDaemon::Login(const QVariantMap &userinfo) {
    if (!userinfo.contains("username")
            || !userinfo.contains("password")
            || !userinfo.contains("interface")) {
        emit Message("Invalid Argument");
        return;
    }

    qDebug() << Q_FUNC_INFO << " " << "[" << userinfo["username"].toString() << "]";

    try {
        if (authservice) authservice->logoff();
        authservice.reset(new EAPAuth(userinfo["username"].toString().toStdString(),
                userinfo["password"].toString().toStdString(),
                userinfo["interface"].toString().toStdString()));
        authservice->set_promote_listener([this] (const std::string& msg) {
            QString repmsg = QString::fromStdString(msg).trimmed();
            if (!repmsg.isEmpty())
                emit Message(repmsg);
        });

        authservice->set_status_listener([this] (int8_t statno) {
            emit Status(statno);
            qDebug() << "Status: " << QString::fromStdString(strstat(statno));
            switch (statno) {
            case EAPAUTH_EAP_SUCCESS:
                hasLogin = true;
                if (auththread.get() != nullptr) auththread->reset_autoretry();
                break;
            case EAPAUTH_EAP_FAILURE:
                hasLogin = false;
                break;
            }
        });
        if (auththread.get() != nullptr)
            auththread->stop();
        auththread.reset(new AuthThread(authservice, this));
        auththread->start();
    }
    catch (const EAPAuthException& exp) {
        qDebug() << Q_FUNC_INFO << " " << exp.what();
        emit Message(exp.what());
    }
}

void EAPDaemon::Logoff() {
    if (authservice.get() != nullptr) {
        authservice->logoff();
        authservice.reset();
        hasLogin = false;
    }
}

bool EAPDaemon::HasLogin() {
    qDebug() << Q_FUNC_INFO << " " << hasLogin;
    return hasLogin;
}

QString EAPDaemon::LoginUser() {
    if (HasLogin() && authservice)
        return QString::fromStdString(authservice->get_user_name());
    else
        return QString();
}

AuthThread::AuthThread(std::shared_ptr<EAPAuth> authservice, EAPDaemon *eapdaemon)
    : authservice(authservice), autoretry_count(5), eapdaemon(eapdaemon) {
}

void AuthThread::run() {
    while (autoretry_count --) {
        try {
            authservice->auth();
        }
        catch (const EAPAuthFailed& expt) {
            qDebug() << Q_FUNC_INFO << "Authentication failed";
            if (!eapdaemon->HasLogin())
                break;
        }
        catch (const EAPAuthException& expt) {
            qDebug() << Q_FUNC_INFO << " " << expt.what();
            if (!eapdaemon->HasLogin())
                break;
        }
        emit eapdaemon->Status(EAPAUTH_AUTH_AUTORETRY);
        QThread::sleep(2);
    }
    emit eapdaemon->Status(EAPAUTH_EAP_FAILURE);
}

void AuthThread::reset_autoretry() {
    autoretry_count = 5;
}

void AuthThread::stop() {
    authservice->logoff();
    this->terminate();
    this->wait();
}
