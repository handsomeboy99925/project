#include "tcpsocket.h"

Tcpsocket::Tcpsocket(int sockID, QObject *parent) :
    QTcpSocket(parent)
{

    my_sockID = sockID;
    connect(this,SIGNAL(readyRead()),this,SLOT(slot_recvData()));
}


Tcpsocket::~Tcpsocket()
{

}


/***********************************
 *名称：slot_recvData
 *功能：处理客户端接收到的数据
 *说明：无
***********************************/
void Tcpsocket::slot_recvData(void)
{
//    QString ip = peerAddress().toString().remove(0,7);
    QString ip = peerAddress().toString();
    QByteArray rec_data = readAll();
    QString valid_data;//有效数据
    QStringList lst_valid_data;
    int func;           //功能码，设置/读取
//    int type;         //数据代表什么类型，0-int,1-float,3-QString
    int reg;            //寄存器位置
    QString data;       //设置或读取的数据

    qDebug() << ip << ": " << rec_data;

    /*通信协议解析，格式<*03,00,00*>*/
    if(rec_data.left(2) == "<*" && rec_data.right(2) == "*>"&& rec_data.count(",") == 2)
    {
        valid_data = rec_data.replace("<*","");
        valid_data = rec_data.replace("*>","");
        lst_valid_data = valid_data.split(",");

        func = lst_valid_data.at(0).toInt();
        reg = lst_valid_data.at(1).toInt();
        data = lst_valid_data.at(2);

        qDebug() << "接收到的数据：" <<lst_valid_data;
        if(func == 03)//查询数据
        {
            read_client_data(reg,data);
        }else if(func == 06)//设置数据
        {

        }
    }
//    slot_sendData(my_sockID,rec_data);
}


/***********************************
 *名称：slot_sendData
 *功能：发送数据给客户端
 *说明：无
***********************************/
void Tcpsocket::slot_sendData(int sockID, const QByteArray& data)
{
    if(sockID == my_sockID && !data.isEmpty())
    {
        this->write(data);
        qDebug() << "发送给客户端的数据：" << data;
    }
}

/***********************************
 *名称：read_client_data
 *功能：读取客户端的数据信息
 *说明：该函数需配合通信协议文档一起阅读理解
***********************************/
void Tcpsocket::read_client_data(int reg,QString data)
{
    //获取当前客户端的名称，寄存器0的位置代表当前连接的客户端的名称。
    int temp = data.toInt();
    if(reg == 0)
    {
        if(temp == 0)//如果编号为0，则是客厅
        {
            g_socket_map.insert(LIVINGROOM,my_sockID);
            qDebug() << "当前连接的是客厅";
            livingroom_status = CONNECT;


        }else if(temp == 1)//如果编号为1，则是卧室
        {
            g_socket_map.insert(BEDROOM,my_sockID);
            qDebug() << "当前连接的是卧室";
        }
    }
}


