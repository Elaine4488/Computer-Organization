#include "mem/cache/tags/llc_dirty_aware.hh"
#include "debug/CacheRepl.hh"

LLCDirtyAware::LLCDirtyAware(const Params *p)
    : BaseSetAssoc(p)
{
}

CacheBlk*
LLCDirtyAware::findVictim(Addr addr)
{
    std::vector<CacheBlk*> locations = getPossibleLocations(addr);

    CacheBlk* clean_candidate = nullptr;
    CacheBlk* fallback_candidate = nullptr;

    for (auto* blk : locations) {
        if (!blk->isValid()) {
            return blk;  // find an empty one, just use it
        }

        if (!blk->isDirty() && clean_candidate == nullptr) {
            clean_candidate = blk;
        }

        if (fallback_candidate == nullptr) {
            fallback_candidate = blk;
        }
    }

    if (clean_candidate != nullptr) {
        DPRINTF(CacheRepl, "set %x, way %x: selecting clean blk for replacement\n",
            clean_candidate->set, clean_candidate->way);
        return clean_candidate;
    }

    DPRINTF(CacheRepl, "set %x, way %x: selecting dirty blk for replacement\n",
        fallback_candidate->set, fallback_candidate->way);
    return fallback_candidate;
}

LLCDirtyAware*
LLCDirtyAwareParams::create()
{
    return new LLCDirtyAware(this);
}

