
#ifndef LFSR_h
#define LFSR_h

#define MINIMAL_SEED_FOR_LFSR 1

/*
 * Implementation Linear Feedback Shift Register
 * I need random generator for range without repeats.
 * If I will be use standard random generator then
 * user could wait a long of time for draw not learn
 * card. Example: Session have 100 cards. User learned
 * to 99 cards. In standard random generator he has 1%
 * chance for draw not learned card and he can wait forever.
 * In LFSR generator in the worst case he must draw 99 cards
 * before draw learned card.
 */
class LFSR
{
  private:
  byte m_previous{ 1 }; //Never 0
  byte m_maxExcluded{0};
  byte m_shiftFactor{0};

  //Source: http://cryptography.wikia.com/wiki/Linear_feedback_shift_register
  //This implementation has special optimization. It limit range of random
  //Method return binary factor depends on the generator polynomial.
  //numbers to near maximum power two.
  void findShiftFactor()
  {
    byte result = 4;
    m_shiftFactor = 2;
    
    while ((m_shiftFactor < 8) && (result < m_maxExcluded)) //byte<0,255>
    {
      result <<= 1;
      m_shiftFactor += 1;
    }

    //We select polynomial parameters depending on the number of bits of the maximum.
    //Maximum number of bits supported in this implementation is 8 (range of byte).
    //See LFSR references for details.
    if (m_shiftFactor == 8)
      m_shiftFactor =  (1 << 7) | (1 << 5) | (1 << 4) | (1 << 3); //0b10111000
    else if (m_shiftFactor == 5)
      m_shiftFactor = (1 << 4) | (1 << 2);                        //0b00010100
    else
      m_shiftFactor = (1 << (m_shiftFactor - 1)) | (1 << (m_shiftFactor - 2));
  }

  //Clear implementation of LFSR algorithm.
  byte nextLFSR()
  {
    return (m_previous >> 1) ^ (-(m_previous & 1) & m_shiftFactor);
  }
  
  public:
  LFSR() {}
  ~LFSR() {}

  //You should call this method before draw numbers.
  //Input arguments:
  //  @seed - It should be equals or greater then 1. (Never 0!) Seed should be equals or less then @maxExcluded.
  //  @maxExcluded - This algorithm draw numbers from 0 to @maxExcluded - 1.
  //                 @maxExcluded cannot be draw.
  void setup(byte seed, byte maxExcluded)
  {
    m_previous = seed == 0 ? 1 : seed;
    m_maxExcluded = maxExcluded;
    findShiftFactor();
  }
  
  //Return nex random byte.
  //Last draw number in period is @seed.
  byte next()
  {
    do
    {
      m_previous = nextLFSR();
    }
    while (m_previous > m_maxExcluded);
    
    return m_previous - 1; //Because we generate number from 1 to m_maxExcluded
  }
};

#endif //LFSR_h
