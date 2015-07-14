import QtQuick 2.0
import QtQuick.Controls 1.2
import Arbore 1.0

Rectangle {
    id: shareDetails
    width: 300
    height: 600

    property Share share

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
            id: text1
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

    Rectangle {
        id: content
        color: "#ffffff"
        anchors.top: header.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }
}
