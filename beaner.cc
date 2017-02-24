#include "beaner.h"

Beaner::Beaner(void)
   : m_score(0)
{
    for(int i = 0; i < sizeof(m_dna) / sizeof(int); ++i)
    {
        m_dna[i] =  rand() % BEHAVIOR; 
    }
    m_x = 5;
    m_y = 5;
}

Beaner::~Beaner(void)
{

}
    
