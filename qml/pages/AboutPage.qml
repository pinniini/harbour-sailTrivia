import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutPage

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

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
                text: qsTr("TODO: basic about text")
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
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
                text: "BSD"
                font.underline: true
                x: Theme.paddingMedium
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("http://choosealicense.com/licenses/bsd-3-clause/")
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
                text: qsTr("Credits")
            }

            Label {
                id: swedishTranslationLabel
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
                x: Theme.paddingMedium
                text: qsTr("TODO: add credits")
            }

            SectionHeader {
                text: qsTr("Ideas/Issues/Want to help")
            }

            Label {
                id: issueLabel
                textFormat: Text.AutoText
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
                x: Theme.paddingMedium

                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://github.com/pinniini/harbour-sailTrivia/issues")
                }
            }

            Label {
                id: translationsLabel
                wrapMode: Text.Wrap
                width: parent.width - Theme.paddingMedium
                x: Theme.paddingMedium

                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://www.transifex.com/pinniini/harbour-sailTrivia")
                }
            }
        }
    }
}
