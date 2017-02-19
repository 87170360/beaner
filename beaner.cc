#include "beaner.h"

Beaner::Beaner(void)
   : m_score(0)
{
    srand (time(NULL));
    for(int i = 0; i < sizeof(m_dna) / sizeof(int); ++i)
    {
        m_dna[i] =  rand() % 3; 
    }
}

Beaner::~Beaner(void)
{

}
