import QtQuick

//: Loading box
Rectangle{

    property string loadingText: "Loading..."

    id: container
    width: 300
    height: width
    anchors.centerIn: parent
    color: "#282828"
    z: 0

    Text{
        anchors.top: container.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: container.horizontalCenter
        text: container.loadingText
        color: "#606060"
        font{
            capitalization: Font.MixedCase
            pixelSize: 15
            weight: Font.Normal
            family: "Poppins"
        }
    }

    //: A dot which move within the container.
    Rectangle{
        
        property int dotSize: container.width/3
        
        id: dot1
        width: dotSize
        height: dotSize
        radius: dotSize
        z: 1

        //: Start point
        x: container.x
        y: container.y

        color: "#FFFFFF"
        state: "topLeft"
        
        states: [
            State{
                //: Changing the state to "topLeft" brings it to the initial point.
                name: "topLeft"
                PropertyChanges{
                    target: dot1
                }
            },
            State{
                //: Changing the state to "topRight", moves the dot from "topLeft" to "topRight". 
                name: "topRight"
                PropertyChanges{
                    target: dot1
                    x: container.x + container.width - dotSize
                }
            },
            State{
                //: Changing the state to "bottomRight", moves the dot from "topRight" to "bottomRight".
                name: "bottomRight"
                PropertyChanges{
                    target: dot1
                    x: container.x + container.width - dotSize
                    y: container.y + container.height - dotSize
                }
            },
            State{
                //: Changing the state to "bottomLeft", moves the dot from "bottomRight" to "bottomLeft".
                name: "bottomLeft"
                PropertyChanges{
                    target: dot1
                    y: container.y + container.height - dotSize
                }
            }
        ]
        
        //: Adds a bouncing animation while moving.
        transitions:[
            Transition{
                PropertyAnimation{
                    properties: "x,y"
                    duration: 900
                    easing.type: Easing.OutBounce
                }
            }
        ]
    }
    
    //: Timer which keeps changing the state of the "dot" to move it from one end to another.
    Timer{
        id: dotDispatchTimer
        repeat: true
        running: true
        interval: 1000
        onTriggered: {
            if(dot1.state == "topLeft")
                dot1.state = "topRight"
            else if(dot1.state == "topRight")
                dot1.state = "bottomRight"
            else if(dot1.state == "bottomRight")
                dot1.state = "bottomLeft"
            else
                dot1.state = "topLeft"
        }
    }
}
