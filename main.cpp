#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QStringListModel>
#include <QList>
#include <QString>


#include "custommodel.h"

using namespace Qt::StringLiterals;


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        [] (const QUrl &url) { QCoreApplication::exit(EXIT_FAILURE); }, Qt::QueuedConnection);

    // model
    CustomModel *mdl = new CustomModel();

    engine.rootContext()->setContextProperty("listModel", mdl);
    engine.rootContext()->setContextObject(mdl);

    engine.loadFromModule(u"Piklema"_s, u"Main"_s);

    return app.exec();
}
