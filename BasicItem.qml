import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {

    id: clayout

    property alias host_default : itm_host.field_default
    property alias port_default : itm_port.field_default
    property alias nick_default : itm_nick.field_default
    property alias pass_default : itm_pass.field_default
    property alias topic_default : itm_topic.field_default
    property alias path_default : itm_path.field_default


    Element {
        id: itm_host
        field_label: "host:"
        field_default: host_default
        field_placeholder: "Write a host addres (test.mosquitto.org) ..."
        field_echo: TextInput.Normal
    }
    Element {
        id: itm_port
        field_label: "port:"
        field_default: port_default
        field_placeholder: "write a port ..."
        field_echo: TextInput.Normal
        field_validator:  IntValidator { bottom: 0; top: 65535; }
    }
    Element {
        id: itm_nick
        field_label: "username:"
        field_default: nick_default
        field_placeholder: "write a nickname ..."
        field_echo: TextInput.Normal
    }
    Element {
        id: itm_pass
        field_label: "password:"
        field_default: pass_default
        field_placeholder: "write a p@s$w0rd ..."
        field_echo: TextInput.Password
    }
    Element {
        id: itm_topic
        field_label: "topic:"
        field_default: topic_default
        field_placeholder: "write a piklema/test ..."
        field_echo: TextInput.Normal
    }
    Element {
        id: itm_path
        field_label: "filepath:"
        field_default: ""
        field_placeholder: "write a file (" +
                           (Qt.platform.os === "linux"
                            ?  "\\home\\piklema\\msg.txt"
                            : Qt.platform.os === "windows"
                              ? "c:\\piklema\\msg.txt"
                              : ""
                            )
                           + ") ..."
        field_echo: TextInput.Normal
    }

    Item {
        Layout.fillHeight: true
    }

    Item {
        Layout.fillWidth: true
    }

    // статистика о количестве строк (сообщений) отправленных в брокер MQTT
    Rectangle {

        visible: listModel.count >= 0

        implicitWidth: clayout.width
        implicitHeight: 40

        color: "transparent"

        Layout.leftMargin: 10
        Layout.rightMargin: 10
        Layout.topMargin: 30
        Layout.bottomMargin: 30
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        RowLayout {
            spacing: 10

            anchors.fill: parent

            Item {
                Layout.fillWidth: true
            }
            Text {
                text: qsTr("number of messages sent:")
                Layout.alignment: Qt.AlignRight
                font.pointSize: 12
                antialiasing: true
            }
            Text {
                visible: true
                text: qsTr("%1").arg(listModel.count < 0 ? "  " : listModel.count)
                Layout.alignment: Qt.AlignLeft
                font.pointSize: 12
                antialiasing: true
                color: visible
                       ? listModel.count < 1 ? "darkred" : "darkgreen"
                : "black"

            }


        }

    }

}
