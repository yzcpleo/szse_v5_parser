// @Copyright 2017, cao.ning, All Rights Reserved
// @Author:   cao.ning
// @Date:     2017/02/21
// @Brief:    深交所V5binary协议中的基础数据类型定义
//            immutable_：不可变类型，只保存指针对象，提供读操作，不提供写操作
//            mutable_  ：可变类型，保存了实际数据，提供了读写操作

#ifndef __CN_SZSE_BINARY_TYPE_H__
#define __CN_SZSE_BINARY_TYPE_H__

#include <stdint.h>
#include <string>
#include <assert.h>
#include <type_traits>

namespace cn
{
namespace szse
{
namespace binary
{

// 判定类型Ty 是否为size长度的整型
template <typename Ty, size_t Size>
struct is_specific_size_integral
    : std::integral_constant<bool, 
        std::is_integral<Ty>::value && sizeof(Ty) == Size>
{
};


// 深圳证券交易所Binary行情数据接口规范1.00 
// 所有整数类型字段都采用高字节序（ BIG-ENDIAN）编码方式

// 单字节数字类型（int8_t, uint8_t）
template <typename IntTy>
typename std::enable_if<is_specific_size_integral<IntTy, 1>::value, IntTy>::type
ChangeEndian(IntTy d)
{
    return d;
}
// 双字节数字类型（int16_t, uint16_t）
template <typename IntTy>
typename std::enable_if<is_specific_size_integral<IntTy, 2>::value, IntTy>::type
ChangeEndian(IntTy d)
{
    return ((d & 0x00FF) << 8) | ((d & 0xFF00) >> 8);
}
// 四字节数字类型（int32_t, uint32_t）
template <typename IntTy>
typename std::enable_if<is_specific_size_integral<IntTy, 4>::value, IntTy>::type
ChangeEndian(IntTy d)
{
    return ((d & 0x000000FF) << 24) | ((d & 0x0000FF00) << 8) |
        ((d & 0x00FF0000) >> 8) | ((d & 0xFF000000) >> 24);
}
// 八字节数字类型（int64_t, uint64_t）
template <typename IntTy>
typename std::enable_if<is_specific_size_integral<IntTy, 8>::value, IntTy>::type
ChangeEndian(IntTy d)
{
    return 
((d & 0x00000000000000FFUI64) << 56) | ((d & 0x000000000000FF00UI64) << 40) |
((d & 0x0000000000FF0000UI64) << 24) | ((d & 0x00000000FF000000UI64) << 8) |
((d & 0x000000FF00000000UI64) >> 8) | ((d & 0x0000FF0000000000UI64) >> 24) |
((d & 0x00FF000000000000UI64) >> 40) | ((d & 0xFF00000000000000UI64) >> 56);
}

typedef bool is_mutable;

namespace immutable_
{

// 基础对象：固定长度的一块不可变的内存数据
template <size_t Size> class base_object
{
protected:
    const char* mem_addr_;
public:
    static const is_mutable mutable_type = false;

    base_object() : mem_addr_(nullptr) {}
    void load(const char* buf) { mem_addr_ = buf; }
    void write(char* buf) const { assert(false && "write immutable_ object"); }
    inline const char* mem_addr() const { return mem_addr_; }
    static size_t mem_size() { return Size; }
};

// 整型
// 设置数据和读取数据时需要进行大小端转换
template <typename IntTy>
class Int : public base_object<sizeof(IntTy)>
{
    // 确保内存中为BIG-ENDIAN
public:
    inline IntTy get_value() const 
    {
        const IntTy* ptr = (const IntTy*)mem_addr_;
        return ChangeEndian(*ptr); 
    }
};

// 浮点类型
// x表示整数与小数总计位数，不包括小数点，y表示小数位数
// 使用int64_t存储
template <int x, int y> class Number : public Int<int64_t>
{
    double pow_;
public:
    Number() : pow_(std::pow(10, y)) {}
    inline double get_value() const { return double(Int::get_value()) / pow_; }
    inline int64_t raw_value() const { return Int::get_value(); }
};

// bool类型
class Boolean : public Int<uint16_t>
{
public:
    static const uint16_t True = 1;
    static const uint16_t False = 0;
    inline uint16_t get_value() const { return Int::get_value(); }
    operator bool() const { return get_value() == True; }
};

// 时间戳类型，YYYYMMDDHHMMSSsss
class LocalTimeStamp : public Int<int64_t>
{
public:
    inline uint32_t year()   const 
    { return (uint32_t)(get_value() / 10000000000000); }
    inline uint32_t month()  const 
    { return (uint32_t)(get_value() % 10000000000000 / 100000000000); }
    inline uint32_t day()    const 
    { return (uint32_t)(get_value() % 100000000000 / 1000000000); }
    inline uint32_t hour()   const 
    { return (uint32_t)(get_value() % 1000000000 / 10000000); }
    inline uint32_t minute() const 
    { return (uint32_t)(get_value() % 10000000 / 100000); }
    inline uint32_t sec()    const 
    { return (uint32_t)(get_value() % 100000 / 1000); }
    inline uint32_t msec()   const 
    { return (uint32_t)(get_value() % 1000); }
};

// 日期类型，YYYYMMDD
class LocalMktDate : public Int<uint32_t>
{
public:
    inline uint32_t year()  const { return get_value() / 10000; }
    inline uint32_t month() const { return get_value() % 10000 / 100; }
    inline uint32_t day()   const { return get_value() % 100; }
};

// 字符串类型，Size表示字符串可装载的最大字节数
// 字符串实际长度小于字段类型最大长度的都后补空格
// REMARK: String类型直接操作的内存，因此不存在最后的 '\0' 字节，直接获取字符串会越界
template <size_t Size> class String : public base_object<Size>
{
public:
    inline char at(uint32_t i) const { assert(i < Size); return mem_addr_[i]; }
    inline const char* c_str() const { assert(mem_addr_); return mem_addr_; }
    std::string to_string() const { return std::string(mem_addr_, Size); }
};


} // namespace immutable END


namespace mutable_
{

// 基础对象：只提供一个尺寸接口
template <size_t Size> class base_object
{
public:
    static const is_mutable mutable_type = true;
    static size_t mem_size() { return Size; }
};

// 整型
template <typename IntTy>
class Int : public base_object<sizeof(IntTy)>
{
    IntTy data_;
public:
    Int() : data_(0) {}
    explicit Int(IntTy v) { data_ = v; }
    Int(const Int& v) { data_ = v.data_; }
    bool operator==(IntTy v) { return data_ == v; }
    bool operator==(const Int& v) { return data_ == v.data_; }
    void load(const char* buf) { data_ = ChangeEndian(*(IntTy*)buf); }
    void write(char* buf) const { *(IntTy*)buf = ChangeEndian(data_); }
    inline IntTy get_value() const { return data_; }
    inline void set_value(IntTy v) { data_ = v; }
};

// 浮点类型
// x表示整数与小数总计位数，不包括小数点，y表示小数位数
// 使用int64_t存储
template <int x, int y> class Number : public Int<int64_t>
{
    typedef Int<uint16_t> base_type;
    double pow_;
public:
    Number() : pow_(std::pow(10, y)) {}
    inline double get_value() const
    {
        return double(base_type::get_value()) / pow_;
    }
    inline int64_t raw_value() const
    {
        return base_type::get_value();
    }
    inline void set_value(double v)
    {
        base_type::set_value((int64_t)(v * pow_));
    }
};

// bool类型
class Boolean : public Int<uint16_t>
{
    typedef Int<uint16_t> base_type;
public:
    static const uint16_t True = 1;
    static const uint16_t False = 0;
    inline bool get_value() const
    {
        return base_type::get_value() == True ? true : false;
    }
    inline void set_value(bool b)
    {
        base_type::set_value(b ? True : False);
    }
    inline uint16_t raw_value() const
    {
        return Int::get_value();
    }
};

// 时间戳类型，YYYYMMDDHHMMSSsss
class LocalTimeStamp : public Int<int64_t>
{
public:
    inline uint32_t year()   const
    {
        return (uint32_t)(get_value() / 10000000000000);
    }
    inline uint32_t month()  const
    {
        return (uint32_t)(get_value() % 10000000000000 / 100000000000);
    }
    inline uint32_t day()    const
    {
        return (uint32_t)(get_value() % 100000000000 / 1000000000);
    }
    inline uint32_t hour()   const
    {
        return (uint32_t)(get_value() % 1000000000 / 10000000);
    }
    inline uint32_t minute() const
    {
        return (uint32_t)(get_value() % 10000000 / 100000);
    }
    inline uint32_t sec()    const
    {
        return (uint32_t)(get_value() % 100000 / 1000);
    }
    inline uint32_t msec()   const
    {
        return (uint32_t)(get_value() % 1000);
    }
};

// 日期类型，YYYYMMDD
class LocalMktDate : public Int<uint32_t>
{
public:
    inline uint32_t year()  const { return get_value() / 10000; }
    inline uint32_t month() const { return get_value() % 10000 / 100; }
    inline uint32_t day()   const { return get_value() % 100; }
};

// 字符串类型，Size表示字符串可装载的最大字节数
// 字符串实际长度小于字段类型最大长度的都后补空格
template <size_t Size> class String : public base_object<Size>
{
    char data_[Size + 1];
    void fill_with_blank() { memset(data_, ' ', Size); data_[Size] = '\0'; }
public:
    String() { fill_with_blank(); }
    inline char at(uint32_t i) const { assert(i < Size); return data_[i]; }
    inline const char* c_str() const { return data_; }
    std::string to_string() const { return std::string(data_); }
    inline size_t size() { return Size; }
    void load(const char* buf) { memcpy(data_, buf, Size); }
    void write(char* buf) const { memcpy(buf, data_, Size); }
};


} // namespace mutable END


template <is_mutable, size_t Size>
struct base_object : public immutable_::base_object<Size>
{
};

template <size_t Size> 
struct base_object<true, Size> : public mutable_::base_object<Size>
{
};

template <is_mutable, typename InnerTy>
struct Int : public immutable_::Int<typename InnerTy>
{
};

template <typename InnerTy> 
struct Int <true, InnerTy> : public mutable_::Int<typename InnerTy>
{
};

template <is_mutable, int x, int y>
struct Number : public immutable_::Number<x, y>
{
};

template <int x, int y> 
struct Number <true, x, y> : public mutable_::Number<x, y>
{
};

template <is_mutable>
struct Boolean : public immutable_::Boolean
{
};

template <> 
struct Boolean <true> : public mutable_::Boolean
{
};

template <is_mutable>
struct LocalTimeStamp : public immutable_::LocalTimeStamp
{
};

template <> 
struct LocalTimeStamp <true> : public mutable_::LocalTimeStamp
{
};

template <is_mutable>
struct LocalMktDate :public immutable_::LocalMktDate
{
};

template <> 
struct LocalMktDate <true> : public mutable_::LocalMktDate
{
};

template <is_mutable, size_t Size>
struct String : public immutable_::String<Size>
{
};

template <size_t Size> 
struct String <true, Size> : public mutable_::String<Size>
{
};

} // namespace binary END
} // namespace szse END
} // namespace cn END

#endif // __CN_SZSE_BINARY_TYPE_H__