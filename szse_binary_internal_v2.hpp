// @Copyright 2017, cao.ning, All Rights Reserved
// @Author:   cao.ning
// @Date:     2017/02/27
// @Brief:    

#ifndef __CN_UTIL_SZSE_BINARY_INTERNAL_H__v2
#define __CN_UTIL_SZSE_BINARY_INTERNAL_H__v2

#include <stdint.h>
#include <assert.h>
#include <type_traits>
#include <vector>

namespace cn
{
namespace szse
{
namespace binary
{

template < typename Ty, typename ...Args >
bool load_from_memory(
    const char*& mem_addr, size_t& mem_size, Ty& var, Args&... rest)
{
    if (mem_addr && mem_size >= Ty:: mem_size())
    {
        var.load(mem_addr);
        mem_addr += Ty::mem_size();
        mem_size -= Ty::mem_size();
        return load_from_memory(mem_addr, mem_size, rest...);
    }
    return false;
}
// end
bool load_from_memory(const char*& mem_addr, size_t& mem_size) { return true; }

template < typename Ty, typename ...Args >
bool write_into_memory(
    char*& mem_addr, size_t& mem_size, Ty& var, Args&... rest)
{
    if (mem_addr && mem_size >= Ty::mem_size())
    {
        var.write(mem_addr);
        mem_addr += Ty::mem_size();
        mem_size -= Ty::mem_size();
        return write_into_memory(mem_addr, mem_size, rest...);
    }
    return false;
}
// end
bool write_into_memory(char*& mem_addr, size_t& mem_size) { return true; }

// 计算数据域尺寸的模板
template <typename Ty, typename ...Args>
size_t byte_size_sum(Ty&, Args&... rest)
{
    return Ty::mem_size() + byte_size_sum(rest...);
}
// end
size_t byte_size_sum() { return 0; }


namespace immutable_
{
// 不可修改的数据结构数组
template <typename TyField> class FieldArray
{
public:
    FieldArray() : mem_addr_(nullptr), mem_tail_(nullptr), field_count_(0) {}
    bool Load(const char* mem_addr, size_t mem_size, size_t load_count)
    {
        // 合法性检查
        TyField check_field;
        const size_t field_size = check_field.Size();
        if (mem_size < load_count * field_size)
        {
            return false;
        }
        mem_addr_ = mem_tail_ = mem_addr;
        field_count_ = load_count;
        while (load_count--)
        {
            if (!check_field.Load(mem_tail_, mem_size))
            {
                return false;
            }
            mem_tail_ += + field_size;
            mem_size -= field_size;
        }
        return true;
    }
    bool Write(char* mem_addr, size_t mem_size)
    {
        // 不提供写函数
        assert(false && "an immutable_ filed array can not write");
        return true;
    }
    void Append(TyField&)
    {
        assert(false && "an immutable_ filed array can not append");
    }
    inline size_t count() const { return field_count_; }
    TyField at(size_t idx) const
    {
        if (idx >= field_count_)
        {
            throw std::exception("overflow");
        }
        TyField item;
        size_t field_size = item.Size();
        item.Load(mem_addr_ + field_size * idx, field_size);
        return item;
    }
    size_t Size() const
    {
        return mem_tail_ - mem_addr_;
    }
private:
    const char* mem_addr_;
    const char* mem_tail_;
    size_t field_count_;
};
} // namespace immutable_ END

namespace mutable_
{
// 可修改的数据结构数组
template <typename TyField> class FieldArray
{
public:
    FieldArray() {}
    bool Load(const char* mem_addr, size_t mem_size, size_t load_count)
    {
        field_list_.clear();

        TyField load_field;
        size_t field_size = load_field.Size();
        while (load_count--)
        {
            if (!load_field.Load(mem_addr, mem_size)) { return false; }
            field_list_.push_back(load_field);
            mem_addr += field_size;
            mem_size -= field_size;
        }
        return true;
    }
    bool Write(char* mem_addr, size_t mem_size)
    {
        size_t field_size = 0;
        for (auto &field_ref : field_list_)
        {
            if (!field_ref.Write(mem_addr, mem_size)) { return false; }
            if (field_size == 0)
            {
                field_size = field_ref.Size();
            }
            mem_addr += field_size;
            mem_size -= field_size;
        }
        return true;
    }
    void Append(TyField& field)
    {
        field_list_.push_back(field);
    }
    size_t Size() const
    {
        return field_list_.empty() ? 0 : 
            field_list_.front().Size() * count();
    }
    inline size_t count() const { return field_list_.size(); }
    TyField at(size_t idx) const
    {
        return field_list_.at(idx);
    }
private:
    std::vector<typename TyField> field_list_;
};
} // namespace mutable_ END

template <is_mutable, typename TyField>
struct FieldArray : public immutable_::FieldArray<TyField>
{
};

template <typename TyField> 
struct FieldArray <true, TyField> : public mutable_::FieldArray<TyField>
{
};



// @Class:   Field
// @Author:  cao.ning
// @Date:    2017/02/21
// @Brief:
template <is_mutable b>
class Field
{
protected:
    // type define
    template <typename Ty>
    using TypeInt            = Int<b, Ty>;

    template <int x, int y>
    using TypeNumber         = Number<b, x, y>;

    using TypeBoolean        = Boolean<b>;
    using TypeLocalTimeStamp = LocalTimeStamp<b>;
    using TypeLocalMktDate   = LocalMktDate<b>;

    template <size_t Size>
    using TypeString         = String<b, Size>;

    using TypeCompID         = String<b, 20>;
    using TypePrice          = Number<b, 13, 4>;
    using TypeQty            = Number<b, 15, 2>;
    using TypeAmt            = Number<b, 18, 4>;
    using TypeSeqNum         = Int<b, int64_t>;
    using TypeLength         = Int<b, uint32_t>;
    using TypeNumInGroup     = Int<b, uint32_t>;
    using TypeSecurityID     = String<b, 8>;

    template <typename Ty>
    using TypeFieldArray     = FieldArray<b, Ty>;
public:
    virtual uint32_t Type() const = 0;
    virtual uint32_t Size() const = 0;
    virtual bool Load(const char* mem_addr, size_t mem_size) = 0;
    virtual bool Write(char* mem_addr, size_t mem_size) = 0;
};


} // namespace binary END
} // namespace szse END
} // namespace cn END

#endif // __CN_UTIL_SZSE_BINARY_INTERNAL_H__v2