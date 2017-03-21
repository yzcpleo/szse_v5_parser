// @Copyright 2017, cao.ning, All Rights Reserved
// @Author:   cao.ning
// @Date:     2017/03/01
// @Brief:    深交所V5binary协议中的行情数据结构定义

#ifndef __CN_SZSE_BINARY_MD_FIELD_H__
#define __CN_SZSE_BINARY_MD_FIELD_H__

#include "szse_binary_type.hpp"
#include "szse_binary_field.hpp"

namespace cn
{
namespace szse
{
namespace binary
{


// 登录消息 Logon
template <is_mutable b>
class Logon : public Field<b>
{
public:
    static const uint32_t kMsgType = 1;

    TypeCompID          SenderCompID;           // 发送方代码
    TypeCompID          TargetCompID;           // 接收方代码
    TypeInt<int32_t>    HeartBtInt;             // 心跳间隔，单位是秒
    TypeString<16>      Password;               // 密码
    TypeString<32>      DefaultApplVerID;       // 二进制协议版本，填写为 n.xy

public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(SenderCompID, TargetCompID, HeartBtInt,
                             Password, DefaultApplVerID);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                SenderCompID, TargetCompID,
                                HeartBtInt, Password,
                                DefaultApplVerID);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 SenderCompID, TargetCompID,
                                 HeartBtInt, Password,
                                 DefaultApplVerID);
    }
};

// 注销消息 Logout
template <is_mutable b>
class Logout : public Field<b>
{
public:
    static const uint32_t kMsgType = 2;

    TypeInt<int32_t>    SessionStatus;          // 退出时的会话状态
    TypeString<200>     Text;                   // 注销原因的进一步补充说明
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(SessionStatus, Text);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size, SessionStatus, Text);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size, SessionStatus, Text);
    }
};

// 心跳消息  Heartbeat
template <is_mutable b>
class Heartbeat : public Field<b>
{
public:
    static const uint32_t kMsgType = 3;
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override { return 0; }
    virtual bool Load(const char*, size_t) override { return true; }
    virtual bool Write(char*, size_t) override { return true; }
};

// 业务拒绝消息 BusinessReject
template <is_mutable b>
class BusinessReject : public Field<b>
{
public:
    static const uint32_t kMsgType = 8;

    TypeSeqNum              RefSeqNum;              // 被拒绝消息的消息序号
    TypeInt<uint32_t>       RefMsgType;             // 被拒绝的消息类型
    TypeString<10>          BusinessRejectRefID;    // 被拒绝消息对应的业务层ID
    TypeInt<uint16_t>       BusinessRejectReason;   // 拒绝原因
    TypeString<50>          BusinessRejectText;     // 拒绝原因说明
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(RefSeqNum, RefMsgType, BusinessRejectRefID,
                             BusinessRejectReason, BusinessRejectText);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                RefSeqNum, RefMsgType, BusinessRejectRefID,
                                BusinessRejectReason, BusinessRejectText);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 RefSeqNum, RefMsgType, BusinessRejectRefID,
                                 BusinessRejectReason, BusinessRejectText);
    }
};

// 频道心跳消息  ChannelHeartbeat
template <is_mutable b>
class ChannelHeartbeat : public Field<b>
{
public:
    static const uint32_t kMsgType = 390095;

    TypeInt<uint16_t>    ChannelNo;          // 频道代码
    TypeSeqNum          ApplLastSeqNum;     // 最后一条行情消息的记录号
    TypeBoolean         EndOfChannel;       // 频道结束标志
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ChannelNo, ApplLastSeqNum, EndOfChannel);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                ChannelNo, ApplLastSeqNum, EndOfChannel);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 ChannelNo, ApplLastSeqNum, EndOfChannel);
    }
};

// 公告消息
template <is_mutable b>
class Announcement : public Field<b>
{
public:
    static const uint32_t kMsgType = 390012;

    TypeLocalTimeStamp      OrigTime;          // 公告时间
    TypeInt<uint16_t>       ChannelNo;         // 频道代码
    TypeString<8>           NewsID;            // 公告唯一标识
    TypeString<128>         Headline;          // 公告标题
    TypeString<8>           RawDataFormat;     // 二进制数据格式
    TypeLength              RawDataLength;     // 二进制数据长度
    const char*             RawData;           // 二进制数据（变长）
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, NewsID, Headline,
                             RawDataFormat, RawDataLength)
                             + RawDataLength.get_value();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (load_from_memory(&mem_addr, &mem_size, OrigTime, ChannelNo,
            NewsID, Headline, RawDataFormat, RawDataLength))
        {
            RawData = static_cast<const char*>(mem_addr);
            return true;
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (write_into_memory(&mem_addr, &mem_size, OrigTime, ChannelNo,
            NewsID, Headline, RawDataFormat, RawDataLength))
        {
            if (RawDataLength.get_value() >= mem_size)
            {
                memcpy(mem_addr, RawData, RawDataLength.get_value());
                return true;
            }
        }
        return false;
    }
};

// 重传消息  ReTransmit
template <is_mutable b>
class ReTransmit : public Field<b>
{
public:
    static const uint32_t kMsgType = 390094;

    TypeInt<uint8_t>     ResendType;        // 重传种类
    TypeInt<uint16_t>    ChannelNo;         // 频道代码
    TypeSeqNum           ApplBegSeqNum;     // 起始序号
    TypeSeqNum           ApplEndSeqNum;     // 结束序号
    TypeString<8>        NewsID;            // 公告唯一标识
    TypeInt<uint8_t>     ResendStatus;      // 重传状态
    TypeString<16>       RejectText;        // 文本
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ResendType, ChannelNo, ApplBegSeqNum,
                             ApplEndSeqNum, NewsID, ResendStatus, RejectText);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                ResendType, ChannelNo, ApplBegSeqNum,
                                ApplEndSeqNum, NewsID, ResendStatus,
                                RejectText);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 ResendType, ChannelNo, ApplBegSeqNum,
                                 ApplEndSeqNum, NewsID, ResendStatus,
                                 RejectText);
    }
};

// 市场实时状态 MarketStatus
template <is_mutable b>
class MarketStatus : public Field<b>
{
public:
    static const uint32_t kMsgType = 390019;

    TypeLocalTimeStamp      OrigTime;            // 数据生成时间
    TypeInt<uint16_t>       ChannelNo;           // 频道代码
    TypeString<8>           MarketID;            // 市场代码
    TypeString<8>           MarketSegmentID;     // 市场板块代码，预留
    TypeString<4>           TradingSessionID;    // 交易会话 ID
    TypeString<4>           TradingSessionSubID; // 交易会话子 ID
    TypeInt<uint16_t>       TradSesStatus;       // 交易会话状态，预留
    TypeLocalTimeStamp      TradSesStartTime;    // 交易会话起始时间，预留
    TypeLocalTimeStamp      TradSesEndTime;      // 交易会话结束时间，预留
    TypeAmt                 ThresholdAmount;     // 每日初始额度
    TypeAmt                 PosAmt;              // 日中剩余额度，额度不可用时
    TypeString<1>           AmountStatus;        // 额度状态
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, MarketID, MarketSegmentID,
                             TradingSessionID, TradingSessionSubID,
                             TradSesStatus, TradSesStartTime, TradSesEndTime,
                             ThresholdAmount, PosAmt, AmountStatus);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size, 
                                OrigTime, ChannelNo, MarketID, MarketSegmentID,
                                TradingSessionID, TradingSessionSubID,
                                TradSesStatus, TradSesStartTime, TradSesEndTime,
                                ThresholdAmount, PosAmt, AmountStatus);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size, 
                                 OrigTime, ChannelNo, MarketID, MarketSegmentID,
                                 TradingSessionID, TradingSessionSubID,
                                 TradSesStatus, TradSesStartTime, TradSesEndTime,
                                 ThresholdAmount, PosAmt, AmountStatus);
    }
};

// 证券实时状态 SecurityStatus
template <is_mutable b>
class SecurityStatus : public Field<b>
{
public:
    static const uint32_t kMsgType = 390013;

    TypeLocalTimeStamp      OrigTime;          // 数据生成时间
    TypeInt<uint16_t>       ChannelNo;         // 频道代码
    TypeSecurityID          SecurityID;        // 证券代码
    TypeString<4>           SecurityIDSource;  // 证券代码源
    TypeString<8>           FinancialStatus;   // 证券状态
    TypeNumInGroup         NoSwitch;            // 开关个数
    struct SecuritySwitch : public Field<b>
    {
        TypeInt<uint16_t>   SecuritySwitchType;     // 开关类别
        TypeBoolean         SecuritySwitchStatus;   // 开关状态
        virtual uint32_t Size() const override
        {
            return byte_size_sum(SecuritySwitchType, SecuritySwitchStatus);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size,
                                    SecuritySwitchType, SecuritySwitchStatus);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size,
                                     SecuritySwitchType, SecuritySwitchStatus);
        }
    };
    TypeFieldArray<SecuritySwitch>      SecuritySwitchArray; // 开关数组
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, SecurityID, SecurityIDSource,
                             FinancialStatus, NoSwitch)
                + SecuritySwitchArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                OrigTime, ChannelNo, SecurityID,
                                SecurityIDSource, FinancialStatus, NoSwitch)
                && SecuritySwitchArray.load(&mem_addr, &mem_size, 
                                            NoSwitch.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 OrigTime, ChannelNo, SecurityID, 
                                 SecurityIDSource, FinancialStatus, NoSwitch)
                && SecuritySwitchArray.write(&mem_addr, &mem_size);
    }
};

// 快照行情频道统计
template <is_mutable b>
class MarketSnapshotStatistic : public Field<b>
{
public:
    static const uint32_t kMsgType = 390090;
    TypeLocalTimeStamp      OrigTime;           // 数据生成时间
    TypeInt<uint16_t>       ChannelNo;          // 频道代码
    TypeNumInGroup          NoMDStreamID;       // 行情类别个数
    struct StreamStatistic : public Field<b>
    {
        TypeString<3>           MDStreamID;         // 行情类别
        TypeInt<uint32_t>       StockNum;           // 证券只数
        TypeString<8>           TradingPhaseCode;   // 闭市状态
        virtual uint32_t Size() const override
        {
            return byte_size_sum(MDStreamID, StockNum, TradingPhaseCode);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size,
                                    MDStreamID, StockNum, TradingPhaseCode);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size, 
                                     MDStreamID, StockNum, TradingPhaseCode);
        }
    };
    TypeFieldArray<StreamStatistic>    StatisticArray; // 统计数组
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, NoMDStreamID)
                + StatisticArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                OrigTime, ChannelNo, NoMDStreamID)
                && StatisticArray.load(&mem_addr, &mem_size,
                                        NoMDStreamID.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size, 
                                 OrigTime, ChannelNo, NoMDStreamID)
                && StatisticArray.write(&mem_addr, &mem_size);
    }
};

// 快照行情的公共字段
template <is_mutable b>
class MarketSnapshotBase : public Field<b>
{
public:
    TypeLocalTimeStamp      OrigTime;           // 数据生成时间
    TypeInt<uint16_t>       ChannelNo;          // 频道代码
    TypeString<3>           MDStreamID;         // 行情类别
    TypeSecurityID          SecurityID;         // 证券代码
    TypeString<4>           SecurityIDSource;   // 证券代码源
    TypeString<8>           TradingPhaseCode;   // 产品所处的交易阶段代码
    TypePrice               PrevClosePx;        // 昨收价
    TypeInt<int64_t>        NumTrades;          // 成交笔数
    TypeQty                 TotalVolumeTrade;   // 成交总量
    TypeAmt                 TotalValueTrade;    // 成交总金额
 
#define MarketSnapshotBase_MemberList       \
OrigTime, ChannelNo, MDStreamID, SecurityID, SecurityIDSource, \
TradingPhaseCode, PrevClosePx, NumTrades, TotalVolumeTrade, TotalValueTrade

};

// 集中竞价交易业务行情快照 MarketSnapshot_300111
// 010 现货（股票，基金，债券等）集中竞价交易快照行情
// 020 质押式回购交易快照行情
// 030 债券分销快照行情
// 040 期权集中竞价交易快照行情
template <is_mutable b>
class MarketSnapshot_300111 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300111;
public:
    // Extend Fields 业务扩展字段
    TypeNumInGroup          NoMDEntries;            // 行情条目个数
    // struct SecurityEntry
    struct SecurityEntry : public Field<b>
    {
        TypeString<2>           MDEntryType;     // 行情条目类别
        TypeInt<int64_t>        MDEntryPx;       // 价格，乘数1000000
        TypeQty                 MDEntrySize;     // 数量
        TypeInt<uint16_t>       MDPriceLevel;    // 买卖盘档位
        TypeInt<int64_t>        NumberOfOrders;  // 价位总委托笔数
        TypeNumInGroup          NoOrders;        // 价位揭示委托笔数
        // struct OrderQty
        struct OrderQty : public Field<b>
        {
            TypeQty             Qty;
            virtual uint32_t Size() const override
            {
                return TypeQty::mem_size();
            }
            virtual bool Load(const char* mem_addr, size_t mem_size) override
            {
                return load_from_memory(&mem_addr, &mem_size, Qty);
            }
            virtual bool Write(char* mem_addr, size_t mem_size) override
            {
                return write_into_memory(&mem_addr, &mem_size, Qty);
            }
        };
        TypeFieldArray<OrderQty>       OrderQtyArray;        // 委托数量
        uint32_t Size() const override
        {
            return byte_size_sum(MDEntryType, MDEntryPx, MDEntrySize,
                                 MDPriceLevel, NumberOfOrders, NoOrders)
                    + OrderQtyArray.Size();
        }
        bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size,
                                    MDEntryType, MDEntryPx, MDEntrySize, 
                                    MDPriceLevel, NumberOfOrders, NoOrders)
                    && OrderQtyArray.load(&mem_addr, &mem_size,
                                            NoOrders.get_value());
        }
        bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size,
                                     MDEntryType, MDEntryPx, MDEntrySize,
                                     MDPriceLevel, NumberOfOrders, NoOrders)
                    && OrderQtyArray.write(&mem_addr, &mem_size);
        }
    };
    // 集中竞价交易业务行情快照扩展字段
    TypeFieldArray<SecurityEntry> SecurityEntryArray;
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(MarketSnapshotBase_MemberList, NoMDEntries)
            + SecurityEntryArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                MarketSnapshotBase_MemberList, NoMDEntries)
            && SecurityEntryArray.load(&mem_addr, &mem_size,
                                        NoMDEntries.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 MarketSnapshotBase_MemberList, NoMDEntries)
            && SecurityEntryArray.write(&mem_addr, &mem_size);
    }
};


// 盘后定价交易业务行情快照 MarketSnapshot_300611
// 060 以收盘价交易的盘后定价交易快照行情
// 061 以成交量加权平均价交易的盘后定价交易快照行情
template <is_mutable b>
class MarketSnapshot_300611 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300611;
public:
    // Extend Fields 各业务扩展字段
    TypeNumInGroup   NoMDEntries;            // 行情条目个数
    struct SecurityEntry : public Field<b>
    {
        TypeString<2>           MDEntryType;     // 行情条目类别，0=买入；1=卖出
        TypeInt<int64_t>        MDEntryPx;       // 价格
        TypeQty                 MDEntrySize;     // 数量
        virtual uint32_t Size() const override
        {
            return byte_size_sum(MDEntryType, MDEntryPx, MDEntrySize);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size,
                                    MDEntryType, MDEntryPx, MDEntrySize);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size,
                                     MDEntryType, MDEntryPx, MDEntrySize);
        }
    };
    // 集中竞价交易业务行情快照扩展字段
    TypeFieldArray<SecurityEntry> SecurityEntryArray;     
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(MarketSnapshotBase_MemberList, NoMDEntries)
            + SecurityEntryArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                MarketSnapshotBase_MemberList, NoMDEntries)
            && SecurityEntryArray.load(&mem_addr, &mem_size,
                                        NoMDEntries.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 MarketSnapshotBase_MemberList, NoMDEntries)
            && SecurityEntryArray.write(&mem_addr, &mem_size);
    }
};

// 港股实时行情 MarketSnapshot_306311
// 630 港股实时行情
template <is_mutable b>
class MarketSnapshot_306311 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 306311;
public:
    // Extend Fields 各业务扩展字段
    TypeNumInGroup   NoMDEntries;            // 行情条目个数
    struct HKMarketEntry : public Field<b>
    {
        TypeString<2>           MDEntryType;     // 行情条目类别，0=买入；1=卖出
        TypeInt<int64_t>        MDEntryPx;       // 价格
        TypeQty                 MDEntrySize;     // 数量
        TypeInt<uint16_t>       MDPriceLevel;    // 买卖盘档位
        virtual uint32_t Size() const override
        {
            return byte_size_sum(MDEntryType, MDEntryPx,
                                 MDEntrySize, MDPriceLevel);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size,
                                    MDEntryType, MDEntryPx,
                                    MDEntrySize, MDPriceLevel);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size,
                                     MDEntryType, MDEntryPx,
                                     MDEntrySize, MDPriceLevel);
        }
    };
    TypeFieldArray<HKMarketEntry>   HKMarketEntryArray;

    TypeNumInGroup      NoComplexEventTimes;    // VCM冷静期个数
    struct ComplexEvent : public Field<b>
    {
        TypeLocalTimeStamp      ComplexEventStartTime;
        TypeLocalTimeStamp      ComplexEventEndTime;
        virtual uint32_t Size() const override
        {
            return byte_size_sum(ComplexEventStartTime, ComplexEventEndTime);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size,
                                    ComplexEventStartTime,
                                    ComplexEventEndTime);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size,
                                     ComplexEventStartTime,
                                     ComplexEventEndTime);
        }
    };
    TypeFieldArray<ComplexEvent>    ComplexEventArray;
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(MarketSnapshotBase_MemberList, NoMDEntries,
                             NoComplexEventTimes)
            + HKMarketEntryArray.Size() + ComplexEventArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                MarketSnapshotBase_MemberList, NoMDEntries)
            && HKMarketEntryArray.load(&mem_addr, &mem_size, 
                                        NoMDEntries.get_value())
            && load_from_memory(&mem_addr, &mem_size, NoComplexEventTimes)
            && ComplexEventArray.load(&mem_addr, &mem_size,
                                        NoComplexEventTimes.get_value());

    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                MarketSnapshotBase_MemberList, NoMDEntries)
            && HKMarketEntryArray.write(&mem_addr, &mem_size)
            && write_into_memory(&mem_addr, &mem_size, NoComplexEventTimes)
            && ComplexEventArray.write(&mem_addr, &mem_size);
    }
};

// 指数行情快照 MarketSnapshot_309011
// 900 指数快照行情
template <is_mutable b>
class MarketSnapshot_309011 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 309011;
public:
    // Extend Fields 各业务扩展字段
    TypeNumInGroup   NoMDEntries;            // 行情条目个数
    struct IndexEntry : public Field<b>
    {
        TypeString<2>           MDEntryType;     // 行情条目类别
        TypeInt<int64_t>        MDEntryPx;       // 指数点位
        virtual uint32_t Size() const override
        {
            return byte_size_sum(MDEntryType, MDEntryPx);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(&mem_addr, &mem_size, MDEntryType, MDEntryPx);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(&mem_addr, &mem_size, MDEntryType, MDEntryPx);
        }
    };
    TypeFieldArray<IndexEntry> IndexEntryArray;     // 集中竞价交易业务行情快照扩展字段
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(MarketSnapshotBase_MemberList, NoMDEntries)
            + IndexEntryArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                MarketSnapshotBase_MemberList, NoMDEntries)
            && IndexEntryArray.load(&mem_addr, &mem_size,
                                        NoMDEntries.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 MarketSnapshotBase_MemberList, NoMDEntries)
            && IndexEntryArray.write(&mem_addr, &mem_size);
    }
};

// 成交量统计指标行情快照 MarketSnapshot_309111
// 910 成交量统计指标快照行情
template <is_mutable b>
class MarketSnapshot_309111 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 309111;
public:
    // Extend Fields 各业务扩展字段
    TypeInt<uint32_t>   StockNum;               // 统计量指标样本个数
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(MarketSnapshotBase_MemberList, StockNum);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                MarketSnapshotBase_MemberList, StockNum);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 MarketSnapshotBase_MemberList, StockNum);
    }
};

// 逐笔委托行情的公共字段
template <is_mutable b>
class OrderSnapshotBase : public Field<b>
{
public:
    TypeInt<uint16_t>       ChannelNo;          // 频道代码
    TypeSeqNum              ApplSeqNum;         // 消息记录号，从 1 开始计数
    TypeString<3>           MDStreamID;         // 行情类别
    TypeSecurityID       SecurityID;         // 证券代码
    TypeString<4>           SecurityIDSource;   // 证券代码源
    TypePrice            Price;              // 委托价格
    TypeQty              OrderQty;           // 委托数量
    TypeString<1>           Side;               // 买卖方向：1=买；2 = 卖；G = 借入；F = 出借
    TypeLocalTimeStamp   OrderTime;          // 委托时间
    
#define OrderSnapshotBase_MemberList       \
ChannelNo, ApplSeqNum, MDStreamID, SecurityID, SecurityIDSource, Price, \
OrderQty, Side, OrderTime

};

// 集中竞价业务逐笔委托行情 OrderSnapshot_300192
// 011 现货（股票，基金，债券等）集中竞价交易逐笔行情
// 021 质押式回购交易逐笔行情
// 041 期权集中竞价交易逐笔行情
template <is_mutable b>
class OrderSnapshot_300192 : public OrderSnapshotBase<b>
{
    typedef OrderSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300192;
public:
    // Extend Fields 各业务扩展字段
    TypeString<2>   OrdType;               // 订单类别：1 = 市价；2 = 限价；U = 本方最优
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrderSnapshotBase_MemberList, OrdType);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                OrderSnapshotBase_MemberList, OrdType);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 OrderSnapshotBase_MemberList, OrdType);
    }
};

// 协议交易业务逐笔委托行情 OrderSnapshot_300592
// 051 协议交易逐笔意向行情
// 052 协议交易逐笔定价行情
template <is_mutable b>
class OrderSnapshot_300592 : public OrderSnapshotBase<b>
{
    typedef OrderSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300592;
public:
    // Extend Fields 各业务扩展字段
    TypeString<8>   ConfirmID;          // 定价行情约定号，ConfirmID 为空表示是意向行情，否则为定价行情
    TypeString<12>  Contactor;          // 联系人
    TypeString<30>  ContactInfo;        // 联系方式
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrderSnapshotBase_MemberList,
                             ConfirmID, Contactor, ContactInfo);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                OrderSnapshotBase_MemberList,
                                ConfirmID, Contactor, ContactInfo);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 OrderSnapshotBase_MemberList,
                                 ConfirmID, Contactor, ContactInfo);
    }
};

// 转融通证券出借业务逐笔委托行情 OrderSnapshot_300792
// 071 转融通证券出借逐笔行情
template <is_mutable b>
class OrderSnapshot_300792 : public OrderSnapshotBase<b>
{
    typedef OrderSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300792;
public:
    // Extend Fields 各业务扩展字段
    TypeInt<uint16_t>   ExpirationDays;               // 期限，单位为天数
    TypeInt<uint8_t>    ExpirationType;               // 期限类型，1 = 固定期限
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrderSnapshotBase_MemberList,
                             ExpirationDays, ExpirationType);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size,
                                OrderSnapshotBase_MemberList, 
                                ExpirationDays, ExpirationType);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size,
                                 OrderSnapshotBase_MemberList,
                                 ExpirationDays, ExpirationType);
    }
};

// 逐笔成交行情的公共字段
template <is_mutable b>
class TransactionSnapshotBase : public Field<b>
{
public:
    TypeInt<uint16_t>       ChannelNo;          // 频道代码
    TypeSeqNum              ApplSeqNum;         // 消息记录号，从 1 开始计数
    TypeString<3>           MDStreamID;         // 行情类别
    TypeSeqNum              BidApplSeqNum;      // 买方委托索引，从 1 开始计数， 0 表示无对应委托
    TypeSeqNum              OfferApplSeqNum;    // 卖方委托索引，从 1 开始计数， 0 表示无对应委托
    TypeSecurityID          SecurityID;         // 证券代码
    TypeString<4>           SecurityIDSource;   // 证券代码源
    TypePrice               LastPx;             // 委托价格
    TypeQty                 LastQty;            // 委托数量
    TypeString<1>           ExecType;           // 成交类别，4 = 撤销，F = 成交
    TypeLocalTimeStamp      TransactTime;       // 委托时间
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ChannelNo, ApplSeqNum, MDStreamID, BidApplSeqNum,
                             OfferApplSeqNum, SecurityID, SecurityIDSource, LastPx, LastQty,
                             ExecType, TransactTime);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(&mem_addr, &mem_size, ChannelNo, ApplSeqNum,
                                MDStreamID, BidApplSeqNum, OfferApplSeqNum, SecurityID,
                                SecurityIDSource, LastPx, LastQty, ExecType, TransactTime);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(&mem_addr, &mem_size, ChannelNo, ApplSeqNum,
                                 MDStreamID, BidApplSeqNum, OfferApplSeqNum, SecurityID,
                                 SecurityIDSource, LastPx, LastQty, ExecType, TransactTime);
    }
};

// 集中竞价业务逐笔成交行情 TransactionSnapshot_300191
// 011 现货（股票，基金，债券等）集中竞价交易逐笔行情
// 021 质押式回购交易逐笔行情
// 041 期权集中竞价交易逐笔行情
template <is_mutable b>
class TransactionSnapshot_300191 : public TransactionSnapshotBase<b>
{
    typedef TransactionSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300191;
public:
    // Extend Fields 各业务扩展字段
    // 无扩展字段
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
};

// 协议交易业务逐笔成交行情 TransactionSnapshot_300591
// 051 协议交易逐笔意向行情
// 052 协议交易逐笔定价行情
template <is_mutable b>
class TransactionSnapshot_300591 : public TransactionSnapshotBase<b>
{
    typedef TransactionSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300591;
public:
    // Extend Fields 各业务扩展字段
    // 无扩展字段
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
};

// 转融通证券出借业务逐笔成交行情 TransactionSnapshot_300791
// 071 转融通证券出借逐笔行情
template <is_mutable b>
class TransactionSnapshot_300791 : public TransactionSnapshotBase<b>
{
    typedef TransactionSnapshotBase<b> base_type;
public:
    static const uint32_t kMsgType = 300791;
public:
    // Extend Fields 各业务扩展字段
    // 无扩展字段
public:
    virtual uint32_t MsgType() const override { return kMsgType; }
};

namespace immutable_
{
typedef cn::szse::binary::Logon<false> Logon;
typedef cn::szse::binary::Logout<false> Logout;
typedef cn::szse::binary::Heartbeat<false> Heartbeat;
typedef cn::szse::binary::BusinessReject<false> BusinessReject;
typedef cn::szse::binary::ChannelHeartbeat<false> ChannelHeartbeat;
typedef cn::szse::binary::Announcement<false> Announcement;
typedef cn::szse::binary::ReTransmit<false> ReTransmit;
typedef cn::szse::binary::MarketStatus<false> MarketStatus;
typedef cn::szse::binary::SecurityStatus<false> SecurityStatus;
typedef cn::szse::binary::MarketSnapshotStatistic<false> MarketSnapshotStatistic;
typedef cn::szse::binary::MarketSnapshotBase<false> MarketSnapshotBase;
typedef cn::szse::binary::MarketSnapshot_300111<false> MarketSnapshot_300111;
typedef cn::szse::binary::MarketSnapshot_300611<false> MarketSnapshot_300611;
typedef cn::szse::binary::MarketSnapshot_306311<false> MarketSnapshot_306311;
typedef cn::szse::binary::MarketSnapshot_309011<false> MarketSnapshot_309011;
typedef cn::szse::binary::MarketSnapshot_309111<false> MarketSnapshot_309111;
typedef cn::szse::binary::OrderSnapshotBase<false> OrderSnapshotBase;
typedef cn::szse::binary::OrderSnapshot_300192<false> OrderSnapshot_300192;
typedef cn::szse::binary::OrderSnapshot_300592<false> OrderSnapshot_300592;
typedef cn::szse::binary::OrderSnapshot_300792<false> OrderSnapshot_300792;
typedef cn::szse::binary::TransactionSnapshotBase<false> TransactionSnapshotBase;
typedef cn::szse::binary::TransactionSnapshot_300191<false> TransactionSnapshot_300191;
typedef cn::szse::binary::TransactionSnapshot_300591<false> TransactionSnapshot_300591;
typedef cn::szse::binary::TransactionSnapshot_300791<false> TransactionSnapshot_300791;
} // namespace immutable_ END

namespace mutable_
{
typedef cn::szse::binary::Logon<true> Logon;
typedef cn::szse::binary::Logout<true> Logout;
typedef cn::szse::binary::Heartbeat<true> Heartbeat;
typedef cn::szse::binary::BusinessReject<true> BusinessReject;
typedef cn::szse::binary::ChannelHeartbeat<true> ChannelHeartbeat;
typedef cn::szse::binary::Announcement<true> Announcement;
typedef cn::szse::binary::ReTransmit<true> ReTransmit;
typedef cn::szse::binary::MarketStatus<true> MarketStatus;
typedef cn::szse::binary::SecurityStatus<true> SecurityStatus;
typedef cn::szse::binary::MarketSnapshotStatistic<true> MarketSnapshotStatistic;
typedef cn::szse::binary::MarketSnapshotBase<true> MarketSnapshotBase;
typedef cn::szse::binary::MarketSnapshot_300111<true> MarketSnapshot_300111;
typedef cn::szse::binary::MarketSnapshot_300611<true> MarketSnapshot_300611;
typedef cn::szse::binary::MarketSnapshot_306311<true> MarketSnapshot_306311;
typedef cn::szse::binary::MarketSnapshot_309011<true> MarketSnapshot_309011;
typedef cn::szse::binary::MarketSnapshot_309111<true> MarketSnapshot_309111;
typedef cn::szse::binary::OrderSnapshotBase<true> OrderSnapshotBase;
typedef cn::szse::binary::OrderSnapshot_300192<true> OrderSnapshot_300192;
typedef cn::szse::binary::OrderSnapshot_300592<true> OrderSnapshot_300592;
typedef cn::szse::binary::OrderSnapshot_300792<true> OrderSnapshot_300792;
typedef cn::szse::binary::TransactionSnapshotBase<true> TransactionSnapshotBase;
typedef cn::szse::binary::TransactionSnapshot_300191<true> TransactionSnapshot_300191;
typedef cn::szse::binary::TransactionSnapshot_300591<true> TransactionSnapshot_300591;
typedef cn::szse::binary::TransactionSnapshot_300791<true> TransactionSnapshot_300791;
} // namespace mutable_ END


} // namespace binary END
} // namespace szse END
} // namespace cn END

#endif // __CN_SZSE_BINARY_MD_FIELD_H__