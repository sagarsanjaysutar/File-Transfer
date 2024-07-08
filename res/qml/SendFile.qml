import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

//: A page which shows a File Explorer to select files to send, followed by a list of devices available on network.
Popup{
    id: page
    focus: true

    // Slide-in enter animation for the entire page.
    enter: Transition {
        NumberAnimation { 
            properties: "x";
            from: x + width;
            to: 0
            easing.type: Easing.InOutQuad;
            duration: 400;
        }
    }

    // Slide-out exit animation for the entire page.
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

    //: Open the File Explorer
    onOpened: fileDialog.open() 

    contentItem: Item{
        
        //: #00. File Explorer Window will open first.
        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            acceptLabel: "Send"
            currentFolder: "/"
            fileMode: FileDialog.OpenFiles
            onAccepted: {
                console.log("You chose: " + fileDialog.selectedFiles)
                loadingBox.active = true
                discovery.startDeviceDiscovery();
            }
            onRejected: page.close()
            visible: false
        }

        //: #01. Loading component will show while searching for devices.
        Loader{
            id: loadingBox
            anchors.centerIn: parent
            sourceComponent: LoadingBox{
                height: 100
                width: 100
                loadingText: "Searching for devices on local network..."           
            }
            active: false
        }

        //: #02. List of discovered devices on the network will be shown
        ListView{
            id: hostList           

            //: Devices founds on the localhost network.
            property var devicesOnNetwork: discovery.devicesOnNetwork
            onDevicesOnNetworkChanged: {
                loadingBox.active = false
               
                //: Add devices to the listview model.
                for(var device in devicesOnNetwork){
                    interfaceList.add({"ipAddr": devicesOnNetwork[device].getIpAddressStr()});
                }
            }

            anchors.centerIn: parent
            width: parent.width * 0.80
            height: parent.height * 0.80
            spacing: 4
            clip: true
            visible: interfaceList.count > 0

            //: Transition for the list items to arise from bottom.
            add: Transition{
                NumberAnimation { 
                    properties: "y";
                    from: y + width;
                    easing.type: Easing.InOutQuad;
                    duration: 400;
                }
            }

            //: Title
            header: Column{
                id: hostListHeader
                width: parent.width
                height: hostList.height * 0.20
                visible: interfaceList.count > 0
                spacing: 5

                Text{
                    text: "Found " + interfaceList.count + " devices"
                    color: "#606060"
                    font{
                        capitalization: Font.MixedCase
                        pixelSize: 30
                        weight: Font.ExtraBold
                        family: "Poppins"
                    }
                }

                Text{
                    text: "Select a device to send: <i>" + fileDialog.selectedFiles + "</i> file."
                    color: "#606060"
                    font{
                        capitalization: Font.MixedCase
                        pixelSize: 15
                        weight: Font.DemiBold
                        family: "Poppins"
                    }
                }
            }

            //: UI for each of devices found.
            delegate: Button{
                id: delegateRec
                width: parent.width
                height: 50
                text: ipAddr
                // z: 3

                background: Rectangle{
                    id: delegateBackground
                    width: delegateRec.width
                    height: delegateRec.height
                    color: "#FFFFFF"
                    z: 1

                    Rectangle {
                        width: delegateRec.width
                        height: delegateRec.height + 2
                        color: "#d3d3d3"
                        z: -1
                    }
                }

                contentItem: Text {
                    z: 2
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: delegateRec.text
                    font{
                        capitalization: Font.MixedCase
                        pixelSize: 15
                        weight: Font.Bold
                        family: "Poppins"
                    }
                }

                onClicked:{
                    delegateBackground.color = "red"
                    console.log(ipAddr)
                }
            }

            model: ListModel{
                id: interfaceList

                // Verifies if the object is unique.
                function isDuplicate(newObj){
                    for(var i = 0; i < interfaceList.count; i++){
                        var existingObj = interfaceList.get(i);
                        if(existingObj.ipAddr === newObj.ipAddr){
                            return true;
                        }
                    }

                    return false;
                }

                // Adds object to the model.
                function add(newObj){
                    if(!isDuplicate(newObj))
                        interfaceList.append(newObj);
                }
            } 
        }
    }
}