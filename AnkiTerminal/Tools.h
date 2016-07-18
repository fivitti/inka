#ifndef Tools_h
#define Tools_h

namespace NumberTools
{
  byte numberOfDigits(byte number)
  {
    byte numDigits = 1;
    while(number >= 10)
    {
      number /= 10;
      numDigits += 1;
    }
    return numDigits;
  }
}

namespace StringTools
{
  bool isEmpty(char * buffer_)
  {
    while (*buffer_)
    {
      if (*buffer_ != ' ' && *buffer_ != '\0')
        return false;
      buffer_++;
    }
    return true;
  }
}

#endif //Tools_h
