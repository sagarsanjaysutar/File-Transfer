import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

//: A page which shows incoming file requests
Popup{
    id: page
    focus: true

    // Start the receiving server
    onOpened: receivingServer.initServer()

    onClosed: receivingServer.stopServer()

    // Slide-in enter animation
    enter: Transition {
        NumberAnimation { 
            properties: "x";
            from: x + width;
            to: 0
            easing.type: Easing.InOutQuad;
            duration: 400;
        }
    }

    // Slide-out exit animation
    exit: Transition {
        NumberAnimation { 
            properties: "x";
            from: 0
            to: x + width;
            easing.type: Easing.InOutQuad;
            duration: 400;
        }
    }  

    background: Rectangle{
        anchors.fill: parent
    }

    contentItem: Item{
        id: content
        anchors.fill: parent
    }

    // An outer-circle border which keeps growing, disappearing & re-creating thereby resulting a probing effect.
    property Component probingCircle: Rectangle{
        anchors.centerIn: parent
        property var size: 400
        height: size 
        width: size
        radius: size / 2
        border.width: 8
        border.color: "#d3d3d3"

        // ===================================================================================
        // The following results in a probing effect.
        // 1. Initially the circle of the specified size is displayed.
        // 2. Then, the size of circle is increased.
        // 3. Then, the entire circle disappears.
        // 4. Lastly, the probingCircleExpansionTimer create a new circle & the loop goes on.
        // ===================================================================================

        Component.onCompleted:{
            // On creation, increase the size by 3
            width = width * 3
            height = height * 3
            radius = radius * 3

            // On creation, change the transparency to 0 so to make it disappear.
            border.width = 0
            border.color.a = 0
        }

        Behavior on height {
            PropertyAnimation {
                duration: probingCircleExpansionTimer.interval
                easing.type: Easing.Linear
            }
        }

        Behavior on width {
            PropertyAnimation {
                duration: probingCircleExpansionTimer.interval
                easing.type: Easing.Linear
            }
        }

        Behavior on radius {
            PropertyAnimation {
                duration: probingCircleExpansionTimer.interval
                easing.type: Easing.Linear
            }
        }

        Behavior on border.width {
            PropertyAnimation {
                duration: probingCircleExpansionTimer.interval
                easing.type: Easing.Linear
            }
        }

        Behavior on border.color {
            PropertyAnimation {
                duration: probingCircleExpansionTimer.interval
                easing.type: Easing.Linear
            }
        }
    }

    // The inner circles.
    property Component innerCircles: Repeater{
        model: 3

        Rectangle{
            id: radarBorder
            anchors.centerIn: parent
            property var size: 100 + (index * 100)
            height: size 
            width: size
            radius: size

            // Gives the inner most cirlce the highest index
            z: count - index    

            // Gives the inner most circle, the dark shade of grey
            color: Qt.rgba(0.827, 0.827, 0.827, (1 - (1 * (index * 0.3))))
        }
    }

    // Right panel for info.
    property Component rightPanel: Rectangle{
        anchors.right: parent.right
        width: parent.width/2
        height: parent.height
        color: "#FFFFFF"
        border.width: 0.5
        border.color: "#33d3d3d3"

        ColumnLayout{
            anchors.verticalCenter: parent.verticalCenter
            x: 15   // Margin
            width: parent.width
            
            spacing: 5
            Layout.alignment: Qt.AlignVCenter

            Text{
                id: title
                text: "Waiting for Sender..."
                color: "#606060"
                font{
                    capitalization: Font.MixedCase
                    pixelSize: 30
                    weight: Font.ExtraBold
                    family: "Poppins"
                }
            }

            Text{
                id: subtext
                text: "Started receiving server at " + discovery.localHostIPAddress
                color: "#606060"
                font{
                    capitalization: Font.MixedCase
                    pixelSize: 15
                    weight: Font.ExtraBold
                    family: "Poppins"
                }
            }

            //: Stop server button
            Button{
                id: sendBtn
                text: "Stop Server"
                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignLeft

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

                onClicked: close()
            }
        }        
    }

    // A timer for expanding & re-creating the outer circle.
    Timer{
        id: probingCircleExpansionTimer
        property int count: 0   // Helper counter.
        interval: 1000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if(count == 0){
                // Dynamically create the inner circles & right panel once.
                innerCircles.createObject(content, {z: 1})
                rightPanel.createObject(content, {z: 3})
            }            
            else{
                // Keep re-creating the outer circle.
                probingCircle.createObject(content, {z: 0})
            }
            count++
        }
    }
}