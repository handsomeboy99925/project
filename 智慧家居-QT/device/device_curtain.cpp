#include "device_curtain.h"
#include "ui_device_curtain.h"

DeviceCurtain::DeviceCurtain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::device_curtain)
{
    ui->setupUi(this);

    //滑动按钮
    my_Slider_btn = new SliderButton(this);
    my_Slider_btn->set_button_color(QColor(36,110,202),QColor(188,188,188),QColor(255,255,255));
    my_Slider_btn->set_button_size(30,40);
    my_Slider_btn->move(230,15);
    connect(my_Slider_btn,SIGNAL(signal_button_off()),this,SLOT(slot_slider_OFF()));
    connect(my_Slider_btn,SIGNAL(signal_button_on()),this,SLOT(slot_slider_ON()));

   // ui->label_curtain->setFixedSize(ui->widget_pic->size());
    //ui->label_curtain->move(0,0);


    this->setStyleSheet("#frame_background{background-color:#f4fcff;border-radius:15px}");
    ui->label_curtain->setStyleSheet("border-image:url(:/png/2off.png)");
    ui->label_curtain->setText("");

    ui->label_lan->setFixedHeight(15);
    ui->label_lan->setStyleSheet("border-image:url(:/png/1off.png)");
    ui->label_lan->setText("");

    ui->btn_up->setStyleSheet("border-image:url(:/png/8off.png)");
    ui->btn_down->setStyleSheet("border-image:url(:/png/17off.png)");

    //动画类实例化
    aniamtion_up = new QPropertyAnimation(ui->label_curtain,"pos");
    aniamtion_down = new QPropertyAnimation(ui->label_curtain,"pos");

    //模拟定时器，每隔data_timer()秒执行slot
    data_timer = new QTimer(this);
    connect(data_timer,SIGNAL(timeout()),this,SLOT(slot_dataChanged()));

}


DeviceCurtain::~DeviceCurtain()
{
    delete ui;
}


/***********************************
 *名称：on_btn_up_pressed
 *功能：按下时开始动画
 *说明：无
***********************************/
void DeviceCurtain::on_btn_up_pressed()
{

    int start_x = ui->label_curtain->pos().x();//动画开始x坐标
    int start_y = ui->label_curtain->pos().y();//动画开始y坐标
    int end_x = ui->label_curtain->pos().x();//动画结束x坐标
    int end_y = 0 - ui->label_curtain->height();//动画结束y坐标
    int run_time = (ui->label_curtain->height() - abs(ui->label_curtain->pos().y()))/5.0 * 200;//动画持续时间

    aniamtion_up->setStartValue(QPoint(start_x,start_y));
    aniamtion_up->setEndValue(QPoint(end_x,end_y));
    aniamtion_up->setDuration(run_time);
    aniamtion_up->start();

    data_timer->start(100);  //0.1秒
}

/***********************************
 *名称：on_btn_up_released
 *功能：鼠标释放时结束动画
 *说明：无
***********************************/
void DeviceCurtain::on_btn_up_released()
{
        aniamtion_up->stop();
        data_timer->stop();

        QString sendData = "<*06,107," + ui->lbl_data->text().replace(" %","") + "*>";
        //emit sig_sendData(g_socket_map.value(room_name),sendData);
}

/***********************************
 *名称：on_btn_down_pressed
 *功能：按下时开始动画
 *说明：无
***********************************/
void DeviceCurtain::on_btn_down_pressed()
{
    int start_x = ui->label_curtain->pos().x(); //动画开始x坐标
    int start_y = ui->label_curtain->pos().y(); //动画开始y坐标
    int end_x = ui->label_curtain->pos().x();   //动画结束x坐标
    int end_y = 0;                          //动画结束y坐标
    int run_time = abs(start_y)/5.0 * 200;  //动画持续时间

    aniamtion_down->setStartValue(QPoint(start_x,start_y));
    aniamtion_down->setEndValue(QPoint(end_x,end_y));
    aniamtion_down->setDuration(run_time);
    aniamtion_down->start();
    data_timer->start(100);
}
/***********************************
 *名称：on_btn_down_released
 *功能：鼠标释放时结束动画
 *说明：无
***********************************/
void DeviceCurtain::on_btn_down_released()
{
    aniamtion_down->stop();
    data_timer->stop();

    QString sendData = "<*06,107," + ui->lbl_data->text().replace(" %","") + "*>";
    //emit sig_sendData(g_socket_map.value(room_name),sendData);

}

/***********************************
 *名称：slot_dataChanged
 *功能：数据变化触发槽函数,显示当前窗帘百分比
 *说明：无
***********************************/
void DeviceCurtain::slot_dataChanged(void)
{
    int scale = abs(ui->label_curtain->pos().y()) * 1.0 /ui->label_curtain->height() * 100;
    ui->lbl_data->setText(QString::number(scale) + " %");
}

/***********************************
 *功能: 总开关-开
***********************************/
void DeviceCurtain::slot_slider_ON(void)
{
    qDebug() << "百叶窗开";
    //emit sig_sendData(g_socket_map.value(room_name),"<*06,106,01*>");

    ui->label_curtain->setStyleSheet("border-image:url(:/png/2on.png)");
    ui->label_lan->setStyleSheet("border-image:url(:/png/1on.png)");

    ui->btn_up->setStyleSheet("border-image:url(:/png/8on.png)");
    ui->btn_down->setStyleSheet("border-image:url(:/png/17on.png)");

    ui->label_onoff->setStyleSheet("color:rgb(36,110,202)");
    ui->label_onoff->setText("ON");

}

/***********************************
 *功能: 总开关-关
***********************************/
void DeviceCurtain::slot_slider_OFF(void)
{
    qDebug() << "百叶窗关";
    //emit sig_sendData(g_socket_map.value(room_name),"<*06,106,00*>");

    ui->label_curtain->setStyleSheet("border-image:url(:/png/2off.png)");
    ui->label_lan->setStyleSheet("border-image:url(:/png/1off.png)");

    ui->btn_up->setStyleSheet("border-image:url(:/png/8off.png)");
    ui->btn_down->setStyleSheet("border-image:url(:/png/17off.png)");


    ui->label_onoff->setStyleSheet("color:rgb(40,40,40)");
    ui->label_onoff->setText("OFF");


}
