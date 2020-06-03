//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_TRAIN_MANAGER_H
#define INC_20200512_TRAIN_MANAGER_H

#include "signal.h"
#include <cstring>
#include <cstdio>
#include "Bptree.hpp"
#include "hzstring.h"
#include <fstream>

class train_system{
private:

    // for train
    Bptree<Train_id, Train> train_record;

    // for station
    struct Station_key{
        //Station_id + Train_id
        Station_id station_id;
        Train_id train_id;
        Station_key(const Station_id &_s, const Train_id &_t): station_id(_s), train_id(_t){}
        bool operator ==(const Station_key &rhs){
            return (station_id == rhs.station_id) && (train_id == rhs.train_id);
        }
        bool operator < (const Station_key &rhs){
            return (station_id < rhs.station_id) ||
                   (station_id == rhs.station_id && train_id < rhs.train_id);
        }
    };

    typedef int Station_value;

    Bptree<Station_key, Station_value> station_record;


    // for ticket order
    struct Order_key{
        Userid user_id;
        int buy_num;

        bool operator ==(const Order_key &rhs){
            return (user_id == rhs.user_id) && (buy_num == rhs.buy_num);
        }

        bool operator < (const Order_key &rhs){
            if(user_id != rhs.user_id) return (user_id < rhs.user_id);
            return (buy_num < rhs.buy_num);
        }
    };

    struct Order_value{
        Userid user_id;
        Date buy_date;
        Train_id train_id;
        Station_id start_station, end_station;
        int ticket_num;
        bool queue;
        Ticket_status status;
    };

    Bptree<Order_key, Order_value> order_record;


    struct Refund_key{

    };

    struct Refund_value{

    };

    Bptree<Refund_key, Refund_value> refund_record;

    typedef pair<bool, Train> find_t_train;

    std::fstream rfile;
    std::fstream tlfile;


    /*Time add_time(const Time &now, const int &min){//Time hr:mi
        int hr = (now[0] - '0') * 10 + (now[1] - '0');
        int mi = (now[3] - '0') * 10 + (now[4] - '0');
        hr += min / 60; mi += min % 60;
        hr += mi / 60; mi %= 60; hr %= 24;
        char tmp[6];
        tmp[0] = hr/10 + '0'; tmp[1] = hr%10 + '0';
        tmp[2] = ':';
        tmp[3] = mi/10 + '0'; tmp[4] = mi%10 + '0';;
        tmp[5] = '\0';
        Time res = tmp;
        return res;
    }
    Time add_time(const Time &now, const int &min){//Time hr:mi
        int hh = (now[0] - '0') * 10 + (now[1] - '0');
        int mi = (now[3] - '0') * 10 + (now[4] - '0');
        int dd = (now[3] - '0') * 10 + (now[4] - '0');
        int mm = (now[1] - '0') * 10 + (now[2] - '0');
        hh += min / 60; mi += min % 60;
        hh += mi / 60; mi %= 60;
        dd += hh / 24; hh %= 24;
        if(dd > DAYS[mm]) {dd -= DAYS[mm]; mm++;}
        char tmp[12];
        tmp[0] = mm/10 + '0'; tmp[1] = mm%10 + '0';
        tmp[2] = '-';
        tmp[3] = dd/10 + '0'; tmp[4] = dd%10 + '0';
        tmp[5] = ' ';
        tmp[6] = hh/10 + '0'; tmp[7] = hh%10 + '0';
        tmp[8] = ':';
        tmp[9] = mi/10 + '0'; tmp[10] = mi%10 + '0';
        tmp[11] = '\0';
        Time res = tmp;
        return res;
    }*/

    int string_to_int(hzstring<50> str){
        int len_str = strlen(str), res = 0;
        for(int i = 0;i < len_str;++i)
            res = res*10 + str[i] - '0';
        return res;
    }

    int Date_to_int(const Date &d){//mm-dd
        int mm = (d[0]-'0')*10 + d[1]-'0';
        int dd = (d[3]-'0')*10 + d[4]-'0';
        int res = dd;
        for(int i = 6;i < mm;++i) res += DAYS[i];
        return res - 1;
    }

    int Time_to_int(const Time &t){//hr:mi
        int hr = (d[0]-'0')*10 + d[1]-'0';
        int mi = (d[3]-'0')*10 + d[4]-'0';
        return hr*60+mi;
    }

    Date int_to_Date(const int &t){//mm-dd
        char tmp[6];
        int mm = 6, dd = t;
        while(dd > DAYS[mm]) {dd -= DAYS[mm]; mm++;}
        tmp[0] = md/10 + '0'; tmp[1] = mm%10 + '0';
        tmp[2] = '-';
        tmp[3] = dd/10 + '0'; tmp[4] = dd%10 + '0';
        tmp[5] = '\0';
        Date res = tmp;
        return res;
    }

    bool Time_comp(const Time &lhs, const Time &rhs){//lhs <= rhs return 1; else return 0;
        int lmin = Time_to_int(lhs), rmin = Time_to_int(rhs);
        return lmin <= rmin;
    }

public:
/* cache
    train_system():train_record("train_record","index_train_record"),
    station_record("station_record", "index_station_record"),
    order_record("order_record", "index_order_record"),
    {}
*/

    train_system():train_record("train_record"),
                   station_record("station_record"),
                   order_record("order_record"),
                   refund_record("refund_record"),
    {
        std::fstream iofile("route_file");
        if(!iofile) init();
        else iofile.close();
        rfile.open("route_file");
        tlfile.open("ticket_left_file");
    }

    void init(){
        train_record.init();
        station_record.init();
        order_record.init();
        refund_record.init();

        std::ofstream ofile;
        ofile.open("route_file");
        ofile.close();
        ofile.open("ticket_left_file");
        ofile.close();
    }

    ~train_system(){
        rfile.close();
        tlfile.close();
    }



    int add_train(std::istream &is){
    //<trainID>-i，<stationNum>-n，<seatNum>-m，<stations>-s，<prices>-p，<startTime>-x
    //<travelTimes>-t，<stopoverTimes>-o，<saleDate>-d，<type>-y
        Train tr;
        Signal _z;
        hzstring<50> _s, _p, _t, _o, _d, _x, _n, _m;
        tr.release = 0;
        is >> _z >> tr.train_id >> _z >> _n >> _z >> _m;
        tr.station_num = string_to_int(_n); tr.seat_num = string_to_int(_m);

        if(train_record.count(tr.train_id)){
            getline(is, _s);
            return FAIL;
        }

        is >> _z >> _s >> _z >> _p >> _z >> _x >> _z >> _t;
        is >> _z >> _o >> _z >> _d >> _z >> T.type;

        int len = tr.station_num;
        Station *tmp_station = new Station [len];

        int p_s = 0, p_t = 0, p_o = 0;
        int l_s = strlen(_s), l_t = strlen(_t), l_o = strlen(_o), l_tmp = 0;
        char tmp[12];

        //---station_id---
        for(int i = 0;i < len;++i) {
            for (l_tmp = 0; _s[p_s] != '|' && p_s < l_s; l_tmp++, p_s++)
                tmp[l_tmp] = _s[p_s];
            tmp[l_tmp] = '\0'; ++p_s;
            tmp_station[i].station_id = tmp;
        }

        //---station_time---
        for(int i = 0;i < 5;++i) tmp[i] = _d[i];
        for(int i = 6;i < 11;++i) tmp[i] = _x[i];
        tmp[5] = ' '; tmp[11] = '\0';
        tr.start_time = tmp; Time now = tmp;

        tmp_station[0].arrive_time = now; tmp_station[0].depart_time = now;
        for(int i = 1;i < len-1;++i){
            int tmp_travel = 0;
            for (; _t[p_t] != '|' && p_t < l_t; p_t++)
                tmp_travel = tmp_travel * 10 + _t[p_t] - '0';
            p_t++;
            now = add_time(now, tmp_travel);
            tmp_station[i].arrive_time = now;

            int tmp_stopover = 0;
            for (; _o[p_o] != '|' && p_o < l_o; p_o++)
                tmp_stopover = tmp_stopover * 10 + _o[p_o] - '0';
            p_o++;
            now = add_time(now, tmp_stopover);
            tmp_station[i].depart_time = now;
        }
        tmp_station[len-1].arrive_time = now;
        tmp_station[len-1].depart_time = now;

        //---station_price---
        int p_p = 0, l_p = strlen(_p);
        tmp_station[0].price = 0;
        for(int i = 1;i < len;++i){
            int tmp_price = 0;
            for (; _p[p_p] != '|' && p_p < l_p; p_p++)
                tmp_price = tmp_price * 10 + _p[p_p] - '0';
            p_p++;
            tmp_station[i].price = tmp_price;
        }

        for(int i = 0;i < len;++i){
            Station_key tmp_key(tmp_station[i].station_id, tr.train_id);
            station_record.insert(tmp_key, i);
        }

        rfile.seekp(0, std::ios::end);
        tr.route_pos = rfile.tellp();
        rfile.write(reinterpret_cast<char*>(tmp_station), sizeof(Station) * len);

        delete [] tmp_station;

        //--ticket_left--
        int tlnum = MAX_DATE * len;
        int *tmp_tl = new int [tlnum];

        //mm-dd|mm-dd
        int start_mm = (_d[0]-'0')*10 + _d[1]-'0', start_dd = (_d[3]-'0')*10 + _d[4]-'0';
        int start_date = start_dd-1; for(int i = 6;i < start_mm;++i) start_date += DAYS[i];

        int end_mm = (_d[6]-'0')*10 + _d[7]-'0', end_dd = (_d[9]-'0')*10 + _d[10]-'0';
        int end_date = end_dd-1; for(int i = 6;i < end_mm;++i) start_date += DAYS[i];

        for(int i = 0;i < tlnum;++i) tmp_tl[i] = -1;

        for(int j = 0;j < len;++j) {
            for (int i = start_date; i <= end_date; ++i)
                tmp_tl[i*len+j] = tr.seat_num;
            if(!time_comp(tmp_station[j].arrive_time, tmp_station[j].depart_time)) {
                start_date++; end_date++;
            }
        }
        tlfile.seekp(0, std::ios::end);
        tr.ticket_left_pos = tlfile.tellp();
        tlfile.write(reinterpret_cast<char*>(tmp_tl), sizeof(int) * tlnum);
        delete [] tmp_tl;
        return SUCCESS;
    }

    int release_train(std::istream &is){//-i   -1 fail   0 success
        Train_id _i;
        Signal _z;
        is >> _z >> _i;

        find_t_train res = train_record.find(_i);
        if(res.first == false) return FAIL;
        Train tr = res.second;
        if(tr.release == true) return FAIL;
        tr.release = true;
        train_record.set(_i, tr);
        return SUCCESS;
    }

    void query_train(std::istream &is, std::ostream &os){//-i -d
        Train_id _i;
        Date _d;
        Signal _z;
        is >> _z >> _i >> _z >> _d;

        find_t_train res = train_record.find(_i);
        if(res.first == false) {os << "-1\n"; return;}

        Train tr = res.second;
        os << tr.train_id << " " << tr.type << "\n";

        int len = tr.station_num;
        rfile.seekg(tr.route_pos, std::ios::beg);
        Station *tmp_station = new Station [len];
        rfile.read(reinterpret_cast<char*> (tmp_station), sizeof(Station) * len);

        int tlnum = len * MAX_DATE;
        tlfile.seekg(tr.ticket_left_pos, std::ios::beg);
        int *tmp_tl = new int [tlnum];
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * tlnum);

        int date = Date_to_int(_d);
        //for start station
        os << tmp_station[0].station_id <<"xx-xx xx:xx -> " << int_to_Date(date) << " ";
        os << tmp_station[0].depart_time << " 0 " << tmp_tl[date*len + 0] << "\n";

        int sum_price = 0;
        for(int i = 1;i < len - 1;++i){
            sum_price += tmp_station[i].price;
            os << tmp_station[i].station_id << " " << tmp_station[i].arrive_time <<" -> ";
            os << tmp_station[i].depart_time << " " << sum_price << " ";
            os << tmp_station[i].ticket_left <<"\n";///////////////////////////////
        }

        //for end station
        os << tmp_station[len-1].station_id << tmp_station[len-1].depart_time <<" -> xx-xx xx:xx ";
        os << sum_price + tmp_station[len-1].price << " x\n";

        delete [] tmp_station;
        return;
    }

    int delete_train(std::istream &is){//-i
        Train_id _i;
        Signal _z;

        is >> _z >> _i;

    }

    int query_ticket(std::istream &is, std::ostream &os){//-s -t -d (-p time)
        Station_name _s, _t;
        Date _d;
        bool p_c = false; // false == time   true == cost
        std::string str;/////////////////////////
        Signal _z;

        is >> _z >> _s >> _z >> _t >> _z >> _d;
        getline(is,str);
        if(str.size() && str[0] == 'c') p_c = true;

    }

    int query_transfer(std::istream &is, std::ostream &os){//-s -t -d (-p)
        Station_name _s, _t;
        Date _d;
        std::string p_c;////////////////////////////
        Signal _z;

        is >> _z >> _s >> _z >> _t >> _z >> _d >> _z >> p_c;

    }

    int buy_ticket(std::istream &is, std::ostream &os){//-u -i -d -n -f -t (-q false)
    //<username>为-u的用户购买：<trainID>为-i，日期为-d，从站-f到站-t的车票-n张。
        Username _u;
        Train_id _i;
        Date _d;
        int _n;
        Station_name _f, _t;
        bool _q = false;
        std::string str;//////////////////////////////
        Signal _z;

        is >> _z >> _u >> _z >> _i >> _z >> _d >> _z >> _n >> _z >> _f >> _z >> _t;
        getline(is, str);
        if(str.size() && str[0] == 't') _q = true;



    }

    int refund_ticket(std::istream &is){//-u (-n 1)
        Username _u;
        int _n = 1;
        Signal _z;
        std::string str;/////////////////

        is >> _z >> _u;
        getline(is, str);
        if(str.size()){
            _n = 0;
            for(int i = 0; i < str.size();++i)
                _n = _n*10 + str[i] - '0';
        }

    }

};

#endif //INC_20200512_TRAIN_MANAGER_H
