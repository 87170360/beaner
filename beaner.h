#ifndef __BEANER_H__
#define __BEANER_H__

#include <time.h>       /* time */
#include <stdlib.h> 

const int DNASIZE = 243;

class Beaner
{
    public:
        Beaner(void);
        ~Beaner(void);

    public:
        int m_dna[DNASIZE];
        int m_score;
};

#endif
