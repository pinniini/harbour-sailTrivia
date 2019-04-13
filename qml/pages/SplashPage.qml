import QtQuick 2.0
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations: Orientation.Portrait
    anchors.fill: parent
    clip: true

    Image {
        id: bg
        source: "../images/splash_bg.png"
        anchors.centerIn: parent
        width: (parent.width < parent.height ? parent.width : parent.height) - Theme.paddingLarge - Theme.paddingLarge
        height: width
        smooth: true
        visible: false
    }

    ColorOverlay {
        anchors.fill: bg
        source: bg
        color: Theme.highlightColor
    }

    Label {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 10
        }

        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeExtraLarge
        color: Theme.highlightColor
        text: qsTr("Loading...")
    }
}
