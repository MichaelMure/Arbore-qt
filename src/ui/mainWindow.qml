import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Arbore 1.0

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 1047
    height: 480

    property ShareList list: list
    property ShareDetails details: details

    Rectangle {
        id: topBar
        height: 40
        color: "#a51e9e"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Button {
            id: share
            text: qsTr("Share")
            iconSource: "plus.png"
            anchors.left: parent.left;
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
        }

        Button {
            id: download
            text: qsTr("Download")
            anchors.left: share.right;
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5

            onClicked: dlfromhash.visible = true
        }

        Image {
            id: arboreLogo
            width: 100
            fillMode: Image.PreserveAspectFit
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            source: "arbore.png"
        }
    }

    SplitView {
        id: splitView1
        anchors.topMargin: 40
        anchors.fill: parent
        orientation: Qt.Horizontal

        MenuPanel {
            id: menu
            Layout.minimumWidth: 200
        }

        ShareList {
            id: list
            app: applicationWindow
            Layout.minimumWidth: 400
        }

        ShareDetails {
            id: details
            Layout.minimumWidth: 400
        }
    }

    DownloadFromHash {
        id: dlfromhash
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
    }
}
