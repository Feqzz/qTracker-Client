import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Window 2.12
import "../component" as Components


Page{
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Login :: qTracker 1.0");

    Rectangle {
        id: loginroot
        color: "#141414"
        property alias inviteLabel: inviteLabel
        width: 1920
        height: 1080

        Rectangle {
            id: loginPage
            width: 1920
            height: 1080
            color: "#141414"
            //Enter login
            signal accepted

            Components.PushButton {
                id: registerButton
                x: 503
                y: 664
                text: "Create Account"
                anchors.verticalCenter: loginButton.verticalCenter
                visible: false
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: title.horizontalCenter
                onClicked: registerUser(usernameField.text, passwordField.text,
                                           emailField.text, inviteField.text, errorMessage)
            }

            Components.PushButton {
                id: loginButton
                x: 488
                y: 393
                text: qsTr("Log in")
                anchors.horizontalCenterOffset: 30
                anchors.horizontalCenter: usernameAndPasswordColumn.horizontalCenter
                anchors.verticalCenterOffset: 96
                anchors.verticalCenter: usernameAndPasswordColumn.verticalCenter
                font.pixelSize: 32
                onClicked: loginUser(usernameField.text, passwordField.text, errorMessage)
            }

            Components.PushButton {
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
                font.pixelSize: 32
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
                    width: 156
                    height: 32
                    text: qsTr("")
                    font.pixelSize: 16
                    placeholderText: qsTr("Username")
                    selectByMouse: true
                }

                TextField {
                    id: passwordField
                    width: 156
                    height: 32
                    text: qsTr("")
                    font.pixelSize: 16
                    placeholderText: "Password"
                    echoMode: "Password"
                    selectByMouse: true
                    onAccepted: loginPage.accepted()


                }
            }

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
                    onClicked: {
                        loginroot.state = "registerState"
                        errorMessage.text = ""
                    }
                }
            }

            Label {
                id: forgotPasswordLabel
                x: 392
                y: 505
                width: 240
                height: 22
                color: forgotPasswordMouseArea.containsMouse ? "#d0d046" : "#ffffff"
                text: qsTr("Forgot your password? Click here..")
                font.pixelSize: 16
                anchors.verticalCenterOffset: 64+32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: loginButton.verticalCenter
                MouseArea {
                    id: forgotPasswordMouseArea
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        loginroot.state = "forgotPasswordState"
                        errorMessage.text = ""
                    }
                }
            }

            Label {
                id: goBackLabel
                x: 280
                y: 429
                width: 240
                height: 22
                color: goBackMouseArea.containsMouse ? "#d0d046" : "#ffffff"
                text: qsTr("Already have an account? Log in..")
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
                MouseArea {
                    id: goBackMouseArea
                    visible: false
                    width: 240
                    height: 22
                    hoverEnabled: true
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
                    width: 156
                    height: 32
                    text: qsTr("")
                    placeholderText: qsTr("Email")
                    selectByMouse: true
                }

                TextField {
                    id: inviteField
                    width: 156
                    height: 32
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
                    anchors.horizontalCenterOffset: 146
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





}
