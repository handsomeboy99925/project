#ifndef DEVICE_FAN_H
#define DEVICE_FAN_H

#include <QWidget>
#include "my_custom/global.h"
#include "my_custom/sliderbutton.h"

namespace Ui {
class device_fan;
}

class DeviceFan : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceFan(QWidget *parent = nullptr);
    ~DeviceFan();

    SliderButton* my_Slider_btn;

signals:
    void sig_sendData(int sockID,QString sendData);

public slots:
    void slot_slider_OFF(void);
    void slot_slider_ON(void);

private:
    Ui::device_fan *ui;
};

#endif // DEVICE_FAN_H
