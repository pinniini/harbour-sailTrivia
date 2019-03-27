import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0

import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: Component { FirstPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.All

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
