#ifndef _CEST_COMPARE_HPP_
#define _CEST_COMPARE_HPP_

#include <compare>
#include <utility> // std::forward

namespace cest {

struct compare_three_way {
  template <class T, class U>
  requires std::three_way_comparable_with<T, U>
  constexpr auto operator()(T &&t, U &&u) const {
    return std::forward<T>(t) <=> std::forward<U>(u);
  }

  using is_transparent = int;
};

} // namespace cest

#endif //  _CEST_COMPARE_HPP_
