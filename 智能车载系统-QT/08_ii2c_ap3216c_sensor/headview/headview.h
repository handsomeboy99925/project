#ifndef HEADVIEW_H
#define HEADVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


class HeadView : public QWidget
{
    Q_OBJECT

public:
    HeadView(QWidget *parent = nullptr);
    ~HeadView();

private:
    QWidget *widget;
    QLabel *textLabel;
    QWidget *iconWidget;
    QWidget *lineWidget;

    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;
};
#endif // HEADVIEW_H
