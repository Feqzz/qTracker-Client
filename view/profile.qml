import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as C

Rectangle {
    id: root
    //width: 1366
    //height: 768
    anchors.fill: parent
    color: "#141414"

    C.NavBar {
        id: navBar
    }

    Column {
        id: column1
        spacing: 16
        anchors.horizontalCenterOffset: -460
        //anchors.verticalCenterOffset: -200
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.verticalCenter: parent.verticalCenter
        y: 300

        Label {
            id: emailLabel
            color: "#ffffff"
            text: "E-mail: " + getEmail()
        }

        Label {
            id: usernameLabel
            color: "#ffffff"
            text: "Username: " + getUsername()
        }

        Label {
            id: dateJoinedLabel
            color: "#ffffff"
            text: "Date Joined: " + getDateJoined().substr(0,10);
        }

        Label {
            id: rankLabel
            color: "#ffffff"
            text: "Rank: " + getRank()
        }

        Label {
            id: invitedLabel
            color: "#ffffff"
            text: "Invited: " + getInvitedCount()
        }

        Row {
            id: row
            spacing: 32

            C.PushButton {
                id: changePasswordButton
                text: "Change Password"
                onClicked: root.state = "changePasswordState"
            }

            C.PushButton {
                id: logoutButton
                text: "Log Out"
                onClicked: logout()
            }
        }
    }



    Column {
        id: column
        x: 583
        y: 328
        visible: false
        anchors.verticalCenterOffset: -100
        anchors.horizontalCenter: navBar.horizontalCenter
        anchors.verticalCenter: navBar.verticalCenter
        spacing: 32

        TextField {
            id: oldPasswordField
            text: qsTr("")
            placeholderText: "Old Password"
            visible: true
            echoMode: "Password"
            selectByMouse: true
        }

        TextField {
            id: newPasswordField
            text: qsTr("")
            placeholderText: "New Password"
            visible: true
            echoMode: "Password"
            selectByMouse: true
        }

        TextField {
            id: confirmPasswordField
            text: qsTr("")
            placeholderText: "Confirm Password"
            visible: true
            echoMode: "Password"
            selectByMouse: true
        }

        C.PushButton {
            id: submitChangeButton
            text: "Submit Changes"
            anchors.horizontalCenterOffset: 32
            anchors.horizontalCenter: confirmPasswordField.horizontalCenter
            visible: true
            onClicked: changePassword(oldPasswordField.text, newPasswordField.text, confirmPasswordField.text, message)
        }

        Label {
            id: message
            text: " ";
            width: 240
            height: 22

        }

    }



    states: [
        State {
            name: "changePasswordState"

            PropertyChanges {
                target: oldPasswordField
                visible: true
            }

            PropertyChanges {
                target: newPasswordField
                visible: true
            }

            PropertyChanges {
                target: confirmPasswordField
                visible: true
            }

            PropertyChanges {
                target: submitChangeButton
                visible: true
            }

            PropertyChanges {
                target: emailLabel
                visible: false
            }

            PropertyChanges {
                target: column1
                visible: false
            }

            PropertyChanges {
                target: changePasswordButton
                visible: false
            }

            PropertyChanges {
                target: column
                visible: true
            }
        }
    ]
}
