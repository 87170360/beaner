#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
#include <string>
#include <iostream> // library that contain basic input/output functions
#include <fstream>  // library that contains file input/output functions
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <dirent.h>
#include <sys/stat.h>

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
const int GENERATION = 148200 * 1000;
const int RACE = 1;
const int DAY = 200;
const int DESTORY = 10000;

char g_buff[256] = {};
float g_best = 0.0;
const char* g_dnafile = "best/219";

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

enum TRACK
{
    track_1     = 100,
    track_2     = 101,
    track_3     = 102,
};

void GetFilesInDirectory(std::vector<string> &out, const string &directory)
{
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
            const string file_name = ent->d_name;
            const string full_file_name = directory + "/" + file_name;
    
            if (file_name[0] == '.')
                continue;
    
            if (stat(full_file_name.c_str(), &st) == -1)
                continue;
    
            const bool is_directory = (st.st_mode & S_IFDIR) != 0;
    
            if (is_directory)
                continue;
    
            out.push_back(full_file_name);
        }
    closedir(dir);
} // GetFilesInDirectory

void swapDna(int p1[], int p2[], int ret1[], int ret2[], int size)
{
    int point = rand() % size + 1;
    //int point = size / 2;
    //std::cout << "point:" << point << std::endl;

    std::copy(p2, p2 + size, ret1);
    std::copy(p1, p1 + point, ret1);

    std::copy(p1, p1 + size, ret2);
    std::copy(p2, p2 + point, ret2);

    for(int i = 0; i < 10; ++i) 
    {
        point = rand() % size;
        ret1[point] = rand() % BEHAVIOR;
        point = rand() % size;
        ret2[point] = rand() % BEHAVIOR;
    }
}

int weightSelect(const std::vector<int>& weight, int total)
{
    int pick = rand() % total;
    //std::cout << "pick:" << pick << "total:" << total;
    int tmp = 0;
    for(int i = 0; i < weight.size(); ++i) 
    {
        tmp += weight[i];    
        if(tmp >= pick)
        {
            return i; 
        }
    }
    //std::cout << "not found weight!" << std::endl;
    return -1;
}

void selectParent(const std::vector<Beaner>& all, int& father, int& mother)
{
    std::vector<int> weight;
    int total = 0;
    int score = 0;

    //获取修正量
    int fixval = 0;
    for(std::vector<Beaner>::const_iterator it = all.begin(); it != all.end(); ++it)
    {
        if((*it).m_score < fixval) 
        {
            fixval = (*it).m_score; 
        }
    }
    fixval = -fixval;

    for(std::vector<Beaner>::const_iterator it = all.begin(); it != all.end(); ++it)
    {
        score = (*it).m_score;
        score += fixval;
        weight.push_back(score); 
        total += score;
    }

    father = weightSelect(weight, total);
    mother = weightSelect(weight, total);
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
            //std::cout << mapinfo[y][x] << " "; 
        }
        std::cout << std::endl;
    }
}

void copyMap(int src[MAP_SIZE][MAP_SIZE], int des[MAP_SIZE][MAP_SIZE])
{
    for(int i = 0; i < MAP_SIZE; ++i)
    {
        for(int j = 0; j < MAP_SIZE; ++j)
        {
            des[i][j] = src[i][j];
        }
    }
}

void countBean(int mapinfo[MAP_SIZE][MAP_SIZE])
{
    int count = 0;
    for(int i = 0; i < MAP_SIZE; ++i)
    {
        for(int j = 0; j < MAP_SIZE; ++j)
        {
            if(mapinfo[j][i] == grid_bean) 
            {
                ++count; 
            }
        }
    }
    cout << "bean num:" << count << endl;
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
    int b1[] = {0,0,0,0,0,0};
    int b2[] = {0,0,0,0,0,0};

    swapDna(x, y, b1, b2, 6);
    for(int i = 0; i < sizeof(b1) / sizeof(int); ++i)
    {
        std::cout << b1[i] << " " ;
    }
    std::cout << std::endl;
    for(int i = 0; i < sizeof(b2) / sizeof(int); ++i)
    {
        std::cout << b2[i] << " " ;
    }
    std::cout << std::endl;
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

void writeString(char str[], const char* filename)
{
    ofstream fout(filename, std::ios::app); 
    if(fout.is_open())
    {
        //file opened successfully so we are here
        //        cout << "File Opened successfully!!!. Writing data from array to file" << endl;

        fout << str; //writing ith character of array in the file
        fout << "\n";
        //        cout << "Array data successfully saved into the file " << filename << endl;
    }
    else //file could not be opened
    {
        cout << "File " << filename << " could not be opened." << endl;
    }
    fout.close();
}

void writeArray(int array[], int size, const char * filename)
{
    ofstream fout(filename); 
    /*checking whether file could be opened or not. If file does not exist or don't have write permissions, file
     *   stream could not be opened.*/
    if(fout.is_open())
    {
        //file opened successfully so we are here
        //        cout << "File Opened successfully!!!. Writing data from array to file" << endl;

        for(int i = 0; i < size; i++)
        {
            fout << array[i]; //writing ith character of array in the file
            fout << " ";
        }
        //       cout << "Array data successfully saved into the file " << filename << endl;
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
            //cout << array[i] << endl;
        }
    }
    else //file could not be opened
    {
        cout << "File:" << filename << " could not be opened." << endl;
    }
    file.close();
}

void testReadArray(void)
{
    int array[5] = {};
    readArray(array, 5, "1.txt");
}

void dayAction(int mapinfo[MAP_SIZE][MAP_SIZE], const std::map<int, int>& sindex, Beaner& beaner)
{
    int status = pos2status(beaner.m_y, beaner.m_x, mapinfo);

    std::map<int, int>::const_iterator iter = sindex.find(status);
    if(iter == sindex.end())
    {
        cout << "status:" << status << "not found!" << endl;
        return;
    }

    //cout << "status:" << status << endl;
    int act_index = iter->second; 
    if(act_index < 0 || act_index >= DNASIZE)
    {
        cout << "outof range, act_index:" << act_index << endl;
        return;
    }
    //cout << "act_index:" << act_index << endl;

    int act = beaner.m_dna[act_index];
    //   cout << "dayAction " << "x:" << beaner.m_x << " y:" << beaner.m_y << " act:" << act << endl;
    int score = calScore(mapinfo, beaner.m_x, beaner.m_y, act);

    /*
    if(score <= 0)
    {
        int nact = 0;
        do
        {
            nact = rand() % BEHAVIOR; 
        }
        while(nact == act);

        beaner.m_dna[act_index] = nact; 
    }
    */

    beaner.m_score += score;
    calMap(mapinfo, beaner.m_x, beaner.m_y, act);
    calPos(mapinfo, beaner.m_x, beaner.m_y, act);
    //cout << "score:" << beaner.m_score << endl;
}

void showDNA(Beaner& beaner)
{
    for(int i = 0; i < DNASIZE; ++i)
    {
        cout << beaner.m_dna[i] << " "; 
    }
    cout << endl;
}

void copyDNA(int src[DNASIZE], int des[DNASIZE])
{
    for(int i = 0; i < DNASIZE; ++i)
    {
        des[i] = src[i];
    }
}

void checkSameDNA(Beaner& b1, Beaner& b2)
{
    bool ret = true;
    for(int i = 0; i < DNASIZE; ++i)
    {
        if(b1.m_dna[i] != b2.m_dna[i])
        {
            ret = false;
        }
    }

    if(ret)
    {
        cout << "DNA same" << endl;
    }
}

void breed(std::vector<Beaner>& all)
{
    std::vector<Beaner> tmp;
    for(int j = 0; j < BEANER_NUM / 2; ++j)
    {
        int top1 = 0, top2 = 0;
        selectParent(all, top1, top2);
        Beaner child1 = Beaner();
        Beaner child2 = Beaner();
        swapDna(all[top1].m_dna, all[top2].m_dna, child1.m_dna, child2.m_dna, DNASIZE);
        tmp.push_back(child1);
        tmp.push_back(child2);
    }

    for(int i = 0; i < 1; ++i)
    {
        tmp.pop_back();
        all[i].m_score = 0;
        all[i].m_x = 5;
        all[i].m_y = 5;
        tmp.push_back(all[i]);
    }

    all.clear();
    all = tmp;
}

void initWithSeeds(std::vector<Beaner>& all)
{
    int bestDNA[DNASIZE] = {};
    std::vector<string> filename;

    GetFilesInDirectory(filename, "./best");
    for(size_t i = 0; i < filename.size() && i < all.size(); ++i)
    {
        readArray(bestDNA, DNASIZE, filename[i].c_str());
        copyDNA(bestDNA, all[i].m_dna);
    }
}

void comet(std::vector<Beaner>& all)
{
    static int gene = 0;
    if(gene > DESTORY)
    {
        gene = 0; 
        sprintf(g_buff, "comet!");
        writeString(g_buff, "data/generation.txt");
    }
    gene++;
    for(int i = int(BEANER_NUM / 2); i < BEANER_NUM; ++i)
    {
        all[i].resetDNA(); 
    }
}

void evolution(void)
{
    //all beaner
    std::vector<Beaner> m_all;
    for(int i = 0; i < BEANER_NUM; ++i)
    {
        m_all.push_back(Beaner());
    }

    //initWithSeeds(m_all);

    std::map<int, int> m_sindex;
    initStatusIndex(m_sindex);

    int m_mapinfo[MAP_SIZE][MAP_SIZE];

    std::vector<Beaner> m_new;
    // showMap(m_mapinfo);
    string strDesc;
    for(int k = 0; k < GENERATION; ++k)
    {
        for(int j = 0; j < BEANER_NUM; ++j)
        {
            for(int r = 0; r < RACE; ++r)
            {
                createMap(m_mapinfo);
                for(int i = 0; i < DAY; ++i)
                {
                    dayAction(m_mapinfo, m_sindex, m_all[j]);
                }
            }
            m_all[j].m_score /= RACE; 
            //cout << "bean:" << j << ", score:" << m_all[j].m_score << endl; 
        }
        sortBeaner(m_all);

        Beaner& bb = m_all[0];
        //sprintf(g_buff, "generation:%d, score:%.0f", k, bb.m_score);
        //writeString(g_buff, "data/generation.txt");

        if(g_best < bb.m_score)
        {
            sprintf(g_buff, "generation:%d, score:%.0f", k, bb.m_score);
            writeString(g_buff, "data/generation.txt");
            cout << g_buff << endl;

            g_best = bb.m_score;
            sprintf(g_buff, "data/%.0f", bb.m_score);
            writeArray(bb.m_dna, DNASIZE, g_buff);
        }

        breed(m_all);
        comet(m_all);
    }
}

void showMap2(int mapinfo[MAP_SIZE][MAP_SIZE])
{
    for(int i = 0; i < 40; ++i)
    {
        cout << endl;
    }

    for(int y = MAP_SIZE - 1; y >= 0; --y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
        {
            switch(mapinfo[y][x])
            {
                case grid_none: cout << "◻︎ ";break;
                case grid_bean: cout << "✢ ";break;
                case grid_wall: cout << "◼︎ ";break;
                case track_1: cout << "◉ ";break;
                case track_2: cout << "◎ ";break;
                case track_3: cout << "◦ ";break;
            }
        }
        cout << endl;
    }

}

void draw(void)
{
    int mapinfo[MAP_SIZE][MAP_SIZE];
    createMap(mapinfo);

    int mapinfo2[MAP_SIZE][MAP_SIZE];
    copyMap(mapinfo, mapinfo2);

    std::map<int, int> sindex;
    initStatusIndex(sindex);

    showMap2(mapinfo2);

    int bestDNA[DNASIZE] = {};
    readArray(bestDNA, DNASIZE, g_dnafile);
    Beaner beaner = Beaner(bestDNA);

    int x1 = beaner.m_x;
    int y1 = beaner.m_y;

    for(int i = 0; i < DAY; ++i)
    {
        dayAction(mapinfo, sindex, beaner);
        x1 = beaner.m_x;
        y1 = beaner.m_y;
        copyMap(mapinfo, mapinfo2);
        mapinfo2[y1][x1] = track_1;
        showMap2(mapinfo2);
        //cout << "day:" << i << " score:" << beaner.m_score << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void testWeightSelect(void)
{
    vector<int> weight;
    weight.push_back(20);
    weight.push_back(80);
    weight.push_back(0);
    weight.push_back(0);
    int first = 0;
    int second = 0;
    for(int i = 0; i < 10000; ++i)
    {
        if(0 == weightSelect(weight, 100)) 
        {
           first++; 
        }
        if(1 == weightSelect(weight, 100)) 
        {
            second++; 
        }
    }
    cout << first << endl;
    cout << second << endl;
}


int main()
{
    srand(time(NULL));

    clock_t ts, te;

    ts = clock();
    evolution();
    //draw();
    te = clock();

    double diff = (te - ts) / CLOCKS_PER_SEC;
    printf("time:%.0f seconds\n", diff);
    sprintf(g_buff, "%.0f seconds\n", diff);
    writeString(g_buff, "data/time.txt");
}
