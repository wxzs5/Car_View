#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "handledata.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);//禁止缩放
    setFixedSize(this->width(), this->height());

    //CCD图像初始话
    ui->ccd1GrayView->setChart(ccd1Data.chart);
    ui->ccd1GrayView->setRenderHint(QPainter::Antialiasing);
    ui->ccd1GrayView->show();
    ui->ccd1GraphicsView->setScene(ccd1Data.scene);
    ui->ccd1GraphicsView->show();

    ui->ccd2GrayView->setChart(ccd2Data.chart);
    ui->ccd2GrayView->setRenderHint(QPainter::Antialiasing);
    ui->ccd2GrayView->show();
    ui->ccd2GraphicsView->setScene(ccd2Data.scene);
    ui->ccd2GraphicsView->show();

    //网页相关
//    QWebEnginePage *page = new QWebEnginePage(this);

//    QWebChannel *channel = new QWebChannel(this);
//    channel->registerObject("handledata", &(this->cardata));
//    page->setWebChannel(channel);


    //查找可用串口
    this->on_btnFindPort_clicked();
    //默认波特率
    ui->BaudBox->setCurrentIndex(6);
    //关闭发送按钮
    ui->sendButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//清空接收窗口
void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

//发送字符框数据
void MainWindow::on_sendButton_clicked()
{
    serial->write(ui->textEdit_2->toPlainText().toLatin1());
}

//读取接收到的数据
void MainWindow::Read_Data()
{
    if(Qt::Checked==ui->checkBoxView->checkState())
    {
        QByteArray buf;
        buf = serial->readAll();
        if((QByteArray)NULL !=buf)
        {
            QString str = ui->textEdit->toPlainText();
            str+=tr(buf);
            ui->textEdit->clear();
            ui->textEdit->append(str);
        }
        buf.clear();
    }
}

//开车
void MainWindow::on_startCarButton_clicked()
{
    if(ui->startCarButton->text()==tr("StartCar"))
    {
        ui->startCarButton->setText(tr("StopCar"));
    }
    else
    {
        ui->startCarButton->setText(tr("StartCar"));
    }
}

//波形开关
void MainWindow::on_blueTooth_clicked()
{
    if(ui->blueTooth->text()==tr("显示波形"))
    {
        ui->blueTooth->setText(tr("关闭波形"));
        ui->webView->load(QUrl("file:///"+qApp->applicationDirPath()+"/html/line.html"));
    }
    else
    {
        ui->blueTooth->setText(tr("显示波形"));
        ui->webView->load(QUrl());
    }
}


//CCD波形显示选择
void MainWindow::on_ccd1CheckBox_clicked()
{
    if(Qt::Checked==ui->ccd1CheckBox->checkState()&&ui->openButton->text()==tr("关闭串口"))
    {
        ccd1Data.showGray();
    }
    else if(Qt::Unchecked==ui->ccd1CheckBox->checkState()&&ui->openButton->text()==tr("关闭串口"))
    {
        ccd1Data.series->hide();
        ccd1Data.scene->clear();
    }
}

void MainWindow::on_ccd2CheckBox_clicked()
{
    if(Qt::Checked==ui->ccd2CheckBox->checkState()&&ui->openButton->text()==tr("关闭串口"))
        ccd2Data.showGray();
    else if(Qt::Unchecked==ui->ccd2CheckBox->checkState()&&ui->openButton->text()==tr("关闭串口"))
    {
        ccd2Data.series->hide();
        ccd2Data.scene->clear();
    }
}

void MainWindow::on_variableButton_clicked()
{

}
