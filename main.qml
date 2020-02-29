import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.3

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1366
    height: 768
    title: qsTr("QTracker")

    // Main stackview
    StackView{
        id: stackView
        focus: true
        anchors.fill: parent
    }

    // After loading show initial Login Page
    Component.onCompleted: {
        stackView.push("qrc:/login.qml")
        login.login()
    }

    function changePage(page) {
        var template = "qrc:/%1.qml";
        var url = template.arg(page);
        stackView.replace(url);
    }

    function loginUser(username, password) {
        if(login.login(username, password))
        {
            changePage("home");
        }
    }

    function registerUser(usernameField, passwordField,
                          emailField, inviteField) {

        if(login.registerUser(usernameField, passwordField,
                                                   emailField, inviteField)) {
            loginroot.state = "base state"
        }
    }

    function generateInvite(email, inviteLabel) {
        var str = invite.generateKey(email);
        inviteLabel.text(str);
    }

    function getRatio() {
        return user.getRatio();
    }

    function getUpload() {
        return user.getUpload();
    }

    function getDownload() {
        return user.getDownload();
    }
}
