import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0

import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: Component { SplashPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.All

    // This should be here only for testing purposes
    Timer {
        id: tempTimer
        interval: 2000
        repeat: false
        running: true

        onTriggered: {
            pageStack.replace(Qt.resolvedUrl("pages/FirstPage.qml"))
        }
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
}
