import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Window 2.12
import "../component" as C




Rectangle {
    id: loginroot
    color: "#141414"
    property alias inviteLabel: inviteLabel
    //width: 1920
    //height: 1080
    anchors.fill: parent

    Rectangle {
        id: loginPage
        //width: 1920
        //height: 1080
        anchors.fill: parent
        color: "#141414"
        //Enter login
        signal accepted

        C.PushButton {
            id: registerButton
            x: 510
            y: 664
            text: "Create Account"
            anchors.verticalCenter: loginButton.verticalCenter
            visible: false
            anchors.horizontalCenterOffset: 30
            anchors.horizontalCenter: title.horizontalCenter
            onClicked: registerUser(usernameField.text, passwordField.text,
                                       emailField.text, inviteField.text, errorMessage)
        }

        C.PushButton {
            id: loginButton
            //x: 600
            //y: 393
            text: qsTr("Log in")
            anchors.horizontalCenterOffset: 50
            anchors.horizontalCenter: usernameAndPasswordColumn.horizontalCenter
            anchors.verticalCenterOffset: 96
            anchors.verticalCenter: usernameAndPasswordColumn.verticalCenter
            font.pixelSize: 32
            onClicked: loginUser(usernameField.text, passwordField.text, errorMessage)
        }

        C.PushButton {
            id: forgotPasswordButton
            x: 488
            y: 393
            text: qsTr("Send new password")
            anchors.horizontalCenter: title.horizontalCenter
            anchors.verticalCenter: emailAndInviteColumn.verticalCenter
            anchors.verticalCenterOffset: 64
            visible: false
            font.pixelSize: 32
            onClicked: resetPassword(emailField.text,errorMessage)
        }

        Text {
            id: title
            x: 449
            y: 100
            color: "#ffffff"
            text: qsTr("qTracker")
            font.pixelSize: 36
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.horizontalCenter: parent.horizontalCenter
            //font.family: Constants.font.family
        }

        Column {
            id: usernameAndPasswordColumn
            x: 412
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: title.horizontalCenter
            anchors.top: title.bottom
            anchors.topMargin: 100
            spacing: 32

            TextField {
                id: usernameField
                width: 200
                //height: 32
                text: qsTr("")
                ///font.pixelSize: 24
                placeholderText: qsTr("Username")
                selectByMouse: true
                focus: true
            }

            TextField {
                id: passwordField
                width: 200
                //height: 32
                text: qsTr("")
                //font.pixelSize: 24
                placeholderText: "Password"
                echoMode: "Password"
                selectByMouse: true
                onAccepted: loginPage.accepted()


            }
        }

        /* Replaced with C.YellowLabel
        Label {
            id: inviteLabel
            x: 392
            y: 505
            width: 240
            height: 22
            color: ma.containsMouse ? "#d0d046" : "#ffffff"
            text: qsTr("Have an invite code? Register here..")
            font.pixelSize: 16
            anchors.verticalCenterOffset: 64
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: loginButton.verticalCenter
            MouseArea {
                id: ma
                hoverEnabled: true
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    loginroot.state = "registerState"
                    errorMessage.text = ""
                }
            }
        }
        */

        C.YellowLabel {
            id: inviteLabel
            x: 392
            y: 505
            width: 240
            height: 22
            text: qsTr("Have an invite code? Register here..")
            font.pixelSize: 16
            anchors.verticalCenterOffset: 64
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: loginButton.verticalCenter
            C.MouseArea {
                anchors.fill: parent
                onClicked: {
                    loginroot.state = "registerState"
                    errorMessage.text = ""
                }
            }
        }

        C.YellowLabel {
            id: forgotPasswordLabel
            x: 392
            y: 505
            width: 240
            height: 22
            text: qsTr("Forgot your password? Click here..")
            font.pixelSize: 16
            anchors.verticalCenterOffset: 64+32
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: loginButton.verticalCenter
            C.MouseArea {
                id: forgotPasswordMouseArea
                anchors.fill: parent
                onClicked: {
                    loginroot.state = "forgotPasswordState"
                    errorMessage.text = ""
                }
            }
        }

        C.YellowLabel {
            id: goBackLabel
            x: 280
            y: 429
            width: 240
            height: 22
            text: qsTr("Already have an account? Log in..")
            anchors.horizontalCenter: parent.horizontalCenter
            visible: false
            C.MouseArea {
                id: goBackMouseArea
                visible: false
                width: 240
                height: 22
                anchors.fill: parent
                onClicked: {
                    loginroot.state = "base state"
                }
            }
            anchors.verticalCenter: loginButton.verticalCenter
            anchors.verticalCenterOffset: 64
            font.pixelSize: 16
        }

        Label {
            id: errorMessage
            x: 280
            y: 700
            width: 240
            height: 22
            color: "#e90000"
            text: "";
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: loginButton.verticalCenter
            anchors.verticalCenterOffset: 128
            anchors.horizontalCenterOffset: 32
        }

        Column {
            id: emailAndInviteColumn
            x: -87
            y: 246
            visible: false
            anchors.horizontalCenter: title.horizontalCenter
            anchors.top: title.bottom
            anchors.topMargin: 100
            spacing: 32

            TextField {
                id: emailField
                width: 200
                //height: 32
                text: qsTr("")
                placeholderText: qsTr("Email")
                selectByMouse: true
            }

            TextField {
                id: inviteField
                width: 200
                //height: 32
                text: qsTr("")
                placeholderText: qsTr("Invite Code")
                selectByMouse: true
            }
        }
        onAccepted: {
            loginUser(usernameField.text, passwordField.text, errorMessage)
            }
    }
    states: [
        State {
            name: "forgotPasswordState"

            /* Components shown */
            PropertyChanges {
                target: emailField
                visible: true
            }
            PropertyChanges {
                target: forgotPasswordButton
                visible: true
            }
            PropertyChanges {
                target: emailAndInviteColumn
                visible: true
            }
            PropertyChanges {
                target: goBackLabel
                text: qsTr("Go back to login page..")
                visible: true
            }
            PropertyChanges {
                target: goBackMouseArea
                visible: true
            }

            /* Components hidden */
            PropertyChanges {
                target: passwordField
                visible: false
            }

            PropertyChanges {
                target: inviteLabel
                visible: false
            }

            PropertyChanges {
                target: loginButton
                visible: false
            }

            PropertyChanges {
                target: registerButton
                visible: false
            }
            PropertyChanges {
                target: forgotPasswordMouseArea
                visible: false
            }
            PropertyChanges {
                target: forgotPasswordLabel
                visible: false
            }

            PropertyChanges {
                target: usernameAndPasswordColumn
                visible: false
            }

            PropertyChanges {
                target: inviteField
                visible: false
            }

        },

        State {

            name: "registerState"

            PropertyChanges {
                target: forgotPasswordButton
                visible: false
            }

            PropertyChanges {
                target: passwordField
                font.pixelSize: 16
                visible: true
            }

            PropertyChanges {
                target: inviteLabel
                anchors.verticalCenterOffset: 32
                anchors.horizontalCenterOffset: 0
                visible: false
            }

            PropertyChanges {
                target: loginButton
                visible: false
            }

            PropertyChanges {
                target: registerButton
                x: 449
                y: 382
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 164
                font.pixelSize: 24
                visible: true
            }

            PropertyChanges {
                target: usernameField
                font.pixelSize: 16
            }

            PropertyChanges {
                target: goBackLabel
                y: 488
                anchors.verticalCenterOffset: 64
                visible: true
                anchors.horizontalCenterOffset: 0
            }

            PropertyChanges {
                target: goBackMouseArea
                visible: true
            }

            PropertyChanges {
                target: usernameAndPasswordColumn
                anchors.horizontalCenterOffset: -128
                anchors.topMargin: 100
            }

            PropertyChanges {
                target: emailField
                visible: true
            }

            PropertyChanges {
                target: inviteField
                visible: true

            }

            PropertyChanges {
                target: emailAndInviteColumn
                x: 437
                y: 243
                anchors.horizontalCenterOffset: 128
                anchors.rightMargin: 207
                visible: true
            }
        }
    ]
}






