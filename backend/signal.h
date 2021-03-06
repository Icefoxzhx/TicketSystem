//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_SIGNAL_H
#define INC_20200512_SIGNAL_H

#include <cstring>
#include "hzstring.h"

const int FAIL = -1, SUCCESS = 0;

typedef hzstring<10> Signal;

//user
typedef hzstring<21> Userid;
typedef hzstring<31> Password;
typedef hzstring<15> Name;
typedef hzstring<31> Email;

struct User{
    Userid user_id;
    Password password;
    Name name;
    Email email;
    int privilege;
};

//train

typedef hzstring<25> Train_id;
typedef hzstring<35> Station_id;
typedef hzstring<15> Date;
typedef hzstring<15> Time;

const int MAX_DATE = 97, DAYS[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};

const Train_id MIN_TRAIN_ID = "!", MAX_TRAIN_ID = "~";

enum Ticket_status{ticket_success, ticket_pending, ticket_refund};

struct Train{
    Train_id train_id;
    int station_num, seat_num, sale_date;
    off_t route_pos, ticket_left_pos;
    int buy_num;
    char type;
    bool release;

    Train():station_num(0), seat_num(0), release(0){}
};

struct Station{
    Station_id station_id;
    int arrive_time, depart_time, price;
};



#endif //INC_20200512_SIGNAL_H
