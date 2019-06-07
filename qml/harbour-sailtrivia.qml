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
    cover: Component { CoverPage { } }
    allowedOrientations: Orientation.All

    DataLoader {
        id: testDataLoader
    }

    Connections {
        id: dataConnections
        target: testDataLoader
        ignoreUnknownSignals: true

        onInitialDataLoaded: {
            console.log("Initial data loaded, go to main page...");

            if (initialTimer.running) {
                initialTimer.stop();
            }

            dataConnections.target = null;
            pageStack.replace(Qt.resolvedUrl("pages/FirstPage.qml"), { 'dataLoader': testDataLoader, 'initialCategoriesData': categoriesData, 'questionModel': questionModel })
        }

        onDataLodingErrorOccured: {
            console.log(errorMessage)
            notification.showMessage("image://theme/icon-system-warning", errorMessage)

            if (initialTimer.running) {
                initialTimer.stop();
            }

            dataConnections.target = null;
            pageStack.replace(Qt.resolvedUrl("pages/FirstPage.qml"), { 'dataLoader': testDataLoader, 'initialCategoriesData': "", 'questionModel': questionModel })
        }

        onSessionTokenLoadingError: {
            console.log(errorMessage)
            notification.showMessage("image://theme/icon-system-warning", errorMessage)
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
