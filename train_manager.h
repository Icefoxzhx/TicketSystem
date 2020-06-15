//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_TRAIN_MANAGER_H
#define INC_20200512_TRAIN_MANAGER_H

#include "signal.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include "Bptree.hpp"
#include "hzstring.h"
#include <fstream>
/*
#include "utility.hpp"
#include "exceptions.hpp"
#include "vector.hpp"
#include "map.hpp"
 */
#include <vector>
#include <map>
#include <functional>

class train_system{
private:

    // for user
    struct User_value{
        int user_pos, buy_num;
    };

    Bptree<Userid, User_value> user_record;
    typedef pair<bool, User_value> find_t_user;
    //sjtu::map<Userid, bool> user_login;
    map<Userid, bool> user_login;

    std::fstream ufile;

    bool Change;


    // for train
    Bptree<Train_id, Train> train_record;

    // for station
    struct Station_key{
        //Station_id + Train_id
        Station_id station_id;
        Train_id train_id;
        Station_key(){}
        Station_key(const Station_id &_s, const Train_id &_t): station_id(_s), train_id(_t){}
        bool operator ==(const Station_key &rhs) const{
            return (station_id == rhs.station_id) && (train_id == rhs.train_id);
        }
        bool operator < (const Station_key &rhs) const{
            return (station_id < rhs.station_id) ||
                   (station_id == rhs.station_id && train_id < rhs.train_id);
        }
    };

    Bptree<Station_key, int> station_record;

    // for ticket order
    struct Order_key{
        Userid user_id;
        int user_buy_order;
        bool operator ==(const Order_key &rhs) const{
            return (user_id == rhs.user_id) && (user_buy_order == rhs.user_buy_order);
        }
        bool operator < (const Order_key &rhs) const{
            if(user_id != rhs.user_id) return (user_id < rhs.user_id);
            return (user_buy_order < rhs.user_buy_order);
        }
    };

    Bptree<Order_key, int> order_record;

    struct Order{
        Userid user_id;
        int start_date, start_time, end_date, end_time;
        int train_buy_order, start_order, end_order, buy_number, price;
        Train_id train_id;
        Station_id start_station_id, end_station_id;
        Ticket_status status;
    };

    struct Pending_key{
        Train_id train_id;
        int pending_buy_time;
        bool operator == (const Pending_key &rhs) const{
            return (train_id == rhs.train_id) && (pending_buy_time == rhs.pending_buy_time);
        }
        bool operator <(const Pending_key &rhs) const{
            if(train_id != rhs.train_id) return (train_id < rhs.train_id);
            return(pending_buy_time < rhs.pending_buy_time);
        }
    };

    Bptree<Pending_key, int> pending_record;

    typedef pair<bool, Train> find_t_train;
    typedef pair<bool, int> find_t_station;
    typedef pair<bool, int> find_t_order;
    /*
    typedef sjtu::vector<pair<Station_key,int> > find_t_v_station;
    typedef sjtu::vector<pair<Pending_key,int> > find_t_v_pending;
    typedef sjtu::vector<pair<Order_key,int> > find_t_v_order;
     */
    typedef vector<pair<Station_key,int> > find_t_v_station;
    typedef vector<pair<Pending_key,int> > find_t_v_pending;
    typedef vector<pair<Order_key,int> > find_t_v_order;


    struct query_ticket_t{
        Train_id train_id;
        int start_time, end_time, price, seat_left, val;

        bool operator < (const query_ticket_t &rhs) const{return (val < rhs.val) || (val == rhs.val && train_id < rhs.train_id);}
        bool operator == (const query_ticket_t &rhs) const{return val == rhs.val && train_id == rhs.train_id;}
        bool operator > (const query_ticket_t &rhs) const{return (val > rhs.val) || (val == rhs.val && train_id > rhs.train_id);}
        bool operator <= (const query_ticket_t &rhs) const{return (val < rhs.val) || (val == rhs.val && train_id <= rhs.train_id);}
        bool operator >= (const query_ticket_t &rhs) const{return (val > rhs.val) || (val == rhs.val && train_id >= rhs.train_id);}
    };


    struct query_transfer_t {
        Train_id train_id;
        int start_time, end_time, price, val, val_1, date_to_begin;
    };
//    sjtu::map<Station_id, query_ticket_t> station_transfer;
    map<Station_id, query_transfer_t> station_transfer;

    std::fstream rfile;
    std::fstream tlfile;
    std::fstream odfile;



    int string_to_int(hzstring<150> str){
        int len_str = strlen(str.ch), res = 0;
        for(int i = 0;i < len_str;++i)
            res = res*10 + str[i] - '0';
        return res;
    }

    int Date_to_int(const Date &d){//mm-dd
        int mm = (d[0]-'0')*10 + d[1]-'0';
        int dd = (d[3]-'0')*10 + d[4]-'0';
        int res = dd;
        if(mm < 6) return -1;
        for(int i = 6;i < mm;++i) res += DAYS[i];
        return res - 1;
    }

    int Time_to_int(const Time &t){//hr:mi
        int hr = (t[0]-'0')*10 + t[1]-'0';
        int mi = (t[3]-'0')*10 + t[4]-'0';
        return hr*60+mi;
    }

    Date int_to_Date(const int &t){//mm-dd
        char tmp[6];
        int mm = 6, dd = t + 1;
        while(dd > DAYS[mm]) {dd -= DAYS[mm]; mm++;}
        tmp[0] = mm/10 + '0'; tmp[1] = mm%10 + '0';
        tmp[2] = '-';
        tmp[3] = dd/10 + '0'; tmp[4] = dd%10 + '0';
        tmp[5] = '\0';
        Date res = tmp;
        return res;
    }

    Time int_to_Time(const int &t){//hr:mi
        char tmp[6];
        int hr = (t%1440) / 60, mi = (t%1440) % 60;
        tmp[0] = hr/10 + '0'; tmp[1] = hr%10 + '0';
        tmp[2] = ':';
        tmp[3] = mi/10 + '0'; tmp[4] = mi%10 + '0';
        tmp[5] = '\0';
        Time res = tmp;
        return res;
    }


    template <typename T>
    void Quick_Sort(T *arr, int Start_Index, int End_Index) {
        if(Start_Index > End_Index) return;
        int sort_l = Start_Index, sort_r = End_Index;
        T sort_key = arr[sort_l];
        while (sort_l < sort_r) {
            while (sort_l < sort_r && arr[sort_r] >= sort_key) sort_r--;
            arr[sort_l] = arr[sort_r];
            while (sort_l < sort_r && arr[sort_l] <= sort_key) sort_l++;
            arr[sort_r] = arr[sort_l];
        }
        arr[sort_l] = sort_key;
        Quick_Sort(arr, Start_Index, sort_l - 1);
        Quick_Sort(arr, sort_l + 1, End_Index);
    }


    //<cur_userid>-c <saledate>/<date>-d <station_id>-f <privilege>-g <trainID>-i
    //<mailaddress>/<seatnum>-m <name>/<station_num>/<ticket_num> -n
    //<stopoverTimes>-o <password>/<prices>/<timeorcost>-p <trueorfalse>-q
    //<stations>/<startstation>-s <traveltimes>/<endstation>-t <userid>-u
    //<starttime>-x <type>-y

    Train_id _i;
    Userid _c, _u;
    Station_id _f, _s_startstation, _t_endstation;
    Email _m_email;
    Name _n_name;
    Time _x;
    char _y;
    Password _p_password;
    int  _g, _m_seatnum, _n_station_num, _n_ticket_num, _n_order;
    bool _p_timeorcost, _q;//false == time   true == cost
    bool f_p_password, f_n_name, f_m_email, f_g;
    //char *_o = nullptr, *_p_price = nullptr, *_s_stations = nullptr, *_t_traveltime = nullptr, _d[15];
    char _o[600], _p_price[800], _s_stations[3500], _t_traveltime[600], _d[15];
    //_o 600 / _p_price 800 / _s_stations 3500 / _t_traveltime 600


    void get_variables(std::istream &is, int m_type, int n_type, int p_type, int s_type, int t_type){
        f_p_password = false ; f_n_name = false; f_m_email = false; f_g = false;
        _p_timeorcost=false; _q=false; _n_order = 1;
        char *tmp_str = new char[35000];
        is.getline(tmp_str, 35000);

        int str_pos = 0, str_len = strlen(tmp_str);
        //std::cout<<"(";
        while(str_pos < str_len && tmp_str[str_pos] == ' ') str_pos++;
        while(str_pos+3 < str_len){
            char str_type = tmp_str[str_pos+1];
            //std::cout<<"[<<str_type<<"-"<<;
            str_pos += 3;
            int t_pos = 0;
            if(str_type == 'c'){
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _c[t_pos] = tmp_str[str_pos];
                _c[t_pos] = '\0';
            }
            else if(str_type == 'd'){
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _d[t_pos] = tmp_str[str_pos];
                _d[t_pos] = '\0';
            }
            else if(str_type == 'f'){
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _f[t_pos] = tmp_str[str_pos];
                _f[t_pos] = '\0';
            }
            else if(str_type == 'g'){
                f_g = true; _g = 0;
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _g = _g*10 + tmp_str[str_pos] -'0';
            }
            else if(str_type == 'i'){
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _i[t_pos] = tmp_str[str_pos];
                _i[t_pos] = '\0';
            }
            else if(str_type == 'm'){
                if(m_type == 1){// mailaddress
                    f_m_email = true;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _m_email[t_pos] = tmp_str[str_pos];
                    _m_email[t_pos] = '\0';
                }
                else{
                    _m_seatnum = 0;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _m_seatnum = _m_seatnum*10 + tmp_str[str_pos] -'0';
                }
            }
            else if(str_type == 'n'){
                if(n_type == 1){//name
                    f_n_name = true;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _n_name[t_pos] = tmp_str[str_pos];
                    _n_name[t_pos] = '\0';
                }
                else if(n_type == 2){//station_num
                    _n_station_num = 0;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _n_station_num = _n_station_num*10 + tmp_str[str_pos]-'0';
                }
                else if(n_type == 3){//ticket_num
                    _n_ticket_num = 0;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _n_ticket_num = _n_ticket_num*10 + tmp_str[str_pos]-'0';
                }
                else{//order
                    _n_order = 0;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _n_order = _n_order*10 + tmp_str[str_pos]-'0';
                }
            }
            else if(str_type == 'o'){
               // _o = new char[600];
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _o[t_pos] = tmp_str[str_pos];
                _o[t_pos] = '\0';
            }
            else if(str_type == 'p'){
                if(p_type == 1){//password
                    f_p_password = true;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _p_password[t_pos] = tmp_str[str_pos];
                    _p_password[t_pos] = '\0';
                }
                else if(p_type == 2){//prices
                  //  _p_price = new char[800];
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _p_price[t_pos] = tmp_str[str_pos];
                    _p_price[t_pos] = '\0';
                }
                else{// timeorcost
                    _p_timeorcost = false;//false == time   true == cost
                    if(tmp_str[str_pos] == 'c') _p_timeorcost = true;
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++);
                }
            }
            else if(str_type == 'q'){
                _q = false;
                if(tmp_str[str_pos] == 't') _q = true;
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++);
            }
            else if(str_type == 's'){
                if(s_type == 1){//stations
                 //   _s_stations = new char [3500];
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _s_stations[t_pos] = tmp_str[str_pos];
                    _s_stations[t_pos] = '\0';
                }
                else{//startstation
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _s_startstation[t_pos] = tmp_str[str_pos];
                    _s_startstation[t_pos] = '\0';
                }
            }
            else if(str_type == 't'){
                if(t_type == 1){//traveltime
                  //  _t_traveltime = new char [600];
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _t_traveltime[t_pos] = tmp_str[str_pos];
                    _t_traveltime[t_pos] = '\0';
                }
                else{//endstation
                    for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                        _t_endstation[t_pos] = tmp_str[str_pos];
                    _t_endstation[t_pos] = '\0';
                }
            }
            else if(str_type == 'u'){
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _u[t_pos] = tmp_str[str_pos];
                _u[t_pos] = '\0';
            }
            else if(str_type == 'x'){
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++, t_pos++)
                    _x[t_pos] = tmp_str[str_pos];
                _x[t_pos] = '\0';
            }
            else{
                _y = tmp_str[str_pos];
                for(;str_pos < str_len && tmp_str[str_pos] != ' ';str_pos++);
            }
            while(str_pos < str_len && tmp_str[str_pos] == ' ') str_pos++;
        }
        //std::cout<<")";
        delete[]tmp_str;
    }

public:

    train_system():train_record("train_record"),
                   station_record("station_record"),
                   order_record("order_record"),
                   pending_record("pending_record"),
                   user_record("user_record")
    {
        std::fstream iofile("route_file");
        if(!iofile) init();
        else iofile.close();
        rfile.open("route_file");
        tlfile.open("ticket_left_file");
        odfile.open("order_file");

        user_login.clear();
        std::fstream iofile2("user_file");
        if(!iofile2)  user_init();
        else {Change = true; iofile2.close();}
        ufile.open("user_file");
    }

    void init(){
        std::ofstream ofile;
        ofile.open("route_file");
        ofile.close();
        ofile.open("ticket_left_file");
        ofile.close();
        ofile.open("order_file");
        ofile.close();
    }

    void user_init(){
        Change = false;
        std::ofstream ofile;
        ofile.open("user_file");
        ofile.close();
    }

    ~train_system(){
        rfile.close();
        tlfile.close();
        odfile.close();
        ufile.close();
    }



    int add_train(std::istream &is){
    //<trainID>-i，<stationNum>-n，<seatNum>-m，<stations>-s，<prices>-p，<startTime>-x
    //<travelTimes>-t，<stopoverTimes>-o，<saleDate>-d，<type>-y
        Train tr;
        get_variables(is,2,2,2,1,1);
        tr.release = 0; tr.buy_num = 0;
        tr.train_id = _i; tr.station_num = _n_station_num; tr.seat_num = _m_seatnum;
        tr.type = _y;

        if(train_record.count(tr.train_id)) {return FAIL;}

        int len = tr.station_num;
        Station *tmp_station = new Station [105];

        int p_s = 0, p_t = 0, p_o = 0;
        int l_s = strlen(_s_stations), l_t = strlen(_t_traveltime), l_o = strlen(_o), l_tmp = 0;
        char tmp[500];

        //---station_id---
        for(int i = 0;i < len;++i) {
            for (l_tmp = 0; _s_stations[p_s] != '|' && p_s < l_s; l_tmp++, p_s++)
                tmp[l_tmp] = _s_stations[p_s];
            tmp[l_tmp] = '\0'; ++p_s;
            tmp_station[i].station_id = tmp;
        }
        //delete[] _s_stations; _s_stations = nullptr;

        //---station_time--- _d sale_date _x start_time
        for(int i = 0;i < 5;++i) tmp[i] = _d[i]; tmp[5] = '\0';
        int start_date = Date_to_int(tmp);
        for(int i = 6;i < 11;++i) tmp[i-6] = _d[i]; tmp[5] = '\0';
        int end_date = Date_to_int(tmp);
        for(int i = 0;i < 5;++i) tmp[i] = _x[i]; tmp[5] = '\0';
        int start_time = Time_to_int(tmp);

        int now = start_date * 1440 + start_time;
        tmp_station[0].arrive_time = now; tmp_station[0].depart_time = now;

        for(int i = 1;i < len-1;++i){
            int tmp_travel = 0;
            for (; _t_traveltime[p_t] != '|' && p_t < l_t; p_t++)
                tmp_travel = tmp_travel * 10 + _t_traveltime[p_t] - '0';
            p_t++;
            now += tmp_travel;
            tmp_station[i].arrive_time = now;

            int tmp_stopover = 0;
            for (; _o[p_o] != '|' && p_o < l_o; p_o++)
                tmp_stopover = tmp_stopover * 10 + _o[p_o] - '0';
            p_o++;
            now += tmp_stopover;
            tmp_station[i].depart_time = now;
        }

        int tmp_travel = 0;
        for (; _t_traveltime[p_t] != '|' && p_t < l_t; p_t++)
            tmp_travel = tmp_travel * 10 + _t_traveltime[p_t] - '0';
        p_t++;
        now += tmp_travel;
        tmp_station[len-1].arrive_time = now;
        tmp_station[len-1].depart_time = now;

        //delete [] _t_traveltime; _t_traveltime = nullptr;
        //delete [] _o; _o = nullptr;

        //---station_price---
        int p_p = 0, l_p = strlen(_p_price);
        tmp_station[0].price = 0;
        for(int i = 1;i < len;++i){
            int tmp_price = 0;
            for (; _p_price[p_p] != '|' && p_p < l_p; p_p++)
                tmp_price = tmp_price * 10 + _p_price[p_p] - '0';
            p_p++;
            tmp_station[i].price = tmp_price + tmp_station[i-1].price;
        }
        //delete[]_p_price; _p_price = nullptr;

        for(int i = 0;i < len;++i){
            Station_key tmp_key;
            tmp_key.station_id = tmp_station[i].station_id;
            tmp_key.train_id = tr.train_id;
            station_record.insert(tmp_key, i);
        }

        rfile.seekp(0, std::ios::end);
        tr.route_pos = rfile.tellp();
        rfile.write(reinterpret_cast<char*>(tmp_station), sizeof(Station) * len);

        //--ticket_left--
        int tlnum = MAX_DATE * len;
        int *tmp_tl = new int [tlnum+5];

        for(int i = 0;i < tlnum;++i) tmp_tl[i] = -1;

        for(int i = start_date;i <= end_date;++i)
            for (int j = 0;j < len-1;++j)
                tmp_tl[i*len+j] = tr.seat_num;

        tlfile.seekp(0, std::ios::end);
        tr.ticket_left_pos = tlfile.tellp();
        tlfile.write(reinterpret_cast<char*>(tmp_tl), sizeof(int) * (tlnum));

        tr.sale_date = start_date * 1000 + end_date;
        train_record.insert(tr.train_id, tr);

        delete [] tmp_tl;
        delete [] tmp_station;
        return SUCCESS;
    }

    int release_train(std::istream &is){// -1 fail   0 success
        get_variables(is,0,0,0,0,0);

        find_t_train res = train_record.find(_i);
        if(res.first == false) return FAIL;
        Train tr = res.second;
        if(tr.release == true) return FAIL;
        tr.release = true;
        train_record.set(_i, tr);
        return SUCCESS;
    }

    void query_train(std::istream &is, std::ostream &os){//-d -i
        get_variables(is,0,0,0,0,0);
        int date = Date_to_int(_d);

        find_t_train res = train_record.find(_i);
        if(!res.first) {os << "-1\n"; return;}

        Train tr = res.second;
        int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date % 1000;
        if(date < start_sale_date || date > end_sale_date) {os << "-1\n"; return;}

        os << tr.train_id << " " << tr.type << "\n";

        int len = tr.station_num;
        rfile.seekg(tr.route_pos, std::ios::beg);
        Station *tmp_station = new Station [105];
        rfile.read(reinterpret_cast<char*> (tmp_station), sizeof(Station) * len);

        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * date * len, std::ios::beg);
        int *tmp_tl = new int [len+5];
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (len-1));

        //for start station
        os << tmp_station[0].station_id <<" xx-xx xx:xx -> " << int_to_Date(date) << " ";
        os << int_to_Time(tmp_station[0].depart_time) << " 0 " << tmp_tl[0] << "\n";

        for(int i = 1;i < len - 1;++i){
            int t1 = tmp_station[i].arrive_time/1440 - tmp_station[0].depart_time/1440;
            int t2 = tmp_station[i].depart_time/1440 - tmp_station[0].depart_time/1440;
            os << tmp_station[i].station_id << " " << int_to_Date(date + t1) << " ";
            os << int_to_Time(tmp_station[i].arrive_time) <<" -> ";
            os << int_to_Date(date + t2) << " ";
            os << int_to_Time(tmp_station[i].depart_time) << " " << tmp_station[i].price << " ";
            os << tmp_tl[i] <<"\n";
        }

        //for end station
        int t1 = tmp_station[len-1].arrive_time/1440 - tmp_station[0].depart_time/1440;
        os << tmp_station[len-1].station_id << " " << int_to_Date(date + t1) << " ";
        os << int_to_Time(tmp_station[len-1].depart_time) <<" -> xx-xx xx:xx ";
        os << tmp_station[len-1].price << " x\n";

        delete [] tmp_station;
        delete [] tmp_tl;
        return;
    }

    int delete_train(std::istream &is){//-i
        get_variables(is,0,0,0,0,0);

        find_t_train res = train_record.find(_i);
        if(!res.first) return FAIL;

        Train tr = res.second;
        if(tr.release) return FAIL;

        int len = tr.station_num;
        rfile.seekg(tr.route_pos, std::ios::beg);
        Station *tmp_station = new Station [len+5];
        rfile.read(reinterpret_cast<char*> (tmp_station), sizeof(Station) * len);

        for(int i = 0;i < len;++i){
            Station_key tmp_key;
            tmp_key.station_id = tmp_station[i].station_id;
            tmp_key.train_id = tr.train_id;
            station_record.remove(tmp_key);
        }

        train_record.remove(tr.train_id);
        delete [] tmp_station;
        return SUCCESS;
    }

    void query_ticket(std::istream &is, std::ostream &os){//-s -t -d (-p time)
        get_variables(is,0,0,3,2,2);

        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _s_startstation == _t_endstation) {os << "-1\n"; return;}

        Station_key start_key_1(_s_startstation, MIN_TRAIN_ID), start_key_2(_s_startstation, MAX_TRAIN_ID);
        find_t_v_station Tmp_start_station = station_record.find(start_key_1, start_key_2);

        Station_key end_key_1(_t_endstation, MIN_TRAIN_ID), end_key_2(_t_endstation, MAX_TRAIN_ID);
        find_t_v_station Tmp_end_station = station_record.find(end_key_1, end_key_2);

        vector<query_ticket_t> vec_train;
        auto iter_start = Tmp_start_station.begin();
        auto iter_end = Tmp_end_station.begin();
        int *tmp_tl = new int [105];

        while(iter_start != Tmp_start_station.end() && iter_end != Tmp_end_station.end()){
            Train_id id_1 = (*iter_start).first.train_id;
            Train_id id_2 = (*iter_end).first.train_id;
            if(id_1 < id_2) {++iter_start; continue;}
            if(id_2 < id_1) {++iter_end; continue;}

            int start_order = (*iter_start).second, end_order = (*iter_end).second;
            if(start_order >= end_order) {++iter_start; ++iter_end; continue;}
            find_t_train res = train_record.find(id_1);
            if(!res.first || !res.second.release) {
                ++iter_start; ++iter_end; continue;
            }

            Train tr = res.second;
            int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
            int len = tr.station_num;
            int l_query = end_order - start_order + 1;

            Station start_station, end_station, begin_station;
            rfile.seekg(tr.route_pos, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * start_order, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * end_order, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

            int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));
            if(date_to_begin < start_sale_date || date_to_begin > end_sale_date){
                ++iter_start; ++iter_end; continue;
            }

            tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (date_to_begin * len + start_order), std::ios::beg);
            tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (l_query-1));

            int tmp_seat_left = tmp_tl[0];

            for(int i = 1;i < l_query-1;++i)
                if(tmp_tl[i] < tmp_seat_left)
                    tmp_seat_left = tmp_tl[i];

            query_ticket_t tmp_t_q;
            int tmp_val;
            tmp_t_q.train_id = id_1;
            tmp_t_q.start_time = start_station.depart_time;
            tmp_t_q.end_time = end_station.arrive_time;
            tmp_t_q.seat_left = tmp_seat_left;
            tmp_t_q.price = end_station.price - start_station.price;
            if(_p_timeorcost) tmp_val = tmp_t_q.price;// cost
            else tmp_val = tmp_t_q.end_time - tmp_t_q.start_time;//time
            tmp_t_q.val = tmp_val;

            vec_train.push_back(tmp_t_q);
            ++iter_start; ++iter_end;
        }

        int len_ans = vec_train.size();
        query_ticket_t *ans = new query_ticket_t [len_ans+5];
        for(int i = 0;i < len_ans;++i) ans[i] = vec_train[i];
        Quick_Sort<query_ticket_t>(ans, 0, len_ans-1);

        //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
        os << len_ans << "\n";
        for(int i = 0;i < len_ans;++i){
            os << ans[i].train_id << " " << _s_startstation << " " << int_to_Date(date) << " ";
            os << int_to_Time(ans[i].start_time) << " -> " << _t_endstation << " ";
            int tmp_date = (ans[i].end_time/1440 - ans[i].start_time/1440);
            os << int_to_Date(date + tmp_date) << " " << int_to_Time(ans[i].end_time) << " ";
            os << ans[i].price << " " << ans[i].seat_left << "\n";
        }

        delete[] tmp_tl;
        delete[] ans;
        return;
    }

    void query_transfer(std::istream &is, std::ostream &os){//-s -t -d (-p)////////////////
        get_variables(is,0,0,3,2,2);

        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _s_startstation == _t_endstation) {os <<"0\n"; return;}

        station_transfer.clear();

        Station_key start_key_1(_s_startstation, MIN_TRAIN_ID), start_key_2(_s_startstation, MAX_TRAIN_ID);
        find_t_v_station Tmp_start_station = station_record.find(start_key_1, start_key_2);

        Station_key end_key_1(_t_endstation, MIN_TRAIN_ID), end_key_2(_t_endstation, MAX_TRAIN_ID);
        find_t_v_station Tmp_end_station = station_record.find(end_key_1, end_key_2);

        if(!Tmp_start_station.size() || !Tmp_end_station.size()) {os<<"0\n";return;}

        auto iter_start = Tmp_start_station.begin(), iter_end = Tmp_end_station.begin();
        Station *all_station = new Station[105];
        int ans_val = -1, *tmp_tl = new int[105], Tmp_end_size = Tmp_end_station.size();
        int Tmp_start_s = Tmp_start_station.size(), *tmp_end_time = new int [Tmp_end_size];
        bool *tmp_start_bool = new bool [Tmp_start_s];
        Station_id trans_station_id;
        query_transfer_t ans1, ans2;

        int now_num = -1;
        while(iter_start != Tmp_start_station.end()){
            ++now_num; tmp_start_bool[now_num] = 0;
            Train_id id_1 = (*iter_start).first.train_id;
            int start_order = (*iter_start).second;
            find_t_train res = train_record.find(id_1);
            if(!res.first || !res.second.release) {++iter_start; continue;}

            Train tr = res.second;
            int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;

            if (start_sale_date > date) {++iter_start; continue;}
            int len = tr.station_num;
            int l_query = tr.station_num - start_order;

            Station begin_station, tmp_station, start_station;
            rfile.seekg(tr.route_pos, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * start_order, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (all_station), sizeof(Station) * l_query);
            start_station = all_station[0];

            //--whether pass the end_station--
            int end_order = -1;
            for(int i = 1;i < l_query;++i){
                if(all_station[i].station_id == _t_endstation){
                    end_order = i; break;
                }
            }
            if(end_order != -1){
                Station end_station = all_station[end_order];
                for(int i = 1;i < end_order;++i){
                    tmp_station = all_station[i];
                    if(station_transfer.find(tmp_station.station_id) != station_transfer.end()){
                        query_transfer_t tmp_trans_ticket = station_transfer[tmp_station.station_id];
                        int tmp_val = tmp_trans_ticket.val;
                        //attention date
                        int arr_t = tmp_trans_ticket.end_time;
                        int arr_d = arr_t / 1440; arr_t %= 1440;
                        int dep_d = arr_d;
                        if(tmp_station.depart_time%1440 < arr_t) dep_d++;

                        if(end_sale_date < dep_d - (tmp_station.depart_time/1440-begin_station.depart_time/1440))
                            continue;

                        dep_d = max(dep_d, start_sale_date + (tmp_station.depart_time/1440-begin_station.depart_time/1440));
                        int dep_t = dep_d*1440 + tmp_station.depart_time % 1440;

                        if(_p_timeorcost) tmp_val += end_station.price - tmp_station.price;//cost
                        else tmp_val = end_station.arrive_time - tmp_station.depart_time + dep_t;

                        if(ans_val == -1 || tmp_val < ans_val || (tmp_val == ans_val &&
                        tmp_trans_ticket.end_time- tmp_trans_ticket.start_time < ans1.end_time- ans1.start_time)){
                            ans_val = tmp_val;
                            trans_station_id = tmp_station.station_id;
                            ans1 = tmp_trans_ticket;
                            //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
                            ans2.train_id = tr.train_id;
                            ans2.start_time = dep_t;
                            ans2.end_time = dep_t + end_station.arrive_time - tmp_station.depart_time;
                            ans2.price = end_station.price - tmp_station.price;
                            ans2.date_to_begin = dep_d - (tmp_station.depart_time/1440-begin_station.depart_time/1440);
                       }
                    }
                }
            }

            int date_to_begin = date - (start_station.depart_time/1440-begin_station.depart_time/1440);
            if(end_sale_date < date_to_begin || start_sale_date > date_to_begin) {++iter_start; continue;}

            tmp_start_bool[now_num] = 1;
            int start_d = date;
            int start_t = start_d*1440 + start_station.depart_time%1440;

            for(int i = 1;i < l_query;++i) {
                tmp_station = all_station[i];
                int tmp_val;
                if(_p_timeorcost) tmp_val = tmp_station.price - start_station.price;//cost
                else tmp_val = tmp_station.arrive_time - start_station.depart_time + start_t;//time;

                bool flag = 0;
                if(station_transfer.find(tmp_station.station_id) == station_transfer.end())
                    flag = 1;
                else{
                    query_transfer_t tmp_trans_last = station_transfer[tmp_station.station_id];
                    if(tmp_val < tmp_trans_last.val ||
                      (tmp_val == tmp_trans_last.val && tr.train_id < tmp_trans_last.train_id) )
                        flag = 1;
                }

                if(flag){
                    query_transfer_t tmp_trans_ticket;
                    tmp_trans_ticket.val = tmp_val;
                    tmp_trans_ticket.start_time = start_t;
                    tmp_trans_ticket.end_time = tmp_station.arrive_time - start_station.depart_time + start_t;
                    tmp_trans_ticket.train_id = tr.train_id;
                    tmp_trans_ticket.price = tmp_station.price - start_station.price;
                    tmp_trans_ticket.date_to_begin = start_d - (start_station.depart_time/1440-begin_station.depart_time/1440);
                    station_transfer[tmp_station.station_id] = tmp_trans_ticket;
                }
            }
            ++iter_start;
        }

        now_num = -1;
        while(iter_end != Tmp_end_station.end()){
            ++now_num; tmp_end_time[now_num] = -1;
            Train_id id_1 = (*iter_end).first.train_id;
            int end_order = (*iter_end).second;
            find_t_train res = train_record.find(id_1);
            if(!res.first || !res.second.release) {++iter_end; continue;}

            Train tr = res.second;
            int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
            int len = tr.station_num;
            int l_query = end_order + 1;

            Station begin_station, tmp_station, end_station;
            rfile.seekg(tr.route_pos, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (all_station), sizeof(Station) * l_query);
            begin_station = all_station[0]; end_station = all_station[end_order];
            tmp_end_time[now_num] = end_station.arrive_time;

            for(int i = 0;i < end_order;++i){
                tmp_station = all_station[i];
                if(station_transfer.find(tmp_station.station_id) != station_transfer.end()){
                    query_transfer_t tmp_trans_ticket = station_transfer[tmp_station.station_id];
                    if(tmp_trans_ticket.train_id == tr.train_id) continue;

                    int tmp_val = tmp_trans_ticket.val;
                    //attention date
                    int arr_t = tmp_trans_ticket.end_time;
                    int arr_d = arr_t / 1440;
                    int dep_d = arr_d;
                    if(tmp_station.depart_time%1440 < arr_t%1440) dep_d++;
                    if(end_sale_date < dep_d - (tmp_station.depart_time/1440-begin_station.depart_time/1440))
                        continue;

                    dep_d = max(dep_d, start_sale_date+(tmp_station.depart_time/1440-begin_station.depart_time/1440));
                    int dep_t = dep_d*1440 + tmp_station.depart_time % 1440;

                    if(_p_timeorcost) tmp_val += end_station.price - tmp_station.price;//cost
                    else tmp_val = end_station.arrive_time - tmp_station.depart_time + dep_t;

                    if(ans_val == -1 || tmp_val < ans_val || (tmp_val == ans_val &&
                    tmp_trans_ticket.end_time - tmp_trans_ticket.start_time < ans1.end_time - ans1.start_time)){
                        ans_val = tmp_val;
                        ans1 = tmp_trans_ticket;
                        trans_station_id = tmp_station.station_id;
                        //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
                        ans2.train_id = tr.train_id;
                        ans2.start_time = dep_t;
                        ans2.end_time = dep_t + end_station.arrive_time - tmp_station.depart_time;
                        ans2.price = end_station.price - tmp_station.price;
                        ans2.date_to_begin = dep_d - (tmp_station.depart_time/1440-begin_station.depart_time/1440);
                    }
                }
            }
            ++iter_end;
        }

        station_transfer.clear();
        if(ans_val == -1) {
            os <<"0\n"; delete[]tmp_tl; delete[] all_station;
            delete[] tmp_end_time;
            delete[] tmp_start_bool;
            return;
        }

        if(!_p_timeorcost) {
            //std::cout<<"("<<ans1.train_id<<")";
            int end_station_time = ans2.end_time;
            iter_end = Tmp_end_station.begin(); now_num = -1;
            while (iter_end != Tmp_end_station.end()) {
                ++now_num;
                if(tmp_end_time[now_num]%1440 != end_station_time%1440) {++iter_end; continue;}

                Train_id id_1 = (*iter_end).first.train_id;
                int end_order = (*iter_end).second;
                find_t_train res = train_record.find(id_1);

                Train tr = res.second;
                int start_sale_date = tr.sale_date / 1000, end_sale_date = tr.sale_date % 1000;

                int len = tr.station_num;
                int l_query = end_order + 1;

                Station begin_station, tmp_station, end_station;

                rfile.seekg(tr.route_pos, std::ios::beg);
                rfile.read(reinterpret_cast<char *> (all_station), sizeof(Station) * l_query);
                end_station = all_station[end_order]; begin_station = all_station[0];
                int date_to_begin =
                        end_station_time / 1440 - (end_station.arrive_time / 1440 - begin_station.depart_time / 1440);

                for (int i = 1; i < l_query-1; ++i) {
                    tmp_station = all_station[i];

                    bool flag = 0;
                    if (station_transfer.find(tmp_station.station_id) == station_transfer.end())
                        flag = 1;
                    else {
                        query_transfer_t tmp_trans_last = station_transfer[tmp_station.station_id];
                        if ((end_station.arrive_time - tmp_station.depart_time) <
                             (tmp_trans_last.end_time - tmp_trans_last.start_time))
                            flag = 1;
                    }

                    if (flag) {
                        query_transfer_t tmp_trans_ticket;
                        tmp_trans_ticket.start_time =
                                end_station_time - (end_station.arrive_time - tmp_station.depart_time);
                        tmp_trans_ticket.end_time = end_station_time;
                        tmp_trans_ticket.train_id = tr.train_id;
                        tmp_trans_ticket.price = end_station.price - tmp_station.price;
                        tmp_trans_ticket.date_to_begin = date_to_begin;
                        station_transfer[tmp_station.station_id] = tmp_trans_ticket;
                    }
                }


                //--whether pass the start_station--
                int start_order = -1;
                for (int i = 0; i < l_query-1; ++i) {
                    if (all_station[i].station_id == _s_startstation) {
                        start_order = i;
                        break;
                    }
                }

                if (start_order != -1) {
                    Station start_station = all_station[start_order];
                    int d_t_b = date - (start_station.depart_time / 1440 - begin_station.depart_time / 1440);
                    if (d_t_b >= start_sale_date && d_t_b <= end_sale_date) {
                        int start_t = date * 1440 + start_station.depart_time%1440;

                        for (int i = start_order + 1; i < end_order; ++i) {
                            tmp_station = all_station[i];
                            if (station_transfer.find(tmp_station.station_id) != station_transfer.end()) {
                                query_transfer_t tmp_trans_ticket = station_transfer[tmp_station.station_id];
                                //attention date
                                int dep_t = tmp_trans_ticket.start_time;
                                int arr_t = tmp_station.arrive_time - start_station.depart_time + start_t;
                                if (arr_t > dep_t) continue;

                                if (tmp_station.arrive_time - start_station.depart_time <
                                    ans1.end_time - ans1.start_time) {
                                    trans_station_id = tmp_station.station_id;
                                    ans2 = tmp_trans_ticket;
                                    //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
                                    ans1.train_id = tr.train_id;
                                    ans1.start_time = start_t;
                                    ans1.end_time = arr_t;
                                    ans1.price = tmp_station.price - start_station.price;
                                    ans1.date_to_begin = d_t_b;
                                }
                            }
                        }
                    }
                }
                ++iter_end;
            }

            now_num = -1;
            iter_start = Tmp_start_station.begin();
            while (iter_start != Tmp_start_station.end()) {
                ++now_num; if(!tmp_start_bool[now_num]) {++iter_start; continue;}
                Train_id id_1 = (*iter_start).first.train_id;
                int start_order = (*iter_start).second;
                find_t_train res = train_record.find(id_1);

                Train tr = res.second;
                int start_sale_date = tr.sale_date / 1000, end_sale_date = tr.sale_date % 1000;
                int len = tr.station_num;
                int l_query = len - start_order;

                Station begin_station, tmp_station, start_station;
                rfile.seekg(tr.route_pos, std::ios::beg);
                rfile.read(reinterpret_cast<char *> (&begin_station), sizeof(Station));
                rfile.seekg(tr.route_pos + sizeof(Station) * start_order, std::ios::beg);
                rfile.read(reinterpret_cast<char *> (all_station), sizeof(Station) * l_query);
                start_station = all_station[0];

                int date_to_begin = date - (start_station.depart_time / 1440 - begin_station.depart_time / 1440);
                int start_t = date * 1440 + start_station.depart_time % 1440;

                for (int i = 1; i < l_query; ++i) {
                    tmp_station = all_station[i];
                    if (station_transfer.find(tmp_station.station_id) != station_transfer.end()) {
                        query_transfer_t tmp_trans_ticket = station_transfer[tmp_station.station_id];
                        if (tmp_trans_ticket.train_id == tr.train_id) continue;
                        //attention date
                        int dep_t = tmp_trans_ticket.start_time;
                        int arr_t = tmp_station.arrive_time - start_station.depart_time + start_t;
                        if (arr_t > dep_t) continue;

                        if (tmp_station.arrive_time - start_station.depart_time < ans1.end_time - ans1.start_time) {
                            trans_station_id = tmp_station.station_id;
                            ans2 = tmp_trans_ticket;
                            //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
                            ans1.train_id = tr.train_id;
                            ans1.start_time = start_t;
                            ans1.end_time = arr_t;
                            ans1.price = tmp_station.price - start_station.price;
                            ans1.date_to_begin = date_to_begin;
                        }
                    }
                }

                ++iter_start;
            }

            station_transfer.clear();
        }
        delete[] tmp_end_time;
        delete[] tmp_start_bool;





        //---ans1---
        Station_key ans1_key_1(_s_startstation, ans1.train_id), ans1_key_2(trans_station_id, ans1.train_id);
        find_t_station ans1_station_1 = station_record.find(ans1_key_1);
        find_t_station ans1_station_2 = station_record.find(ans1_key_2);
        Train tr = train_record.find(ans1.train_id).second;

        int arrd_to_begin = ans1.date_to_begin;
        int len = tr.station_num, start_order = ans1_station_1.second, end_order = ans1_station_2.second;
        int l_query = end_order - start_order + 1;
        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (arrd_to_begin * len + start_order), std::ios::beg);
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (l_query-1));

        int tmp_seat_left = tmp_tl[0];
        for(int j = 1;j < l_query-1;++j)
            if(tmp_tl[j] < tmp_seat_left)
                tmp_seat_left = tmp_tl[j];

        os << ans1.train_id << " " << _s_startstation << " " << int_to_Date(ans1.start_time/1440) << " ";
        os << int_to_Time(ans1.start_time%1440) << " -> " << trans_station_id << " ";
        os << int_to_Date(ans1.end_time/1440) << " " << int_to_Time(ans1.end_time%1440) << " ";
        os << ans1.price << " " << tmp_seat_left << "\n";

        //---ans2---
        Station_key ans2_key_1(trans_station_id, ans2.train_id), ans2_key_2(_t_endstation, ans2.train_id);
        find_t_station ans2_station_1 = station_record.find(ans2_key_1);
        find_t_station ans2_station_2 = station_record.find(ans2_key_2);
        tr = train_record.find(ans2.train_id).second;

        arrd_to_begin = ans2.date_to_begin;
        len = tr.station_num, start_order = ans2_station_1.second, end_order = ans2_station_2.second;
        l_query = end_order - start_order + 1;
        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (arrd_to_begin * len + start_order), std::ios::beg);
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (l_query-1));

        tmp_seat_left = tmp_tl[0];
        for(int j = 1;j < l_query-1;++j)
            if(tmp_tl[j] < tmp_seat_left)
                tmp_seat_left = tmp_tl[j];

        os << ans2.train_id << " " << trans_station_id << " " << int_to_Date(ans2.start_time/1440);
        os << " " << int_to_Time(ans2.start_time%1440) << " -> " << _t_endstation << " ";
        os << int_to_Date(ans2.end_time/1440) << " " << int_to_Time(ans2.end_time%1440) << " ";
        os << ans2.price << " " << tmp_seat_left << "\n";

        delete[] tmp_tl;
        delete[] all_station;
        return;
    }

    void buy_ticket(std::istream &is, std::ostream &os){//-u -i -d -n -f -t (-q false)
    //<username>为-u的用户购买：<trainID>为-i，日期为-d，从站-f到站-t的车票-n张。
        get_variables(is,0,3,0,0,2);

        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _f == _t_endstation) {os << "-1\n";return;}

        find_t_user res_user = user_record.find(_u);
        if(!res_user.first || !user_login[_u]) {os << "-1\n"; return;}
        User_value tmp_user_value = res_user.second;

        find_t_train res = train_record.find(_i);
        if(!res.first) {os <<"-1\n"; return;}
        Train tr = res.second;
        if(!tr.release) {os <<"-1\n"; return;}

        Station_key start_key(_f, _i), end_key(_t_endstation, _i);
        find_t_station tmp_start_station = station_record.find(start_key);
        find_t_station tmp_end_station = station_record.find(end_key);/////
        if(!tmp_start_station.first || !tmp_end_station.first) {
           // if(!tmp_start_station.first) os<<"["<<start_key.station_id<<" "<<start_key.train_id<<"]";
           // if(!tmp_end_station.first) os<<"("<<end_key.station_id<<" "<<end_key.train_id<<")";
            os <<"-1\n"; return;
        }

        int start_order = tmp_start_station.second, end_order = tmp_end_station.second;
        if(start_order >= end_order || _n_ticket_num > tr.seat_num) {os <<"-1\n"; return;}

        int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
        int len = tr.station_num;
        int l_query = end_order - start_order + 1;

        Station start_station, end_station, begin_station;
        rfile.seekg(tr.route_pos, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
        rfile.seekg(tr.route_pos + sizeof(Station) * start_order, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));

        int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));

        if(date_to_begin < start_sale_date || date_to_begin > end_sale_date) {os <<"-1\n"; return;}

        rfile.seekg(tr.route_pos + sizeof(Station) * end_order, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

        int *tmp_tl = new int [105];
        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (date_to_begin * len + start_order), std::ios::beg);
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (l_query-1));

        int tmp_seat_left = tmp_tl[0];
        for(int i = 1;i < l_query-1;++i)
            if(tmp_tl[i] < tmp_seat_left)
                tmp_seat_left = tmp_tl[i];

        if(tmp_seat_left < _n_ticket_num && !_q) {os <<"-1\n"; delete[]tmp_tl; return;}

        tmp_user_value.buy_num++; tr.buy_num++;
        user_record.set(_u,tmp_user_value);
        train_record.set(tr.train_id,tr);
        int tmp_pr = end_station.price - start_station.price;


        Order tmp_order;
        tmp_order.user_id = _u; tmp_order.train_id = _i; tmp_order.start_station_id = _f;
        tmp_order.end_station_id = _t_endstation; tmp_order.start_time = start_station.depart_time;
        tmp_order.start_date = date; tmp_order.end_time = end_station.arrive_time;
        tmp_order.end_date = date + (end_station.arrive_time/1440 - start_station.depart_time/1440);
        tmp_order.train_buy_order = tr.buy_num;
        tmp_order.start_order = start_order; tmp_order.end_order = end_order;
        tmp_order.buy_number = _n_ticket_num; tmp_order.price = tmp_pr;

        Order_key tmp_order_key;
        tmp_order_key.user_id = _u;
        tmp_order_key.user_buy_order = tmp_user_value.buy_num;

        if(tmp_seat_left >= _n_ticket_num){
            for(int i = 0;i < l_query-1;++i) tmp_tl[i] -= _n_ticket_num;

            tlfile.seekp(tr.ticket_left_pos + sizeof(int) * (date_to_begin * len + start_order), std::ios::beg);
            tlfile.write(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (l_query-1));

            os << ((long long)tmp_pr) * _n_ticket_num << "\n";

            tmp_order.status = ticket_success;
            odfile.seekp(0, std::ios::end);
            int order_pos = odfile.tellp();
            odfile.write(reinterpret_cast<char*>(&tmp_order), sizeof(Order));
            order_record.insert(tmp_order_key, order_pos);

            delete[]tmp_tl;
            return;
        }

        //---queue---
        os <<"queue\n";

        tmp_order.status = ticket_pending;
        odfile.seekp(0, std::ios::end);
        int order_pos = odfile.tellp();
        odfile.write(reinterpret_cast<char*>(&tmp_order), sizeof(Order));
        order_record.insert(tmp_order_key, order_pos);

        Pending_key tmp_pending_key;
        tmp_pending_key.train_id = tr.train_id;
        tmp_pending_key.pending_buy_time = tr.buy_num;
        pending_record.insert(tmp_pending_key, order_pos);

        delete[]tmp_tl;
        return;
    }

    int refund_ticket(std::istream &is){//-u (-n 1)
        get_variables(is,0,4,0,0,0);
        //std::cout<<"111111111111111111111111111111 "<<_u <<" "<<_n_order<<" ";
        find_t_user tmp_res_user = user_record.find(_u);
        if(!tmp_res_user.first || !user_login[_u]) {return FAIL;}

        User_value tmp_user_value = tmp_res_user.second;

        Order_key tmp_order_key;
        tmp_order_key.user_id = _u;
        tmp_order_key.user_buy_order = tmp_user_value.buy_num - _n_order + 1;
        find_t_order tmp_res_order = order_record.find(tmp_order_key);
        if(!tmp_res_order.first) {return FAIL;}

        int tmp_order_pos = tmp_res_order.second;
        Order tmp_order;
        odfile.seekg(tmp_order_pos, std::ios::beg);
        odfile.read(reinterpret_cast<char*> (&tmp_order), sizeof(Order));

        if(tmp_order.status == ticket_refund) {return FAIL;}

        Ticket_status origin_ticket_status = tmp_order.status;
        tmp_order.status = ticket_refund;
        odfile.seekp(tmp_order_pos, std::ios::beg);
        odfile.write(reinterpret_cast<char*> (&tmp_order), sizeof(Order));

        if(origin_ticket_status == ticket_pending) {
            Pending_key tmp_pending_key;
            tmp_pending_key.train_id = tmp_order.train_id;
            tmp_pending_key.pending_buy_time = tmp_order.train_buy_order;
            pending_record.remove(tmp_pending_key);
            return SUCCESS;
        }

        //---success---

        Station_key start_key(tmp_order.start_station_id, tmp_order.train_id);
        Station_key end_key(tmp_order.end_station_id, tmp_order.train_id);
        find_t_station tmp_start_station = station_record.find(start_key);
        find_t_station tmp_end_station = station_record.find(end_key);

        find_t_train tmp_res_tr = train_record.find(tmp_order.train_id);
        Train tr = tmp_res_tr.second;

        int *tmp_tl = new int [105];
        int start_order = tmp_start_station.second, end_order = tmp_end_station.second;

        int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
        int len = tr.station_num;

        Station start_station, end_station, begin_station;
        rfile.seekg(tr.route_pos, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
        rfile.seekg(tr.route_pos + sizeof(Station) * start_order, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));

        int date = tmp_order.start_date;
        int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));

        rfile.seekg(tr.route_pos + sizeof(Station) * end_order, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * date_to_begin * len, std::ios::beg);
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (len-1));

        int refund_num = tmp_order.buy_number;
        for(int i = start_order;i < end_order;++i)
            tmp_tl[i] += refund_num;

        Pending_key tmp_pending_key_1;
        tmp_pending_key_1.train_id = tmp_order.train_id;
        tmp_pending_key_1.pending_buy_time = tmp_order.train_buy_order;
        Pending_key tmp_pending_key_2;
        tmp_pending_key_2.train_id = tmp_order.train_id;
        tmp_pending_key_2.pending_buy_time = tr.buy_num + 1;
        find_t_v_pending vec_pending = pending_record.find(tmp_pending_key_1,tmp_pending_key_2);

        //std::cout << "size " << vec_pending.size()<< std::endl;
        auto iter_pending = vec_pending.begin();

        while(iter_pending != vec_pending.end()){
            Pending_key tmp_pending_key = (*iter_pending).first;

            tmp_order_pos = (*iter_pending).second;
            odfile.seekg(tmp_order_pos, std::ios::beg);
            odfile.read(reinterpret_cast<char*> (&tmp_order), sizeof(Order));
///////////////////
            start_order = tmp_order.start_order; end_order = tmp_order.end_order;
            rfile.seekg(tr.route_pos + sizeof(Station)*start_order, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));
            int order_date_to_begin = tmp_order.start_date - (start_station.depart_time/1440 - begin_station.depart_time/1440);

            if(order_date_to_begin != date_to_begin) {++iter_pending;continue;}

            int tmp_seat_left = tmp_tl[start_order];
            for(int i = start_order;i < end_order;++i)
                if(tmp_tl[i] < tmp_seat_left)
                    tmp_seat_left = tmp_tl[i];

            if(tmp_seat_left >= tmp_order.buy_number){
                tmp_order.status = ticket_success;
                for(int i = start_order;i < end_order;++i)
                    tmp_tl[i] -= tmp_order.buy_number;

                pending_record.remove(tmp_pending_key);

                odfile.seekp(tmp_order_pos, std::ios::beg);
                odfile.write(reinterpret_cast<char*> (&tmp_order), sizeof(Order));
            }

            ++iter_pending;
        }

        tlfile.seekp(tr.ticket_left_pos + sizeof(int) * date_to_begin * len, std::ios::beg);
        tlfile.write(reinterpret_cast<char*> (tmp_tl), sizeof(int) * (len-1));

        delete [] tmp_tl;
        return SUCCESS;
    }

    void query_order(std::istream &is, std::ostream &os){//-u
        get_variables(is,0,0,0,0,0);

        find_t_user res_user = user_record.find(_u);
        if(!res_user.first || !user_login[_u]) {os <<"-1\n"; return;}

        User_value tmp_user_value = res_user.second;
        int tmp_buy_num = tmp_user_value.buy_num;

        Order_key tmp_order_key_1, tmp_order_key_2;
        tmp_order_key_1.user_id = _u; tmp_order_key_1.user_buy_order = 1;
        tmp_order_key_2.user_id = _u; tmp_order_key_2.user_buy_order = tmp_buy_num + 1;
        find_t_v_order vec_order = order_record.find(tmp_order_key_1, tmp_order_key_2);

        os << tmp_buy_num << "\n";
        if(!tmp_buy_num) return;

        auto iter_order = vec_order.end() - 1;

        while(1){
            int tmp_order_pos = (*iter_order).second;
            Order tmp_order;

            odfile.seekg(tmp_order_pos, std::ios::beg);
            odfile.read(reinterpret_cast<char*> (&tmp_order), sizeof(Order));

//[pending] HAPPY_TRAIN 上院 08-17 05:24 -> 下院 08-17 15:24 628 500
            if(tmp_order.status == ticket_success) os << "[success] ";
            if(tmp_order.status == ticket_pending) os << "[pending] ";
            if(tmp_order.status == ticket_refund) os << "[refunded] ";

            os << tmp_order.train_id << " " << tmp_order.start_station_id << " ";
            os << int_to_Date(tmp_order.start_date) << " " << int_to_Time(tmp_order.start_time);
            os << " -> " << tmp_order.end_station_id << " " << int_to_Date(tmp_order.end_date) << " ";
            os << int_to_Time(tmp_order.end_time) << " " << tmp_order.price << " ";
            os << tmp_order.buy_number <<"\n";


            if(iter_order == vec_order.begin()) break;
            --iter_order;
        }

        return;
    }

    int add_user(std::istream &is){
        //<cur_username>-c，<username>-u，<password>-p，<name>-n，<mailAddr>-m，<privilege>-g
        get_variables(is,1,1,1,0,0);
        //std::cout<<"["<<_p_password<<" "<<_n_name<<" "<<_m_email<<" "<<_g<<"]";

        User U;
        U.user_id = _u; U.password = _p_password; U.name = _n_name; U.email = _m_email; U.privilege = _g;

        find_t_user res_user = user_record.find(U.user_id);
        if(res_user.first) return FAIL;

        find_t_user res_cur = user_record.find(_c);
        if(Change && !res_cur.first) return FAIL;

        if(Change) {
            if(!user_login[_c]) return FAIL;
            int pos_cur = res_cur.second.user_pos;
            User user_cur;
            ufile.seekg(pos_cur, std::ios::beg);
            ufile.read(reinterpret_cast<char *> (&user_cur), sizeof(User));
            if (user_cur.privilege <= U.privilege) return FAIL;
        }

        user_login[U.user_id] = false;
        if(!Change) {U.privilege = 10; Change = true;}

        User_value tmp_value; tmp_value.buy_num = 0;
        ufile.seekp(0, std::ios::end);
        tmp_value.user_pos = ufile.tellp();
        ufile.write(reinterpret_cast<char*> (&U), sizeof(User));

        user_record.insert(U.user_id, tmp_value);
        return SUCCESS;
    }

    int login(std::istream &is){
        get_variables(is,0,0,1,0,0);

        find_t_user res = user_record.find(_u);
        if(!res.first) {return FAIL;}
        if(user_login[_u] == true) {return FAIL;}
        int user_pos = res.second.user_pos;

        User tmp_user;
        ufile.seekg(user_pos, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&tmp_user), sizeof(User));

        if(tmp_user.password != _p_password) {return FAIL;}

        user_login[_u] = true;
        return SUCCESS;
    }

    int logout(std::istream &is){
        get_variables(is,0,0,0,0,0);

        find_t_user res = user_record.find(_u);
        if(!res.first) return FAIL;
        if(user_login[_u] == false) return FAIL;

        user_login[_u] = false;
        return SUCCESS;
    }

    void query_profile(std::istream &is, std::ostream &os){
        get_variables(is,0,0,0,0,0);

        if(!user_login[_c]) {os << "-1\n"; return;}
        find_t_user res_cur = user_record.find(_c);
        if(!res_cur.first) {os << "-1\n"; return;}
        find_t_user res_query = user_record.find(_u);
        if(!res_query.first) {os << "-1\n"; return;}

        int user_pos_cur = res_cur.second.user_pos, user_pos_query = res_query.second.user_pos;
        User user_cur, user_query;
        ufile.seekg(user_pos_cur, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&user_cur), sizeof(User));

        ufile.seekg(user_pos_query, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&user_query), sizeof(User));
        if(user_cur.privilege <= user_query.privilege && _c != _u) {os << "-1\n"; return;}

        os << user_query.user_id << " " << user_query.name << " ";
        os << user_query.email << " " << user_query.privilege <<"\n";
        return;
    }

    void modify_profile(std::istream &is, std::ostream &os){
        get_variables(is,1,1,1,0,0);

        if(!user_login[_c]) {os << "-1\n"; return;}
        find_t_user res_cur = user_record.find(_c);
        if(!res_cur.first) {os << "-1\n"; return;}
        find_t_user res_query = user_record.find(_u);
        if(!res_query.first) {os << "-1\n"; return;}

        int user_pos_cur = res_cur.second.user_pos, user_pos_query = res_query.second.user_pos;
        User user_cur, user_query;
        ufile.seekg(user_pos_cur, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&user_cur), sizeof(User));

        ufile.seekg(user_pos_query, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&user_query), sizeof(User));

        if(user_cur.privilege < user_query.privilege ||
        (user_cur.privilege == user_query.privilege && _c != _u)) {os << "-1\n"; return;}

        if(f_p_password) user_query.password = _p_password;
        if(f_n_name) user_query.name = _n_name;
        if(f_m_email) user_query.email = _m_email;
        if(f_g) user_query.privilege = _g;

        ufile.seekp(user_pos_query, std::ios::beg);
        ufile.write(reinterpret_cast<char*> (&user_query), sizeof(User));

       // os << user_query.password<<"                ";

        os << user_query.user_id << " " << user_query.name << " ";
        os << user_query.email << " " << user_query.privilege <<"\n";
        return;
    }
};

#endif //INC_20200512_TRAIN_MANAGER_H
