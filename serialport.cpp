#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "handledata.h"



//解析串口
void MainWindow::readCCDGrap()
{
    int ii=0,cnt=0;
    qint64 len=0,read_len=0;
    static QByteArray serialData,restData;
    QByteArray serialTempData=serial->readAll();
    if(!restData.isEmpty())
    {
        serialData.append(restData);
        restData.clear();
    }
    serialData.append(serialTempData);
    serialTempData.clear();
    if(serialData.size()>260)
    {
        len=serialData.size();
        ui->statusBar->showMessage(tr("Received Data!"),200);
        while(ii<len)
        {
            if(ii<len-131)
            {

                if(serialData.at(ii)=='*'&&serialData.at(ii+1)=='z')    //CCD1灰度图
                {
                    ii=ii+2;
                    read_len=128;
                    cnt=0;
                    while(read_len>0)
                    {
                        ccd1Data.ccdGray[cnt++]=(uchar)serialData.at(ii++);
                        read_len--;
                    }
                    if(Qt::Checked==ui->ccd1CheckBox->checkState())
                        ccd1Data.showGray();
                }
                else if(serialData.at(ii)=='*'&&serialData.at(ii+1)=='y')//CCD2灰度图
                {
                    ii=ii+2;
                    read_len=128;
                    cnt=0;
                    while(read_len>0)
                    {
                        ccd2Data.ccdGray[cnt++]=(uchar)serialData.at(ii++);
                        read_len--;
                    }
                    if(Qt::Checked==ui->ccd2CheckBox->checkState())
                        ccd2Data.showGray();
                }
                else
                {
                    ii++;
                }
            }

            else
            {
                restData=serialData.mid(ii,len);
                break;
            }

        }
        serialData.clear();
    }
}



//打开/关闭串口
void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //设置串口缓冲区大小
//        serial->setReadBufferSize(500);


        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));
        ui->startCarButton->setText(tr("StartCar"));
        ui->blueTooth->setText(tr("显示波形"));
        ui->sendButton->setEnabled(true);
        ui->btnFindPort->setEnabled(false);

        ui->blueTooth->setEnabled(true);
        ui->steerSendButton->setEnabled(true);
        ui->steerGetButton->setEnabled(true);
        ui->motorGetButton->setEnabled(true);
        ui->motorSendButton->setEnabled(true);
        ui->startCarButton->setEnabled(true);
        ui->modelComboBox->setEnabled(true);


        //判断显示CCD曲线信息
        if(Qt::Checked==ui->ccd1CheckBox->checkState())
            ccd1Data.showGray();
        if(Qt::Checked==ui->ccd2CheckBox->checkState())
            ccd2Data.showGray();
        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::readCCDGrap);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->startCarButton->setText(tr("StartCar"));
        ui->blueTooth->setText(tr("显示波形"));
        ui->sendButton->setEnabled(false);
        ui->btnFindPort->setEnabled(true);

        ui->blueTooth->setEnabled(false);
        ui->steerSendButton->setEnabled(false);
        ui->steerGetButton->setEnabled(false);
        ui->motorGetButton->setEnabled(false);
        ui->motorSendButton->setEnabled(false);
        ui->startCarButton->setEnabled(false);
        ui->modelComboBox->setEnabled(false);
    }
}



//查找可用的串口
void MainWindow::on_btnFindPort_clicked()
{
       ui->PortBox->clear();
       foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
       {
           QSerialPort serial;
           serial.setPort(info);
           if(serial.open(QIODevice::ReadWrite))
           {
               ui->PortBox->addItem(serial.portName());
               serial.close();
           }
       }
}