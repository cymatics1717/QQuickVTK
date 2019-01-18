import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.0

import QMLVtk 1.0

ApplicationWindow {
    id: root
    visible: true
    width: Screen.width *.6
    height: Screen.height*.6
    title: qsTr("Hello")

    VTKScene {
        id: scene
        anchors.fill: parent
        focus: true
    }

    Label {
        id: info
        visible: false
        width: textMetrics.boundingRect.width
//        anchors.centerIn:  parent
        height: 50
        color: "blue"
        background: Rectangle{
            color: "#21be2b"
            border.color: "yellow"
            border.width: 2
            radius: 5
        }
        TextMetrics {
            id: textMetrics
            text: info.text
            font: info.font
        }
        verticalAlignment: Text.AlignVCenter
        text: "hello,world"
        font.pixelSize: 20
        z:1
        RotationAnimation on rotation {
            loops: Animation.Infinite
            from: 0
            to: 360
            duration: notice.interval
        }
    }


    Connections {
        target: scene;
        onSelectedModel:{
            console.log("_____" + modelFileName)
            info.visible = true;
            info.text = modelFileName
            info.x = Math.random()*root.width/3+root.width/4
            info.y = root.height/2
            notice.start()
        }
    }

    FileDialog {
          id: fileDialog
          title: "Open a file"
//          folder: shortcuts.documents
//          selectFolder: true
          width: root.width
          onAccepted: {
              console.log("You chose: " + fileDialog.fileUrl)
              scene.openFile(fileDialog.fileUrl)
          }
          onRejected: {
              console.log("Canceled")
          }
          Component.onCompleted: visible = false
      }

    header: ToolBar {
        id: bar
        contentHeight: 40
        ToolButton {
            id:fileButton
            text: qsTr("Open")
            onClicked: {
                menu.open()
            }
            Menu {
                id: menu
                y: fileButton.height
                MenuItem {
                    text: "Open file"
                    onTriggered: {
                        fileDialog.selectFolder = false;
                        fileDialog.visible = true;
                    }
                }
                MenuItem {
                    text: "Open directory"
                    onTriggered: {
                        fileDialog.selectFolder = true;
                        fileDialog.visible = true;
                    }
                }
            }

        }
        Image {
            height: bar.height
            anchors.right: parent.right
            fillMode: Image.PreserveAspectFit
            source: "qrc:/res/puffin.jpeg"
        }
    }

    footer: ToolBar {
        id: foo
        height:  20

        RowLayout{
            anchors.fill: parent
            Label {
                id:corner
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    Timer {
        id: timer
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            corner.text = Date().toString()
        }
    }

    Timer {
        id: notice
        interval: 2000;
        onTriggered: {
            corner.text = Date().toString()
            info.visible = false;
        }
    }

}
