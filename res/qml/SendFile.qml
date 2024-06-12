import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

//: A page which shows a File Explorer to select files to send, followed by a list of devices available on network.
Popup{
    id: page
    focus: true

    background: Rectangle{
        anchors.fill: parent
    }

    //: Open the File Explorer
    onOpened: fileDialog.open() 

    contentItem: Item{
        
        //: File Explorer Window
        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            acceptLabel: "Send"
            currentFolder: "/"
            fileMode: FileDialog.OpenFiles
            onAccepted: {
                console.log("You chose: " + fileDialog.selectedFiles)
                loadingBox.active = true
                discovery.initHostInterfaces();
            }
            onRejected: page.close();
        }

        //: Loading component
        Loader{
            id: loadingBox
            // anchors.fill: parent
            anchors.centerIn: parent
            sourceComponent: LoadingBox{
                height: 100
                width: 100                
            }
            active: false
        }

        //: List of devices on the network
        ListView{
            id: hostList

            //: Hosts/devices founds on the network.
            property var hosts: discovery.hosts
            onHostsChanged: {
                loadingBox.active = false

                //: Add devices to the listview model.
                for(var host in hosts){
                    interfaceList.add({"ipAddr": hosts[host].getIpAddressStr()});
                }
            }

            anchors.centerIn: parent
            width: parent.width * 0.80
            height: parent.height * 0.80
            spacing: 4
            clip: true
            visible: hosts.length > 0

            //: Title
            header: Column{
                id: hostListHeader
                width: parent.width
                height: hostList.height * 0.20
                visible: hosts.length > 0
                spacing: 5

                Text{
                    text: "Select Device"
                    color: "#606060"
                    font{
                        capitalization: Font.AllUppercase
                        pixelSize: 30
                        weight: Font.DemiBold
                        family: "Roboto"
                    }
                }

                Text{
                    text: "to send: <i>" + fileDialog.selectedFiles + "</i>"
                    font{
                        capitalization: Font.AllUppercase
                        pixelSize: 15
                        weight: Font.Normal
                        family: "Roboto"
                    }
                }
            }

            //: UI for each of devices found.
            delegate: Button{
                id: delegateRec
                width: parent.width
                height: 50
                text: ipAddr

                background: Rectangle{
                    id: delegateBackground
                    width: delegateRec.width
                    height: delegateRec.height
                    radius: 5
                    border{
                        color: "#d3d3d3"
                        width: 1
                    }
                }

                contentItem: Text {
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: delegateRec.text
                    font{
                        capitalization: Font.AllUppercase
                        pixelSize: 15
                        weight: Font.DemiBold
                        family: "Roboto"
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