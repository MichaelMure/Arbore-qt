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
                height: 23
                anchors.top: parent.top
                anchors.topMargin: 82
                anchors.right: parent.right
                anchors.rightMargin: -292
                anchors.left: parent.left
                anchors.leftMargin: 8
                value: share.progress
            }

            Text {
                id: title
                height: 29
                text: share.title
                anchors.right: parent.right
                anchors.rightMargin: -292
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.top: parent.top
                anchors.topMargin: 8
            }

            Text {
                id: text1
                height: 33
                text: share.description
                anchors.top: parent.top
                anchors.topMargin: 43
                anchors.right: parent.right
                anchors.rightMargin: -292
                anchors.left: parent.left
                anchors.leftMargin: 8
            }

            CheckBox {
                id: starred
                x: 8
                y: 108
                text: qsTr("Starred")
                onClicked: share.starred = checked
                Component.onCompleted: checked = share.starred
                Connections {
                    target: share
                    onShareChanged: starred.checked = share.starred
                }
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
