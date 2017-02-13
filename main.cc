#include<iostream>
#include<map>
#include "beaner.h"
using std::map;

const int BEANER_NUM = 200;

int main()
{
    map<int, Beaner> g_bk;
    //init beaner
    for(int i = 0; i < BEANER_NUM; ++i)
    {
        g_bk[i] = Beaner(); 
    }

    Beaner bean;

    for(int i = 0; i < sizeof(bean.m_dna)/sizeof(int); ++i)
    {
        std::cout << bean.m_dna[i] << " " ;
    }
}
