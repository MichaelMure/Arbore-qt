import QtQuick 2.0
import QtQuick.Controls 1.3

Item {
    id: root
    width: 300
    height: 100

    Rectangle {
        id: rectangle1
        color: "#ffffff"
        anchors.fill: parent

        // Block mouse event for underlying object
        MouseArea {
            anchors.fill: parent
            onClicked: { }
        }

        TextField {
            id: hash
            height: 22
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 13
            anchors.top: parent.top
            anchors.topMargin: 20
            placeholderText: qsTr("hash")
        }

        Button {
            id: download
            text: qsTr("Download")
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: 0
            anchors.top: hash.bottom
            anchors.topMargin: 15
            anchors.left: parent.left
            anchors.leftMargin: 20

            onClicked: {
                shareModel.addShare(hash.text)
                root.visible = false
                hash.text = ""
            }
        }

        Button {
            id: cancel
            text: qsTr("Cancel")
            anchors.top: hash.bottom
            anchors.topMargin: 15
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: 0

            onClicked: {
                root.visible = false
                hash.text = ""
            }
        }
    }
}

