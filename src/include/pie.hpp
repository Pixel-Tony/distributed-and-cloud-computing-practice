#ifndef PIESHKOV_DCC_PIE_HPP
#define PIESHKOV_DCC_PIE_HPP

#include <cmath>

#define local_str2(a) #a
#define local_str(a) local_str2(a)
static constexpr const auto PI_String = local_str(M_PI);
#undef local_str
#undef local_str2


#endif //PIESHKOV_DCC_PIE_HPP
