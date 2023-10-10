#include "device_ac.h"
#include "ui_device_ac.h"

DeviceAC::DeviceAC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::device_ac)
{
    ui->setupUi(this);

    //数字滚动器
    my_digitalScroller = new DigitalScroller(this);
    connect(my_digitalScroller,SIGNAL(currentValueChanged(int)),this,SLOT(slot_temp_value_channged(int)));
    my_digitalScroller->move(275,100);
    my_digitalScroller->hide();

    //滑动按钮
    my_Slider_btn = new SliderButton(this);

    my_Slider_btn->set_button_color(QColor(36,110,202),QColor(188,188,188),QColor(255,255,255));
    my_Slider_btn->set_button_size(30,40);
    my_Slider_btn->move(230,15);
    connect(my_Slider_btn,SIGNAL(signal_button_off()),this,SLOT(slot_slider_OFF()));
    connect(my_Slider_btn,SIGNAL(signal_button_on()),this,SLOT(slot_slider_ON()));

    //外观设置
    this->setStyleSheet("#frame_background{background-color:#f4fcff;border-radius:15px}");
    ui->label_ac->setStyleSheet("border-image:url(:/png/5off.png)");
    ui->label_ac->setText("");

    ui->label_fenge->setFixedHeight(2);  //高度
    ui->label_fenge->setStyleSheet("border-image:url(:/png/6off.png)");
    ui->label_fenge->setText("");

    ui->btn_mode->setStyleSheet("border-image:url(:/png/7off.png)");
    ui->btn_speed->setStyleSheet("QPushButton{border-image:url(:/png/4off.png)}");

    ui->btn_auto->setStyleSheet("QPushButton:checked{border-image:url(:/png/16on.png)}"
                                "QPushButton:!checked{border-image:url(:/png/16off.png)}");
    ui->btn_dry->setStyleSheet("QPushButton:checked{border-image:url(:/png/9on.png)}"
                               "QPushButton:!checked{border-image:url(:/png/9off.png)}");
    ui->btn_cold->setStyleSheet("QPushButton:checked{border-image:url(:/png/15on.png)}"
                                "QPushButton:!checked{border-image:url(:/png/15off.png)}");
    ui->btn_wind->setStyleSheet("QPushButton:checked{border-image:url(:/png/12on.png)}"
                                "QPushButton:!checked{border-image:url(:/png/12off.png)}");

    ui->btn_xiao->setStyleSheet("QPushButton:checked{border-image:url(:/png/14on.png)}"
                                "QPushButton:!checked{border-image:url(:/png/14off.png)}");
    ui->btn_low->setStyleSheet("QPushButton:checked{border-image:url(:/png/13on.png)}"
                               "QPushButton:!checked{border-image:url(:/png/13off.png)}");
    ui->btn_mid->setStyleSheet("QPushButton:checked{border-image:url(:/png/10on.png)}"
                               "QPushButton:!checked{border-image:url(:/png/10off.png)}");
    ui->btn_high->setStyleSheet("QPushButton:checked{border-image:url(:/png/11on.png)}"
                                "QPushButton:!checked{border-image:url(:/png/11off.png)}");


}

DeviceAC::~DeviceAC()
{
    delete ui;
}


/***********************************
 *slot_slider_ON
 *功能: 总开关-开
 *说明：无
***********************************/
void DeviceAC::slot_slider_ON(void)
{
    qDebug() << "空调开";
    //emit sig_sendData(g_socket_map.value(room_name),"<*06,102,01*>");


    ui->btn_auto->setCheckable(true);
    ui->btn_dry->setCheckable(true);
    ui->btn_cold->setCheckable(true);
    ui->btn_wind->setCheckable(true);
    ui->btn_xiao->setCheckable(true);
    ui->btn_low->setCheckable(true);
    ui->btn_mid->setCheckable(true);
    ui->btn_high->setCheckable(true);

    ui->btn_auto->setEnabled(true);
    ui->btn_dry->setEnabled(true);
    ui->btn_cold->setEnabled(true);
    ui->btn_wind->setEnabled(true);
    ui->btn_xiao->setEnabled(true);
    ui->btn_low->setEnabled(true);
    ui->btn_mid->setEnabled(true);
    ui->btn_high->setEnabled(true);

    ui->btn_auto->setChecked(true);
    ui->btn_xiao->setChecked(true);

    ui->label_ac->setStyleSheet("border-image:url(:/png/5on.png)");
    ui->label_fenge->setStyleSheet("border-image:url(:/png/6on.png)");

    ui->btn_mode->setStyleSheet("border-image:url(:/png/7on.png)");
    ui->btn_speed->setStyleSheet("QPushButton{border-image:url(:/png/4on.png)}");

    ui->label_onoff->setStyleSheet("color:rgb(36,110,202)");
    ui->label_onoff->setText("ON");

    my_digitalScroller->show();


}

/***********************************
 *名称：slot_slider_OFF
 *功能: 总开关-关
 *说明：无
***********************************/
void DeviceAC::slot_slider_OFF(void)
{
    qDebug() << "空调关";
    //emit sig_sendData(g_socket_map.value(room_name),"<*06,102,00*>");


    ui->btn_auto->setChecked(false);
    ui->btn_dry->setChecked(false);
    ui->btn_cold->setChecked(false);
    ui->btn_wind->setChecked(false);
    ui->btn_xiao->setChecked(false);
    ui->btn_low->setChecked(false);
    ui->btn_mid->setChecked(false);
    ui->btn_high->setChecked(false);

    ui->btn_auto->setEnabled(false);
    ui->btn_dry->setEnabled(false);
    ui->btn_cold->setEnabled(false);
    ui->btn_wind->setEnabled(false);
    ui->btn_xiao->setEnabled(false);
    ui->btn_low->setEnabled(false);
    ui->btn_mid->setEnabled(false);
    ui->btn_high->setEnabled(false);

    ui->label_ac->setStyleSheet("border-image:url(:/png/5off.png)");
    ui->label_fenge->setStyleSheet("border-image:url(:/png/6off.png)");

    ui->btn_mode->setStyleSheet("border-image:url(:/png/7off.png)");
    ui->btn_speed->setStyleSheet("QPushButton{border-image:url(:/png/4off.png)}");

    ui->label_onoff->setStyleSheet("color:rgb(40,40,40)");
    ui->label_onoff->setText("OFF");

    my_digitalScroller->hide();

}
