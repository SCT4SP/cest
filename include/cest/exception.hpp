#ifndef _CEST_EXCEPTION_HPP_
#define _CEST_EXCEPTION_HPP_

namespace cest {

class exception {
public:
  constexpr exception() noexcept {}
  constexpr exception(const exception &other) noexcept {}
  constexpr exception &operator=(const exception &other) noexcept {
    return *this;
  }
  constexpr virtual const char *what() const noexcept {
    return "cest::exception";
  }
  constexpr virtual ~exception() {}
};

} // namespace cest

#endif // _CEST_EXCEPTION_HPP_
