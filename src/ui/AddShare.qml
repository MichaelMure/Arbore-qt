import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.3

Item {
    width: 300
    height: 400

    Rectangle {
        color: "#ffffff"
        border.width: 2
        anchors.fill: parent

        ColumnLayout {
            id: columnLayout1
            spacing: 6
            anchors.fill: parent

            Rectangle {
                id: header
                height: 40
                border.width: 2
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                Image {
                    id: header_icon
                    anchors.verticalCenterOffset: 0
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    sourceSize.height: 24
                    sourceSize.width: 24
                    source: "images/contactIcon.png"
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    id: text1
                    text: qsTr("New Share")
                    anchors.leftMargin: 10
                    anchors.left: header_icon.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 17
                }

            }
            TextField {
                id: title
                text: ""
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                placeholderText: qsTr("Title")
                Layout.fillWidth: true
            }

            TextAreaPlaceholder {
                id: description
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                Layout.fillWidth: true
            }


            ListView {
                id: listView1
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 100

                delegate: Item {
                    x: 5
                    width: 80
                    height: 40
                    Row {
                        id: row1

                        Text {
                            text: name
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                        }
                        spacing: 10
                    }
                }
                model: ListModel {
                    ListElement {
                        name: "Grey"
                        colorCode: "grey"
                    }

                    ListElement {
                        name: "Red"
                        colorCode: "red"
                    }

                }

                Button {
                    id: add
                    width: 20
                    height: 20
                    text: qsTr("+")
                    anchors.right: remove.left
                    anchors.rightMargin: 0
                    anchors.top: parent.bottom
                    anchors.topMargin: 0
                }

                Button {
                    id: remove
                    width: 20
                    height: 20
                    text: qsTr("-")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.top: parent.bottom
                    anchors.topMargin: 0
                }
            }
            Button {
                id: button1
                text: qsTr("Share")
                anchors.right: parent.right
                anchors.rightMargin: 8
            }
        }

    }
}

