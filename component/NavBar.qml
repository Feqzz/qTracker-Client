import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12
import "../component" as C

Rectangle {
    id: rootNav
    anchors.fill: parent
    color: "#141414"

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
        C.MouseArea {
            id: maTitle
            hoverEnabled: true
            anchors.fill: parent
            onClicked: changePage("home")
        }
    }

    Row {
        x: 341
        y: 138
        spacing: 32
        anchors.verticalCenterOffset: 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: title.verticalCenter

        C.YellowLabel {
            id: profileLabel
            text: "Profile"
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("profile");
            }
        }

        Label {
            id: ratioLabel
            color: "#ffffff"
            text: (getRatio() >= 1) ? "Ratio: " + "<font color=\"#00e940\">" + getRatio().toFixed(2) + "</font>"
                                   : "Ratio: " + "<font color=\"#e90000\">" + getRatio().toFixed(2) + "</font>"
        }

        Label {
            id: upLabel
            color: "#ffffff"
            text: "Up: " + getUpload()
        }

        Label {
            id: downLabel
            color: "#ffffff"
            text: "Down: " + getDownload()
        }

        C.YellowLabel {
            id: hnrsLabel
            text: "HnRs: " + hitAndRunHandler.getNumberOfHnRs(user.getId())
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("hitAndRun")
            }
        }

        Label {
            id: pointsLabel
            color: "#ffffff"
            text: "Points: " + getPoints()
        }

        C.YellowLabel {
            id: rulesLabel
            text: qsTr("Rules")
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("rules");
            }
        }

        C.YellowLabel {
            id: torrentsLabel
            text: qsTr("Torrents")
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("torrent");
            }
        }

        C.YellowLabel {
            id: inviteLabel
            text: qsTr("Invite")
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("invite");
            }
        }

        C.YellowLabel {
            id: uploadLabel
            text: qsTr("Upload")
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("upload");
            }
        }

        C.YellowLabel {
            id: adminLabel
            text: qsTr("Admin")
            C.MouseArea {
                anchors.fill: parent
                onClicked: changePage("admin");
            }
            visible: checkPrivilege()
        }
    }
}


