#include <customize.hh>
#include <cstdint>
#include <span>
#include <array>

namespace ursa {


// template <typename word_t>
struct serial {
    //how to parameterize the span type?
    static void write(auto&& v, std::span<uint8_t const> data);

    static void read_into(auto&& v, std::span<uint8_t>& buffer)
    {
        //default implementation loops over single byte reads
        for(auto& word : buffer) {
            word = read<1>(v);
        }
    }
    [[nodiscard]] static bool tx_busy(auto&&);
    [[nodiscard]] static bool rx_busy(auto&&);

    //FIXME: not sure how to handle cases where the
    //customization is also templated; I think the template
    //must be instantiated in order to get at its annotations
    template <size_t N>
    static std::array<uint8_t,N> read(auto&& v);
    // {
    //     return customize::invoke<^^N>(^^read<N>, std::forward<decltype(v)>(v));
    // }

};

inline constexpr auto serial_interface = customize::interface<serial>{};

} //namespace ursa





