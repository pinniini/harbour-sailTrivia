import QtQuick 2.0
import Sailfish.Silica 1.0

import fi.pinniini.sailTrivia 1.0

Page {
    id: page

    property int currentCategoryId: -1
    property bool categoriesLoaded: false
    property string currentDifficulty: ""

    Component.onCompleted: {
        var req = new XMLHttpRequest();
        req.onreadystatechange = function() {
            if (req.readyState === 4 && req.status === 200) {
                var json = JSON.parse(req.responseText);

                // Add categories to model.
                var count = json.trivia_categories.length;
                for(var i = 0; i < count; ++i) {
                    var cat = json.trivia_categories[i];
                    var item = { itemId: cat.id, itemName: cat.name };
                    categoryModel.append(item)
                }

                categoriesLoaded = true;

                // Test dataloader.
                dataLoader.loadCategories();
            }
        };
        req.open("GET", "https://opentdb.com/api_category.php", true);
        req.send();
    }

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    DataLoader {
        id: dataLoader

        onCategoriesLoaded: {
            console.log("DataLoader: categories loaded...");
            console.log(categoriesData);
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            busy: categoriesLoaded
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
                        model: categoryModel
                        MenuItem { text: itemName; onClicked: currentCategoryId = itemId}
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
                        onClicked: currentDifficulty = ""
                    }
                    MenuItem {
                        text: qsTr("Easy")
                        onClicked: currentDifficulty = "easy"
                    }
                    MenuItem {
                        text: qsTr("Medium")
                        onClicked: currentDifficulty = "medium"
                    }
                    MenuItem {
                        text: qsTr("Hard")
                        onClicked: currentDifficulty = "hard"
                    }
                }
            }

            // Start the game.
            Button {
                id: startButton
                text: qsTr("Start")
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: console.log("Questions: " + questionCountSlider.value + ", Category id/name: " + currentCategoryId + "/" + categoryCombo.value + ", Difficulty: " + currentDifficulty)
            }
        }
    }

    ListModel {
        id: categoryModel

        ListElement {
            itemId: -1
            itemName: "All"
        }
    }
}
