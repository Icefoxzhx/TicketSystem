//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_PARSER_H
#define INC_20200512_PARSER_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include "signal.h"
#include "train_manager.h"

train_system Manager;

stringstream is;
stringstream os;
void get_command_frontend(char *frontend_command_str, char *frontend_result_str)
{
	stringstream is(frontend_command_str);
    char command[50]="";
    while(is >> command){
        //user
        //-----test-----
        if(strcmp(command, "add_user") == 0){// -c -u -p -n -m -g
            os << Manager.add_user(is) << '\n';
            continue;
        }
        if(strcmp(command, "login") == 0){// -u -p
            os << Manager.login(is) << '\n';
            continue;
        }
        if(strcmp(command, "logout") == 0){//-u
            os << Manager.logout(is) << '\n';
            continue;
        }
        if(strcmp(command, "query_profile") == 0){//-u
            Manager.query_profile(is, os);
            continue;
        }
        if(strcmp(command, "modify_profile") == 0){//-c -u (-p) (-n) (-m) (-g)
            Manager.modify_profile(is, os);
            continue;
        }

        if(strcmp(command, "query_order") == 0){//-u
            Manager.query_order(is, os);
            continue;
        }


        //train
        if(strcmp(command, "add_train") == 0){//-i -n -m -s -p -x -t -o -d -y
            os << Manager.add_train(is) << '\n';
            continue;
        }
        if(strcmp(command, "release_train") == 0){//-i
            os << Manager.release_train(is) << '\n';
            continue;
        }
        if(strcmp(command, "query_train") == 0){// -i -d
            Manager.query_train(is, os);
            continue;
        }
        if(strcmp(command, "delete_train") == 0){//-i
            os << Manager.delete_train(is) << '\n';
            continue;
        }
        if(strcmp(command, "query_ticket") == 0){//-s -t -d -p
            Manager.query_ticket(is, os);
            continue;
        }
        if(strcmp(command, "query_transfer") == 0){
            Manager.query_transfer(is, os);
            continue;
        }
        if(strcmp(command, "buy_ticket") == 0){
            Manager.buy_ticket(is, os);
            continue;
        }
        if(strcmp(command, "refund_ticket") == 0){
            os << Manager.refund_ticket(is) << '\n';
            continue;
        }



        if(strcmp(command, "clean") == 0){
            Manager.init();
            Manager.user_init();
            os << "1\n";
            continue;
        }

        if(strcmp(command, "exit") == 0){
            os<<"bye\n";
            break;
        }
    }
    strcpy(frontend_result_str, os.str().c_str());
    is.clear();
    is.str("");
	os.clear();
    os.str("");
}

#endif //INC_20200512_PARSER_H
