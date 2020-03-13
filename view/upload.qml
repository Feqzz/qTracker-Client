import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.0
import "../component" as Components

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    Components.NavBar{
        id: navBar
    }

    Column {
        id: column
        anchors.verticalCenter: navBar.verticalCenter
        anchors.horizontalCenter: navBar.horizontalCenter
        spacing: 32

        /*Label {
            id: text
            color: "#ffffff"
            text: "Here is your upload thingy:"
        }
        TextEdit {
            id: torrentPassLabel
            color: "#ffffff"
            text: "https://www.tarves.no:1337/" + user.getTorrentPass() + "/announce"
            selectByKeyboard: true
            selectByMouse: true
            selectedTextColor: "#d0d046"
            readOnly: true
        }*/
        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: shortcuts.home
            visible: false;
            onAccepted:function(){
                //console.log(fileDialog.fileUrl);
                var bytes = secureSocket.getFileString(fileDialog.fileUrl);
                //console.log(bytes);
                secureSocket.sendMessage(2,[user.getId(),bytes]);
            }
            onRejected: {
                console.log("Canceled")
            }
        }
        Components.PushButton {
            id: uploadButton
            text: "Upload file"
            visible: true
            anchors.verticalCenter: title.verticalCenter
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenter: title.horizontalCenter
            anchors.horizontalCenterOffset: 0
            onClicked: function(){
                fileDialog.visible = true;
            };
        }
    }
}
