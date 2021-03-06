import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.0
import "../component" as C

Rectangle {
    id: root
    //width: 1920
    //height: 1080
    anchors.fill: parent
    color: "#141414"

    C.NavBar{
        id: navBar
    }

    Column {
        id: column
        //anchors.verticalCenter: navBar.verticalCenter
        anchors.horizontalCenter: navBar.horizontalCenter
        y: 300
        spacing: 32

        Rectangle {
            id: infoRectangle
            width: 720
            height: 128
            color: "#303030"
            radius: 16
            border.width: 0

            Text {
                id: infoText

                width: 680
                height: 70
                anchors.verticalCenter: infoRectangle.verticalCenter
                anchors.horizontalCenter: infoRectangle.horizontalCenter
                color: "#ffffff"
                text: qsTr("Please use your personal announce URL below when creating \n a new torrent.")
                font.pixelSize: 24
            }
        }

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: shortcuts.home
            visible: false;
            onAccepted:function(){
                uploadHandler.setId(user.getId())
                uploadHandler.setUrl(fileDialog.fileUrl)
                uploadUrlText.text = fileDialog.fileUrl.toString().split('/').pop();
            }
            onRejected: {
                console.log("Canceled")
            }
        }

        Rectangle {
            id: announceUrlRectangle
            width: 720
            height: 72
            color: "#303030"
            radius: 16
            border.width: 0

            TextEdit {
                id: torrentPassLabel
                width: 680
                height: 36
                anchors.verticalCenter: announceUrlRectangle.verticalCenter
                anchors.horizontalCenter: announceUrlRectangle.horizontalCenter
                color: "#ffffff"
                text: "http://www.feqzz.no:1337/" + user.getTorrentPass() + "/announce"
                selectByKeyboard: true
                selectByMouse: true
                selectedTextColor: "#d0d046"
                readOnly: true
                font.pixelSize: 20
            }
        }

        Rectangle {
            id: torrentUploadRectangle
            width: 720
            height: 72
            color: "#303030"
            radius: 16
            border.width: 0

            Text {
                id: uploadText
                anchors.verticalCenter: torrentUploadRectangle.verticalCenter
                anchors.horizontalCenter: torrentUploadRectangle.horizontalCenter
                anchors.horizontalCenterOffset: -300
                text: "File"
                color: "#ffffff"
                font.pixelSize: 24
            }

            C.PushButton {
                id: uploadButton
                anchors.verticalCenter: torrentUploadRectangle.verticalCenter
                anchors.horizontalCenter: torrentUploadRectangle.horizontalCenter
                anchors.horizontalCenterOffset: -200
                text: "Browse..."
                onClicked: fileDialog.visible = true;
            }

            Text {
                id: uploadUrlText
                anchors.verticalCenter: torrentUploadRectangle.verticalCenter
                x: 250
                text: "No file selected."
                color: "#ffffff"
            }
        }

        Rectangle {
            id: titleRectangle

            width: 720
            height: 256
            color: "#303030"
            radius: 16
            border.width: 0

            Text {
                id: titleText
                anchors.verticalCenter: titleRectangle.verticalCenter
                anchors.horizontalCenter: titleRectangle.horizontalCenter
                anchors.verticalCenterOffset: -64
                color: "#ffffff"
                text: qsTr("Please enter a title")
                font.pixelSize: 24
            }

            TextField {
                id: titleTextField
                anchors.verticalCenter: titleRectangle.verticalCenter
                anchors.horizontalCenter: titleRectangle.horizontalCenter
                anchors.verticalCenterOffset: -8
                width: 512
                height: 40
                text: ""
                placeholderText: "Title"
                font.pointSize: 12
            }

            C.PushButton {
                id: confirmUploadButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 54
                text: "Upload"
                onClicked: function(){
                    if (uploadUrlText.text != "No file selected.") {
                        if (titleTextField.text != "") {
                            uploadHandler.setTitle(titleTextField.text)
                            uploadHandler.setTorrentPass(user.getTorrentPass())
                            var success = uploadHandler.uploadFile()
                            console.log(success)
                            if (success) {
                                errorText.color = "#00e940"
                                errorText.text = "Successfully uploaded the torrent. Remember to seed!"
                            } else {
                                errorText.color = "#e90000"
                                errorText.text = "Failed to upload file"
                            }
                        } else {
                            errorText.color = "#e90000"
                            errorText.text = "Please enter a title"
                        }
                    } else {
                        errorText.color = "#e90000"
                        errorText.text = "Please select a file"
                    }
                }
            }

            Text {
                id: errorText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 96
                text: ""
                color: "#e90000"
                font.pointSize: 16
            }
        }
    }

    Component.onCompleted: {
        setTitle("qTracker :: Upload");
    }
}
