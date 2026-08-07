#include <stdbool.h>
#include <stddef.h>

#define ArraySize(Arr) ( sizeof(Arr)/sizeof(Arr[0]) )

struct StaticList
{
    size_t n_data;
    const char** data;
};

typedef struct StaticList TeSrcs;
typedef struct StaticList TeCmpFlags;
typedef struct StaticList TeRunArgs;


static TeSrcs te_srcs();
static TeSrcs te_cmp_flags();

static TeRunArgs te_run_flags();
