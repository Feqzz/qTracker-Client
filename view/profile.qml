import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as C

Rectangle {
    id: root
    width: 1366
    height: 768
    color: "#141414"



    C.NavBar {
        id: navBar
    }

    Column {
        id: column1
        spacing: 16
        anchors.horizontalCenterOffset: -400
        anchors.verticalCenterOffset: -200
        anchors.horizontalCenter: navBar.horizontalCenter
        anchors.verticalCenter: navBar.verticalCenter

        Label {
            id: emailLabel
            color: "#ffffff"
            text: "Email: " + getEmail()
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

        C.PushButton {
            id: changePasswordButton

            text: "Change Password"
            onClicked: root.state = "changePasswordState"
        }
    }



    Column {
        id: column
        x: 583
        y: 245
        anchors.horizontalCenter: navBar.horizontalCenter
        anchors.verticalCenter: navBar.verticalCenter
        spacing: 32

        TextField {
            id: oldPasswordField
            text: qsTr("Text Field")
            visible: false
        }

        TextField {
            id: newPasswordField
            text: qsTr("Text Field")
            visible: false
        }

        TextField {
            id: confirmPasswordField
            text: qsTr("Text Field")
            visible: false
        }

        C.PushButton {
            id: submitChangeButton
            text: "Submit Changes"
            visible: false
            anchors.horizontalCenterOffset: 32
            anchors.horizontalCenter: confirmPasswordField.horizontalCenter
            anchors.verticalCenterOffset: 64
            anchors.verticalCenter: confirmPasswordField.verticalCenter
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
