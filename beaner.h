#ifndef __BEANER_H__
#define __BEANER_H__

#include <time.h>       /* time */
#include <stdlib.h> 

class Beaner
{
    public:
        Beaner(void);
        ~Beaner(void);

    public:
        int m_dna[243];
        int m_score;
};

#endif
