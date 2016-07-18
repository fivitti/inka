//Linear Feedback Shift Register
#ifndef LFSR_h
#define LFSR_h

#define MINIMAL_SEED_FOR_LFSR 1

class LFSR
{
  private:
  byte m_previous{ 1 }; //Never 0
  byte m_maxExcluded{0};
  byte m_shiftFactor{0};

  //Source: http://cryptography.wikia.com/wiki/Linear_feedback_shift_register
  void findShiftFactor()
  {
    byte result = 4;
    m_shiftFactor = 2;
    
    while ((m_shiftFactor < 8) && (result < m_maxExcluded)) //byte<0,255>
    {
      result <<= 1;
      m_shiftFactor += 1;
    }

    if (m_shiftFactor == 8)
      m_shiftFactor =  (1 << 7) | (1 << 5) | (1 << 4) | (1 << 3);
    else if (m_shiftFactor == 5)
      m_shiftFactor = (1 << 4) | (1 << 2);
    else
      m_shiftFactor = (1 << (m_shiftFactor - 1)) | (1 << (m_shiftFactor - 2));
  }

  byte nextLFSR()
  {
    return (m_previous >> 1) ^ (-(m_previous & 1) & m_shiftFactor);
  }
  
  public:
  LFSR() {}
  LFSR(byte seed, byte maxExcluded) : m_previous(seed), m_maxExcluded(maxExcluded) {
    findShiftFactor();
  };
  ~LFSR() {}

  void setup(byte seed, byte maxExcluded)
  {
    m_previous = seed;
    m_maxExcluded = maxExcluded;
    findShiftFactor();
  }
  
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
