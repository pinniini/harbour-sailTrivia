import QtQuick 2.0
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations: Orientation.All
    anchors.fill: parent

    Image {
        id: bg
        source: "../images/splash_bg.png"
        anchors.fill: parent
        smooth: true
        visible: false
    }

    ColorOverlay {
        anchors.fill: bg
        source: bg
        color: Theme.highlightColor
    }

    Label {
//        anchors.centerIn: parent
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 10
        }

        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeExtraLarge
        color: Theme.highlightColor
        text: qsTr("Loading initial data...")
    }
}
