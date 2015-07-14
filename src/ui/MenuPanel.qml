import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Rectangle {
    Text {
        id: text1
        x: 8
        y: 8
        text: qsTr("Text")
        font.pixelSize: 12
    }
}

/*ListView {
    id: shareList
    width: 400
    height: 400
    spacing: 40
    anchors.fill: parent

    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    focus: true

    model: ListModel {
        ListElement {
            name: "News"
        }
        ListElement {
            name: "Inbox"
        }
        ListElement {
            name: "My shares"
        }
    }

    delegate: Item {
        height: menuLabel.lineHeight
        width: parent.width
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                shareList.currentIndex = index;
            }
        }

        Text {
            id: menuLabel
            text: ">" + name
            //height: 15
            anchors.left: parent.left
            anchors.leftMargin: 10
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

        }
    }

}*/
