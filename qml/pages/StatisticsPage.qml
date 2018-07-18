import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: statisticsPage

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: statisticsFlick
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        contentHeight: contentColumn.height

        // Place page content to a column.
        Column {
            id: contentColumn

            width: statisticsPage.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Statistics")
            }
        }
    }
}
