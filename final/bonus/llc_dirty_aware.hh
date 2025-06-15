#ifndef __MEM_CACHE_TAGS_LLCDIRTYAWARE_HH__
#define __MEM_CACHE_TAGS_LLCDIRTYAWARE_HH__

#include "mem/cache/tags/base_set_assoc.hh"
#include "params/LLCDirtyAware.hh"

class LLCDirtyAware : public BaseSetAssoc
{
  public:
    using Params = LLCDirtyAwareParams;

    LLCDirtyAware(const Params *p);

    CacheBlk* findVictim(Addr addr) override;
};

#endif // __MEM_CACHE_TAGS_LLCDIRTYAWARE_HH__

