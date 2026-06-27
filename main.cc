#include <cstdio>
#include <serial.hh>
#include <format.hh>
#include <function_traits.hh>

struct placeholder {};

int main() 
{
    ursa::pretty_print(ursa::function_traits<^^ursa::serial_<uint8_t>::write<placeholder>>{});

    return 0;
}
