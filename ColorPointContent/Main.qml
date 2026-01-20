import QtQuick
import QtQuick.Controls
import Core 1.0

MainScreen {
    id: mainScreen

    linkBtnMouseArea.onClicked: {
        Cp.connectBtn = true
        console.log("linkBtnMouseArea clicked",Cp.connectBtn)
    }

    confirmBtnMouseArea.onClicked: {
        if(Cp.integTime !== Number(confirmTextField.text)){
            Cp.integTime = Number(confirmTextField.text)
            console.log("integrationTime:",Cp.integTime,Number(confirmTextField.text))
        }
    }
    autoSettingBtnMouseArea.onClicked: {
        if(Cp.integTime !== Number(confirmTextField.text)){
            Cp.integTime = Number(confirmTextField.text)
            console.log("integrationTime:",Cp.integTime,Number(confirmTextField.text))
        }
    }
    scanBtnMouseArea.onClicked: {
        Cp.scanBtn = true
        console.log("scanBtnMouseArea clicked",Cp.scanBtn)
    }
    whiteScanBtnMouseArea.onClicked: {
        Cp.whiteBtn = true
        console.log("whiteScanBtnMouseArea clicked",Cp.whiteBtn)
    }
    saveFileBtnMouseArea.onClicked: {
        if (lableTextField.text !== labelText  ){
            Cp.label = lableTextField.text
            console.log("lableTextField: ", labelText,Cp.label ,lableTextField.text)
        }

        if (fileNameTextField.text !== fileNameText  ){
            Cp.fileName = fileNameTextField.text
            console.log("fileNameTextField: ",Cp.fileName,fileNameText,fileNameTextField.text )
        }

    }
}
