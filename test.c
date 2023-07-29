#include<stdio.h>
#include<stdlib.h>
int main(){
    remove("userlist.bin");
    rename("tempuserlist.bin", "userlist.bin");
}