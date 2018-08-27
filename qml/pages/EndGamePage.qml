import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: endGamePage
    clip: true

    property int totalCount: 10
    property int correctCount: 1
    property int incorrectCount: 9
    property int skippedCount: 0

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: endGamePage.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Game over")
            }

            DetailItem {
                label: qsTr("Question count:")
                value: totalCount
            }

            DetailItem {
                label: qsTr("Correct answers:")
                value: correctCount
            }

            DetailItem {
                label: qsTr("Incorrect answers:")
                value: incorrectCount
            }
        }
    }
}
