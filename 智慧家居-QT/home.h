#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QTimer>

#include "tcp/tcpserver.h"
#include "device/device_ac.h"
#include "device/device_curtain.h"
#include "device/device_lamp.h"
#include "device/device_fan.h"
#include "my_custom/global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class home; }
QT_END_NAMESPACE

class Home : public QWidget
{
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

    DeviceAC* wid_DeviceAC;
    DeviceCurtain *wid_DeviceCurtain;
    DeviceLamp * wid_DeviceLamp;
    DeviceFan * wid_DeviceFan;

private:
    Ui::home *ui;

public slots:
    void slot_timeout(void);
};
#endif // HOME_H
