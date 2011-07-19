/* Author: Junfeng Yang (junfeng@cs.columbia.edu) */
#include <string.h>
#include "syncfuncs.h"

#undef DEF
#undef DEFTERNAUTO
#undef DEFTERNUSER

namespace tern {
namespace syncfunc {

const int kind[] = {
# define DEF(func,kind,...) kind,
# define DEFTERNAUTO(func)      TernAuto,
# define DEFTERNUSER(func)  TernUser,
# include "syncfuncs.def.h"
# undef DEF
# undef DEFTERNAUTO
# undef DEFTERNUSER
};

const char* name[] = {
# define DEF(func,kind,...) #func,
# define DEFTERNAUTO(func)      #func,
# define DEFTERNUSER(func)  #func,
# include "syncfuncs.def.h"
# undef DEF
# undef DEFTERNAUTO
# undef DEFTERNUSER
};

const char* nameInTern[] = {
# define DEF(func,kind,...) "tern_"#func,
# define DEFTERNAUTO(func)  #func,
# define DEFTERNUSER(func)  #func"_real",
# include "syncfuncs.def.h"
# undef DEF
# undef DEFTERNAUTO
# undef DEFTERNUSER
};

unsigned getNameID(const char* name) {
  for(unsigned i=0; i<num_syncs; ++i)
    if(strcmp(name, getName(i)) == 0)
      return i;
  return not_sync;
}

unsigned getTernNameID(const char* name) {
  for(unsigned i=0; i<num_syncs; ++i)
    if(strcmp(name, getTernName(i)) == 0)
      return i;
  return not_sync;
}

}
}