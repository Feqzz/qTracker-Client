import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12

Rectangle {
    id: root
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
        MouseArea {
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

        Label {
            id: profileLabel
            color: maProfileLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: "Profile"
            MouseArea {
                id: maProfileLabel
                hoverEnabled: true
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

        Label {
            id: hnrsLabel
            color: maHnRsLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: "HnRs: " + hitAndRunHandler.getNumberOfHnRs(user.getId())
            MouseArea {
                id: maHnRsLabel
                hoverEnabled: true
                anchors.fill: parent
                onClicked: changePage("hitAndRun")
            }

        }

        Label {
            id: pointsLabel
            color: "#ffffff"
            text: "Points: " + getPoints()
        }

        Label {
            id: donateLabel
            color: "#ffffff"
            text: qsTr("Donate")
        }

        Label {
            id: rulesLabel
            color: maRulesLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: qsTr("Rules")
            MouseArea {
                id: maRulesLabel
                hoverEnabled: true
                anchors.fill: parent
                onClicked: changePage("rules");
            }
        }

        Label {
            id: torrentsLabel
            color: maTorrentLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: qsTr("Torrents")
            MouseArea {
                id: maTorrentLabel
                hoverEnabled: true
                anchors.fill: parent
                onClicked: changePage("torrent");
            }
        }

        Label {
            id: inviteLabel
            color: maInviteLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: qsTr("Invite")
            MouseArea {
                id: maInviteLabel
                hoverEnabled: true
                anchors.fill: parent
                onClicked: changePage("invite");
            }
        }

        Label {
            id: uploadLabel
            color: maUploadLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: qsTr("Upload")
            MouseArea {
                id: maUploadLabel
                hoverEnabled: true
                anchors.fill: parent
                onClicked: changePage("upload");
            }
        }

        Label {
            id: adminLabel
            color: maAdminLabel.containsMouse ? "#d0d046" : "#ffffff"
            text: qsTr("Admin")
            MouseArea {
                id: maAdminLabel
                hoverEnabled: true
                anchors.fill: parent
                onClicked: changePage("admin");
            }
            visible: checkPrivilege()
        }
    }
}


