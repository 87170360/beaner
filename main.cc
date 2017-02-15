#include<iostream>
#include<map>
#include "beaner.h"
using std::map;

const int BEANER_NUM = 200;

void swapDna(int p1[], int p2[], int ret[])
{
    srand (time(NULL));
    int size = sizeof(p1) / sizeof(int);
    std::cout << "size:" << size << std::endl;

    std::copy(p2, p2 + size, ret);
    for(int i = 0; i < sizeof(ret) / sizeof(int); ++i)
    {
        std::cout << ret[i] << " " ;
    }
    std::cout << std::endl;

    int point = rand() % size + 1;
    std::cout << "point:" << point << std::endl;
    std::copy(p1, p1 + point, ret);
}

int main()
{
    int x[] = {1,2,3,4,5,6};
    int y[] = {11,22,33,44,55,66};
    int b[] = {0,0,0,0,0,0};
    swapDna(x, y, b);
    //int a[] = {5,6,7};

    //std::copy(a, a + 3, b);

    for(int i = 0; i < sizeof(b) / sizeof(int); ++i)
    {
        std::cout << b[i] << " " ;
    }
    std::cout << std::endl;
}
