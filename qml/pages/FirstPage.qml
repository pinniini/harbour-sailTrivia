import QtQuick 2.0
import Sailfish.Silica 1.0

import fi.pinniini.sailTrivia 1.0

Page {
    id: page

    property int currentCategoryId: -1
    property bool categoriesLoaded: false
    property int currentDifficulty: Difficulty.All

    Component.onCompleted: {
        dataLoader.loadCategories();
    }

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    DataLoader {
        id: dataLoader

        // Handle categories load.
        onCategoriesLoaded: {
            categoriesModel.fillModel(categoriesData);
            page.categoriesLoaded = true;
        }

        // Handle questions load.
        onQuestionsLoaded: {
            questionModel.fillModel(questionData);
        }

        // Handle invalid parameters.
        onInvalidParameters: {
            console.log(errorMessage);
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            busy: dataLoader.loading
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Statistics")
                onClicked: pageStack.push(Qt.resolvedUrl("StatisticsPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("SailTrivia")
            }

            // Choose question count.
            Slider {
                id: questionCountSlider
                anchors.left: parent.left
                anchors.right: parent.right
                width: parent.width// - Theme.horizontalPageMargin
                minimumValue: 1
                maximumValue: 50
                stepSize: 1
                value: 10
                valueText: qsTr("Questions: ") + value
            }

            // Choose category.
            ComboBox {
                id: categoryCombo
                anchors.left: parent.left
                anchors.right: parent.right
                width: parent.width
                label: qsTr("Category:")
                enabled: categoriesLoaded

                menu: ContextMenu {
                    Repeater {
                        model: categoriesModel
                        MenuItem { text: name; onClicked: currentCategoryId = id}
                    }
                }
            }

            // Choose difficulty.
            ComboBox {
                id: difficultyCombo
                anchors.left: parent.left
                anchors.right: parent.right
                width: parent.width
                label: qsTr("Difficulty:")

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Any")
                        onClicked: currentDifficulty = Difficulty.All
                    }
                    MenuItem {
                        text: qsTr("Easy")
                        onClicked: currentDifficulty = Difficulty.Easy
                    }
                    MenuItem {
                        text: qsTr("Medium")
                        onClicked: currentDifficulty = Difficulty.Medium
                    }
                    MenuItem {
                        text: qsTr("Hard")
                        onClicked: currentDifficulty = Difficulty.Hard
                    }
                }
            }

            // Start the game.
            Button {
                id: startButton
                text: qsTr("Start")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: !dataLoader.loading

                onClicked: {
                    console.log("Questions: " + questionCountSlider.value + ", Category id/name: " + currentCategoryId + "/" + categoryCombo.value + ", Difficulty: " + currentDifficulty)
                    dataLoader.loadQuestions(questionCountSlider.value, currentCategoryId, currentDifficulty);
                }
            }

            // This is here just to remind how the QuestionModel could be used.
//            Repeater {
//                model: questionModel
//                ComboBox {
//                    label: question
//                    menu: ContextMenu {
//                        Repeater {
//                            model: answers
//                            MenuItem {
//                                text: modelData
//                            }
//                        }
//                    }
//                }
//            }
        }
    }

    CategoryModel {
        id: categoriesModel
    }

    QuestionModel {
        id: questionModel
    }
}
