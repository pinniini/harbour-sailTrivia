#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include "dataloader.h"
#include "difficulty.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> a(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qmlRegisterType<DataLoader>("fi.pinniini.sailTrivia", 1, 0, "DataLoader");
    //qmlRegisterType<Difficulty>("fi.pinniini.sailTrivia", 1, 0, "Difficulty");
    qmlRegisterUncreatableType<Difficulty>("fi.pinniini.sailTrivia", 1, 0, "Difficulty", "I'm here just for the enums. You cannot make me be an object.");

    view->rootContext()->setContextProperty("appVersion", APP_VERSION);
    view->setSource(SailfishApp::pathToMainQml());
    view->show();
    return a->exec();

    // SailfishApp::main() will display "qml/harbour-sailTrivia.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    //    return SailfishApp::main(argc, argv);
}
