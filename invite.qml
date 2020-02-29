import QtQuick 2.12
import QtQuick.Controls 2.3

Rectangle {
    id: root
    width: 1366
    height: 768
    color: "#141414"

    NavBar{
        id: navBar
    }

    Column {
        id: column
        x: 583
        y: 166
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 32

        TextField {
            id: emailField
            text: qsTr("")
            placeholderText: "Email"
        }

        PushButton {
            id: inviteButton
            text: "Invite"
            anchors.horizontalCenter: emailField.horizontalCenter
            onClicked: {
                generateInvite(emailField.text, inviteKeyLabel);

        }

        Label {
            id: inviteKeyLabel
            color: "#ffffff"
            text: ""
            anchors.horizontalCenter: inviteButton.horizontalCenter
        }
    }
}



