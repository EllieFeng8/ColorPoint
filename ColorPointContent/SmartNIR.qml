import QtQuick
import QtQuick.Controls
//import Qt.labs.qmlmodels
import QtQuick.Effects
import QtQml.Models

Rectangle {
    id: root
    color: "#00000000" // 背景深灰色
    property var columnMap: ["time", "label", "v1", "v2", "v3", "v4"]
    property var headerText: ["Time", "Label", "1", "2", "3", "4"]
    property var columnWidths: [220, 120, 100, 100, 100, 100]
    property ListModel dataModel: ListModel {}
    Rectangle {
        id: dataList
        width: parent.width
        height: parent.height * 6 / 7
        color: "#33424242"
        radius: 8

        ListModel {
                id: dataModel
                ListElement { time: "20251023-143015"; label: "white"; v1: "4750"; v2: "4820"; v3: "5100"; v4: "5800" }
                ListElement { time: "20251023-143005"; label: "white"; v1: "4800"; v2: "4869"; v3: "5138"; v4: "5861" }
                ListElement { time: "20251023-143010"; label: "102";   v1: "0.67"; v2: "0.8";  v3: "13.9"; v4: "-23.7" }
            }

        // 2. 表格標題列 (Header)
        HorizontalHeaderView {
            id: horizontalHeader
            syncView: tableView
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            clip: true

            model: headerText
            delegate: Rectangle {
                implicitWidth: tableView.columnWidthProvider(model.index)
                implicitHeight: 40
                color: "transparent"
                Text {
                    text: headerText[model.index]
                    color: "white"
                    font.bold: true
                    font.pixelSize: 22
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: model.index === 0 ? Text.AlignLeft : Text.AlignHCenter
                    leftPadding: model.index === 0 ? 15 : 0
                }
            }
        }

        // 3. 數據內容區
        TableView {
            id: tableView
            anchors.top: horizontalHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true

            // 關鍵：將 ListModel 直接作為 model
            model: dataModel
            columnWidthProvider: function (column) { return columnWidths[column] }

            delegate: Rectangle {
                id:test1
                implicitHeight: 45
                color: "transparent"

                Text{
                    text: time
                    anchors.centerIn: parent
                    color: "#e0e0e0"
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: column === 0 ? Text.AlignLeft : Text.AlignHCenter
                    leftPadding: column === 0 ? 15 : 0
                }

                Text{
                    text: label
                    anchors.centerIn: parent
                    color: "#e0e0e0"
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 250
                }
                Text{
                    text: v1
                    anchors.centerIn: parent
                    color: "#e0e0e0"
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 370
                }
                Text{
                    text: v2
                    anchors.centerIn: parent
                    color: "#e0e0e0"
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 470
                }
                Text{
                    text: v3
                    anchors.centerIn: parent
                    color: "#e0e0e0"
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 570
                }
                Text{
                    text: v4
                    anchors.centerIn: parent
                    color: "#e0e0e0"
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 670
                }
            }
        }
    }
    //clear_List BTN
    Rectangle {
        id: clearListBtn

        anchors.top: dataList.bottom
        anchors.topMargin: 20
        height: parent.height * 0.1
        width: parent.width * 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        //Layout.alignment: Qt.AlignHCenter
        color: "#3a3a3a"
        radius: 10

        Text {
            id: clearListTxt
            color: "#ffffff"
            font.family: "Poppins"
            font.pixelSize: clearListMouseArea.containsMouse ? 24 : 22
            font.weight: Font.Bold
            //horizontalAlignment: Text.AlignHCenter
            text: "Clear List"
            textFormat: Text.PlainText
            anchors.centerIn: parent  //字垂直置中 Text.WordWrap }
        }
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: clearListMouseArea.containsMouse ? true : false
            shadowColor: "gray"
            shadowBlur: 0.8
        }
        MouseArea {
            id: clearListMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: dataModel.clear()
        }
    }
    Image {
        id: freshButton
        source: "assets/refresh.png"
        scale: freshButtonMouseArea.containsMouse ? 1.5 : 1.2
        anchors.top: clearListBtn.top
        anchors.topMargin: 5
        anchors.left: clearListBtn.right
        anchors.leftMargin: 180

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: freshButtonMouseArea.containsMouse ? true : false
            shadowColor: "gray"
            shadowBlur: 0.8
        }
        MouseArea {
            id: freshButtonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                updateAllData();
            }
        }
    }
    function updateAllData() {
        // newJsonData 格式必須是：[{ "time": "...", "label": "..." }, { ... }]
        // 生成當前時間字串
        let currentTime = new Date().toLocaleTimeString(Qt.locale("zh_TW"), "hhmmss");
        // 隨機生成一個數值
        let randomValue = (Math.random() * 5000).toFixed(0);

        dataModel.append({ time: "20260120-" +
            currentTime, // 更新為當前年份
           label: "Sensor_A", v1: randomValue,
           v2: (Math.random() * 5000).toFixed(0),
           v3: (Math.random() * 5000).toFixed(0),
           v4: (Math.random() * 5000).toFixed(0)
        });
    }
}
