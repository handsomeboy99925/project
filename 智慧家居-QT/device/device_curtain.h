#ifndef DEVICE_CURTAIN_H
#define DEVICE_CURTAIN_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QTimer>
#include <QDebug>
#include "my_custom/sliderbutton.h"


namespace Ui {
class device_curtain;
}

class DeviceCurtain : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceCurtain(QWidget *parent = nullptr);
    ~DeviceCurtain();

    SliderButton *my_Slider_btn;

    QPropertyAnimation* aniamtion_up;
    QPropertyAnimation* aniamtion_down;
    QTimer* data_timer;

signals:

private:
    Ui::device_curtain *ui;

public slots:
    void slot_slider_OFF(void);
    void slot_slider_ON(void);

    void on_btn_down_pressed();

    void on_btn_down_released();

    void on_btn_up_pressed();

    void on_btn_up_released();

    void slot_dataChanged();

};

#endif // DEVICE_CURTAIN_H
