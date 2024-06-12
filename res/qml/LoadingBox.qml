import QtQuick

//: Loading box
Rectangle{
    id: container
    width: 300
    height: 300
    anchors.centerIn: parent
    
    //: A black container box.
    Rectangle{
        id: mainBox
        anchors.centerIn: parent
        anchors.fill: parent
        color: "#282828"
    }
    
    //: A dot which move within the container.
    Rectangle{
        
        property int dotSize: mainBox.width/3
        
        id: dot1
        width: dotSize
        height: dotSize
        radius: dotSize

        //: Start point
        x: mainBox.x
        y: mainBox.y

        color: "#d3d3d3"
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
                    x: mainBox.x + mainBox.width - dotSize
                }
            },
            State{
                //: Changing the state to "bottomRight", moves the dot from "topRight" to "bottomRight".
                name: "bottomRight"
                PropertyChanges{
                    target: dot1
                    x: mainBox.x + mainBox.width - dotSize
                    y: mainBox.y + mainBox.height - dotSize
                }
            },
            State{
                //: Changing the state to "bottomLeft", moves the dot from "bottomRight" to "bottomLeft".
                name: "bottomLeft"
                PropertyChanges{
                    target: dot1
                    y: mainBox.y + mainBox.height - dotSize
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
