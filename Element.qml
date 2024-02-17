import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.12
import QtQuick.Dialogs

ColumnLayout {
    id: itm

    property alias field_label : lbl.text
    property alias field_default : edtText.text
    property alias field_placeholder : edtText.placeholderText
    property alias field_validator : edtText.validator

    property alias field_echo: edtText.echoMode


    implicitWidth: clayout.width
    implicitHeight: 5


    // label
    Rectangle  {

        id:label_rect

        implicitWidth: clayout.width
        implicitHeight: 15

        color: "transparent"

        Layout.leftMargin: 10
        Layout.rightMargin: 10
        Layout.topMargin: 25
        Layout.bottomMargin: 5
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignLeft

        Text {
            id: lbl
            text: field_label
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pointSize: 12
            antialiasing: true
        }

    }


    Rectangle  {

        implicitWidth: clayout.width
        implicitHeight: 40

        color: "lightgray"

        Layout.leftMargin: 10
        Layout.rightMargin: 10
        Layout.topMargin: 0
        Layout.bottomMargin: 0

        Layout.margins: 5
        Layout.fillWidth: true

        TextField {
            id: edtText
            selectByMouse: true
            anchors.fill: parent
            placeholderText: field_placeholder
            font.pointSize: 12
            color: acceptableInput ? "black" : "red"

            verticalAlignment: Qt.AlignVCenter
            leftPadding: 10

            background: Rectangle {
                id: bg_text_field
                implicitWidth: 80
                implicitHeight: 40
                color: "white"
                border.color: "blue"
                border.width: 1
                radius: 7
            }

            text: field_default
            echoMode: field_echo
            passwordMaskDelay: field_echo == TextInput.Password ? 3 : 0

            antialiasing: true

            onActiveFocusChanged: {
                if (activeFocus) {
                    bg_text_field.color = Qt.rgba(0, 1, 0, 0.1)
                    bg_text_field.border.color = Qt.rgba(0, 0.4, 0)
                    bg_text_field.border.width = 2
                } else {
                    bg_text_field.color = "white"
                    bg_text_field.border.color = "blue"
                    bg_text_field.border.width = 1
                }
            }

            onAcceptableInputChanged: {
                if (acceptableInput)
                    return;
                messageDialog.visible = true
            }

            MessageDialog {
                id: messageDialog
                visible: false

                title: "Unavailable input"
                flags: MessageDialog.Ok

                onAccepted: {
                    close();
                }
            }

        }
    }



}
