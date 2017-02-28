#include "beaner.h"

Beaner::Beaner(void)
   : m_score(0)
{
    for(int i = 0; i < DNASIZE; ++i)
    {
        m_dna[i] =  rand() % BEHAVIOR; 
    }
    m_x = 5;
    m_y = 5;
}
        
Beaner::Beaner(const int dna[DNASIZE])
{
    for(int i = 0; i < DNASIZE; ++i)
    {
        m_dna[i] =  dna[i]; 
    }
    m_x = 5;
    m_y = 5;
}

Beaner::~Beaner(void)
{

}
    
        
void Beaner::resetDNA(void)
{
    for(int i = 0; i < DNASIZE; ++i)
    {
        m_dna[i] =  rand() % BEHAVIOR; 
    }
}
