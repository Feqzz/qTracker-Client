import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12

Window {
    visible: true
    width: 1366
    height: 768
    title: qsTr("Home :: qTracker 1.0");

    Rectangle {
        id: root
        width: 1366
        height: 768
        color: "#141414"

        Rectangle {
            id: homePage
            width: 1366
            height: 768
            color: "#141414"
        }

        Rectangle {
            id: invitePage
            width: 1366
            height: 768
            color: "#141414"

            TextField {
                id: emailField
                x: 583
                y: 232
                text: qsTr("")
                anchors.verticalCenterOffset: -50
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                visible: false
                placeholderText: qsTr("Email")
            }

            PushButton {
                id: inviteButton
                x: 683
                y: 304
                text: "Invite"
                anchors.horizontalCenterOffset: 50
                anchors.verticalCenterOffset: 64
                anchors.horizontalCenter: emailField.horizontalCenter
                anchors.verticalCenter: emailField.verticalCenter
                visible: false
            }

            Label {
                id: inviteKeyLabel
                x: 661
                y: 373
                color: "#ffffff"
                text: qsTr("")
                visible: false
                anchors.verticalCenterOffset: 150
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: emailField.verticalCenter
                font.pixelSize: 24
            }
        }

        Text {
            id: title
            x: 612
            y: 50
            color: "#ffffff"
            text: qsTr("qTracker")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 50
            font.pixelSize: 36
            MouseArea {
                id: maTitle
                hoverEnabled: true
                anchors.fill: parent
                onClicked: root.state = "base state"
            }
        }

        Row {
            x: 341
            y: 138
            spacing: 32
            anchors.verticalCenterOffset: 100
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: title.verticalCenter

            Label {
                id: profileLabel
                color: "#ffffff"
                text: qsTr("Profile")
            }

            Label {
                id: ratioLabel
                color: "#ffffff"
                text: qsTr("Ratio:")
            }

            Label {
                id: upLabel
                color: "#ffffff"
                text: qsTr("Up: ")
            }

            Label {
                id: downLabel
                color: "#ffffff"
                text: qsTr("Down:")
            }

            Label {
                id: hnrsLabel
                color: "#ffffff"
                text: qsTr("HnRs: ")
            }

            Label {
                id: donateLabel
                color: "#ffffff"
                text: qsTr("Donate")
            }

            Label {
                id: rulesLabel
                color: "#ffffff"
                text: qsTr("Rules")
            }

            Label {
                id: torrentsLabel
                color: "#ffffff"
                text: qsTr("Torrents")
            }

            Label {
                id: inviteLabel
                color: maInviteLabel.containsMouse ? "#d0d046" : "#ffffff"
                text: qsTr("Invite")
                MouseArea {
                    id: maInviteLabel
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: root.state = "inviteState"
                }
            }

            Label {
                id: pointsLabel
                color: "#ffffff"
                text: qsTr("Points")
            }
        }

        states: [
            State {
                name: "inviteState"

                PropertyChanges {
                    target: emailField
                    visible: true
                }

                PropertyChanges {
                    target: inviteButton
                    anchors.horizontalCenterOffset: 50
                    visible: true
                }

                PropertyChanges {
                    target: inviteKeyLabel
                    visible: true
                }

                PropertyChanges {
                    target: invitePage
                }
            }
        ]
    }
}

/*##^## Designer {
    D{i:3;anchors_x:583;anchors_y:232}
}
 ##^##*/
