import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutPage

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    property string issuesText: qsTr("Found bugs? Got some great improvement ideas? Please report them to github and I am happy to look them through :)")
    property string generalAboutText: qsTr("This is a simple trivia-game for Sailfish OS. The game uses Open Trivia Database as the source for the questions. The questions are only in English and this is a limitation in the database.")

    Component.onCompleted: {
        issueLabel.text = Theme.highlightText(issuesText, "github", Theme.highlightColor)
        generalAboutLabel.text = Theme.highlightText(generalAboutText, "Open Trivia Database", Theme.highlightColor)
    }

    SilicaFlickable {
        id: aboutFlick
        anchors.fill: parent

        contentHeight: contentColumn.height

        // Place page content to a column.
        Column {
            id: contentColumn

            width: aboutPage.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("About")
            }

            SectionHeader {
                text: qsTr("Info")
            }

            Label {
                id: generalAboutLabel
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium - Theme.paddingMedium
                textFormat: Text.AutoText
                x: Theme.paddingMedium
            }

            Label {
                text: qsTr("Version ") + appVersion
                x: Theme.paddingMedium
            }

            SectionHeader {
                text: qsTr("By who")
            }

            Label {
                text: qsTr("Joni Korhonen, also known as pinniini")
                x: Theme.paddingMedium
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
            }

            SectionHeader {
                text: qsTr("License")
            }

            Label {
                text: "MIT"
                font.underline: true
                x: Theme.paddingMedium
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://opensource.org/licenses/MIT")
                }
            }

            SectionHeader {
                text: qsTr("Code")
            }

            Label {
                text: "https://github.com/pinniini/harbour-sailTrivia"
                font.underline: true
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
                x: Theme.paddingMedium
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://github.com/pinniini/harbour-sailTrivia")
                }
            }

            SectionHeader {
                text: qsTr("Questions API")
            }

            Label {
                text: "https://opentdb.com/"
                font.underline: true
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
                x: Theme.paddingMedium
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://opentdb.com/")
                }
            }

            SectionHeader {
                text: qsTr("Credits")
            }

            Label {
                id: applicationIcon
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
                x: Theme.paddingMedium
                text: Theme.highlightText(qsTr("Application icon: JSEHV"), "JSEHV", Theme.highlightColor)

                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://github.com/JSEHV")
                }
            }

            SectionHeader {
                text: qsTr("Ideas/Issues/Want to help")
            }

            Label {
                id: issueLabel
                textFormat: Text.AutoText
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium - Theme.paddingMedium
                x: Theme.paddingMedium

                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://github.com/pinniini/harbour-sailTrivia/issues")
                }
            }
        }
    }
}
