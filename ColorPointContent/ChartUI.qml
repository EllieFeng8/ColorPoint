import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts 1.15

Rectangle {
    id: chart_container
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#21555353"
    radius: 20

    // --- 模擬數據與配置 ---
    property var chartData: Cp.charData

readonly property real xMin: 900    // X軸最大值
    readonly property real xMax: 1700    // X軸最大值
    readonly property real yMax: 66000 // Y軸最大值
    readonly property int xDivisions: 10 // X軸分10等分
    readonly property int yDivisions: 4  // Y軸分4等分

    readonly property real plotWidth: 594
    readonly property real plotHeight: 347
readonly property real plotHeightOffset: 40

    // --- 輔助函數：生成 SVG 路徑字串 ---
    function generatePath(data) {
        if (data.length === 0) return "";
        let pathStr = "";
        for (let i = 0; i < data.length; i++) {
            // 轉換座標：(原始值 / 最大值) * 畫布寬高
            let px = ((data[i].x-xMin) / (xMax-xMin)) * plotWidth;
            let py = plotHeight - (data[i].y / yMax) * plotHeight -plotHeightOffset;

            if (i === 0) {
                pathStr += "M " + px + " " + py;
            } else {
                pathStr += " L " + px + " " + py;
                // 如果需要平滑曲線，可以在此邏輯中計算貝氏曲線控制點(C)
            }
        }
        return pathStr;
    }

    Item {
        id: line_chart
        anchors.centerIn: parent
        height: 376
        width: 661
        // scale:1.3
        clip: true //裁切超出範圍的內容
        // --- Y 軸刻度文字 (動態生成) ---
        Repeater {
            id:y_axisNum
            model: yDivisions + 1
            Text {
                property real val: (yMax / yDivisions) * index
                x: 0
                // 計算 Y 位置，需對齊軸線
                y: plotHeight - (val / yMax) * plotHeight + 5 // +5 是修正座標軸 y:5.61 的位移
                width: 45
                color: "#a3a3a3"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignRight
                text: val.toFixed(0)
            }
        }

        // --- X 軸刻度文字 (動態生成) ---
        Repeater {
            model: xDivisions + 1
            Text {
                property real val: ((xMax-xMin) / xDivisions) * index +xMin
                x: 52.86 + (index * (plotWidth / xDivisions)) - width/2
                y: 360 // 位於 X 軸下方
                width: 30
                color: "#a3a3a3"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignHCenter
                text: val.toFixed(0)
            }
        }

        // --- X 軸線 ---
        Shape {
            id: x_axis
            x: 52.86; y: 353
            ShapePath {
                strokeColor: "#f0f0f0"; strokeWidth: 1; fillColor: "transparent"
                PathLine { x: plotWidth; y: 0 }
            }
        }

        // --- Y 軸線 ---
        Shape {
            id: y_axis
            x: 52.26; y: 5.61
            ShapePath {
                strokeColor: "#f0f0f0"; strokeWidth: 1; fillColor: "transparent"
                PathLine { x: 0; y: plotHeight }
            }
        }

        // --- 動態折線 ---
        Shape {
            id: data_line
            x: 52.86; y: 35.61 // 這裡 y 的起點要跟 Y 軸一致
            width: plotWidth; height: plotHeight

            ShapePath {
                id: line_ShapePath0
                fillColor: "transparent"
                strokeColor: "#e59263"
                strokeWidth: 3
                capStyle: ShapePath.RoundCap

                PathSvg {
                    // 調用函數，根據數據生成路徑

                    path: chart_container.generatePath(chart_container.chartData)
                }
            }
        }
    }
}

