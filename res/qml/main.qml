import QtQuick
import QtQuick.Layouts

Window{
    width: 500
    height: 500
    visible: true
    title: "File Transfer"

    FontLoader {
        id: webFont
        source: "qrc:fonts/Roboto/Roboto-Regular.ttf"
    }

    GridLayout {
        rows: 2
        columns: 3
        anchors.centerIn: parent

        Column{
            Layout.row: 0

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
        }

        Row{
            Layout.row: 1
            spacing: 5

            Rectangle{
                width: 200
                height: 40
                color: "#d3d3d3"
                radius: 5

                Text{
                    text: "SEND"
                    color: "#606060"
                    // Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    anchors.centerIn: parent

                    font{
                        pixelSize: 20
                        family: "Roboto"
                    }
                }
            }        

            Rectangle{
                width: 200
                height: 40
                color: "#d3d3d3"            
                radius: 5

                Text{
                    text: "RECEIVE"
                    color: "#606060"
                    // Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    anchors.centerIn: parent

                    font{
                        pixelSize: 20
                        family: "Roboto"
                    }
                }
            }
        }
    }
}