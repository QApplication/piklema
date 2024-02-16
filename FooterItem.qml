import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {

    id: footer_rect

    property alias log_pushed : btn_log.checked

    RowLayout {
        id: rlayout

        spacing: 10
        anchors.fill: parent

        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 5
        anchors.bottomMargin: 5

        Button {
            id: btn_log
            text: "log"

            implicitWidth: 80
            implicitHeight: 40

            Layout.alignment: Qt.AlignVCenter
            display: AbstractButton.TextOnly

            checkable: true
            checked: log_pushed

            onClicked: {
                btn_default.enabled = !checked
                btn_run.enabled = !checked

                log_list.visible = checked
            }

        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            id: btn_default
            text: "default"

            implicitWidth: 80
            implicitHeight: 40

            Layout.alignment: Qt.AlignVCenter
            display: AbstractButton.TextOnly

            onClicked: {
                setDefaultConnectionSettings();
            }

        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            id: btn_run
            text: "Run"

            implicitWidth: 80
            implicitHeight: 40

            Layout.alignment: Qt.AlignVCenter
            display: AbstractButton.TextOnly

            onClicked: {
                run(clayout.host_default, clayout.port_default,
                    clayout.nick_default, clayout.pass_default,
                    clayout.topic_default, clayout.path_default);          // execute of main processing
            }

        }

    }

}
