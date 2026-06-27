#include <customize.hh>
#include <cstdint>
#include <span>
#include <array>

namespace ursa {


template <typename word_t>
struct serial_ {
    static void write(auto&&, std::span<word_t const> data);
    static void read_into(auto&& v, std::span<word_t>& buffer)
    {
        //default implementation loops over single byte reads
        for(auto& word : buffer) {
            word = read<1>(v);
        }
    }
    [[nodiscard]] static bool tx_busy(auto&&);
    [[nodiscard]] static bool rx_busy(auto&&);

    template <size_t N>
    static std::array<word_t,N> read(auto&&);

};

//how to partition the interface to support read- or write-only
//serial interfaces?
//is there something here around interface composition?
//maybe have separate interfaces defined for read and write,
//and then have a way to define an interface that is the union
//or, maybe better, have a way to assert that an interface
//is a superset of another
//I think it's important for the caller to be able to
//specify the difference between the two, and for the customizer
//
//substitutability in the liskov sense: if an interface subsumes
//another, the user should be able to use customizations from
//the subsumed interface (e.g. if user code depends only
//on the read interface, it should be able to use
//types that customize the read-write interface... maybe?)

// template <typename word_t>
// using serial = customize::interface<serial_<word_t>>;
template <typename word_t>
struct serial {
    static constexpr customize::function<^^serial_<word_t>::write> write{};
    static constexpr customize::function<^^serial_<word_t>::read_into> read_into{};
    static constexpr customize::function<^^serial_<word_t>::tx_busy> tx_busy{};
    static constexpr customize::function<^^serial_<word_t>::rx_busy> rx_busy{};
    template <size_t N> //this isn't right
    static constexpr customize::function<^^serial_<word_t>::read> read{};
};

} //namespace ursa


namespace my_stuff {

struct my_serial {

};

[[=ursa::serial<uint8_t>::write]] void write(my_serial&, std::span<uint8_t const> data) {}

//FIXME: how to opt into using the default?
[[=ursa::serial<uint8_t>::read_into]] void read_into(my_serial&, std::span<uint8_t>& buffer);
// [[=ursa::serial<uint8_t>::read_into]] constexpr auto read_into = customize::dfault;

template <size_t N>
[[=ursa::serial<uint8_t>::read<N>]] std::array<uint8_t,N> read(my_serial&) { return {}; }

[[=ursa::serial<uint8_t>::read<0>]] std::array<uint8_t,0> read(my_serial&) { return {}; }

[[=ursa::serial<uint8_t>::tx_busy]] bool tx_busy(my_serial&) { return false; }
[[=ursa::serial<uint8_t>::rx_busy]] bool rx_busy(my_serial&) { return false; }

static_assert(customize::implements<my_serial, ursa::serial<uint8_t>>);


} //namespace my_stuff


