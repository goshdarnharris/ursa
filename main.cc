#include <cstdio>
#include <serial.hh>
#include <format.hh>

namespace my_stuff {

struct my_serial {

};

[[=^^ursa::serial::write]] void my_wacky_write(my_serial&, std::span<uint8_t const> data) {
    fmt::println("my_serial::write called with data");
}

// //FIXME: how to opt into using the default?
[[=^^ursa::serial::read_into]] void read_into(my_serial&, std::span<uint8_t>& buffer);
// // [[=^^ursa::serial::read_into]] constexpr auto read_into = customize::dfault;

// template <size_t N>
// [[=^^ursa::serial::read<N>]] std::array<uint8_t,N> read(my_serial&) { return {}; }

// //FIXME:
// // [[=^^ursa::serial::read<0>]] std::array<uint8_t,0> read(my_serial&) { return {}; }

[[=^^ursa::serial::tx_busy]] bool tx_busy(my_serial&) { return false; }
[[=^^ursa::serial::rx_busy]] bool rx_busy(my_serial&) { return false; }

inline constexpr auto _ = ursa::customize::opt_in<ursa::serial, my_serial>();

} //namespace my_stuff

int main() 
{
    return 0;
}
