import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "./"

//: Main page with Send/Receive Buttons.
ApplicationWindow{
    id: window
    width: 800
    height: 500
    visible: true
    title: "File Transfer"

    background: Rectangle{
        anchors.fill: parent
    }

    //: Loads up the font.
    FontLoader {
        id: webFont
        source: "qrc:fonts/Roboto/Roboto-Regular.ttf"
    }

    //: Loader for "Send File" Page.
    Loader{
        id: sendFilePage
        width: window.width
        height: window.height

        sourceComponent: SendFile{
            parent: window.overlay
            width: window.width
            height: window.height
        } 
        active: false

        Connections{
            target: sendFilePage.item
            function onClosed(){ sendFilePage.active = false }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent

        //: Title
        Text{
            text: "File Transfer"
            color: "#606060"
            font{
                capitalization: Font.AllUppercase
                pixelSize: 30
                weight: Font.DemiBold
                family: "Roboto"
            }
        }

        //: Subtitle
        Text{
            text: "v1.0"
            color: "#282828"
            font{
                italic: true
                pixelSize: 15
                weight: Font.Light
                family: "Roboto"
            }
        }

        //: Send button
        Button{
            id: sendBtn
            text: "SEND"
            Layout.preferredWidth: 250
            Layout.preferredHeight: 40
            contentItem: Text{
                text: sendBtn.text
                font{
                    pixelSize: 20
                    family: "Roboto"
                }
                color: "#606060"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            } 

            background: Rectangle{
                width: sendBtn.width
                height: sendBtn.height
                color: "#d3d3d3"
                radius: 5
            }

            onClicked: { 
                sendFilePage.active = true
                sendFilePage.item.open()
            } 
        }
        
        //: Receive Button
        Button{
            id: receiveBtn
            text: "RECEIVE"
            Layout.preferredWidth: 250
            Layout.preferredHeight: 40
            contentItem: Text{
                text: receiveBtn.text
                font{
                    pixelSize: 20
                    family: "Roboto"
                }
                color: "#606060"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle{
                width: receiveBtn.width
                height: receiveBtn.height
                color: "#d3d3d3"
                radius: 5
            }

            onClicked: { receivingServer.receiveFile() }
        }
    }
}