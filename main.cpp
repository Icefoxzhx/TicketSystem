#include <iostream>
#include <cstdio>
#include <cstring>
#include "parser.h"

int main() {
    /*
<<<<<<< HEAD
    freopen("2.in","r",stdin);
    freopen("2.out","w",stdout);
=======
  //  freopen("1.in","r",stdin);
 //   freopen("1.out","w",stdout);
>>>>>>> fb39cb9bd52340a9005df92643a0b118b3dfe404*/
    //system("chcp 65001");
//    char tmp[50]="";
//    std::cin >> tmp ;
//scanf("%s",tmp);
//    std::cout << tmp <<"\n";

/*
    std::ofstream ofile;
    ofile.open("test_file");
    ofile.close();

    std::fstream tfile;
    tfile.open("test_file");
    User t;t.password = "adsfasdfasdfadsfasdfasdfsaerdfs,bhgsdfgfskxcvkhlafkaerngf";
    t.email="aaaaaaaaaaaaa";
    t.user_id="user_id";
    t.privilege = 10;
    t.name = "asdfjk;asdjfasdaff";

    tfile.seekp(0, std::ios::end);
    int t_pos = tfile.tellp();
    tfile.write(reinterpret_cast<char*> (&t), sizeof(User));

    User t2;
    tfile.seekg(t_pos, std::ios::beg);
    tfile.read(reinterpret_cast<char*> (&t2), sizeof(User));
*/
    get_command(std::cin,std::cout);
    return 0;
}
