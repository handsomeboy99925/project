#ifndef DEVICE_AC_H
#define DEVICE_AC_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include "my_custom/digital_scroller.h"
#include "my_custom/sliderbutton.h"
#include "tcp/tcpsocket.h"

#include "my_custom/global.h"

namespace Ui {
class device_ac;
}

class DeviceAC : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceAC(QWidget *parent = nullptr);
    ~DeviceAC();


    DigitalScroller* my_digitalScroller;
    SliderButton* my_Slider_btn;

private slots:
    void slot_slider_OFF(void);
    void slot_slider_ON(void);

signals:
    //void sig_sendData(int sockID,QString sendData);

private:
    Ui::device_ac *ui;
};

#endif // DEVICE_AC_H
