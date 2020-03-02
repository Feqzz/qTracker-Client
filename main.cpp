#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include "core/securesocket.h"
#include "model/user.h"
#include "handlers/loginHandler.h"
#include "handlers/inviteHandler.h"
#include "handlers/texthandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QScopedPointer<SecureSocket> secureSocket(new SecureSocket);
    User sessionUser;
        QScopedPointer<User> user(&sessionUser);
    QScopedPointer<InviteHandler> inviteHandler(new InviteHandler());
    QScopedPointer<LoginHandler> loginHandler(new LoginHandler());
    QScopedPointer<TextHandler> textHandler(new TextHandler());

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/view/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("secureSocket", secureSocket.data());
    engine.rootContext()->setContextProperty("user", user.data());
    engine.rootContext()->setContextProperty("loginHandler", loginHandler.data());
    engine.rootContext()->setContextProperty("inviteHandler", inviteHandler.data());
    engine.rootContext()->setContextProperty("textHandler", textHandler.data());

    engine.load(url);

    //

    return app.exec();
}
