#ifndef __BEANER_H__
#define __BEANER_H__

#include <time.h>       /* time */
#include <stdlib.h> 

const int DNASIZE = 243;
const int BEHAVIOR = 7;

class Beaner
{
    public:
        //Beaner(int sand);
        Beaner(void);
        Beaner(const int dna[DNASIZE]);
        ~Beaner(void);

    public:
        int m_dna[DNASIZE];
        float m_score;
        int m_x;
        int m_y;
};

#endif
