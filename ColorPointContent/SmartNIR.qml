import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtQml.Models
import Core 1.0

// Rectangle {
//     id: root
//     color: "#00000000" // 背景深灰色
//
//     property var headerText: ["Time", "Label", "1", "2", "3", "4","5","6","7"]
//     property var columnWidths: [200, 140, 100, 100, 100, 100,100,100,100]//[10, 240, 380, 480, 580, 680,780,880,980]
//     //property alias listView : listView
//     property alias tableView:tableView
//
//     Rectangle {
//         id: dataList
//         width: parent.width
//         height: parent.height * 6 / 7
//         color: "#33424242"
//         radius: 8
//         readonly property var columnMap: ["time", "label", "v1", "v2", "v3", "v4", "v5", "v6", "v7"]
//         HorizontalHeaderView {
//             id: horizontalHeader
//             syncView: tableView
//             anchors.top: parent.top
//             anchors.leftMargin: 0
//             anchors.rightMargin: 0
//             anchors.topMargin: 0
//             anchors.left: parent.left
//             anchors.right: parent.right
//             height: 37
//             clip: true
//             model: headerText
//
//             delegate: Rectangle {
//                 implicitWidth: tableView.columnWidthProvider(model.index)
//                 implicitHeight: 40
//                 color: "transparent"
//
//                 Text {
//                     text: headerText[model.index]//headerText[model.index]
//                     color: "white"
//                     font.bold: true
//                     font.pixelSize: 22
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     horizontalAlignment: model.index === 0 ? Text.AlignLeft : Text.AlignHCenter
//                     leftPadding: model.index === 0 ? 15 : 0
//                 }
//             }
//         }
//         // 3. 數據內容區
//         TableView {
//             id: tableView
//             anchors.top:horizontalHeader.bottom
//             anchors.left: parent.left
//             anchors.right: parent.right
//             anchors.bottom: parent.bottom
//             clip: true // 關鍵：將 ListModel 直接作為 model
//             model: dataModel
//             // property var columnMap: ["time","label", "v1", "v2", "v3", "v4", "v5", "v6"]
//             //
//             columnWidthProvider: function (column) { return columnWidths[column] }
//             delegate: Rectangle {
//                 id:test1
//                 implicitHeight: 45
//                 color: "transparent"
//                 // Text {
//                 //     // 關鍵：使用 model[dataList.columnMap[column]]
//                 //     // 這會根據目前的列索引(column)，去抓取 model 裡對應的 role (如 "time" 或 "v1")
//                 //     text: {
//                 //         let roleName = dataList.columnMap[column];
//                 //         return model[roleName] !== undefined ? model[roleName] : ""
//                 //     }
//                 //     anchors.fill: parent
//                 //     color: "#e0e0e0"
//                 //     font.pixelSize: 18
//                 //     verticalAlignment: Text.AlignVCenter
//                 //     horizontalAlignment: column === 0 ? Text.AlignLeft : Text.AlignHCenter
//                 //     leftPadding: column === 0 ? 15 : 0
//                 // }
//                 Text{
//                     text: time
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     horizontalAlignment: column === 0 ? Text.AlignLeft : Text.AlignHCenter
//                     leftPadding: column === 0 ? 15 : 0
//                 }
//                 Text{
//                     text: label
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     horizontalAlignment: column === 0 ? Text.AlignLeft : Text.AlignHCenter
//                     leftPadding: 240
//                 }
//                 Text{
//                     text: v1
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 370
//                 }
//                 Text{
//                     text: v2
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 470
//                 }
//                 Text{
//                     text: v3
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 570
//                 }
//                 Text{
//                     text: v4
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 670
//                 }
//                 Text{
//                     text: v5
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 770
//                 }
//                 Text{
//                     text: v6
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 870
//                 }
//                 Text{
//                     text: v7
//                     anchors.centerIn: parent
//                     color: "#e0e0e0"
//                     font.pixelSize: 18
//                     anchors.fill: parent
//                     verticalAlignment: Text.AlignVCenter
//                     leftPadding: 970
//                 }
//             }
//         }
//     }
Rectangle {
    id: root
    color: "#00000000" // 背景深灰色


    //property var headerText: ["Time", "Label", "1", "2", "3", "4","5","6","7"]
    //property var columnWidths: [200, 140, 100, 100, 100, 100,100,100,100]//[10, 240, 380, 480, 580, 680,780,880,980]
    //property alias listView : listView
    property var columnMap: ["time", "label", "listData"]
    property var headerText: ["Time", "Label"]
    property var columnWidths: [220, 120, 100, 100, 100, 100, 100, 100, 100] // 請根據實際寬度調整
    property alias tableView:tableView
    //property ListModel dataModel: ListModel {}

    Rectangle {
        id: dataList
        width: parent.width
        height: parent.height * 6 / 7
        color: "#33424242"
        radius: 8
        property var columnMap: ["time", "label", "listData"]
        property var headerText: ["Time", "Label"]
        property var columnWidths: [220, 120, 100, 100, 100, 100, 100, 100, 100,100,100] // 請根據實際寬度調整

        //readonly property var columnMap: ["time", "label", "v1", "v2", "v3", "v4", "v5", "v6", "v7"]
        HorizontalHeaderView {
            id: horizontalHeader
            syncView: tableView
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            width: parent.width
            height: 37
            clip: true
            model: headerText

            delegate: Rectangle {
                implicitWidth: tableView.columnWidthProvider(model.index)
                implicitHeight: 40
                color: "transparent"

                Text {
                    text: headerText[model.index]//headerText[model.index]
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
            anchors.fill: parent
            anchors.top:horizontalHeader.bottom
            anchors.topMargin: 40
            clip: true // 關鍵：將 ListModel 直接作為 model
            model: dataModel
            columnWidthProvider: function (column) { return columnWidths[column] }
            delegate: Item {
                implicitWidth: tableView.width
                implicitHeight: 45
                readonly property int rowIndex: row
                // 1. 關鍵：在每一行的最外層，先抓好這一行的 model 資料
                // 這樣在裡面的 Repeater 就不會找不到資料，也不會跟 Repeater 的 index 搞混
                readonly property var rowData: model

                Row {
                    anchors.fill: parent
                    Repeater {
                        model: dataList.columnMap.length // 這是 9 欄

                        delegate: Rectangle {
                            readonly property int colIndex: index
                            width: dataList.columnWidths[colIndex] // 這裡的 index 是「第幾欄」
                            height: 45
                            color: "transparent"

                            Text {
                                anchors.fill: parent
                                color: "#e0e0e0"
                                font.pixelSize: 18
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: index === 0 ? Text.AlignLeft : Text.AlignHCenter
                                leftPadding: index === 0 ? 15 : 0

                                // 使用 JS 的邏輯運算符確保回傳的一定是字串
                                text: {
                                    var key = dataList.columnMap[index]; // 取得對應的欄位名，如 "time"
                                    var value = rowData[key];            // 從 rowData 抓值
                                    return (value !== undefined) ? String(value) : "";
                                }
                            }
                        }
                    }
                }
                Image {
                    id: b11
                    width: 32
                    height: 32
                    x:365
                    y:5
                    source: "assets/refresh.png"
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: deleteButtonMouseArea.containsMouse ? true : false
                        shadowColor: "gray"
                        shadowBlur: 0.8
                    }
                    MouseArea {
                        id:deleteButtonMouseArea
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: dataModel.remove(rowIndex, 1)//dataModel.count - 1
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
            onClicked: dataModel.clear()//dataModel.remove(dataModel.count - 1)
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
           // v2: (Math.random() * 5000).toFixed(0),
           // v3: (Math.random() * 5000).toFixed(0),
           // v4: (Math.random() * 5000).toFixed(0)
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
