#include "device_fan.h"
#include "ui_device_fan.h"

DeviceFan::DeviceFan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::device_fan)
{
    ui->setupUi(this);

    //滑动按钮
    my_Slider_btn = new SliderButton(this);
    my_Slider_btn->set_button_color(QColor(36,110,202),QColor(188,188,188),QColor(255,255,255));
    my_Slider_btn->set_button_size(30,40);
    my_Slider_btn->move(170,15);
    connect(my_Slider_btn,SIGNAL(signal_button_off()),this,SLOT(slot_slider_OFF()));
    connect(my_Slider_btn,SIGNAL(signal_button_on()),this,SLOT(slot_slider_ON()));

    this->setStyleSheet("#frame_background{background-color:#f4fcff;border-radius:15px}");
    ui->label_fan->setStyleSheet("border-image:url(:/png/3off.png)");
    ui->label_fan->setText("");
}

DeviceFan::~DeviceFan()
{
    delete ui;
}

/***********************************
 *slot_slider_ON
 *功能: 总开关-开
 *说明：无
***********************************/
void DeviceFan::slot_slider_ON(void)
{
    qDebug() << "风扇开";
    //emit sig_sendData(g_socket_map.value(room_name),"<*06,101,01*>");

    ui->label_fan->setStyleSheet("border-image:url(:/png/3on.png)");
    ui->label_onoff->setStyleSheet("color:rgb(36,110,202)");
    ui->label_onoff->setText("ON");

}

/***********************************
 *名称：slot_slider_OFF
 *功能: 总开关-关
 *说明：无
***********************************/
void DeviceFan::slot_slider_OFF(void)
{
    qDebug() << "风扇关";
    //emit sig_sendData(g_socket_map.value(room_name),"<*06,101,00*>");

    ui->label_fan->setStyleSheet("border-image:url(:/png/3off.png)");
    ui->label_onoff->setStyleSheet("color:rgb(40,40,40)");
    ui->label_onoff->setText("OFF");

}
