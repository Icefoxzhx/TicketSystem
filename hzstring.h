//
// Created by H.z on 2020/5/13.
//

#ifndef INC_20200512_HZSTRING_H
#define INC_20200512_HZSTRING_H

#include <iostream>
#include <cstring>

template <int N>
class hzstring{
private:

public:
    char ch[N+1];
    hzstring() {ch[0] = '\0';}

    hzstring(const char *rhs){
        if(rhs != nullptr){
            int i = 0;
            while(rhs[i] != '\0' && i < N){
                ch[i] = rhs[i];
                ++i;
            }
            ch[i] = '\0';
        }
        else{
            ch[0] = '\0';
        }
    }

    hzstring(const hzstring &rhs){
        int i = 0;
        while(rhs.ch[i] != '\0' && i < N){
            ch[i] = rhs.ch[i];
            ++i;
        }
        ch[i] = '\0';
    }

    hzstring operator=(const hzstring &rhs){
        if((&rhs) == this) return *this;
        int i = 0;
        while(rhs.ch[i] != '\0' && i < N){
            ch[i] = rhs.ch[i];
            ++i;
        }
        ch[i] = '\0';
        return *this;
    }

    hzstring operator=(const char* rhs){
        if(rhs != nullptr) {
            int i = 0;
            while (rhs[i] != '\0' && i < N) {
                ch[i] = rhs[i];
                ++i;
            }
            ch[i] = '\0';
        }
        return *this;
    }

    bool operator <(const hzstring &rhs) const{
        if(rhs == nullptr) return false;
        int l1, l2, l;
        l1 = strlen(ch);
        l2 = strlen(rhs.ch);
        l = (l1 < l2) ? l1 : l2;
        for(int i = 0;i < l;++i)
            if(ch[i] != rhs.ch[i])
                return ch[i] < rhs.ch[i];
        return (l1 < l2);
    }

    bool operator ==(const hzstring &rhs) const{
        if(rhs == nullptr) return false;
        if(ch[0] == '\0') return false;
        int l1, l2;
        l1 = strlen(ch);
        l2 = strlen(rhs.ch);
        if(l1 != l2) return false;
        for(int i = 0;i < l1;++i)
            if(ch[i] != rhs.ch[i])
                return false;
        return true;
    }

    bool operator >(const hzstring &rhs) const{
        return rhs < (*this);
    }

    bool operator !=(const hzstring &rhs) const{
        return !((*this) == rhs);
    }

    bool operator <=(const hzstring &rhs) const{
        return ((*this) == rhs) || ((*this) < rhs);
    }

    bool operator >=(const hzstring &rhs) const{
        return ((*this) == rhs) || ((*this) > rhs);
    }

    bool operator ==(const char* rhs) const{
        if(rhs == nullptr) return false;
        int l1, l2;
        l1 = strlen(ch);
        l2 = strlen(rhs);
        if(l1 != l2) return false;
        for(int i = 0;i < l1;++i)
            if(ch[i] != rhs[i])
                return false;
        return true;
    }

    bool operator !=(const char* rhs) const{
        return !((*this) == rhs);
    }

    char& operator[](int index){// or size_t????????????????????
        return ch[index];
    }

    const char& operator[](int index) const{// or size_t????????????????????
        return ch[index];
    }

    friend std::istream & operator >> (std::istream &is, hzstring &rhs){
        is >> rhs.ch;
        return is;
    }

    friend std::ostream & operator << (std::ostream &os, const hzstring &rhs){
        os << rhs.ch;
        return os;
    }
};
// whether operator + & inttostring are needed?

#endif //INC_20200512_HZSTRING_H
