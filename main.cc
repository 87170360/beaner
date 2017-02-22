#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
#include <iostream> // library that contain basic input/output functions
#include <fstream>  // library that contains file input/output functions
using namespace std;
#include "beaner.h"
using std::map;

/*
   y
   ^
   |
   |
   |
   -------------> x
   (0,0)
   */

const int BEANER_NUM = 200;
const int MAP_SIZE = 12;
enum ACT
{
    act_up      = 0,
    act_down    = 1,
    act_left    = 2,
    act_right   = 3,
    act_eat     = 4,
    act_stay    = 5,
    act_rand    = 6,
};

enum GRID
{
    grid_none   = 1,
    grid_bean   = 2,
    grid_wall   = 3,
};

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

void createMap(int mapinfo[MAP_SIZE][MAP_SIZE])
{
    //wall
    for(int i = 0; i < MAP_SIZE; ++i)
    {
        mapinfo[0][i]   = grid_wall; 
        mapinfo[11][i]  = grid_wall; 
        mapinfo[i][0]   = grid_wall; 
        mapinfo[i][11]  = grid_wall; 
    }
    //bean
    for(int i = 1; i <= 10; ++i)
    {
        for(int j = 1; j <= 10; ++j)
        {
            mapinfo[j][i] = rand() % 2 + 1; 
        }
    }
}
void showMap(int mapinfo[MAP_SIZE][MAP_SIZE])
{
    for(int y = MAP_SIZE - 1; y >= 0; --y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
        {
            std::cout << "x" << x << "y" << y << ":" << mapinfo[y][x] << " "; 
        }
        std::cout << std::endl;
    }
}

int calScore(int mapinfo[MAP_SIZE][MAP_SIZE], int x, int y, int act)
{
    int grid = mapinfo[y][x];
    if(act == act_rand) 
    {
        act = rand() % 4; 
    }

    switch(act)
    {
        case act_up: { grid = mapinfo[y + 1][x]; } break;
        case act_down: { grid = mapinfo[y - 1][x]; } break;
        case act_left: { grid = mapinfo[y][x - 1]; } break;
        case act_right: { grid = mapinfo[y][x + 1]; } break;
    }

    if(act == act_eat && grid == grid_bean)
    {
        return 10; 
    }

    if(act == act_eat && grid == grid_none)
    {
        return -1; 
    }

    if(grid == grid_wall)
    {
        return -5; 
    }
    return 0;
}

void calPos(int mapinfo[MAP_SIZE][MAP_SIZE], int& x, int& y, int act)
{
    int grid = mapinfo[y][x];
    if(act == act_rand) 
    {
        act = rand() % 4; 
    }

    switch(act)
    {
        case act_up:    { grid = mapinfo[y + 1][x]; } break;
        case act_down:  { grid = mapinfo[y - 1][x]; } break;
        case act_left:  { grid = mapinfo[y][x - 1]; } break;
        case act_right: { grid = mapinfo[y][x + 1]; } break;
    }

    if(grid != grid_wall)
    {
        switch(act)
        {
            case act_up:    { ++y; } break;
            case act_down:  { --y; } break;
            case act_left:  { --x; } break;
            case act_right: { ++x; } break;
        }
    }
}

void testCalPos(int mapinfo[MAP_SIZE][MAP_SIZE])
{
    int x = 1, y = 1;
    std::cout << "x:" << x << " y:" << y << std::endl;

    calPos(mapinfo, x, y, act_up); 
    std::cout << "up" << "x:" << x << " y:" << y << std::endl;
    x = 1;
    y = 1;

    calPos(mapinfo, x, y, act_down); 
    std::cout << "down" << "x:" << x << " y:" << y << std::endl;
    x = 1;
    y = 1;

    calPos(mapinfo, x, y, act_left); 
    std::cout << "left" << "x:" << x << " y:" << y << std::endl;
    x = 1;
    y = 1;

    calPos(mapinfo, x, y, act_right); 
    std::cout << "right" << "x:" << x << " y:" << y << std::endl;
    x = 1;
    y = 1;
}

void initStatusIndex(std::map<int, int>& sindex)
{
    int indexCount = 0;
    int indexKey = 0;
    for(int i = grid_none; i <= grid_wall; ++i)
    {
        for(int j = grid_none; j <= grid_wall; ++j)
        {
            for(int k = grid_none; k <= grid_wall; ++k)
            {
                for(int l = grid_none; l <= grid_wall; ++l)
                {
                    for(int m = grid_none; m <= grid_wall; ++m)
                    {
                        indexCount++;  
                        indexKey = i * 10000 + j * 1000 + k * 100 + l * 10 + m; 
                        sindex[indexKey] = indexCount;
                    }
                }
            }
        }
    }
    /*
       for(std::map<int, int>::iterator it = sindex.begin(); it != sindex.end(); ++it)
       {
       std::cout << "key:" << it->first << " , value:" << it->second << " "; 
       }
       std::cout << std::endl;
       std::cout << "size:" << sindex.size() << std::endl;
       */
}

void testSwapDna(void)
{
    int x[] = {1,2,3,4,5,6};
    int y[] = {11,22,33,44,55,66};
    int b[] = {0,0,0,0,0,0};
    swapDna(x, y, b, 6);
    for(int i = 0; i < sizeof(b) / sizeof(int); ++i)
    {
        std::cout << b[i] << " " ;
    }
    std::cout << std::endl;
}

void testWeightSelect(void)
{
    //weight list
    int tmp_weight[BEANER_NUM];
    //total weight
    int tmp_totalWeight = 0;

    for(int i = 0; i < BEANER_NUM; ++i)
    {
        tmp_weight[i] = BEANER_NUM - i;
        tmp_totalWeight += tmp_weight[i];
    }

    std::map<int, int> tmp_testweight;
    for(int i = 0; i < BEANER_NUM; ++i)
    {
        tmp_testweight[tmp_weight[i]] = 0;
    }

    for(int i = 0; i < 100000000; ++i)
    {
        int val = weightSelect(tmp_weight, BEANER_NUM, tmp_totalWeight); 
        tmp_testweight[tmp_weight[val]] += 1;
    }

    for(int i = 0; i < BEANER_NUM; ++i)
    {
        std::cout << tmp_weight[i] << ":" << tmp_testweight[tmp_weight[i]] / 1000000.0f << "% ";
    }
}

void testPos2StatusIndex(int mapinfo[MAP_SIZE][MAP_SIZE], std::map<int, int>& sindex)
{
    for(int j = MAP_SIZE - 2; j > 0; --j)
    {
        for(int i = 1; i < MAP_SIZE - 1; ++i)
        {
            int status = pos2status(j, i, mapinfo);
            //std::cout << status << " "; 
            std::cout << sindex[status] << "  "; 
        }
        std::cout << std::endl;
    }
}

void testCalScore(int mapinfo[MAP_SIZE][MAP_SIZE])
{
    int mup = 0;
    mup = calScore(mapinfo, 1, 1, act_up);
    std::cout << "act_up:" << mup << std::endl;
    mup = calScore(mapinfo, 1, 1, act_down);
    std::cout << "act_down:" << mup << std::endl;
    mup = calScore(mapinfo, 1, 1, act_left);
    std::cout << "act_left:" << mup << std::endl;
    mup = calScore(mapinfo, 1, 1, act_right);
    std::cout << "act_right:" << mup << std::endl;
    mup = calScore(mapinfo, 1, 1, act_stay);
    std::cout << "act_stay:" << mup << std::endl;
    mup = calScore(mapinfo, 1, 1, act_eat);
    std::cout << "act_eat:" << mup << std::endl;
}

void writeArray(void)
{
    //char array[] = {'H','e','l','l','o',' ','W','o','r','l','d','!','\0'}; //array to write into file 
    int array[] = {1, 2, 3, 4, 5};
    ofstream fout("test.txt"); 
    /*checking whether file could be opened or not. If file does not exist or don't have write permissions, file
     *   stream could not be opened.*/
    if(fout.is_open())
    {
        //file opened successfully so we are here
        cout << "File Opened successfully!!!. Writing data from array to file" << endl;

        for(int i = 0; i < sizeof(array) / sizeof(int); i++)
        {
            fout << array[i]; //writing ith character of array in the file
            fout << " ";
        }
        cout << "Array data successfully saved into the file test.txt" << endl;
    }
    else //file could not be opened
    {
        cout << "File could not be opened." << endl;
    }
    fout.close();
}

void readArray()
{
    ifstream file("test.txt");
    if(file.is_open())
    {
        string myArray[5];

        for(int i = 0; i < 5; ++i)
        {
            file >> myArray[i];
            cout << myArray[i] << endl;
        }
    }
    else //file could not be opened
    {
        cout << "File could not be opened." << endl;
    }
    file.close();
}

int main()
{
    srand(time(NULL));

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

    std::map<int, int> m_sindex;
    initStatusIndex(m_sindex);

    int m_mapinfo[MAP_SIZE][MAP_SIZE];
    createMap(m_mapinfo);
    //showMap(m_mapinfo);

    //writeArray();
    //readArray();
}
