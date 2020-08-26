#ifndef _CEST_NUMERIC_HPP_
#define _CEST_NUMERIC_HPP_

#include <numeric>

namespace cest {

//  using std::accumulate;

  using std::accumulate;
  using std::reduce;
  using std::inner_product;
  using std::transform_reduce;
  using std::partial_sum;
  using std::exclusive_scan;
  using std::inclusive_scan;
  using std::transform_exclusive_scan;
  using std::transform_inclusive_scan;
  using std::adjacent_difference;
  using std::iota;

} // namespace cest

#endif // _CEST_NUMERIC_HPP_
