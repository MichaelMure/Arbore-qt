import QtQuick 2.0
import QtQuick.Controls 1.2
import Arbore 1.0

ScrollView {
    id: shareDetails
    width: 300
    height: 600

    property Share share

    Rectangle {
        id: panel
        color: "#ffffff"
        anchors.fill: parent

        Rectangle {
            id: header
            height: 123
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            ProgressBar {
                id: progressBar1
                x: 8
                y: 50
                width: 284
                height: 23
                value: share.progress
            }

            Text {
                id: name
                x: 0
                y: 123
                height: 24
                text: share.name
                anchors.right: parent.right
                anchors.rightMargin: 47
                anchors.left: parent.left
                anchors.leftMargin: 47
                anchors.top: header.bottom
                anchors.topMargin: -103
                font.pixelSize: 12
            }
        }

        Text {
            id: content
            text: share.textual_arborescence
            anchors.top: header.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
    }
}
