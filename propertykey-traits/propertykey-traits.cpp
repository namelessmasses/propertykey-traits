// TestTemplateValueArgs.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//

#include "PropertyKey.hpp"
#include "CompileTimePropertyKey.hpp"
#include "RuntimePropertyKey.hpp"
#include "concept.hpp"

#include <cguid.h>
#include <propkey.h>
#include <devpkey.h>

#include <iostream>
#include <format>
#include <string>
#include <cassert>

constexpr GUID guid = constexpr_guid_v<0x12345678,
                                       0x9012,
                                       0x3456,
                                       0x78,
                                       0x90,
                                       0x12,
                                       0x34,
                                       0x56,
                                       0x78,
                                       0x90,
                                       0x12>;

DEFINE_PROPERTYKEY_(PROPERTYKEY, PKEY_Test_Key1, guid, 42);
static_assert(PKEY_Test_Key1_PropertyKey::fmtid_value == guid);
static_assert(PKEY_Test_Key1_PropertyKey::fmtid_value != CONSTEXPR_GUID_NULL);
static_assert(PKEY_Test_Key1_PropertyKey::pid_value == 42);
static_assert(PKEY_Test_Key1.fmtid() == guid);
static_assert(PKEY_Test_Key1.pid() == 42);
static_assert(PKEY_Test_Key1_PropertyKey::nameA_value == "PKEY_Test_Key1");
static_assert(PKEY_Test_Key1_PropertyKey::nameW_value == L"PKEY_Test_Key1");
static_assert(PKEY_Test_Key1.nameA() == "PKEY_Test_Key1");
static_assert(PKEY_Test_Key1.nameW() == L"PKEY_Test_Key1");

DEFINE_PROPERTYKEY(PKEY_Test_Key2,
                   0x12345678,
                   0x9012,
                   0x3456,
                   0x78,
                   0x90,
                   0x12,
                   0x34,
                   0x56,
                   0x78,
                   0x90,
                   0x12,
                   43);
static_assert(PKEY_Test_Key2_PropertyKey::fmtid_value == guid);
static_assert(PKEY_Test_Key2_PropertyKey::fmtid_value != CONSTEXPR_GUID_NULL);
static_assert(PKEY_Test_Key2_PropertyKey::pid_value == 43);
static_assert(PKEY_Test_Key2.fmtid() == guid);
static_assert(PKEY_Test_Key2.pid() == 43);
static_assert(PKEY_Test_Key2_PropertyKey::nameA_value == "PKEY_Test_Key2");
static_assert(PKEY_Test_Key2_PropertyKey::nameW_value == L"PKEY_Test_Key2");
static_assert(PKEY_Test_Key2.nameA() == "PKEY_Test_Key2");
static_assert(PKEY_Test_Key2.nameW() == L"PKEY_Test_Key2");

struct OtherPropertyKey
{
    GUID fmtid;
    long pid;
};

DEFINE_PROPERTYKEY_(OtherPropertyKey, OPKEY_Test_Key3, guid, 44);
static_assert(OPKEY_Test_Key3_PropertyKey::fmtid_value == guid);
static_assert(OPKEY_Test_Key3_PropertyKey::fmtid_value != CONSTEXPR_GUID_NULL);
static_assert(OPKEY_Test_Key3_PropertyKey::pid_value == 44);
static_assert(OPKEY_Test_Key3.fmtid() == guid);
static_assert(OPKEY_Test_Key3_PropertyKey::nameA_value == "OPKEY_Test_Key3");
static_assert(OPKEY_Test_Key3_PropertyKey::nameW_value == L"OPKEY_Test_Key3");
static_assert(OPKEY_Test_Key3.nameA() == "OPKEY_Test_Key3");
static_assert(OPKEY_Test_Key3.nameW() == L"OPKEY_Test_Key3");

DEFINE_DEVPROPKEY(DEVPKEY_Test_Key4,
                  0x12345678,
                  0x9012,
                  0x3456,
                  0x78,
                  0x90,
                  0x12,
                  0x34,
                  0x56,
                  0x78,
                  0x90,
                  0x12,
                  45);
static_assert(DEVPKEY_Test_Key4_PropertyKey::fmtid_value == guid);
static_assert(DEVPKEY_Test_Key4_PropertyKey::fmtid_value
              != CONSTEXPR_GUID_NULL);
static_assert(DEVPKEY_Test_Key4_PropertyKey::pid_value == 45);
static_assert(DEVPKEY_Test_Key4.fmtid() == guid);
static_assert(DEVPKEY_Test_Key4.pid() == 45);
static_assert(DEVPKEY_Test_Key4_PropertyKey::nameA_value
              == "DEVPKEY_Test_Key4");
static_assert(DEVPKEY_Test_Key4_PropertyKey::nameW_value
              == L"DEVPKEY_Test_Key4");
static_assert(DEVPKEY_Test_Key4.nameA() == "DEVPKEY_Test_Key4");
static_assert(DEVPKEY_Test_Key4.nameW() == L"DEVPKEY_Test_Key4");

int
main()
{
    assert(std::format("{}", guid) == "{12345678-9012-3456-7890-123456789012}");
    assert(std::format(L"{0}", guid)
           == L"{12345678-9012-3456-7890-123456789012}");
    std::cout << guid << std::endl;
    std::wcout << guid << std::endl;
    std::cout << std::endl;

    CompileTimePropertyKey<CONSTEXPR_GUID_NULL, 0x00000000> ctpk;
    assert(std::format("{}", ctpk)
           == "({00000000-0000-0000-0000-000000000000},0)");
    assert(std::format(L"{0}", ctpk)
           == L"({00000000-0000-0000-0000-000000000000},0)");
    std::cout << std::format("{}", ctpk) << std::endl;
    std::wcout << std::format(L"{0}", ctpk) << std::endl;
    std::cout << ctpk << std::endl;
    std::wcout << ctpk << std::endl;
    std::cout << std::endl;

    CompileTimePropertyKey<guid, 32> ctpk2;
    assert(std::format("{}", ctpk2)
           == "({12345678-9012-3456-7890-123456789012},32)");
    assert(std::format(L"{0}", ctpk2)
           == L"({12345678-9012-3456-7890-123456789012},32)");
    std::cout << std::format("{}", ctpk2) << std::endl;
    std::wcout << std::format(L"{0}", ctpk2) << std::endl;
    std::cout << ctpk2 << std::endl;
    std::wcout << ctpk2 << std::endl;
    std::cout << std::endl;

    PROPERTYKEY pk{guid, 33};
    assert(std::format("{}", pk)
           == "({12345678-9012-3456-7890-123456789012},33)");
    assert(std::format(L"{0}", pk)
           == L"({12345678-9012-3456-7890-123456789012},33)");
    std::cout << std::format("{}", pk) << std::endl;
    std::wcout << std::format(L"{0}", pk) << std::endl;
    std::cout << pk << std::endl;
    std::wcout << pk << std::endl;
    std::cout << std::endl;

    DEVPROPKEY dpk{guid, 34};
    assert(std::format("{}", dpk)
           == "({12345678-9012-3456-7890-123456789012},34)");
    assert(std::format(L"{0}", dpk)
           == L"({12345678-9012-3456-7890-123456789012},34)");
    std::cout << std::format("{}", dpk) << std::endl;
    std::wcout << std::format(L"{0}", dpk) << std::endl;
    std::cout << dpk << std::endl;
    std::wcout << dpk << std::endl;
    std::cout << std::endl;

    {
        using runtime_type     = PKEY_Test_Key1_PropertyKey::runtime_type;
        using compiletime_type = PKEY_Test_Key1_PropertyKey::compiletime_type;

        assert(static_cast<runtime_type const &>(PKEY_Test_Key1).fmtid == guid);
        assert(static_cast<runtime_type const &>(PKEY_Test_Key1).pid == 42);

        assert(std::format("{}", (runtime_type)PKEY_Test_Key1)
               == "({12345678-9012-3456-7890-123456789012},42)");

        assert(std::format("{}", (compiletime_type)PKEY_Test_Key1)
               == "({12345678-9012-3456-7890-123456789012},42)");

        std::cout << std::format("{}", PKEY_Test_Key1) << std::endl;
        std::cout << PKEY_Test_Key1 << std::endl;
        std::cout << std::endl;
    }

    {
        using runtime_type     = OPKEY_Test_Key3_PropertyKey::runtime_type;
        using compiletime_type = OPKEY_Test_Key3_PropertyKey::compiletime_type;

        assert(static_cast<runtime_type const &>(OPKEY_Test_Key3).fmtid
               == guid);
        assert(static_cast<runtime_type const &>(OPKEY_Test_Key3).pid == 44);

        assert(std::format("{}", (runtime_type)OPKEY_Test_Key3)
               == "({12345678-9012-3456-7890-123456789012},44)");

        assert(std::format("{}", (compiletime_type)OPKEY_Test_Key3)
               == "({12345678-9012-3456-7890-123456789012},44)");

        std::cout << std::format("{}", OPKEY_Test_Key3) << std::endl;
        std::cout << OPKEY_Test_Key3 << std::endl;
        std::cout << std::endl;
    }

    {
        using runtime_type = DEVPKEY_Test_Key4_PropertyKey::runtime_type;
        using compiletime_type
            = DEVPKEY_Test_Key4_PropertyKey::compiletime_type;

        assert(static_cast<runtime_type const &>(DEVPKEY_Test_Key4).fmtid
               == guid);
        assert(static_cast<runtime_type const &>(DEVPKEY_Test_Key4).pid == 45);

        assert(std::format("{}", (runtime_type)DEVPKEY_Test_Key4)
               == "({12345678-9012-3456-7890-123456789012},45)");

        assert(std::format("{}", (compiletime_type)DEVPKEY_Test_Key4)
               == "({12345678-9012-3456-7890-123456789012},45)");

        std::cout << std::format("{}", DEVPKEY_Test_Key4) << std::endl;
        std::cout << DEVPKEY_Test_Key4 << std::endl;
        std::cout << std::endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
