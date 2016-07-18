#ifndef UnitTestUtils_h
#define UnitTestUtils_h

#define ANY_CHAR '`'
bool equalCharArrays(const char * arr, const char * correctArr)
{
  while (*correctArr)
  {
    if (! *arr)
      return false;
      
    if (*correctArr != ANY_CHAR)
    {
      if (*arr != *correctArr)
        return false;
    }
    *arr++;
    *correctArr++;
  }
  return true;
}

#endif //UnitTestUtils_h
