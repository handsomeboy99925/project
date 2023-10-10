#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::home)
{
    ui->setupUi(this);

    /*空调窗口*/
    wid_DeviceAC = new DeviceAC(this);
    //connect(wid_DeviceAC,SIGNAL(sig_sendData(int,QString)),my_tcpServer,SLOT(slot_deviceSendData(int,QString)));
    wid_DeviceAC->move(660,220);

    /*百叶窗窗口*/
    wid_DeviceCurtain = new DeviceCurtain(this);
    //connect(wid_DeviceCurtain,SIGNAL(sig_sendData(int,QString)),my_tcpServer,SLOT(slot_deviceSendData(int,QString)));
    wid_DeviceCurtain->move(1050,220);

    /*吊灯窗口*/
    wid_DeviceLamp = new DeviceLamp(this);
    //connect(wid_DeviceLamp,SIGNAL(sig_sendData(int,QString)),my_tcpServer,SLOT(slot_deviceSendData(int,QString)));
    wid_DeviceLamp->move(330,220);

    /*风扇窗口*/
    wid_DeviceFan = new DeviceFan(this);
    //connect(wid_DeviceFan,SIGNAL(sig_sendData(int,QString)),my_tcpServer,SLOT(slot_deviceSendData(int,QString)));
    wid_DeviceFan->move(330,530);

    /* 设置样式*/
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/png/background.png")));
    setPalette(pal);

    ui->label_headpic->setStyleSheet("border-image:url(:/png/avart.png)");
    ui->label_headpic->setText("");

    ui->frame_left->setStyleSheet("#frame_left{background-color:rgba(0,0,0,70%);}");

    ui->label_keting->setStyleSheet("color:#FFFFFF");
    ui->label_time_1->setStyleSheet("color:#FFFFFF");
    ui->label_time_2->setStyleSheet("color:#FFFFFF");
    ui->label_status->setStyleSheet("color:#FFFFFF");

    ui->label_fenge->setFixedWidth(3);
    ui->label_fenge->setText("");
    ui->label_fenge->setStyleSheet("background-color:#FFFFFF");

    /*获取时间*/
    ui->label_time_1->setText(QTime::currentTime().toString("hh:mm"));
    ui->label_time_2->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd ddd"));

}

Home::~Home()
{
    delete ui;
}

void Home::slot_timeout(void)
{
#if 0
    /*获取时间*/
    ui->label_time_1->setText(QTime::currentTime().toString("hh:mm"));
    ui->label_time_2->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd ddd"));


    if(livingroom_status == CONNECT)
    {
        ui->label_status->setText("状态：在线");
    }else
    {
        ui->label_status->setText("状态：离线");
    }
#endif
}
