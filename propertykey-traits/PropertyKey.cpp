#include "PropertyKey.hpp"

#include <map>

namespace details
{

std::map<IRuntimePropertyKey const *, IRuntimePropertyKeyTraits const *>
    g_PropertyKeyTraits;

void
AddPropertyKey(IRuntimePropertyKeyTraits const &key)
{
    auto insertResult = g_PropertyKeyTraits.insert(
        std::make_pair(&key, &key));
    if (!insertResult.second)
    {
        // The key is already in the map. Overwrite it with the new value.
        insertResult.first->second = &key;
    }
}

IRuntimePropertyKeyTraits const *
GetPropertyKey(IRuntimePropertyKey const &key)
{
    auto it = g_PropertyKeyTraits.find(&key);
    if (it == g_PropertyKeyTraits.end())
    {
        return nullptr;
    }
    return it->second;
}

}