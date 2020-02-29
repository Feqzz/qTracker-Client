import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Window 2.12

Page{
    visible: true
    width: 1366
    height: 768
    title: qsTr("Login :: qTracker 1.0");

    Rectangle {
        id: loginroot
        color: "#141414"
        property alias inviteLabel: inviteLabel
        width: 1366
        height: 768

        Rectangle {
            id: loginPage
            width: 1366
            height: 768
            color: "#141414"

            PushButton {
                id: registerButton
                x: 503
                y: 664
                text: "Create Account"
                anchors.verticalCenter: loginButton.verticalCenter
                visible: false
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: title.horizontalCenter
                onClicked: registerUser(usernameField.text, passwordField.text,
                                           emailField.text, inviteField.text)
            }

            PushButton {
                id: loginButton
                x: 488
                y: 393
                text: qsTr("Log in")
                anchors.horizontalCenterOffset: 30
                anchors.horizontalCenter: column.horizontalCenter
                anchors.verticalCenterOffset: 96
                anchors.verticalCenter: column.verticalCenter
                font.pixelSize: 32
                onClicked: loginUser(usernameField.text, passwordField.text)
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
                font.family: Constants.font.family
            }

            Column {
                id: column
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
                }

                TextField {
                    id: passwordField
                    width: 156
                    height: 32
                    text: qsTr("")
                    font.pixelSize: 16
                    placeholderText: "Password"
                    echoMode: "Password"
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
                    onClicked: loginroot.state = "registerState"
                }
            }

            Label {
                id: goBackLabel
                x: 280
                y: 429
                width: 240
                height: 22
                color: ma1.containsMouse ? "#d0d046" : "#ffffff"
                text: qsTr("Already have an account? Log in..")
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
                MouseArea {
                    id: ma1
                    visible: false
                    hoverEnabled: true
                    onClicked: loginroot.state = "base state"
                }
                anchors.verticalCenter: loginButton.verticalCenter
                anchors.verticalCenterOffset: 64
                font.pixelSize: 16
            }

            Column {
                id: column1
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
                }

                TextField {
                    id: inviteField
                    width: 156
                    height: 32
                    text: qsTr("")
                    placeholderText: qsTr("Invite Code")
                }
            }
        }
        states: [
            State {
                name: "registerState"

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
                    target: ma1
                    x: 0
                    y: 0
                    width: 240
                    height: 22
                    visible: true
                    anchors.topMargin: 0
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                }

                PropertyChanges {
                    target: column
                    anchors.horizontalCenterOffset: -128
                    anchors.topMargin: 100
                }

                PropertyChanges {
                    target: emailField
                    x: 0
                    y: 0
                    font.pixelSize: 16
                }

                PropertyChanges {
                    target: inviteField
                    x: 0
                    y: 644
                    font.pixelSize: 16
                }

                PropertyChanges {
                    target: column1
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
