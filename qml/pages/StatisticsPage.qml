import QtQuick 2.0
import Sailfish.Silica 1.0
import "../js/statistics.js" as Stats

Page {
    id: statisticsPage

//    property int gamesPlayed: 0
//    property int totalCount: 0
//    property int correctCount: 0
//    property int incorrectCount: 0
//    property int skippedCount: 0

    Component.onCompleted: {
//        var statistics = Stats.getStatistics()
//        if (statistics !== false) {
//            for (var i = 0; i < statistics.rows.length; ++i) {
//                var sta = statistics.rows[i]

//                if (sta.key === 'gamesPlayed') {
//                    gamesPlayed = sta.numericValue
//                }
//                if (sta.key === 'totalCount') {
//                    totalCount = sta.numericValue
//                }
//                else if (sta.key === 'correctCount') {
//                    correctCount = sta.numericValue
//                }
//                if (sta.key === 'incorrectCount') {
//                    incorrectCount = sta.numericValue
//                }
//                if (sta.key === 'skippedCount') {
//                    skippedCount = sta.numericValue
//                }
//            }
//        }
    }

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

            DetailItem {
                label: qsTr("Games started:")
                value: statistics.getNumericStat("gamesStarted")
            }

            DetailItem {
                label: qsTr("Games played:")
                value: statistics.getNumericStat('gamesPlayed') //gamesPlayed
            }

            DetailItem {
                label: qsTr("Games aborted:")
                value: statistics.getNumericStat("gamesAborted")
            }

            DetailItem {
                label: qsTr("Questions asked:")
                value: statistics.getNumericStat('totalCount') //totalCount
            }

            DetailItem {
                label: qsTr("Correct answers:")
                value: statistics.getNumericStat('correctCount') //correctCount
            }

            DetailItem {
                label: qsTr("Incorrect answers:")
                value: statistics.getNumericStat('incorrectCount') //incorrectCount
            }

            DetailItem {
                label: qsTr("Skipped questions:")
                value: statistics.getNumericStat('skippedCount') //skippedCount
            }
        }
    }
}
