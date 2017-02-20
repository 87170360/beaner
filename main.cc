#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
#include "beaner.h"
using std::map;

const int BEANER_NUM = 200;
const int MAP_SIZE = 12;

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

int pos2status(int x, int y, int mapinfo[MAP_SIZE][MAP_SIZE])
{
   int up       = mapinfo[y - 1][x]; 
   int down     = mapinfo[y + 1][x]; 
   int left     = mapinfo[y][x - 1]; 
   int right    = mapinfo[y][x + 1]; 
   int cur      = mapinfo[y][x]; 
   return up * 10000 + down * 1000 + left * 100 + right * 10 + cur;
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

    //init status index, 0 none, 1 bean, 2 wall
    std::map<int, int> m_sindex;
    int indexCount = 0;
    int indexKey = 0;
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            for(int k = 0; k < 3; ++k)
            {
                for(int l = 0; l < 3; ++l)
                {
                    for(int m = 0; m < 3; ++m)
                    {
                       indexCount++;  
                       indexKey = i * 10000 + j * 1000 + k * 100 + l * 10 + m; 
                       m_sindex[indexKey] = indexCount;
                    }
                }
            }
        }
    }

    /*
    for(std::map<int, int>::iterator it = m_sindex.begin(); it != m_sindex.end(); ++it)
    {
        std::cout << "key:" << it->first << " , value:" << it->second << " "; 
    }
    std::cout << std::endl;
    std::cout << "size:" << m_sindex.size();
    */

    //init map data 12 * 12,  y * x
    int m_mapinfo[MAP_SIZE][MAP_SIZE];
    //wall
    for(int i = 0; i < MAP_SIZE; ++i)
    {
        m_mapinfo[0][i] = 2; 
        m_mapinfo[11][i] = 2; 
        m_mapinfo[i][0] = 2; 
        m_mapinfo[i][11] = 2; 
    }
    //bean
    for(int i = 1; i <= 10; ++i)
    {
        for(int j = 1; j <= 10; ++j)
        {
            m_mapinfo[j][i] = rand() % 2; 
        }
    }
     
    //show map
    /*
    for(int i = 0; i < MAP_SIZE; ++i)
    {
        for(int j = 0; j < MAP_SIZE; ++j)
        {
            std::cout << m_mapinfo[j][i] << " "; 
        }
        std::cout << std::endl;
    }
    */

    //test pos info -> status
    /*
    for(int i = 1; i < MAP_SIZE - 1; ++i)
    {
        for(int j = 1; j < MAP_SIZE - 1; ++j)
        {
            int status = pos2status(j, i, m_mapinfo);
            std::cout << m_sindex[status] << " "; 
        }
        std::cout << std::endl;
    }
    */
}
