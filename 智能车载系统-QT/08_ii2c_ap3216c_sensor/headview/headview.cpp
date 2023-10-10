#include "headview.h"

HeadView::HeadView(QWidget *parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    widget = new QWidget();

    iconWidget = new QWidget(this);
    iconWidget->setFixedSize(48, 48);
    iconWidget->setStyleSheet("background:url(:/images/dataviewicon.png)");

    textLabel = new QLabel(this);
    textLabel->setFixedSize(200, 48);
    textLabel->setText("数据可视化情况");
    textLabel->setStyleSheet("QLabel {font-size: 20px; color: white}");

    lineWidget = new QWidget(this);
    lineWidget->setFixedHeight(2);
    lineWidget->setStyleSheet("QWidget {background: #eeeeeeee}");

    vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(widget);
    vBoxLayout->addWidget(lineWidget);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);

    hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(iconWidget);
    hBoxLayout->addWidget(textLabel);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setAlignment(Qt::AlignLeft);
    widget->setLayout(hBoxLayout);

    this->setLayout(vBoxLayout);
    this->adjustSize();
    this->setMaximumHeight(48);
}

HeadView::~HeadView()
{
}

