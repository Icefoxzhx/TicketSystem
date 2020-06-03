//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_USER_MANAGER_H
#define INC_20200512_USER_MANAGER_H

#include "signal.h"
#include "Bptree.hpp"
#include <fstream>
#include <map>

class user_system{
private:

    Bptree<Userid, int> user_record;
    typedef pair<bool, int> find_t_user;
    map<Userid, bool> user_login;

    std::fstream ufile;

    bool Change;

public:

    user_system():user_record("user_record"){
        std::fstream iofile("user_file");
        if(!iofile) {Change = false; init();}
        else {Change = true; iofile.close();}
        ufile.open("user_file");
    }

    ~user_system(){
        ufile.close();
    }

    void init(){
        std::ofstream ofile;
        ofile.open("user_file");
        ofile.close();
    }

    bool check_privilege(const User &lhs, const User &rhs){// lhs.g > rhs.g
        return lhs.privilege > rhs.privilege;
    }

    int add_user(std::istream &is){
    //<cur_username>-c，<username>-u，<password>-p，<name>-n，<mailAddr>-m，<privilege>-g
        User U;
        Userid cur_id;
        Signal _z;
        char _g[10];
        is >> _z >> cur_id >> _z >> U.user_id >> _z >> U.password >> _z >> U.name;
        is >> _z >> U.email >> _z >> _g;

        U.privilege = _g[0] - '0';
        if(strlen(_g) > 1) U.privilege = U.privilege*10 + _g[1]-'0';

        find_t_user res_user = user_record.find(U.user_id);
        if(res_user.first) return FAIL;

        find_t_user res_cur = user_record.find(cur_id);
        if(!res_cur.first) return FAIL;

        int pos_cur = res_cur.second;
        User user_cur;
        ufile.seekg(pos_cur, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&user_cur), sizeof(User));

        if(Change && !check_privilege(user_cur, U)) return FAIL;

        user_login[U.user_id] = false;
        if(!Change) {U.privilege = 10; Change = true;}
        U.buy_cnt = 0;

        ufile.seekp(0, std::ios::end);
        int user_pos = ufile.tellp();
        ufile.write(reinterpret_cast<char*> (&U), sizeof(User));

        user_record.insert(U.user_id, user_pos);
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
        int user_pos = res.second;

        User tmp_user;
        ufile.seekg(user_pos, std::ios::beg);
        ufile.read(reinterpret_cast<char*> (&tmp_user), sizeof(User));
        if(tmp_user.password != _p) return FAIL;

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

        int user_pos_cur = res_cur.second, user_pos_query = res_query.second;
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

        int user_pos_cur = res_cur.second, user_pos_query = res_query.second;
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

#endif //INC_20200512_USER_MANAGER_H
