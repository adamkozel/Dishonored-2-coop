#pragma once
#include <cstddef>
// 1059: mission maps carry far more pickups than the Dunwall fixture (51).
// Bounded per-map catalog; both peers must still agree on its digest.
constexpr std::size_t kCoinCatalogMax=512;
