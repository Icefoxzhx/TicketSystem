#include <cstdio>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
	int num1 = argv[1][0] - '0';
	int num2 = argv[2][0] - '0';
	cout << num1 + num2 << endl;
	return 0;	
} 
