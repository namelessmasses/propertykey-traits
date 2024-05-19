# propertykey-traits

## LICENSE

Licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007

See the license files under
* `COPYING`, and
* `COPYING.LESSER`

## General

Requires C++20.

Implemnets compile time "traits" for Windows PROPERTYKEY and DEVPROPKEY. 

~~Keys declared by `DEFINE_PROPERTYKEY` or `DEFINE_DEVPROPKEY` are now `constexpr` and no longer require the `INITGUID` idiom for declaring `extern` versus defining/initializing.~~
Working on a means to "inline".

Keys are distinct types. For example, `PKEY_Key_1` is a distinct type from `PKEY_Key_2` and therefore their values cannot be mistakenly assigned to one another. However, any key will decay to its underlying type. 

For example,

```
DEFINE_PROPERTYKEY(PKEY_Key_1, 0x12345678, 0x90ab, 0xcdef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 42);
DEFINE_PROPERTYKEY(PKEY_Key_2, 0x12345678, 0x90ab, 0xcdef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 43);

...

PKEY_Key_1 = PKEY_Key_2; // Fails to compile.
```

Given any funciton that takes a `PROPERTYKEY`, i.e.,

```
void f(PROPERTYKEY const *pkey);

f(&PKEY_Key_1); // will succeed and behave as expected with the values assigned in the declaration.
```

`DEFINE_PROPERTYKEY_(underlying_key_type, name, fmtid, pid)` allows any `underlying_key_type` that satisfies the following C++ concept.

```
template<typename RuntimePropertyKeyT>
concept RuntimePropertyKeyConcept
    = requires(RuntimePropertyKeyT lhs, RuntimePropertyKeyT rhs) {
          std::same_as<decltype(lhs.fmtid), GUID>;
          std::same_as<decltype(lhs.pid), long>;
      };
```

Direct access to `fmtid` and `pid` become via `constexpr GUID fmtid() const` and `constexpr long pid() const` allowing them to be further used in `constexpr` compile-time expressions.

Furthermore, access to the actual underlying key type can be obtained via `static_cast< <underlying_key_type> >(PKEY_Key_1)`. At that level, `fmtid` and `pid` names refer to those of the underlying type as expected.

In this version, compile time traits include 
* fmtid_value
* pid_value
* nameA_value: a `std::string_view` of the `name` passed to the definition macro, i.e., `"PKEY_Key_1"`.
* nameW_value: a `std::wstring_view` of the `name` passed to the definition macro, i.e., `L"PKEY_Key_1"`.

## Coming soon... (as in the next few hours)
A version to come in the next few days will support
* [COMPLETED] Optionally assigning of a _metatype_ to the value of the property.
* Runtime retrieval of "traits" via any structure that meets the `RuntimePropertyKeyConcept` concept.