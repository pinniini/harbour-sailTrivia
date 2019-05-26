import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0

import "cover"
import "pages"
import fi.pinniini.sailTrivia 1.0

ApplicationWindow
{
    id: appWindow
    initialPage: Component { SplashPage { } }
//    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    cover: Component { CoverPage { } }
    allowedOrientations: Orientation.All

    DataLoader {
        id: testDataLoader

        onInitialDataLoaded: {
            if (initialTimer.running) {
                initialTimer.stop();
            }

            pageStack.replace(Qt.resolvedUrl("pages/FirstPage.qml"), { 'dataLoader': testDataLoader, 'initialCategoriesData': categoriesData, 'questionModel': questionModel })
        }
    }

    QuestionModel {
        id: questionModel
    }

    Notification {
        id: notification
        body: ""
        previewBody: ""
        expireTimeout: 3000
        maxContentLines: 5

        function showMessage(visibleIcon, message) {
            if (visibleIcon && visibleIcon !== "") {
                icon = visibleIcon
            }

            if (body !== message) {
                body = message
                previewBody = message
            }
            publish()
        }
    }

    Timer {
        id: initialTimer
        interval: 10000
        repeat: false
        running: true

        onTriggered: {
            testDataLoader.stopInitialLoading();
            pageStack.replace(Qt.resolvedUrl("pages/FirstPage.qml"), { 'dataLoader': testDataLoader, 'initialCategoriesData': "", 'questionModel': questionModel })
        }
    }
}
