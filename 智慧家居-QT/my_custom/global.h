#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMap>

#define LIVINGROOM  0
#define BEDROOM     1
#define CONNECT 1
#define ERROR   0

extern QMap<int,int> g_socket_map; //客户端名称与端口号关联起来
extern int livingroom_status;
class Global
{
public:
    Global();
    static int curr_socket;

};





#endif // GLOBAL_H
