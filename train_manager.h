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
    typedef vector<pair<Station_key,Station_value>> find_t_station;

    struct query_ticket_t{
        Train_id train_id;
        int start_time, end_time, price, seat_left, val;

        bool operator < (const query_ticket_t &rhs){
            return val < rhs.val;
        }
    };


    std::fstream rfile;
    std::fstream tlfile;




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
        T sort_key = array[first];
        while (sort_l < sort_r) {
            while (sort_l < sort_r && arr[sort_r] >= sort_key) sort_r--;
            arr[sort_l] = arr[sort_r];
            while (sort_l < sort_r && arr[sort_l] <= sort_key) sort_l++;
            arr[sort_r] = arr[sort_l];
        }
        arr[sort_l] = key;
        Quick_Sort(arr, Start_Index, sort_l - 1);
        Quick_Sort(arr, sort_l + 1, End_Index);
    }


public:

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
            char tmp[1000];
            is.getline(tmp,1000);
            return FAIL;
        }

        is >> _z >> _s >> _z >> _p >> _z >> _x >> _z >> _t;
        is >> _z >> _o >> _z >> _d >> _z >> T.type;

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
            Station_key tmp_key(tmp_station[i].station_id, tr.train_id);
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
            Station_key tmp_key(tmp_station[i].station_id, tr.train_id);
            station_record.remove(tmp_key);
        }
        train_record.remove(tr.train_id);
        delete [] tmp_station;
        return SUCCESS;
    }

    int query_ticket(std::istream &is, std::ostream &os){//-s -t -d (-p time)
        Station_name _s, _t;
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
        find_t_station tmp_start_station = station_record.find(start_key_1, start_key_2);

        Station_key end_key_1(_t, MIN_TRAIN_ID), end_key_2(_t, MAX_TRAIN_ID);
        find_t_station tmp_end_station = station_record.find(end_key_1, end_key_2);

        vector<query_ticket_t> vec_train;
        auto iter_start = tmp_start_station.begin();
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
        delete[] ans;
        return SUCCESS;
    }

    int query_transfer(std::istream &is, std::ostream &os){//-s -t -d (-p)
        Station_name _s, _t;
        Date _d;
        bool p_c = false; // false time true cost
        Signal _z;

        is >> _z >> _s >> _z >> _t >> _z >> _d;
        is.getline(str,50);
        if(strlen(str) && str[0] == 'c') p_c = true;

        int date = Date_to_int(_d);
        if(date < 0 || date >= MAX_DATE || _s == _t) return FAIL;

        Station_key start_key_1(_s, MIN_TRAIN_ID), start_key_2(_s, MAX_TRAIN_ID);
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
        delete[] ans;
        return SUCCESS;
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

    int query_order(std::istream &is, std::ostream &os){//-u
        Username _u;
        Signal _z;

        is >> _z >> _u;

    }
};

#endif //INC_20200512_TRAIN_MANAGER_H
