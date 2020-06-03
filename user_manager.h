//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_USER_MANAGER_H
#define INC_20200512_USER_MANAGER_H

#include "signal.h"

class user_system{
private:

public:
    bool check_privilege(const Userid &a, const Userid &b){// a.g >= b.g

    }

    int add_user(std::istream &is){
    //<cur_username>-c，<username>-u，<password>-p，<name>-n，<mailAddr>-m，<privilege>-g
        User U;
        Userid cur_id;
        Signal _z;
        is >> _z >> cur_id >> _z >> U.user_id >> _z >> U.name;
        is >> _z >> U.password >> _z >> U.privilege;

        if(!check_privilege(cur_id, ))
    }

    int login(std::istream &is){
        Username _u;
        Password _p;
        Signal _z;
        is >> _z >> _u >> _z >> _p;

    }

    int logout(std::istream &is){
        Username _u;
        Signal _z;
        is >> _z >> _u;

    }

    int query_profile(std::istream &is, std::ostream &os){
        Username _c, _u;
        Signal _z;
        is >> _z >> _c >> _z >> _u;

    }

    int modify_profile(std::istream &is, std::ostream &os){
        Username  _c, _u;
        Password _p;
        Name _n;
        Email _m;
        Privilege _g;
        Signal _z;
        is >> _z >> _c >> _z >> _u;
        if(!check_privilege(_c,_u)) return 0;

        //initialize _p, _n, _m, _g

        std::string str;
        getline(is, str);
        for(int i = 0;i < str.size(); ++i){

        }
    }

    int query_order(std::istream &is, std::ostream &os){//-u
        Username _u;
        Signal _z;

        is >> _z >> _u;

    }
};

#endif //INC_20200512_USER_MANAGER_H
