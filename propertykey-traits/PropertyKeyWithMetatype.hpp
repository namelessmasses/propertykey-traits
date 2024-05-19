#pragma once

#if 0
template <MeetsBasePKT BasePKT, typename MTT, MTT mt>
struct PKwMTT
    : pk<typename BasePKT::rtt, BasePKT::fmtid_value.i, BasePKT::pid_value>
{
    using base_type = pk<typename BasePKT::rtp, BasePKT::fmtid_value.i, BasePKT::pid_value>;

    using mtt = MTT;
    static constexpr mtt mtt_value = mt;

    constexpr PKwMTT()
        : base_type{}
    {}
};
template <typename T>
void test_output(T const &t)
{
    std::cout << "Format: " << std::format("{0}", t) << std::endl;
    //std::wcout << "Format (w): " << std::format(L"{0}", t) << std::endl;
    //std::cout << t << std::endl;
    //std::wcout << t << std::endl;
}

#endif
