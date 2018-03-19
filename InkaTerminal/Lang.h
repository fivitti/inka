#ifndef Lang_h
#define Lang_h

//This project support compilation-time choose language
//You should only create your version language file and
//set define @LANG to name of header file with translation.

//List available languages
#define LANG_PL "LangPL.h"
#define LANG_EN "LangEN.h"

//Selected language
#define LANG LANG_PL

#include LANG

//Clean up
#undef LANG
#undef LANG_PL
#undef LANG_EN

#endif //Lang_h