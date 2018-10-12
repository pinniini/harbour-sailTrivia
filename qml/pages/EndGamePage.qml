import QtQuick 2.0
import Sailfish.Silica 1.0

import "../js/statistics.js" as Stats

Page {
    id: endGamePage
    clip: true
    allowedOrientations: Orientation.All

    property int totalCount: 10
    property int correctCount: 1
    property int incorrectCount: 9
    property int skippedCount: 0

    Component.onCompleted: {
        var gp = 1
        var tc = totalCount
        var cc = correctCount
        var ic = incorrectCount
        var sc = skippedCount

        // Get already existing statistics.
        var stats = Stats.getStatistics()
        if (stats !== false) {
            for (var i = 0; i < stats.rows.length; ++i) {
                var sta = stats.rows[i]

                if (sta.key === "gamesPlayed") {
                    gp = sta.numericValue + 1
                }
                else if (sta.key === "totalCount") {
                    tc = sta.numericValue + totalCount
                }
                else if (sta.key === "correctCount") {
                    cc = sta.numericValue + correctCount
                }
                else if (sta.key === "incorrectCount") {
                    ic = sta.numericValue + incorrectCount
                }
                else if (sta.key === "skippedCount") {
                    sc = sta.numericValue + skippedCount
                }
            }
        }

        Stats.upsertStatistic("gamesPlayed", gp, gp.toString())
        Stats.upsertStatistic("totalCount", tc, tc.toString())
        Stats.upsertStatistic("correctCount", cc, cc.toString())
        Stats.upsertStatistic("incorrectCount", ic, ic.toString())
        Stats.upsertStatistic("skippedCount", sc, sc.toString())
    }

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

            DetailItem {
                label: qsTr("Skipped answers:")
                value: skippedCount
            }
        }
    }
}
