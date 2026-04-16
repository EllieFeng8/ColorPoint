import QtQuick
import ColorPoint
import QtQuick.Controls
import QtQuick.Layouts
import Core 1.0

Window {
    width: mainScreen.width
    height: mainScreen.height

    visible: true
    title: "ColorPoint"

    Main {
        id: mainScreen
    }
    // ===== 對外呼叫 =====
    function showAbnormal(msg) {
        abnormalDialog.message = msg
        abnormalDialog.open()
        // autoCloseTimer.restart()
    }

    Item {
        id: root1
        width: 800
        height: 480
        anchors.centerIn: parent


        // ===== reset Dialog =====
        Dialog {
            id: abnormalDialog

            modal: true          // 是否阻擋背景操作
            focus: true
            anchors.centerIn: parent
            // ⭐ 重點：禁止點外面關閉
            closePolicy: Popup.NoAutoClose

            // ⭐ 可選：禁止按 ESC 關閉
            Keys.onEscapePressed: {
                // do nothing
            }

            property string message: "提醒"

            width: 400
            height: 180

            background: Rectangle {
                radius: 12
                color: "#2b2b2b"
                border.color: "#ff4444"
                border.width: 2
            }

            contentItem: ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Label {
                    text: "⚠ 提醒"
                    color: "#ff4444"
                    font.pixelSize: 20
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: abnormalDialog.message
                    color: "white"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }

                Button {
                    text: "Reset"
                    font.bold: true
                    font.pointSize: 10
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        Cp.heightSet = 0;
                        abnormalDialog.close();
                        console.log("Cp.heightSet",Cp.heightSet)
                    }
                }
            }

        }

        Component.onCompleted: {
            Qt.callLater(function() {
                showAbnormal("機台高度重置")
            })
        }
    }
}

