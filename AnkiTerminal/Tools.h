#ifndef Tools_h
#define Tools_h

namespace NumberTools
{
  //Return number of digits from @number.
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
  //Return true if buffer is empty.
  //Buffer is empty when all its characters are
  //space ' ' or null chars '\0'
  bool isEmpty(const char * buffer_)
  {
    while (*buffer_)
    {
      if (*buffer_ != ' ' && *buffer_ != '\0')
        return false;
      buffer_++;
    }
    return true;
  }

  //Return true where @first string and @second string are equals.
  //If length of string are not equals then if begin chars are equals
  //and end chars are null chars then strings are equals.
  //
  //Equals: "aaa" and "aaa" are equals,
  //        "aaa\0\0\0" and "aaa" are equals.
  //        "aaa" and "bbb" are not equals,
  //        "a"   and "aaa" are not equals,
  //        "aaa    " and "aaa" are not equals,
  bool equals(const char * first, const char * second)
  {
    while (*second && *first)
    {
      if (*first != *second)
        return false;

      first++;
      second++;
    }
    return !(*first) && !(*second);
  }
}

#endif //Tools_h
