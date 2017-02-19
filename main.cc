#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
#include "beaner.h"
using std::map;

const int BEANER_NUM = 200;

void swapDna(int p1[], int p2[], int ret[], int size)
{
    std::copy(p2, p2 + size, ret);
    int point = rand() % size + 1;
    //std::cout << "point:" << point << std::endl;
    std::copy(p1, p1 + point, ret);

    point = rand() % size;
    //std::cout << "point:" << point << std::endl;
    ret[point] = rand() % BEHAVIOR;
}

int weightSelect(int weight[], int size, int total)
{
    int pick = rand() % total;
    //std::cout << "pick:" << pick << "total:" << total;
    int tmp = 0;
    for(int i = 0; i < size; ++i) 
    {
        tmp += weight[i];    
        if(tmp >= pick)
        {
            return i; 
        }
    }
    //std::cout << "not found weight!" << std::endl;
    return 0;
}

bool compareBeaner(const Beaner& a, const Beaner& b)
{
    return a.m_score > b.m_score;
}

void sortBeaner(std::vector<Beaner>& all)
{
    std::sort(all.begin(), all.end(), compareBeaner);
}


int main()
{
    srand(time(NULL));
/* test swapDna
    int x[] = {1,2,3,4,5,6};
    int y[] = {11,22,33,44,55,66};
    int b[] = {0,0,0,0,0,0};
    swapDna(x, y, b, 6);
    for(int i = 0; i < sizeof(b) / sizeof(int); ++i)
    {
        std::cout << b[i] << " " ;
    }
    std::cout << std::endl;
*/


    //all beaner
    std::vector<Beaner> m_all;
    //weight list
    int m_weight[BEANER_NUM];
    //total weight
    int m_totalWeight = 0;
    for(int i = 0; i < BEANER_NUM; ++i)
    {
        Beaner bean = Beaner();
        bean.m_score = i;
        m_all.push_back(bean);
        m_weight[i] = BEANER_NUM - i;
        m_totalWeight += m_weight[i];
    }

    sortBeaner(m_all);

    //test weightSelect
    /*
    std::map<int, int> m_testweight;
    for(int i = 0; i < BEANER_NUM; ++i)
    {
        m_testweight[m_weight[i]] = 0;
    }

    for(int i = 0; i < 100000000; ++i)
    {
        int val = weightSelect(m_weight, BEANER_NUM, m_totalWeight); 
        m_testweight[m_weight[val]] += 1;
    }

    for(int i = 0; i < BEANER_NUM; ++i)
    {
        std::cout << m_weight[i] << ":" << m_testweight[m_weight[i]] / 1000000.0f << "% ";
    }
    */
}
