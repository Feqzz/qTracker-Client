import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as Components

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#141414"

    Components.NavBar{
        id: navBar
    }

    property var invitePrice: 1000

    Column {
        id: column
        anchors.verticalCenter: navBar.verticalCenter
        anchors.horizontalCenter: navBar.horizontalCenter
        anchors.verticalCenterOffset: 0
        spacing: 32

        Rectangle {
            id: infoRectangle
            width: 720
            height: 180
            color: "#303030"
            radius: 16
            border.width: 0

            Text {
                id: infoText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: -32
                text: "Each invite costs 1,000 Bonus Points."
                color: "white"
                font.pixelSize: 24
            }

            Text {
                id: ruleBreakInfoText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 16
                text: "You have full responsibility for every rule your invitee breaks"
                color: "white"
                font.pixelSize: 24
            }
        }

        Rectangle {
            id: inviteRectangle
            width: 720
            height: 320
            color: "#303030"
            radius: 16
            border.width: 0

            Text {
                id: inviteInfoText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: -64
                text: "Please enter your friend's Email Address"
                color: "white"
                font.pixelSize: 24
            }

            TextField {
                id: emailField
                width: 512
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 0
                text: qsTr("")
                placeholderText: "Email"
                selectByMouse: true
            }

            Components.PushButton {
                id: inviteButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 64
                text: "Invite"
               // anchors.horizontalCenter: emailField.horizontalCenter
                onClicked: {
                    if (user.getPoints() - invitePrice >= 0) {
                        generateInvite(emailField.text, inviteKeyLabel, invitePrice);
                        updateUsersData();
                        navBar.update();
                    } else {
                        inviteKeyLabel.color = "#e90000"
                        inviteKeyLabel.text = "Insufficient funds"
                    }
                }
            }

            Label {
                id: inviteKeyLabel
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 120
                text: ""
                font.pixelSize: 24
                //anchors.horizontalCenter: inviteButton.horizontalCenter
            }
        }
    }
}
