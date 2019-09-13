#include <vector>

#if __cplusplus > 201800
#include <compare>
#endif

namespace
{
    using ull = unsigned long long;
}

namespace ly::bigint
{
    template<
        ull      num_size   = 256,
        typename num_t      = unsigned char,
        typename temp_num_t = long
    >
    struct big_integer_unsigned
      {
          using data_t  = std::vector<num_t>;

          data_t data;
        
          friend big_integer_unsigned &operator+=(big_integer_unsigned &, const big_integer_unsigned &);
          friend big_integer_unsigned &operator-=(big_integer_unsigned &, const big_integer_unsigned &);
          friend big_integer_unsigned &operator*=(big_integer_unsigned &, big_integer_unsigned);
          friend big_integer_unsigned &operator/=(big_integer_unsigned &, big_integer_unsigned);
          friend big_integer_unsigned &operator%=(big_integer_unsigned &, big_integer_unsigned);

          friend big_integer_unsigned operator+(big_integer_unsigned, const big_integer_unsigned &);
          friend big_integer_unsigned operator-(big_integer_unsigned, const big_integer_unsigned &);
          friend big_integer_unsigned operator*(big_integer_unsigned, big_integer_unsigned);
          friend big_integer_unsigned operator/(big_integer_unsigned, big_integer_unsigned);
          friend big_integer_unsigned operator%(big_integer_unsigned, big_integer_unsigned);

          friend big_integer_unsigned &operator+=(big_integer_unsigned &, ull);
          friend big_integer_unsigned &operator-=(big_integer_unsigned &, ull);
          friend big_integer_unsigned &operator*=(big_integer_unsigned &, ull);
          friend big_integer_unsigned &operator/=(big_integer_unsigned &, ull);
          friend big_integer_unsigned &operator%=(big_integer_unsigned &, ull);

          friend big_integer_unsigned operator+(big_integer_unsigned, ull);
          friend big_integer_unsigned operator-(big_integer_unsigned, ull);
          friend big_integer_unsigned operator*(big_integer_unsigned, ull);
          friend big_integer_unsigned operator/(big_integer_unsigned, ull);
          friend big_integer_unsigned operator%(big_integer_unsigned, ull);

        #if __cplusplus < 201800
          friend bool operator< (const big_integer_unsigned &, const big_integer_unsigned &);
          friend bool operator> (const big_integer_unsigned &, const big_integer_unsigned &);
          friend bool operator<=(const big_integer_unsigned &, const big_integer_unsigned &);
          friend bool operator>=(const big_integer_unsigned &, const big_integer_unsigned &);
          friend bool operator==(const big_integer_unsigned &, const big_integer_unsigned &);
          friend bool operator!=(const big_integer_unsigned &, const big_integer_unsigned &);

          friend bool operator< (const big_integer_unsigned &, ull);
          friend bool operator> (const big_integer_unsigned &, ull);
          friend bool operator<=(const big_integer_unsigned &, ull);
          friend bool operator>=(const big_integer_unsigned &, ull);
          friend bool operator==(const big_integer_unsigned &, ull);
          friend bool operator!=(const big_integer_unsigned &, ull);
        #else
          friend std::strong_ordering operator<=>(const big_integer_unsigned &, const big_integer_unsigned &);
          friend std::strong_ordering operator<=>(const big_integer_unsigned &, ull);
        #endif

          big_integer_unsigned() = default;
          big_integer_unsigned(const big_integer_unsigned &) = default;
          big_integer_unsigned(big_integer_unsigned &&n) { data.swap(n.data); }

          big_integer_unsigned &operator=(const big_integer_unsigned &) = default;
          big_integer_unsigned &operator=(big_integer_unsigned &&n) { data.swap(n.data); }

          ~big_integer_unsigned() = default;
      };
}