import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: questionPage
    clip: true
    backNavigation: false

    allowedOrientations: Orientation.All

    property int questionNumber: 3
    property int questionCount: 42
    property var questionModel
    property string category: ""
    property string difficulty: ""
    property string question: ""
    property string correctAnswer: ""
    property var answers
    property int correctCount: 0
    property int incorrectCount: 0
    property int skippedCount: 0
    property bool answered: false

//    signal answerCorrect()
//    signal answerIncorrect()
//    signal endGame(int correctCount, int incorrectCount)

    Component.onCompleted: {
        var que = questionModel.get(questionNumber - 1);
        category = que.category;
        difficulty = que.difficulty;
        question = que.question;
        correctAnswer = que.correctAnswer
        answers = que.answers
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("End game")
                onClicked: {
                    console.log("We should end the game...");
                    endGame();
                }
            }
            MenuItem {
                text: qsTr("Skip question")
                onClicked: {
                    console.log("We should skip the question...");
                    ++skippedCount;

                    if (questionNumber === questionCount) {
                        endGame();
                    }
                    else {
                        nextQuestion();
                    }
                }
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: questionPage.width - Theme.paddingLarge - Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Question " + questionNumber + "/" + questionCount)
            }

            Label {
                id: categoryLabel
                text: category //questionModel.get(questionNumber - 1).category
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.highlightColor
            }

            Label {
                id: difficultyLabel
                text: difficulty //questionModel.get(questionNumber - 1).difficulty
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.highlightColor
            }

            Label {
                id: questionLabel
                anchors.left: parent.left
                anchors.right: parent.right
                width: parent.width
                text: question //questionModel.get(questionNumber - 1).question
                textFormat: Text.RichText
                wrapMode: Text.Wrap
            }

            Repeater {
                model: answers //questionModel.get(questionNumber - 1).answers

                Rectangle {
                    anchors.horizontalCenter: column.horizontalCenter
                    radius: 10
                    color: questionPage.answered
                           ? (modelData === questionPage.correctAnswer ? Theme.rgba("#00ff00", Theme.highlightBackgroundOpacity) /*"#F200ff00"*/ : Theme.rgba("#ff0000", Theme.highlightBackgroundOpacity)/*"#F2ff0000"*/)
                           : Theme.rgba("gray", Theme.highlightBackgroundOpacity) //"#808080F2"
                    width: column.width - Theme.paddingMedium - Theme.paddingMedium
                    height: children[0].height + Theme.paddingMedium + Theme.paddingMedium

                    Label {
                        text: modelData
                        //anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: Theme.paddingMedium
                        anchors.left: parent.left
                        anchors.leftMargin: Theme.paddingMedium
                        wrapMode: Text.Wrap
                        textFormat: Text.RichText
                        width: parent.width - Theme.paddingMedium - Theme.paddingMedium
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (questionPage.answered === true)
                            {
                                return;
                            }

                            parent.border.width = 4;
                            questionPage.answered = true;

                            console.log("User answered: " + modelData);
                            console.log(questionNumber + "/" + questionCount);
                            console.log(questionModel.rowCount());
                            console.log(correctAnswer)
                            console.log(pageStack.depth);

                            // Check answer.
                            if (modelData === correctAnswer)
                            {
                                ++correctCount;
                                parent.color = "green";
                            }
                            else
                            {
                                ++incorrectCount;
                                parent.color = "red";
                            }

                            questionChangeTimer.start();
                        }
                    }
                }
            }
        }
    }

    Timer {
        id: questionChangeTimer
        interval: 1000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            console.log("Change question triggered...");

            // Still questions left.
            if (questionNumber != questionCount)
            {
                nextQuestion();
            }
            else // End game.
            {
                endGame();
            }
        }
    }

    function nextQuestion() {
        pageStack.replace(Qt.resolvedUrl("QuestionPage.qml"), {"questionNumber": questionNumber + 1, "questionCount": questionCount, "questionModel": questionModel, "correctCount": correctCount, "incorrectCount": incorrectCount, "skippedCount": skippedCount});
    }

    function endGame() {
        pageStack.replace(Qt.resolvedUrl("EndGamePage.qml"), {"totalCount": questionCount, "correctCount": correctCount, "incorrectCount": incorrectCount, "skippedCount": skippedCount});
    }
}
