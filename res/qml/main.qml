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
        // /home/ssutar/Tutorials/File-Transfer/res/fonts/Poppins/Poppins-Light.ttf
        id: webFont
        source: "qrc:fonts/Poppins/Poppins-Regular.ttf"
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

    //: Loader for "Receive File" Page.
    Loader{
        id: receiveFilePage
        width: window.width
        height: window.height

        sourceComponent: ReceiveFile{
            parent: window.overlay
            width: window.width
            height: window.height
            
        } 
        active: false

        Connections{
            target: receiveFilePage.item
            function onClosed(){ receiveFilePage.active = false }
        }
    }

    RowLayout{
        anchors.fill: parent

        // Left panel containing send/receive buttons
        ColumnLayout {
            Layout.preferredWidth: parent.width/2
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            z: 1
            spacing: 5

            //: Title
            Text{
                text: "File Transfer"
                Layout.preferredWidth: parent.width
                horizontalAlignment: Text.AlignRight
                color: "#606060"

                font{
                    capitalization: Font.MixedCase
                    pixelSize: 30
                    weight: Font.ExtraBold
                    family: "Poppins"
                }
            }

            //: Subtitle
            Text{
                text: "Securely transfer files on a local network."
                Layout.preferredWidth: parent.width
                horizontalAlignment: Text.AlignRight
                color: "#606060"

                font{
                    italic: true
                    pixelSize: 15
                    weight: Font.DemiBold
                    family: "Poppins"
                }
            }

            //: Send button
            Button{
                id: sendBtn
                text: "Send"
                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignRight

                contentItem: Text{
                    text: sendBtn.text
                    font{
                        pixelSize: 20
                        weight: Font.Bold
                        family: "Poppins"
                    }
                    color: "#606060"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                } 

                background: Rectangle{
                    width: sendBtn.width
                    height: sendBtn.height
                    color: "#d3d3d3"
                    radius: 2
                }

                onClicked: { 
                    sendFilePage.active = true
                    sendFilePage.item.open()
                } 
            }
            
            //: Receive Button
            Button{
                id: receiveBtn
                text: "Receive"
                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignRight

                contentItem: Text{
                    text: receiveBtn.text
                    font{
                        pixelSize: 20
                        weight: Font.Bold
                        family: "Poppins"
                    }
                    color: "#606060"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle{
                    width: receiveBtn.width
                    height: receiveBtn.height
                    color: "#d3d3d3"
                    radius: 2
                }

                onClicked: { 
                    receiveFilePage.active = true
                    receiveFilePage.item.open()
                }
            }
        }

        //: Right panel containing UI Circles.
        Rectangle{
            id: rightPanel
            Layout.preferredWidth: parent.width/2
            Layout.preferredHeight: parent.height
            color: "transparent"
            border.width: 0.5
            border.color: "#33d3d3d3"
            z: 0

            Rectangle{
                anchors.top: parent.top
                anchors.topMargin: -100
                anchors.right: parent.right
                anchors.rightMargin: -100
                property int size: parent.width/1.2
                width: size
                height: size
                radius: size/2
                color: "transparent"
                border.width: 8
                border.color: "#d3d3d3"
            }


            Rectangle{
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -parent.height/2
                // anchors.right: parent.right
                // anchors.rightMargin: -100
                property int size: parent.width
                width: size
                height: size
                radius: size/2
                color: "transparent"
                border.width: 8
                border.color: "#d3d3d3"
            }

            Rectangle{
                anchors.centerIn: parent
                anchors.verticalCenterOffset: parent.width/8
                // anchors.bottomMargin: -parent.height/2
                // anchors.right: parent.right
                // anchors.rightMargin: -100
                property int size: parent.width/1.5
                width: size
                height: size
                radius: size/2
                color: "transparent"
                border.width: 8
                border.color: "#d3d3d3"
            }
            
        }
    }
}