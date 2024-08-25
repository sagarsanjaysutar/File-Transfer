#include "DeviceDiscovery.h"
#include "networknamespace.h"

DeviceDiscovery::DeviceDiscovery(QSharedPointer<DeviceInterface> interface, QObject *parent) : QObject(parent),
                                                                                               m_interface(interface)
{
    qDebug() << "DeviceDiscovery: Constructor called.";
}

DeviceDiscovery::~DeviceDiscovery()
{
    qDebug() << "DeviceDiscovery: Destructor called.";
}

void DeviceDiscovery::slot_startDeviceDiscovery()
{

    qDebug() << "DeviceDiscovery: Starting the discovery process...";

    // List of empty interfaces.
    QList<QSharedPointer<DeviceInterface>> devicesOnNetwork;

    // nmap response
    QString nmapResp;

    // Initialise nmap process to search devices on localhost network.
    QProcess *nmapProcess = new QProcess();
    QObject::connect(nmapProcess, &QProcess::started, [=]()
                     { qDebug() << "DeviceDiscovery: Discovery execution started."; });
    QObject::connect(nmapProcess, &QProcess::finished, [=](int exitCode, QProcess::ExitStatus exitStatus)
                     { qDebug() << "DeviceDiscovery: Discovery execution finished." << exitCode << exitStatus; });
    QObject::connect(nmapProcess, &QProcess::readyReadStandardOutput, [&]()
                     { nmapResp.append(nmapProcess->readAllStandardOutput()); });
    QObject::connect(nmapProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
                     { qDebug() << "DeviceDiscovery: Error Occured " << error; });
    QObject::connect(nmapProcess, &QProcess::stateChanged, [=](QProcess::ProcessState state)
                     { qDebug() << "DeviceDiscovery: State Changed " << state; });

    // nmap command.
    QString cmd;
    QTextStream(&cmd) << "nmap"
                      << " " << "-p"
                      << " " << QString::number(Network::PORT)
                      << " " << "--exclude"
                      << " " << m_interface.data()->getGatewayAddress().toString()
                      << " " << m_interface.data()->getCIDRAddress();

    qDebug() << "Starting nmap process: " << cmd;

    nmapProcess->setProgram("nmap");
    nmapProcess->setArguments(QStringList()
                              << "-p" << QString::number(Network::PORT)
                              << "--exclude"
                              << m_interface.data()->getGatewayAddress().toString()
                              << m_interface.data()->getCIDRAddress());
    nmapProcess->start();

    // Wait for the process to start
    if (!nmapProcess->waitForStarted())
    {
        qDebug() << "DeviceDiscovery: Failed to start the discovery process.";
        emit sig_discoveredDevicesOnNetwork(devicesOnNetwork);
    }

    // Wait until the process is completed.
    if (!nmapProcess->waitForFinished())
    {
        qDebug() << "DeviceDiscovery: Discovery process timedout.";
        emit sig_discoveredDevicesOnNetwork(devicesOnNetwork);
    }

    // Misc. cleanup
    delete nmapProcess;

    // Populate hostsOnInterface with the generated IP address from nmap process.
    // Note: All the IP address found by a nmap search would have same mask address.
    for (QHostAddress ipAddr : parseNmapResp(nmapResp))
    {
        devicesOnNetwork.append(QSharedPointer<DeviceInterface>(new DeviceInterface(ipAddr, m_interface.data()->getMaskAddress())));
    }

    qDebug() << "DeviceDiscovery: Discovery execution completed. Found" << devicesOnNetwork.count() << " devices on" << m_interface.data()->getIpAddress().toString();
    emit sig_discoveredDevicesOnNetwork(devicesOnNetwork);
}

QList<QHostAddress> DeviceDiscovery::parseNmapResp(QString resp)
{

    if (resp.isEmpty())
    {
        qDebug() << "DeviceDiscovery: Nmap response is empty.";
        return QList<QHostAddress>();
    }

    // Extract list of IP addresses from the nmap response.
    QList<QHostAddress> deviceAddressList = QList<QHostAddress>();

    // Parse the nmap response. Refer the nmap output to understand the following logic
    QStringList dataList = resp.split("\n");
    int idx = 0;
    while (idx < dataList.count())
    {
        QString line = dataList.at(idx);
        qDebug() << line;
        if (line.contains("Nmap scan report"))
        {
            QString portLine = dataList.at(idx + 4);

            if (portLine.contains(QString::number(Network::PORT)) && portLine.contains("open"))
            {
                // Regex to fetch the IP address.
                QRegularExpression rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                QRegularExpressionMatch ipMatch = rx.match(line);
                deviceAddressList << QHostAddress(ipMatch.captured(0));
                // qDebug() << ipMatch.captured(0);
            }
        }

        idx++;
    }

    return deviceAddressList;
}