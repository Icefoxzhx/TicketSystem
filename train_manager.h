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
    typedef vector<pair<Station_key,int> > find_t_v_station;
    typedef vector<pair<Pending_key,int> > find_t_v_pending;
    typedef vector<pair<Order_key,int> > find_t_v_order;

    struct query_ticket_t{
        Train_id train_id;
        int start_time, end_time, price, seat_left, val;

        bool operator < (const query_ticket_t &rhs) const{return val < rhs.val;}
        bool operator == (const query_ticket_t &rhs) const{return val == rhs.val;}
        bool operator > (const query_ticket_t &rhs) const{return val > rhs.val;}
        bool operator <= (const query_ticket_t &rhs) const{return val <= rhs.val;}
        bool operator >= (const query_ticket_t &rhs) const{return val >= rhs.val;}
    };


    std::fstream rfile;
    std::fstream tlfile;
    std::fstream odfile;



    int string_to_int(hzstring<50> str){
        int len_str = strlen(str.ch), res = 0;
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
        int hr = (t[0]-'0')*10 + t[1]-'0';
        int mi = (t[3]-'0')*10 + t[4]-'0';
        return hr*60+mi;
    }

    Date int_to_Date(const int &t){//mm-dd
        char tmp[6];
        int mm = 6, dd = t;
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
        int hr = (t%1440) / 60, mi = (t%1400) % 60;
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
        Signal _z;
        char _s[50], _p[50], _t[50], _o[50], _d[50], _x[50], _n[50], _m[50];
        tr.release = 0; tr.buy_num = 0;
        is >> _z >> tr.train_id >> _z >> _n >> _z >> _m;
        tr.station_num = string_to_int(_n); tr.seat_num = string_to_int(_m);

        if(train_record.count(tr.train_id)){
            char tmp[1000];
            is.getline(tmp,1000);
            return FAIL;
        }

        is >> _z >> _s >> _z >> _p >> _z >> _x >> _z >> _t;
        is >> _z >> _o >> _z >> _d >> _z >> tr.type;

        int len = tr.station_num;
        Station *tmp_station = new Station [len];

        int p_s = 0, p_t = 0, p_o = 0;
        int l_s = strlen(_s), l_t = strlen(_t), l_o = strlen(_o), l_tmp = 0;
        char tmp[21];

        //---station_id---
        for(int i = 0;i < len;++i) {
            for (l_tmp = 0; _s[p_s] != '|' && p_s < l_s; l_tmp++, p_s++)
                tmp[l_tmp] = _s[p_s];
            tmp[l_tmp] = '\0'; ++p_s;
            tmp_station[i].station_id = tmp;
        }

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
            for (; _t[p_t] != '|' && p_t < l_t; p_t++)
                tmp_travel = tmp_travel * 10 + _t[p_t] - '0';
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
        for (; _t[p_t] != '|' && p_t < l_t; p_t++)
            tmp_travel = tmp_travel * 10 + _t[p_t] - '0';
        p_t++;
        now += tmp_travel;
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
            tmp_station[i].price = tmp_price + tmp_station[i-1].price;
        }

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
        int *tmp_tl = new int [tlnum];

        for(int i = 0;i < tlnum;++i) tmp_tl[i] = -1;

        for(int i = start_date;i <= end_date;++i)
            for (int j = 0;j < len;++j)
                tmp_tl[i*len+j] = tr.seat_num;

        tlfile.seekp(0, std::ios::end);
        tr.ticket_left_pos = tlfile.tellp();
        tlfile.write(reinterpret_cast<char*>(tmp_tl), sizeof(int) * tlnum);

        tr.sale_date = start_date * 1000 + end_date;
        train_record.insert(tr.train_id, tr);

        delete [] tmp_tl;
        delete [] tmp_station;
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
        int date = Date_to_int(_d);

        find_t_train res = train_record.find(_i);
        if(res.first == false) {os << "-1\n"; return;}

        Train tr = res.second;
        int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date % 1000;
        if(date < start_sale_date || date > end_sale_date) {os << "-1\n"; return;}

        os << tr.train_id << " " << tr.type << "\n";

        int len = tr.station_num;
        rfile.seekg(tr.route_pos, std::ios::beg);
        Station *tmp_station = new Station [len];
        rfile.read(reinterpret_cast<char*> (tmp_station), sizeof(Station) * len);

        tlfile.seekg(tr.ticket_left_pos + (date-1) * len, std::ios::beg);
        int *tmp_tl = new int [len];
        tlfile.read(reinterpret_cast<char*> (tmp_tl), sizeof(int) * len);

        //for start station
        os << tmp_station[0].station_id <<"xx-xx xx:xx -> " << int_to_Date(date) << " ";
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
        Train_id _i;
        Signal _z;
        is >> _z >> _i;

        find_t_train res = train_record.find(_i);
        if(!res.first) return FAIL;

        Train tr = res.second;
        if(tr.release) return FAIL;

        int len = tr.station_num;
        rfile.seekg(tr.route_pos, std::ios::beg);
        Station *tmp_station = new Station [len];
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

    int query_ticket(std::istream &is, std::ostream &os){//-s -t -d (-p time)
        Station_id _s, _t;
        Date _d;
        bool p_c = false; // false == time   true == cost
        char str[50];
        Signal _z;

        is >> _z >> _s >> _z >> _t >> _z >> _d;
        is.getline(str,50);
        if(strlen(str) && str[0] == 'c') p_c = true;

        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _s == _t) return FAIL;

        Station_key start_key_1(_s, MIN_TRAIN_ID), start_key_2(_s, MAX_TRAIN_ID);
        find_t_v_station tmp_start_station = station_record.find(start_key_1, start_key_2);

        Station_key end_key_1(_t, MIN_TRAIN_ID), end_key_2(_t, MAX_TRAIN_ID);
        find_t_v_station tmp_end_station = station_record.find(end_key_1, end_key_2);

        vector<query_ticket_t> vec_train;
        auto iter_start = tmp_start_station.begin();
        auto iter_end = tmp_end_station.begin();
        int *tmp_tl = new int [105];

        while(iter_start != tmp_start_station.end() && iter_end != tmp_end_station.end()){
            Train_id id_1((*iter_start).first.train_id), id_2((*iter_end).first.train_id);
            if(id_1 < id_2) {++iter_start; continue;}
            if(id_2 < id_1) {++iter_end; continue;}

            int start_order = (*iter_start).second, end_order = (*iter_end).second;
            if(start_order >= end_order) {++iter_start; ++iter_end; continue;}
            find_t_train res = train_record.find(id_1);
            if(!res.first || !res.second.release) {++iter_start; ++iter_end; continue;}

            Train tr = res.second;
            int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
            int len = tr.station_num;
            int l_query = end_order - start_order + 1;

            Station start_station, end_station, begin_station;
            rfile.seekg(tr.route_pos, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * (start_order - 1), std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));
            int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));
            if(date_to_begin < start_sale_date || date_to_begin > end_sale_date){
                ++iter_start; ++iter_end; continue;
            }

            rfile.seekg(tr.route_pos + sizeof(Station) * (end_order - 1), std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

            tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (date_to_begin-1) * len + start_order - 1, std::ios::beg);
            tlfile.read(reinterpret_cast<char*> (&tmp_tl), sizeof(int) * l_query);

            int tmp_seat_left = tmp_tl[0];
            for(int i = 1;i < l_query;++i)
                if(tmp_tl[i] < tmp_seat_left)
                    tmp_seat_left = tmp_tl[i];

            query_ticket_t tmp_t_q;
            int tmp_val;
            tmp_t_q.train_id = id_1;
            tmp_t_q.start_time = start_station.depart_time;
            tmp_t_q.end_time = end_station.arrive_time;
            tmp_t_q.seat_left = tmp_seat_left;
            tmp_t_q.price = end_station.price - start_station.price;
            if(p_c) tmp_val = tmp_t_q.price;// cost
            else tmp_val = tmp_t_q.end_time - tmp_t_q.start_time;//time
            tmp_t_q.val = tmp_val;

            vec_train.push_back(tmp_t_q);
        }

        int len_ans = vec_train.size();
        query_ticket_t *ans = new query_ticket_t [len_ans];
        for(int i = 0;i < len_ans;++i) ans[i] = vec_train[i];
        Quick_Sort<query_ticket_t>(ans, 0, len_ans-1);

        //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
        os << len_ans << "\n";
        for(int i = 0;i < len_ans;++i){
            os << ans[i].train_id << " " << _s << " " << int_to_Date(date) << " ";
            os << int_to_Time(ans[i].start_time) << " -> " << _t << " ";
            int tmp_date = (ans[i].end_time/1440 - ans[i].start_time/1440);
            os << int_to_Date(date + tmp_date) << " " << int_to_Time(ans[i].end_time) << " ";
            os << ans[i].price << " " << ans[i].seat_left << "\n";
        }

        delete[] tmp_tl;
        delete[] ans;
        return SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////

    int query_transfer(std::istream &is, std::ostream &os){//-s -t -d (-p)////////////////
        Station_id _s, _t;
        Date _d;
        bool p_c = false; // false time true cost
        Signal _z;
        char tmp_str[50];

        is >> _z >> _s >> _z >> _t >> _z >> _d;
        is.getline(tmp_str,50);
        if(strlen(tmp_str) && tmp_str[0] == 'c') p_c = true;

        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _s == _t) return FAIL;


     /*   Station_key start_key_1(_s, MIN_TRAIN_ID), start_key_2(_s, MAX_TRAIN_ID);
        find_t_station tmp_start_station = station_record.find(start_key_1, start_key_2);

        Station_key end_key_1(_t, MIN_TRAIN_ID), end_key_2(_t, MAX_TRAIN_ID);
        find_t_station tmp_end_station = station_record.find(end_key_1, end_key_2);

        vector<query_ticket_t> vec_train_start;
        auto iter_start = tmp_start_station.begin();
        while(iter_start != tmp_start_station.end()){
            Train_id id_1((*iter_start).first.train_id);
            int start_order = (*iter_start).second;
            find_t_train res = train_record.find(id_1);
            if(!res.first || !res.second.release) {++iter_start; continue;}

            Train tr = res.second;
            int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
            int len = tr.station_num;
            int l_query = end_order - start_order + 1;

            Station start_station, end_station, begin_station;
            rfile.seekg(tr.route_pos, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * (start_order - 1), std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));
            int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));
            if(date_to_begin < start_sale_date || date_to_begin > end_sale_date){
                ++iter_start; continue;
            }



            ++iter_start;
        }




        auto iter_end = tmp_end_station.begin();
        int *tmp_tl = new int [105];

        while(iter_start != tmp_start_station.end() && item_end != tmp_end_station.end()){
            Train_id id_1((*iter_start).first.train_id), id_2((*iter_end).first.train_id);
            if(id_1 < id_2) {++iter_start; continue;}
            if(id_2 < id_1) {++iter_end; continue;}

            int start_order = (*iter_start).second, end_order = (*iter_end).second;
            if(start_order >= end_order) {++iter_start; ++iter_end; continue;}
            find_t_train res = train_record.find(id_1);
            if(!res.first || !res.second.release) {++iter_start; ++iter_end; continue;}

            Train tr = res.second;
            int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
            int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));
            if(date_to_begin < start_sale_date || date_to_begin > end_sale_date){
                ++iter_start; ++iter_end; continue;
            }

            int len = tr.station_num;
            int l_query = end_order - start_order + 1;

            Station start_station, end_station, begin_station;
            rfile.seekg(tr.route_pos, std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * (start_order - 1), std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));
            rfile.seekg(tr.route_pos + sizeof(Station) * (end_order - 1), std::ios::beg);
            rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

            tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (date_to_begin-1) * len + start_order - 1, std::ios::beg);
            tlfile.read(reinterpret_cast<char*> (&tmp_tl), sizeof(int) * l_query);

            int tmp_seat_left = tmp_tl[0];
            for(int i = 1;i < l_query;++i)
                if(tmp_tl[i] < tmp_ticket_left)
                    tmp_seat_left = tmp_tl[i];

            query_ticket_t tmp_t_q;
            int tmp_val;
            tmp_t_q.train_id = id_1;
            tmp_t_q.start_time = start_station.depart_time;
            tmp_t_q.end_time = end_station.arrive_time;
            tmp_t_q.seat_left = tmp_seat_left;
            tmp_t_q.price = end_station.price - start_station.price;
            if(p_c) tmp_val = tmp_t_q.price;// cost
            else tmp_val = tmp_t_q.end_time - tmp_t_q.start_time;//time
            tmp_t_q.val = tmp_val;

            vec_train.push_back(tmp_t_q);
        }

        int len_ans = vec_train.size();
        query_ticket_t *ans = new query_ticket_t [len_ans];
        for(int i = 0;i < len_ans;++i) ans[i] = vec_train[i];
        Quick_Sort<query_ticket_t>(ans, 0, len_ans-1);

        //HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000
        os << len_ans << "\n";
        for(int i = 0;i < len_ans;++i){
            os << ans[i].second.train_id << " " << _s << " " << int_to_Date(date) << " ";
            os << int_to_Time(ans[i].second.start_time) << " -> " << _t << " ";
            int tmp_date = (ans[i].second.end_time/1440 - ans[i].second.start_time/1440);
            os << int_to_Date(date + tmp_date) << " " << int_to_Time(ans[i].second.end_time) << " ";
            os << ans[i].second.price << " " << ans[i].second.seat_left << "\n";
        }

        delete[] tmp_tl;
        delete[] ans;*/
        return SUCCESS;
    }

    void buy_ticket(std::istream &is, std::ostream &os){//-u -i -d -n -f -t (-q false)
    //<username>为-u的用户购买：<trainID>为-i，日期为-d，从站-f到站-t的车票-n张。
        Userid _u;
        Train_id _i;
        Date _d;
        int _n;
        Station_id _f, _t;
        bool _q = false;
        char tmp_str[20];
        Signal _z;

        is >> _z >> _u >> _z >> _i >> _z >> _d >> _z >> _n >> _z >> _f >> _z >> _t;
        is.getline(tmp_str, 20);
        if(strlen(tmp_str) && tmp_str[0] == 't') _q = true;
        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _f == _t) {os << "-1\n";return;}

        find_t_user res_user = user_record.find(_u);
        if(!res_user.first) {os << "-1\n"; return;}
        User_value tmp_user_value = res_user.second;

        find_t_train res = train_record.find(_i);
        if(!res.first) {os <<"-1\n"; return;}
        Train tr = res.second;
        if(!tr.release) {os <<"-1\n"; return;}

        Station_key start_key(_f, _i), end_key(_t, _i);
        find_t_station tmp_start_station = station_record.find(start_key);
        find_t_station tmp_end_station = station_record.find(end_key);
        if(!tmp_start_station.first || !tmp_end_station.first) {os <<"-1\n"; return;}

        int *tmp_tl = new int [105];
        int start_order = tmp_start_station.second, end_order = tmp_end_station.second;
        if(start_order >= end_order) {os <<"-1\n"; return;}

        int start_sale_date = tr.sale_date/1000, end_sale_date = tr.sale_date%1000;
        int len = tr.station_num;
        int l_query = end_order - start_order + 1;

        Station start_station, end_station, begin_station;
        rfile.seekg(tr.route_pos, std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&begin_station), sizeof(Station));
        rfile.seekg(tr.route_pos + sizeof(Station) * (start_order - 1), std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));

        int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));
        if(date_to_begin < start_sale_date || date_to_begin > end_sale_date) {os <<"-1\n"; return;}

        rfile.seekg(tr.route_pos + sizeof(Station) * (end_order - 1), std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (date_to_begin-1) * len + start_order - 1, std::ios::beg);
        tlfile.read(reinterpret_cast<char*> (&tmp_tl), sizeof(int) * l_query);

        int tmp_seat_left = tmp_tl[0];
        for(int i = 1;i < l_query;++i)
            if(tmp_tl[i] < tmp_seat_left)
                tmp_seat_left = tmp_tl[i];

        if(tmp_seat_left < _n && !_q) {os <<"-1\n"; return;}

        tmp_user_value.buy_num++; train_record.set(tr.train_id,tr);
        tr.buy_num++; user_record.set(_u,tmp_user_value);
        int tmp_pr = end_station.price - start_station.price;

        Order tmp_order;
        tmp_order.user_id = _u; tmp_order.train_id = _i; tmp_order.start_station_id = _f;
        tmp_order.end_station_id = _t; tmp_order.start_time = start_station.depart_time;
        tmp_order.start_date = date; tmp_order.end_time = end_station.arrive_time;
        tmp_order.end_date = date + (end_station.arrive_time/1440 - start_station.depart_time/1440);
        tmp_order.train_buy_order = tr.buy_num;
        tmp_order.start_order = start_order; tmp_order.end_order = end_order;
        tmp_order.buy_number = _n; tmp_order.price = tmp_pr;

        Order_key tmp_order_key;
        tmp_order_key.user_id = _u;
        tmp_order_key.user_buy_order = tmp_user_value.buy_num;

        if(tmp_seat_left >= _n){
            for(int i = 0;i < l_query;++i) tmp_tl[i] -= _n;
            tlfile.seekp(tr.ticket_left_pos + sizeof(int) * (date_to_begin-1) * len + start_order - 1, std::ios::beg);
            tlfile.write(reinterpret_cast<char*> (&tmp_tl), sizeof(int) * l_query);
            os << (tmp_pr * _n) << "\n";

            tmp_order.status = ticket_success;
            odfile.seekp(0, std::ios::end);
            int order_pos = odfile.tellp();
            odfile.write(reinterpret_cast<char*>(&tmp_order), sizeof(Order));
            order_record.insert(tmp_order_key, order_pos);


            int tmp_seat_left = tmp_tl[0], tmp_start = tmp_order.start_order;
            for(int i = 1;i < l_query;++i)
                if(tmp_tl[i] < tmp_seat_left)
                    tmp_seat_left = tmp_tl[i];

            return;
        }

        //---queue---

        tmp_order.status = ticket_pending;
        odfile.seekp(0, std::ios::end);
        int order_pos = odfile.tellp();
        odfile.write(reinterpret_cast<char*>(&tmp_order), sizeof(Order));
        order_record.insert(tmp_order_key, order_pos);

        Pending_key tmp_pending_key;
        tmp_pending_key.train_id = tr.train_id;
        tmp_pending_key.pending_buy_time = tr.buy_num;
        pending_record.insert(tmp_pending_key, order_pos);

        return;
    }

    int refund_ticket(std::istream &is){//-u (-n 1)
        Userid _u;
        int _n = 1;
        Signal _z;
        char str[50];

        is >> _z >> _u;
        is.getline(str,50);
        if(strlen(str)){ _n = 0; for(int i = 0; i < strlen(str);++i) _n = _n*10 + str[i] - '0';}

        find_t_user tmp_res_user = user_record.find(_u);
        if(!tmp_res_user.first || !user_login[_u]) return FAIL;

        User_value tmp_user_value = tmp_res_user.second;

        Order_key tmp_order_key;
        tmp_order_key.user_id = _u;
        tmp_order_key.user_buy_order = tmp_user_value.buy_num - _n + 1;
        find_t_order tmp_res_order = order_record.find(tmp_order_key);
        if(!tmp_res_order.first)return FAIL;

        int tmp_order_pos = tmp_res_order.second;
        Order tmp_order;
        odfile.seekg(tmp_order_pos, std::ios::beg);
        odfile.read(reinterpret_cast<char*> (&tmp_order), sizeof(Order));

        if(tmp_order.status == ticket_refund) return FAIL;

        tmp_order.status = ticket_refund;
        odfile.seekp(tmp_order_pos, std::ios::beg);
        odfile.write(reinterpret_cast<char*> (&tmp_order), sizeof(Order));

        Pending_key tmp_pending_key;
        tmp_pending_key.train_id = tmp_order.train_id;
        tmp_pending_key.pending_buy_time = tmp_order.train_buy_order;
        pending_record.remove(tmp_pending_key);

        if(tmp_order.status == ticket_pending) {return SUCCESS;}

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
        rfile.seekg(tr.route_pos + sizeof(Station) * (start_order - 1), std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&start_station), sizeof(Station));

        int date = tmp_order.start_date;
        int date_to_begin = date - ((start_station.depart_time/1440 - begin_station.depart_time/1440));

        rfile.seekg(tr.route_pos + sizeof(Station) * (end_order - 1), std::ios::beg);
        rfile.read(reinterpret_cast<char*> (&end_station), sizeof(Station));

        tlfile.seekg(tr.ticket_left_pos + sizeof(int) * (date_to_begin-1) * len, std::ios::beg);
        tlfile.read(reinterpret_cast<char*> (&tmp_tl), sizeof(int) * len);



        Pending_key tmp_pending_key_1;
        tmp_pending_key.train_id = tmp_order.train_id;
        tmp_pending_key.pending_buy_time = 1;
        Pending_key tmp_pending_key_2;
        tmp_pending_key.train_id = tmp_order.train_id;
        tmp_pending_key.pending_buy_time = tmp_order.train_buy_order;
        find_t_v_pending vec_pending = pending_record.find(tmp_pending_key_1,tmp_pending_key_2);

        auto iter_pending = vec_pending.begin();

        while(iter_pending != vec_pending.end()){
            Pending_key tmp_pending_key = (*iter_pending).first;

            tmp_order_pos = (*iter_pending).second;
            odfile.seekg(tmp_order_pos, std::ios::beg);
            odfile.read(reinterpret_cast<char*> (&tmp_order), sizeof(Order));

            start_order = tmp_order.start_order; end_order = tmp_order.end_order;

            int tmp_seat_left = tmp_tl[start_order];
            for(int i = start_order;i <= end_order;++i)
                if(tmp_tl[i] < tmp_seat_left)
                    tmp_seat_left = tmp_tl[i];

            if(tmp_seat_left >= tmp_order.buy_number){
                tmp_order.status = ticket_success;
                for(int i = start_order;i <= end_order;++i)
                    tmp_tl[i] -= tmp_order.buy_number;

                odfile.seekp(tmp_order_pos, std::ios::beg);
                odfile.write(reinterpret_cast<char*> (&tmp_order), sizeof(Order));
            }
        }

        for(int i = start_order;i <= end_order;++i) tmp_tl[i] += _n;
        tlfile.seekp(tr.ticket_left_pos + sizeof(int) * (date_to_begin-1) * len, std::ios::beg);
        tlfile.write(reinterpret_cast<char*> (&tmp_tl), sizeof(int) * len);

        delete [] tmp_tl;
        return SUCCESS;
    }

    void query_order(std::istream &is, std::ostream &os){//-u
        Userid _u;
        Signal _z;

        is >> _z >> _u;

        find_t_user res_user = user_record.find(_u);
        if(!res_user.first || !user_login[_u]) {os <<"-1\n"; return;}

        User_value tmp_user_value = res_user.second;
        int tmp_buy_num = tmp_user_value.buy_num;

        Order_key tmp_order_key_1, tmp_order_key_2;
        tmp_order_key_1.user_id = _u; tmp_order_key_1.user_buy_order = 1;
        tmp_order_key_2.user_id = _u; tmp_order_key_2.user_buy_order = tmp_buy_num + 1;
        find_t_v_order vec_order = order_record.find(tmp_order_key_1, tmp_order_key_2);

        os << tmp_buy_num << "\n";

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
        }

        return;
    }

    bool check_privilege(const User &lhs, const User &rhs){// lhs.g > rhs.g
        return lhs.privilege > rhs.privilege;
    }

    int add_user(std::istream &is){
        //<cur_username>-c，<username>-u，<password>-p，<name>-n，<mailAddr>-m，<privilege>-g
        User U;
        Userid cur_id;
        Signal _z;
        is >> _z >> cur_id >> _z >> U.user_id >> _z >> U.password >> _z >> U.name;
        is >> _z >> U.email >> _z >> U.privilege;

        find_t_user res_user = user_record.find(U.user_id);
        if(res_user.first) return FAIL;

        find_t_user res_cur = user_record.find(cur_id);
        if(Change && !res_cur.first) return FAIL;

        if(Change) {
            int pos_cur = res_cur.second.user_pos;
            User user_cur;
            ufile.seekg(pos_cur, std::ios::beg);
            ufile.read(reinterpret_cast<char *> (&user_cur), sizeof(User));
            if (!check_privilege(user_cur, U)) return FAIL;
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
        Userid _u;
        Password _p;
        Signal _z;
        is >> _z >> _u >> _z >> _p;

        find_t_user res = user_record.find(_u);
        if(!res.first) return FAIL;
        if(user_login[_u] == true) return FAIL;
        int user_pos = res.second.user_pos;

        User tmp_user;
        ufile.seekg(user_pos, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&tmp_user), sizeof(User));

        if(strcmp(tmp_user.password.ch,_p.ch)) return FAIL;

        user_login[_u] = true;
        return SUCCESS;
    }

    int logout(std::istream &is){
        Userid _u;
        Signal _z;
        is >> _z >> _u;

        find_t_user res = user_record.find(_u);
        if(!res.first) return FAIL;
        if(user_login[_u] == false) return FAIL;

        user_login[_u] = false;
        return SUCCESS;
    }

    void query_profile(std::istream &is, std::ostream &os){
        Userid _c, _u;
        Signal _z;
        is >> _z >> _c >> _z >> _u;

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
        if(!user_login[user_cur.user_id]) {os << "-1\n"; return;}
        if(user_cur.privilege <= user_query.privilege && _c != _u) {os << "-1\n"; return;}

        os << user_query.user_id << " " << user_query.name << " ";
        os << user_query.email << " " << user_query.privilege <<"\n";
        return;
    }

    void modify_profile(std::istream &is, std::ostream &os){
        Userid  _c, _u;
        Signal _z;
        char str[200];
        is >> _z >> _c >> _z >> _u;
        is.getline(str,200);

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

        if(!user_login[user_cur.user_id]) {os << "-1\n"; return;}
        if(user_cur.privilege <= user_query.privilege && _c != _u) {os << "-1\n"; return;}

        int str_len = strlen(str), str_p = 0, tmp_p;
        char tmp[30];
        while(str_p < str_len){//-p-n-m-g
            char modify_type = str[str_p + 1];
            str_p += 3;
            for(tmp_p = 0;str[str_p] != ' ';str_p++, tmp_p++)
                tmp[tmp_p] = str[str_p];
            tmp[tmp_p] = '\0';
            str_p++;

            if(modify_type == 'p') {user_query.password = tmp; continue;}
            if(modify_type == 'n') {user_query.name = tmp; continue;}
            if(modify_type == 'm') {user_query.email = tmp; continue;}
            if(modify_type == 'g'){
                user_query.privilege = tmp[0]-'0';
                if(strlen(tmp) > 1) user_query.privilege = user_query.privilege*10 + tmp[1]-'0';
            }
        }

        ufile.seekp(user_pos_query, std::ios::beg);
        ufile.write(reinterpret_cast<char*> (&user_query), sizeof(User));

        os << user_query.user_id << " " << user_query.name << " ";
        os << user_query.email << " " << user_query.privilege <<"\n";
        return;
    }
};

#endif //INC_20200512_TRAIN_MANAGER_H
