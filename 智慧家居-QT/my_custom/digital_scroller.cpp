#include "digital_scroller.h"
#include "ui_digital_scroller.h"

DigitalScroller::DigitalScroller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::digital_scroller)

{
    ui->setupUi(this);
    this->setFixedSize(50,200);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);//设置窗口背景透明

    timer_channgedValue = new QTimer(this);
    connect(timer_channgedValue,SIGNAL(timeout()),this,SLOT(slot_channgedValue()));

    //初始化变量
    m_minRange=16;
    m_maxRange=31;
    m_currentValue=26;
    isDragging=0;
    m_deviation=0;
    m_numSize=8;//设置数字的大小比例,m_numSize数值越大，显示的数字就越小
    isMiddle=false;
    m_prefix="";
    m_suffix="";

    homingAni  = new QPropertyAnimation(this,""); //创建一个动画类
    homingAni->setDuration(500); //设置动画持续时间
    homingAni->setEasingCurve(QEasingCurve::OutQuad); //设置动画的缓动曲线，这里OutQuad是二次函数缓和曲线，减速到零速度

    connect(homingAni,SIGNAL(finished()),this,SLOT(slotChoseFinshed()));
}

DigitalScroller::~DigitalScroller()
{

}



/***********************************
 *名称：setRange
 *功能：设置范围
 *说明：无
***********************************/
void DigitalScroller::setRange(int min, int max)
{
    m_minRange = min;
    m_maxRange = max;
    if(m_currentValue < min) //如果选中的当前值小于最小值，就等于最小值，不会再减了
    {
        m_currentValue = min;
    }
    if(m_currentValue > max) //最大值同理
    {
        m_currentValue = max;
    }
    //计算字符尺寸
    m_numSize = 3;
    int temp = m_maxRange;
    while(temp > 0)
    {
        temp /= 10;
        m_numSize++;
    }
    update(); //重绘此组件，需要进行重绘时,要使用update

}

/***********************************
 *名称：readValue
 *功能：获取当前值
 *说明：无
***********************************/
int DigitalScroller::readValue(void)
{
    return m_currentValue;
}

QString DigitalScroller::getValueStr(void)
{
    QString strValue;
    if(m_currentValue < 10){
        strValue="0";
    }
    strValue+=QString::number(m_currentValue);
    return strValue;
}
//设置显示格式
/***********************************
 *名称：setFormat
 *功能：设置显示格式
 *说明：无
***********************************/
void DigitalScroller::setFormat(QString prefix, QString suffix)
{
    m_prefix=prefix;
    m_suffix=suffix;
}
/***********************************
 *名称：setCurValue
 *功能：设置当前显示
 *说明：无
***********************************/
void DigitalScroller::setCurValue(int Value)
{
    //判断是否在有效区间内
    if(Value > m_maxRange)
    {
        Value=m_maxRange;
    }
    else if(Value < m_minRange)
    {
        Value=m_minRange;
    }
    m_currentValue=Value;
    if(this->isVisible())
    {
        update();
    }
}
//鼠标按下事件
void DigitalScroller::mousePressEvent(QMouseEvent *event)
{
    isDragging = true; //鼠标是否按下的标志位，布尔类型
    m_mouseSrcPos = event->pos().y(); //鼠标按下时候的y轴作标记录下来

    QWidget::mousePressEvent(event);
}
//鼠标移动事件
void DigitalScroller::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging)
       {
           //数值到边界时，阻止继续往对应方向移动
           if((m_currentValue == m_minRange && event->pos().y() >= m_mouseSrcPos)||
             (m_currentValue == m_maxRange && event->pos().y() <= m_mouseSrcPos))
               return;

           m_deviation = event->pos().y() - m_mouseSrcPos; //垂直偏移量 = 现在的坐标 - 开始的作标

           //若移动速度过快时进行限制
           if(m_deviation > (height()-1)/5)
           {
               m_deviation = (height()-1)/5;
           }

           else if(m_deviation < -(height()-1)/5)
           {
               m_deviation = -(height()-1)/5;
           }

           emit deviationChange((float)m_deviation/((height()-1)/5)); //发出偏移量改变的信号，随着偏移量改变，重绘数字，就变成数字滚动的效果
           update();
       }

    QWidget::mouseMoveEvent(event);
}
//鼠标松开事件
void DigitalScroller::mouseReleaseEvent(QMouseEvent *event)
{
    if(isDragging) //如果鼠标松开了，那么还原一下鼠标按下标志位
    {
        homing();
        isDragging = false;
        update();
    }

    QWidget::mouseReleaseEvent(event);
}
//绘图事件
void DigitalScroller::paintEvent(QPaintEvent *m_painter)
{
    QPainter painter(this); //创建画家类，指定绘图设备，也就是在哪画
    painter.setRenderHint(QPainter::Antialiasing, true); //反走样，就是抗锯齿

    int Width = width()-1; //设置宽高
    int Height = height()-1;

    if(m_deviation >= Height/5 && m_currentValue > m_minRange) //偏移量大于1/5 高的时候，数字减一
    {
        m_mouseSrcPos += Height/5; //鼠标起始位置重新设置，即加上1/5的高度
        m_deviation -= Height/5; //偏移量重新设置，即减去1/5的高度
        m_currentValue--;
    }
    if(m_deviation <= -Height/5 && m_currentValue < m_maxRange) //同理，数字加一
    {
        m_mouseSrcPos -= Height/5;
        m_deviation += Height/5;
        m_currentValue++;
    }

    //中间数字
    paintNum(painter,m_currentValue,m_deviation,isMiddle=1); //将选中数字画到中间

    //两侧数字1
    if(m_currentValue != m_minRange) //选中的数字不是最小，不是最大，那么就有两侧数字，然后画出两侧数字
        paintNum(painter,m_currentValue-1,m_deviation-Height*2/10,isMiddle=0);
    if(m_currentValue != m_maxRange)
        paintNum(painter,m_currentValue+1,m_deviation+Height*2/10,isMiddle=0);

    //两侧数字2,超出则不显示
    if(m_deviation >= 0 && m_currentValue-2 >= m_minRange)
        paintNum(painter,m_currentValue-2,m_deviation-Height*4/10,isMiddle=0);
    if(m_deviation <= 0 && m_currentValue+2 <= m_maxRange)
        paintNum(painter,m_currentValue+2,m_deviation+Height*4/10,isMiddle=0);

    //画边框，中间数字两侧的边框
    painter.setPen(QPen(QColor(0,0,0,150),4));
    painter.drawLine(0,Height/8*3,Width,Height/8*3);
    painter.drawLine(0,Height/8*5,Width,Height/8*5);

    QWidget::paintEvent(m_painter);

}

/***********************************
 *名称：paintNum
 *功能：画数字函数
 *说明：无
***********************************/
void DigitalScroller::paintNum(QPainter &painter, int num, int deviation, bool isMiddle)
{
    int Width = this->width()-1;
    int Height = this->height()-1;

    int size = (Height - qAbs(deviation))/m_numSize; //qAbs 返回输入参数对应类型的绝对值。
    int transparency = 255-510*qAbs(deviation)/Height; //设置透明度
    int height = Height/2-3*qAbs(deviation)/5;
    int y = Height/2+deviation-height/2;
    QFont font;
    font.setPixelSize(size); //设置像素大小
    painter.setFont(font); //设置字体
    if(isMiddle)
    {
        painter.setPen(QColor(36,110,202,transparency)); //设置画笔，中间颜色
    }
    else
    {
        painter.setPen(QColor(0,0,0,transparency)); //设置画笔，上下两侧数据颜色
    }
    QString str_date;
    if(num<=9)
    {
        str_date=QString("0")+QString::number(num);
    }
    else
    {
        str_date=QString::number(num);
    }
    str_date=m_prefix+str_date+m_suffix;
    painter.drawText(QRectF(0,y,Width,height), //画文本，参数：QRectF参数：位置xy，长宽大小；对齐方式，中间对齐；内容
                     Qt::AlignCenter,
                     str_date);

}


/***********************************
 *名称：homing
 *功能：使选中的数字回到屏幕中间
 *说明：无
***********************************/
void DigitalScroller::homing(void)
{

    //将数字矫正到中心
    if(m_deviation > height()/20)
    {
        homingAni->setStartValue((height()-1)/8-m_deviation);
        homingAni->setEndValue(0);
        m_currentValue--;
    }
    else if(m_deviation > -(height())/20)
    {
        homingAni->setStartValue(m_deviation);
        homingAni->setEndValue(0);
    }
    else if(m_deviation < -(height())/20)
    {
        homingAni->setStartValue(-(height()-1)/8-m_deviation);
        homingAni->setEndValue(0);
        m_currentValue++;
    }
    m_deviation=0;

    homingAni->start(); //开始动画
}
/***********************************
 *名称：slotChoseFinshed
 *功能：动画结束，开始启动定时器，2秒内没有数据修改，则发送数据
 *说明：无
***********************************/
void DigitalScroller::slotChoseFinshed()
{
    timer_channgedValue->start(2000);
}

/***********************************
 *名称：slot_channgedValue
 *功能：定时结束，则发送当前的数据给父窗口
 *说明：无
***********************************/
void DigitalScroller::slot_channgedValue()
{
    //emit currentValueChanged(m_currentValue); //发送当前数值改变信号
    timer_channgedValue->stop();
}


