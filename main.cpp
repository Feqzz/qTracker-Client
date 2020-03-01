#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include "model/login.h"
#include "core/securesocket.h"
#include "model/invite.h"
#include "model/user.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QScopedPointer<SecureSocket> secureSocket(new SecureSocket);
    User sessionUser;
    QScopedPointer<Login> login(new Login(&sessionUser));
    QScopedPointer<Invite> invite(new Invite);
    QScopedPointer<User> user(&sessionUser);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("login", login.data());
    engine.rootContext()->setContextProperty("secureSocket", secureSocket.data());
    engine.rootContext()->setContextProperty("invite", invite.data());
    engine.rootContext()->setContextProperty("user", user.data());
    engine.load(url);

    //

    return app.exec();
}
