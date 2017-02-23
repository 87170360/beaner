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
const int GENERATION = 1;
const int RACE = 1;
const int DAY = 100;


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
            mapinfo[j][i] = grid_none; 
        }
    }

    int beannum = 50;
    int x = 0, y = 0;
    while(beannum > 0)
    {
        x = rand() % 10 + 1;
        y = rand() % 10 + 1;
        if(mapinfo[y][x] == grid_none)
        {
            mapinfo[y][x] = grid_bean;
            --beannum;
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

void calMap(int mapinfo[MAP_SIZE][MAP_SIZE], int& x, int& y, int act)
{
    int grid = mapinfo[y][x];
    if(act == act_eat && grid == grid_bean)
    {
        mapinfo[y][x] = grid_none; 
    }
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

void writeArray(int array[], int size, const char * filename)
{
    ofstream fout(filename); 
    /*checking whether file could be opened or not. If file does not exist or don't have write permissions, file
     *   stream could not be opened.*/
    if(fout.is_open())
    {
        //file opened successfully so we are here
        cout << "File Opened successfully!!!. Writing data from array to file" << endl;

        for(int i = 0; i < size; i++)
        {
            fout << array[i]; //writing ith character of array in the file
            fout << " ";
        }
        cout << "Array data successfully saved into the file " << filename << endl;
    }
    else //file could not be opened
    {
        cout << "File " << filename << " could not be opened." << endl;
    }
    fout.close();
}

void testWriteArray(void)
{
    int array[] = {111,222,333,444,5555};
    writeArray(array, sizeof(array) / sizeof(int), "1.txt");
}

void readArray(int array[], int size, const char * filename)
{
    ifstream file(filename);
    if(file.is_open())
    {
        for(int i = 0; i < size; ++i)
        {
            file >> array[i];
            cout << array[i] << endl;
        }
    }
    else //file could not be opened
    {
        cout << "File" << filename << " could not be opened." << endl;
    }
    file.close();
}

void testReadArray(void)
{
    int array[5] = {};
    readArray(array, 5, "1.txt");
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
        m_all.push_back(bean);
        m_weight[i] = BEANER_NUM - i;
        m_totalWeight += m_weight[i];
    }

    std::map<int, int> m_sindex;
    initStatusIndex(m_sindex);

    int m_mapinfo[MAP_SIZE][MAP_SIZE];

    int status = 0;
    int act_index = 0;
    int act = 0;
    for(int i = 0; i < GENERATION; ++i)    
    {
        createMap(m_mapinfo);
        showMap(m_mapinfo);
        for(int j = 0; j < BEANER_NUM; ++j)
        {
            Beaner& bean = m_all[j];
            for(int k = 0; k < RACE; ++k)
            {
                for(int l = 0; l < DAY; ++l)
                {
                    status = pos2status(bean.m_y, bean.m_x, m_mapinfo);
                    if(m_sindex.find(status) == m_sindex.end())
                    {
                        cout << "status:" << status << "not found!" << endl;
                        break;
                    }
                    //cout << "status:" << status << endl;
                    act_index = m_sindex[status]; 
                    if(act_index < 0 || act_index >= 243)
                    {
                        cout << "act_index:" << act_index << endl;
                        break;
                    }
                    //cout << "act_index:" << act_index << endl;

                    act = bean.m_dna[act_index];
                    //cout << "day: " << l << " x:" << bean.m_x << " y:" << bean.m_y << " act:" << act << endl;
                    bean.m_score += calScore(m_mapinfo, bean.m_x, bean.m_y, act);
                    calMap(m_mapinfo, bean.m_x, bean.m_y, act);
                    calPos(m_mapinfo, bean.m_x, bean.m_y, act);
                }
            }
            cout << "bean:" << j << " total score:" << bean.m_score << endl;
        }
    }
}
