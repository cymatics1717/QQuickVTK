#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qvtkfboitem.hpp"

int main(int argc, char *argv[])
{

#ifdef __linux
    qputenv("MESA_GL_VERSION_OVERRIDE",QString("4.5").toUtf8());
//    qputenv("MESA_GL_VERSION_OVERRIDE",QString("4.0FC").toUtf8());
//    qputenv("QT_QUICK_CONTROLS_STYLE",QString("universal").toUtf8());
//    qputenv("QT_QUICK_CONTROLS_STYLE",QString("Fusion").toUtf8());
//    qputenv("QT_QUICK_CONTROLS_STYLE",QString("Imagine").toUtf8());

#endif //LINUX

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("...");
    app.setOrganizationDomain("...");;
    app.setApplicationName("...");;
//    QString QT_MESSAGE_PATTERN ="[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{message}";
    QString QT_MESSAGE_PATTERN ="[%{time yyyyMMdd h:mm:ss.zzz}-%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{function}:%{line}-%{message}";
    qSetMessagePattern(QT_MESSAGE_PATTERN);

    qmlRegisterType<QVTKFBOItem>("QMLVtk", 1, 0, "VTKScene");

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:main.qml"));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
