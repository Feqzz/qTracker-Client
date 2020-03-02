import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.3
import "../component" as Components

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
        stackView.push("qrc:/view/login.qml")
    }

    function changePage(page) {
        var template = "qrc:/view/%1.qml";
        var url = template.arg(page);
        stackView.replace(url);
    }

    function loginUser(username, password, label) {
        if(loginHandler.loginUser(user, username, password))
        {
            changePage("home");
            label.text = "";

        }
        else
        {
            label.text = loginHandler.getErrorMessage();
        }
    }

    function registerUser(usernameField, passwordField,
                          emailField, inviteField, label) {

        if(loginHandler.registerUser(user, usernameField, passwordField,
                                                   emailField, inviteField)) {
            changePage("home");
            label.text = "";
        }
        else
        {
            label.text = loginHandler.getErrorMessage();
        }
    }

    function generateInvite(email, inviteLabel) {
        var inviteKey = inviteHandler.inviteUser(user.getId(),email);

        if(inviteKey && secureSocket.sendMessage(0,[email,inviteKey],2))
        {
            inviteLabel.text("User has been invited!")
        }
        else
        {
            inviteLabel.text = inviteHandler.getErrorMessage();
        }
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
