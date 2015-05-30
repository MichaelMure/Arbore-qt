import QtQuick 2.0
import QtQuick.Controls 1.2

ListView {
    id: shareList
    width: 400
    height: 400
    anchors.fill: parent

    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    focus: true

    model: shareModel

    delegate: Item {
        height: 81
        width: parent.width
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                shareList.currentIndex = index;
            }
        }

        Image {
            id: image1
            width: 65
            height: 65
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 8
            fillMode: Image.PreserveAspectFit
            source: "contactIcon.png"
        }

        Text {
            id: text1
            height: 36
            text: name
            wrapMode: Text.WordWrap
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.left: image1.right
            anchors.leftMargin: 8
            font.pixelSize: 12
        }
    }
}
