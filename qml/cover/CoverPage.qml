import QtQuick 2.0
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    CoverPlaceholder {
        id: placeholder
        icon.source: "harbour-sailtrivia.png"
        text: "SailTrivia"
    }

    onStatusChanged: {
        if (status == Cover.Active || status == Cover.Activating) {
            var question = questionModel.getCurrentQuestion();
            if (question) {
                categoryLabel.text = question.category;
                difficultyLabel.text = question.difficulty;
                questionLabel.text = question.question;

                placeholder.visible = false;
            }
        }
        else {
            categoryLabel.text = "";
            difficultyLabel.text = "";
            questionLabel.text = "";

            placeholder.visible = true;
        }
    }

    Column {
        id: coverColumn
        anchors.fill: parent
        spacing: Theme.paddingLarge
        anchors.margins: Theme.paddingMedium

        Label {
            id: categoryLabel
            width: parent.width
            elide: Text.ElideRight
            text: ""
            color: Theme.highlightColor
        }

        Label {
            id: difficultyLabel
            width: parent.width
            elide: Text.ElideRight
            text: ""
            color: Theme.highlightColor
        }

        Label {
            id: questionLabel
            width: parent.width
            height: parent.height - categoryLabel.height - difficultyLabel.height - parent.spacing - parent.spacing - Theme.paddingMedium - Theme.paddingSmall
            elide: Text.ElideRight
            text: ""
            maximumLineCount: 3
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            textFormat: Text.RichText
            clip: true
        }
    }
}
