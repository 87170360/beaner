#include<iostream>
#include<map>
#include "beaner.h"
using std::map;

const int BEANER_NUM = 200;

void swapDna(int p1[], int p2[], int ret[], int size)
{
    srand(time(NULL));
    std::copy(p2, p2 + size, ret);
    int point = rand() % size + 1;
    //std::cout << "point:" << point << std::endl;
    std::copy(p1, p1 + point, ret);

    point = rand() % size;
    //std::cout << "point:" << point << std::endl;
    ret[point] = rand() % BEHAVIOR;
}

void selectChampion(Beaner all[], int weight[], int size, int ret[])
{

}

void sortBeaner(Beaner all[])
{

}

int main()
{
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
    Beaner m_all[BEANER_NUM];
    //weight
    int m_weight[BEANER_NUM];
    for(int i = 0; i < BEANER_NUM; ++i)
    {
        Beaner bean = Beaner();
        bean.m_score = i;
        m_all[i] = bean; 
        m_weight[i] = BEANER_NUM - i;
    }
}
