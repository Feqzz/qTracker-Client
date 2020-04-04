import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as C

Rectangle {
    id: root
    //width: 1920
    //height: 1080
    anchors.fill: parent
    border.color: "#4b4b4b"

    C.NavBar {
        id: navBar
    }

    ScrollView {
        id: flick
        clip: true
        x: 250
        y: 300
        width: 1500
        height: 700
        contentWidth: -1
        //anchors.verticalCenterOffset: 50
        //anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        TextEdit {
            id: textEdit
            width: flick.width

            color: "#ffffff"
            text: ""
            selectionColor: "#f3d539"

            font.pixelSize: 24
            readOnly: true
            selectByMouse: true
            wrapMode: TextEdit.Wrap
            focus: true
        }
    }

    Row {
        x: 0
        y: 0
        spacing: 32
        anchors.horizontalCenterOffset: 600
        anchors.verticalCenterOffset: 400
        anchors.verticalCenter: flick.verticalCenter
        anchors.horizontalCenter: flick.horizontalCenter

        C.PushButton {
            id: editButton
            visible: checkPrivilege() ? true : false
            text: "Edit"
            onClicked: {
                textEdit.readOnly = false
            }
        }

        C.PushButton {
            id: saveButton
            visible: checkPrivilege() ? true : false
            text: "Save"
            onClicked: {
                textEdit.readOnly = true
                updateText(textEdit.text, 1)
            }
        }
    }
    Component.onCompleted: getText(textEdit, 1)
}
