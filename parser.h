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

train_system Manager;


void get_command(std::istream &is, std::ostream &os){
    std::string command;
    while(is >> command){
        //user
        if(command == "add_user"){// -c -u -p -n -m -g
            os << Manager.add_user(is) << '\n';
            continue;
        }
        if(command == "login"){// -u -p
            os << Manager.login(is) << '\n';
            continue;
        }
        if(command == "logout"){//-u
            os << Manager.logout(is) << '\n';
            continue;
        }
        if(command == "query_profile"){//-u
            Manager.query_profile(is, os);
            continue;
        }
        if(command == "modify_profile"){//-c -u (-p) (-n) (-m) (-g)
            Manager.modify_profile(is, os);
            continue;
        }

        if(command == "query_order"){//-u
            Manager.query_order(is, os);
            continue;
        }


        //train
        if(command == "add_train"){//-i -n -m -s -p -x -t -o -d -y
            os << Manager.add_train(is) << '\n';
            continue;
        }
        if(command == "release_train"){//-i
            os << Manager.release_train(is) << '\n';
            continue;
        }
        if(command == "query_train"){// -i -d
            Manager.query_train(is, os);
            continue;
        }
        if(command == "delete_train"){//-i
            os << Manager.delete_train(is) << '\n';
            continue;
        }
        if(command == "query_ticket"){//-s -t -d -p
            Manager.query_ticket(is, os);
            continue;
        }
        if(command == "query_transfer"){
            Manager.query_transfer(is, os);
            continue;
        }
        if(command == "buy_ticket"){
            Manager.buy_ticket(is, os);
            continue;
        }
        if(command == "refund_ticket"){
            os << Manager.refund_ticket(is) << '\n';
            continue;
        }



        if(command == "clean"){
            Manager.init();
            Manager.user_init();
            os << "1\n";
            continue;
        }

        if(command == "exit"){
            os<<"bye\n";
            break;
        }
    }
}

#endif //INC_20200512_PARSER_H
