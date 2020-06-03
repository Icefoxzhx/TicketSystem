//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_PARSER_H
#define INC_20200512_PARSER_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include "signal.h"
#include "train_manager.h"
#include "station_manager.h"
#include "user_manager.h"

user_system users;
train_system trains;
station_system stations;


void get_command(std::istream &is, std::ostream &os){
    std::string command;
    while(is >> command){
        //user
        if(command == "add_user"){// -c -u -p -n -m -g
            os << users.add_user(is) << '\n';
            continue;
        }
        if(command == "login"){// -u -p
            os << users.login(is) << '\n';
            continue;
        }
        if(command == "logout"){//-u
            os << users.logout(is) << '\n';
            continue;
        }
        if(command == "query_profile"){//-u
            users.query_profile(is, os);
            continue;
        }
        if(command == "modify_profile"){//-c -u (-p) (-n) (-m) (-g)
            users.modify_profile(is, os);
            continue;
        }

        if(command == "query_order"){//-u
            users.query_order(is, os);
            continue;
        }


        //train
        if(command == "add_train"){//-i -n -m -s -p -x -t -o -d -y
            os << trains.add_train(is) << '\n';
            continue;
        }
        if(command == "release_train"){//-i
            os << trains.release_train(is) << '\n';
            continue;
        }
        if(command == "query_train"){// -i -d
            trains.query_train(is, os);
            continue;
        }
        if(command == "delete_train"){//-i
            os << trains.delete_train(is) << '\n';
            continue;
        }
        if(command == "query_ticket"){//-s -t -d -p
            trains.query_ticket(is, os);
            continue;
        }
        if(command == "query_transfer"){
            trains.query_transfer(is, os);
            continue;
        }
        if(command == "buy_ticket"){
            os << trains.buy_ticket(is, os) << '\n';
            continue;
        }
        if(command == "refund_ticket"){
            os << trains.refund_ticket(is) << '\n';
            continue;
        }



        if(command == "clean"){

            continue;
        }

        if(command == "exit"){

            //?????????????
            break;
        }
    }
}

#endif //INC_20200512_PARSER_H
