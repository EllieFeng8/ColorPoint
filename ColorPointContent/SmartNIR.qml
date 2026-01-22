import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtQml.Models
import Core 1.0

Rectangle {
    id: root
    color: "#00000000" // 背景深灰色
    property var columnMap: ["time", "label", "v1", "v2", "v3", "v4"]
    property var headerText: ["Time", "Label", "1", "2", "3", "4"]
    property var columnWidths: [10, 240, 380, 480, 580, 680]
    property alias listView : listView
    // property ListModel dataModel: ListModel {}
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
        ColumnLayout {
            anchors.fill: parent
            spacing: 5
            Rectangle {
                    //Layout.fillWidth: true
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.1

                    //height: 50
                    color: "transparent"
                    Text {
                        text: headerText[0]
                        x: columnWidths[0]
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 26
                    }
                    Text {
                        text: headerText[1]
                        x: columnWidths[1]
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 26
                    }
                    Text {
                        text: headerText[2]
                        x: columnWidths[2]
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 26
                    }
                    Text {
                        text: headerText[3]
                        x: columnWidths[3]
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 26
                    }
                    Text {
                        text: headerText[4]
                        x: columnWidths[4]
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 26
                    }
                    Text {
                        text: headerText[5]
                        x: columnWidths[5]
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                        font.bold: true
                        font.pixelSize: 26
                    }
                }

            ListView {
                id: listView
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.9
                model: dataModel
                //headerPositioning: ListView.OverlayHeader
                clip: true
                delegate: ItemDelegate {
                    id:test1
                    implicitHeight: 40
                    Text{
                        text: time
                        anchors.centerIn: parent
                        color: "#e0e0e0"
                        font.pixelSize: 18
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:  Text.AlignLeft
                        leftPadding: 10
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
                //updateAllData();
                listModelToCsv(dataModel);


            }
        }
    }

    function updateAllData() { // newJsonData 格式必須是：[{ "time": "...", "label": "..." }, { ... }]
        // 生成當前時間字串
        let currentTime = new Date().toLocaleTimeString(Qt.locale("zh_TW"), "hh:mm:ss");
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

    function listModelToCsv(model) {
        //var headers = ["time", "label", "v1", "v2", "v3", "v4"]
        var csv = columnMap.join(",") + "\n"
        console.log(model)
        for (var i = 0; i < model.count; i++) {
            var row = model.get(i)
            var line = []
            for (var j = 0; j < columnMap.length; j++) {
                line.push(row[columnMap[j]])
            }
            csv += line.join(",") + "\n"
        }
        console.log("csv",csv)
        Cp.saveCsvFile("",Cp.fileName,csv)
        //return csv
    }
}
