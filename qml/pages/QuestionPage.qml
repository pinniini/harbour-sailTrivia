import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: questionPage
    clip: true
    backNavigation: false

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
    property bool answered: false

    signal answerCorrect()
    signal answerIncorrect()
    signal endGame(int correctCount, int incorrectCount)

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
                onClicked: console.log("We should end the game...")
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: questionPage.width
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
                Button {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    width: parent.width
                    text: modelData

                    onClicked: {
                        if (questionPage.answered === true)
                        {
                            return;
                        }

                        questionPage.answered = true;

                        console.log("User answered: " + modelData);
                        console.log(questionNumber + "/" + questionCount);
                        console.log(questionModel.rowCount());
                        console.log(correctAnswer) //questionModel.get(questionNumber - 1).correctAnswer)
                        console.log(pageStack.depth);

                        if (modelData === correctAnswer) //questionModel.get(questionNumber - 1).correctAnswer)
                        {
                            highlightBackgroundColor = color = "green";
                            ++correctCount;
                            answerCorrect()
                        }
                        else
                        {
                            highlightBackgroundColor = color = "red";
                            ++incorrectCount;
                            answerIncorrect()
                        }

                        questionChangeTimer.start();
                    }
                }
            }
        }
    }

    Timer {
        id: questionChangeTimer
        interval: 2000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            // Still questions left.
            if (questionNumber != questionCount)
            {
                pageStack.replace(Qt.resolvedUrl("QuestionPage.qml"), {"questionNumber": questionNumber + 1, "questionCount": questionCount, "questionModel": questionModel});
            }
            else // End game.
            {
//                backNavigation = true;
                endGame(correctCount, incorrectCount);
                pageStack.pop();
            }
        }
    }
}
