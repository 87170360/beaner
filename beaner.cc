#include "beaner.h"

Beaner::Beaner(void)
   : m_score(0)
{
    srand (time(NULL));
    for(int i = 0; i < sizeof(m_dna) / sizeof(int); ++i)
    {
        m_dna[i] =  rand() % BEHAVIOR; 
    }
    m_x = rand() % 10 + 1;
    m_y = rand() % 10 + 1;
}

Beaner::~Beaner(void)
{

}
