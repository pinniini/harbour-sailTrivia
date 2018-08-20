#include <QtQuick>
#include <QtQml>
#include <sailfishapp.h>

#include "dataloader.h"
#include "difficulty.h"
#include "categorymodel.h"
#include "questionmodel.h"
#include "question.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> a(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qmlRegisterType<DataLoader>("fi.pinniini.sailTrivia", 1, 0, "DataLoader");
    qmlRegisterUncreatableType<Difficulty>("fi.pinniini.sailTrivia", 1, 0, "Difficulty", "I'm here just for the enums. You cannot make me be an object.");
    qmlRegisterType<CategoryModel>("fi.pinniini.sailTrivia", 1, 0, "CategoryModel");
    qmlRegisterType<QuestionModel>("fi.pinniini.sailTrivia", 1, 0, "QuestionModel");
    qmlRegisterType<Question>("fi.pinniini.sailTrivia", 1, 0, "Question");

    view->rootContext()->setContextProperty("appVersion", APP_VERSION);
    view->setSource(SailfishApp::pathToMainQml());
    view->show();
    return a->exec();
}
