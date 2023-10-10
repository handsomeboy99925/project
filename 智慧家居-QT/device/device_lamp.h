#ifndef DEVICE_LAMP_H
#define DEVICE_LAMP_H

#include <QWidget>
#include "my_custom/global.h"
#include "my_custom/sliderbutton.h"

namespace Ui {
class device_lamp;
}

class DeviceLamp : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceLamp(QWidget *parent = nullptr);
    ~DeviceLamp();

    SliderButton* my_Slider_btn;

signals:
    void sig_sendData(int sockID,QString sendData);

private:
    Ui::device_lamp *ui;

public slots:
    void slot_slider_OFF(void);
    void slot_slider_ON(void);



};

#endif // DEVICE_LAMP_H
