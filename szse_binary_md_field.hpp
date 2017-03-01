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

// @Class:   MsgHeader
// @Author:  cao.ning
// @Date:    2017/02/21
// @Brief:
template <is_mutable b>
class MsgHeader : public Field<b>
{
public:
    // 
    static size_t SSize()
    {
        static size_t ssize = TypeInt<uint32_t>::mem_size() * 2;
        return ssize;
    }
    virtual uint32_t Type() const { return 0; }
    virtual uint32_t Size() const { return SSize(); }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, MsgType, BodyLength);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, MsgType, BodyLength);
    }

    TypeInt<uint32_t> MsgType;      // 消息类型
    TypeInt<uint32_t> BodyLength;   // 消息体长度
};


// 登录消息 Logon
template <is_mutable b>
class Logon : public Field<b>
{
public:
    static const uint32_t TypeID = 1;

    TypeCompID          SenderCompID;           // 发送方代码
    TypeCompID          TargetCompID;           // 接收方代码
    TypeInt<int32_t>    HeartBtInt;             // 心跳间隔，单位是秒。 用户行情系统登陆时提供给行情网关。
    TypeString<16>      Password;               // 密码
    TypeString<32>      DefaultApplVerID;       // 二进制协议版本，填写为 n.xy （1.00）
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(SenderCompID, TargetCompID, HeartBtInt,
            Password, DefaultApplVerID);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, SenderCompID, TargetCompID,
            HeartBtInt, Password, DefaultApplVerID);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, SenderCompID, TargetCompID,
            HeartBtInt, Password, DefaultApplVerID);
    }
};

// 注销消息 Logout
template <is_mutable b>
class Logout : public Field<b>
{
public:
    static const uint32_t TypeID = 2;
    static const int32_t kSessionStatus_type_0 = 0;     //会话活跃
    static const int32_t kSessionStatus_type_1 = 1;     //会话口令已更改
    static const int32_t kSessionStatus_type_2 = 2;     //将过期的会话口令
    static const int32_t kSessionStatus_type_3 = 3;     //新会话口令不符合规范
    static const int32_t kSessionStatus_type_4 = 4;     //会话退登完成
    static const int32_t kSessionStatus_type_5 = 5;     //不合法的用户名或口令
    static const int32_t kSessionStatus_type_6 = 6;     //账户锁定
    static const int32_t kSessionStatus_type_7 = 7;     //当前时间不允许登录
    static const int32_t kSessionStatus_type_8 = 8;     //口令过期
    static const int32_t kSessionStatus_type_9 = 9;     //收到的 MsgSeqNum(34)太小
    static const int32_t kSessionStatus_type_10 = 10;    //收到的 NextExpectedMsgSeqNum(789)太大.
    static const int32_t kSessionStatus_type_101 = 101;   //其他
    static const int32_t kSessionStatus_type_102 = 102;   //无效消息

    TypeInt<int32_t>    SessionStatus;          // 退出时的会话状态
    TypeString<200>     Text;                   // 注销原因的进一步补充说明
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(SessionStatus, Text);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, SessionStatus, Text);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, SessionStatus, Text);
    }
};

// 心跳消息  Heartbeat
template <is_mutable b>
class Heartbeat : public Field<b>
{
public:
    static const uint32_t TypeID = 3;
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override { return 0; }
    virtual bool Load(const char*, size_t) override { return true; }
    virtual bool Write(char*, size_t) override { return true; }
};

// 业务拒绝消息 BusinessReject
template <is_mutable b>
class BusinessReject : public Field<b>
{
public:
    static const uint32_t TypeID = 8;
public:
    TypeSeqNum              RefSeqNum;              // 被拒绝消息的消息序号
    TypeInt<uint32_t>       RefMsgType;             // 被拒绝的消息类型
    TypeString<10>          BusinessRejectRefID;    // 被拒绝消息对应的业务层ID
    TypeInt<uint16_t>       BusinessRejectReason;   // 拒绝原因
    TypeString<50>          BusinessRejectText;     // 拒绝原因说明
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(RefSeqNum, RefMsgType, BusinessRejectRefID,
            BusinessRejectReason, BusinessRejectText);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, RefSeqNum, RefMsgType,
            BusinessRejectRefID, BusinessRejectReason, BusinessRejectText);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, RefSeqNum, RefMsgType,
            BusinessRejectRefID, BusinessRejectReason, BusinessRejectText);
    }
};

// 频道心跳消息  ChannelHeartbeat
template <is_mutable b>
class ChannelHeartbeat : public Field<b>
{
public:
    static const uint32_t TypeID = 390095;
public:
    TypeInt<uint16_t>    ChannelNo;          // 频道代码
    TypeSeqNum          ApplLastSeqNum;     // 最后一条行情消息的记录号
    TypeBoolean         EndOfChannel;       // 频道结束标志
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ChannelNo, ApplLastSeqNum, EndOfChannel);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size,
            ChannelNo, ApplLastSeqNum, EndOfChannel);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size,
            ChannelNo, ApplLastSeqNum, EndOfChannel);
    }
};

// 公告消息
template <is_mutable b>
class Announcement : public Field<b>
{
public:
    static const uint32_t TypeID = 390012;
public:
    TypeLocalTimeStamp      OrigTime;          // 公告时间
    TypeInt<uint16_t>       ChannelNo;         // 频道代码
    TypeString<8>           NewsID;            // 公告唯一标识
    TypeString<128>         Headline;          // 公告标题
    TypeString<8>           RawDataFormat;     // 二进制数据格式
    TypeLength              RawDataLength;     // 二进制数据长度
    const char*             RawData;           // 二进制数据（变长）
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, NewsID, Headline,
            RawDataFormat, RawDataLength)
            + RawDataLength.get_value();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (load_from_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            NewsID, Headline, RawDataFormat, RawDataLength))
        {
            RawData = static_cast<const char*>(mem_addr);
            return true;
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (write_into_memory(mem_addr, mem_size, OrigTime, ChannelNo,
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
    static const uint32_t TypeID = 390094;

    static const uint8_t kResendType_type_1 = 1;      //逐笔行情数据
    static const uint8_t kResendType_type_2 = 2;      //公告信息

    static const uint8_t kResendStatus_type_1 = 1;     //完成
    static const uint8_t kResendStatus_type_2 = 2;     //部分完成（有部分请求的数据没有返回）
    static const uint8_t kResendStatus_type_3 = 3;     //无权限
    static const uint8_t kResendStatus_type_4 = 4;     //数据不可用
public:
    TypeInt<uint8_t>    ResendType;         // 重传种类
    TypeInt<uint16_t>   ChannelNo;          // 频道代码
    TypeSeqNum          ApplBegSeqNum;      // 起始序号，当 ResendType=1 时生效，指定记录序号的范围
    TypeSeqNum          ApplEndSeqNum;      // 结束序号，当 ResendType=1 时生效，当 ApplEndSeqNum=0 时，
    // 行情网关会将 ApplEndSeqNum 设置为收到重传请求时，
    // 该频道数据在内存中的最大值
    TypeString<8>        NewsID;            // 公告唯一标识，当 ResendType=2 时生效，为空时表示申请公告概要
    TypeInt<uint8_t>     ResendStatus;      // 重传状态，仅在行情网关前置机返回给用户行情系统服务器时有效
    TypeString<16>       RejectText;        // 仅在行情网关前置机返回给用户行情系统服务器时有效
    // 如果请求被行情网关前置机拒绝，错误代码在这个域返回
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ResendType, ChannelNo, ApplBegSeqNum,
            ApplEndSeqNum, NewsID, ResendStatus, RejectText);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, ResendType, ChannelNo,
            ApplBegSeqNum, ApplEndSeqNum, NewsID, ResendStatus, RejectText);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, ResendType, ChannelNo,
            ApplBegSeqNum, ApplEndSeqNum, NewsID, ResendStatus, RejectText);
    }
};

// 市场实时状态 MarketStatus
template <is_mutable b>
class MarketStatus : public Field<b>
{
public:
    static const uint32_t TypeID = 390019;
public:
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
    TypeAmt                 PosAmt;              // 日中剩余额度，额度不可用时，发布固定值 0.0000
    TypeString<1>           AmountStatus;        // 额度状态
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, MarketID, MarketSegmentID,
            TradingSessionID, TradingSessionSubID, TradSesStatus,
            TradSesStartTime, TradSesEndTime, ThresholdAmount, PosAmt,
            AmountStatus);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            MarketID, MarketSegmentID, TradingSessionID, TradingSessionSubID,
            TradSesStatus, TradSesStartTime, TradSesEndTime, ThresholdAmount,
            PosAmt, AmountStatus);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            MarketID, MarketSegmentID, TradingSessionID, TradingSessionSubID,
            TradSesStatus, TradSesStartTime, TradSesEndTime, ThresholdAmount,
            PosAmt, AmountStatus);
    }
};

// 证券实时状态 SecurityStatus
template <is_mutable b>
class SecurityStatus : public Field<b>
{
public:
    static const uint32_t TypeID = 390013;
    static const uint16_t kSecuritySwitchType_type_1 = 1;      // 融资买入(适用于融资标的证券)
    static const uint16_t kSecuritySwitchType_type_2 = 2;      // 融券卖出(适用于融券标的证券)
    static const uint16_t kSecuritySwitchType_type_3 = 3;      // 申购(适用于 ETF， LOF 等开放式基金，对于黄金 ETF 是指现金申购)
    static const uint16_t kSecuritySwitchType_type_4 = 4;      // 赎回(适用于 ETF， LOF 等开放式基金，对于黄金 ETF 是指现金赎回开关)
    static const uint16_t kSecuritySwitchType_type_5 = 5;      // 认购(适用于网上发行认购代码)
    static const uint16_t kSecuritySwitchType_type_6 = 6;      // 转股(适用于处于转股回售期的可转债)
    static const uint16_t kSecuritySwitchType_type_7 = 7;      // 回售(适用于处于转股回售期的可转债)
    static const uint16_t kSecuritySwitchType_type_8 = 8;      // 行权(适用于处于行权期的权证或期权)
    static const uint16_t kSecuritySwitchType_type_10 = 10;      // 买开仓(适用于期权等衍生品)
    static const uint16_t kSecuritySwitchType_type_11 = 11;      // 卖开仓(适用于期权等衍生品)
    static const uint16_t kSecuritySwitchType_type_12 = 12;      // 黄金 ETF实物申(适用于黄金 ETF)
    static const uint16_t kSecuritySwitchType_type_13 = 13;      // 黄金ETF实物赎(适用于黄金 ETF)
    static const uint16_t kSecuritySwitchType_type_14 = 14;      // 预受要约(适用于处于要约收购期的股票)
    static const uint16_t kSecuritySwitchType_type_15 = 15;      // 解除要约(适用于处于要约收购期的股票)
    static const uint16_t kSecuritySwitchType_type_18 = 18;      // 转股撤单(适用于处于转股回售期的可转债)
    static const uint16_t kSecuritySwitchType_type_19 = 19;      // 回售撤单(适用于处于转股回售期的可转债)
    static const uint16_t kSecuritySwitchType_type_20 = 20;      // 质押(适用于质押式回购可质押入库证券)
    static const uint16_t kSecuritySwitchType_type_21 = 21;      // 解押(适用于质押式回购可质押入库证券)
    static const uint16_t kSecuritySwitchType_type_22 = 22;      // 表决权(适用于优先股)
    static const uint16_t kSecuritySwitchType_type_23 = 23;      // 股票质押式回(适用于可开展股票质押式回购业务的证券)
    static const uint16_t kSecuritySwitchType_type_24 = 24;      // 实时分拆(适用于分级基金)
    static const uint16_t kSecuritySwitchType_type_25 = 25;      // 实时合并(适用于分级基金)
    static const uint16_t kSecuritySwitchType_type_26 = 26;      // 备兑开仓(适用于期权等衍生品)
    static const uint16_t kSecuritySwitchType_type_27 = 27;      // 做市商报价(适用于期权等支持做市商报价的证券)
    static const uint16_t kSecuritySwitchType_type_28 = 28;      // 港股通整手买
    static const uint16_t kSecuritySwitchType_type_29 = 29;      // 港股通整手卖
    static const uint16_t kSecuritySwitchType_type_30 = 30;      // 港股通零股买
    static const uint16_t kSecuritySwitchType_type_31 = 31;      // 港股通零股卖
public:
    TypeLocalTimeStamp      OrigTime;          // 数据生成时间
    TypeInt<uint16_t>       ChannelNo;         // 频道代码
    TypeSecurityID          SecurityID;        // 证券代码
    TypeString<4>           SecurityIDSource;  // 证券代码源,102=深圳证券交易所,103=香港交易所
    TypeString<8>           FinancialStatus;   // 证券状态
    // A = 上市公司早间披露提示
    // B = 上市公司午间披露提示
    // 每个字节揭示一种状态，最多可同时揭示八种状态
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
            return load_from_memory(mem_addr, mem_size,
                SecuritySwitchType, SecuritySwitchStatus);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(mem_addr, mem_size,
                SecuritySwitchType, SecuritySwitchStatus);
        }
    };
    TypeFieldArray<SecuritySwitch>      SecuritySwitchArray; // 开关数组
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, SecurityID, SecurityIDSource,
            FinancialStatus, NoSwitch)
            + SecuritySwitchArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            SecurityID, SecurityIDSource, FinancialStatus, NoSwitch)
            && SecuritySwitchArray.Load(mem_addr, mem_size, NoSwitch.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            SecurityID, SecurityIDSource, FinancialStatus, NoSwitch)
            && SecuritySwitchArray.Write(mem_addr, mem_size);
    }
};

// 快照行情频道统计
template <is_mutable b>
class MarketSnapshotStatistic : public Field<b>
{
public:
    static const uint32_t TypeID = 390090;
    TypeLocalTimeStamp      OrigTime;           // 数据生成时间
    TypeInt<uint16_t>       ChannelNo;          // 频道代码
    TypeNumInGroup          NoMDStreamID;       // 行情类别个数
    struct StreamStatistic : public Field<b>
    {
        TypeString<3>           MDStreamID;         // 行情类别
        TypeInt<uint32_t>       StockNum;           // 证券只数
        TypeString<8>           TradingPhaseCode;   // 闭市状态，第 0 位：T=连续竞价；E=已闭市
        virtual uint32_t Size() const override
        {
            return byte_size_sum(MDStreamID, StockNum, TradingPhaseCode);
        }
        virtual bool Load(const char* mem_addr, size_t mem_size) override
        {
            return load_from_memory(mem_addr, mem_size, MDStreamID, StockNum,
                TradingPhaseCode);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(mem_addr, mem_size, MDStreamID, StockNum,
                TradingPhaseCode);
        }
    };
    TypeFieldArray<StreamStatistic>    StatisticArray; // 统计数组
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, NoMDStreamID)
            + StatisticArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            NoMDStreamID)
            && StatisticArray.Load(mem_addr, mem_size, NoMDStreamID.get_value());
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, OrigTime, ChannelNo,
            NoMDStreamID)
            && StatisticArray.Write(mem_addr, mem_size);
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
    // 010 现货（股票，基金，债券等）集中竞价交易快照行情
    // 020 质押式回购交易快照行情
    // 030 债券分销快照行情
    // 040 期权集中竞价交易快照行情
    // 060 以收盘价交易的盘后定价交易快照行情
    // 061 以成交量加权平均价交易的盘后定价交易快照行情
    // 900 指数快照行情
    // 910 成交量统计指标快照行情
    TypeSecurityID          SecurityID;         // 证券代码
    TypeString<4>           SecurityIDSource;   // 证券代码源
    TypeString<8>           TradingPhaseCode;   // 产品所处的交易阶段代码
    // 第 0 位：
    // S=启动（开市前）；O=开盘集合竞价；T=连续竞价；B=休市
    // C=收盘集合竞价；E=已闭市；H=临时停牌；A=盘后交易
    // V=波动性中断
    // 第 1 位：
    // 0=正常状态；1=全天停牌
    TypePrice               PrevClosePx;        // 昨收价
    TypeInt<int64_t>        NumTrades;          // 成交笔数
    TypeQty                 TotalVolumeTrade;   // 成交总量
    TypeAmt                 TotalValueTrade;    // 成交总金额
    virtual uint32_t Size() const override
    {
        return byte_size_sum(OrigTime, ChannelNo, MDStreamID, SecurityID,
            SecurityIDSource, TradingPhaseCode, PrevClosePx,
            NumTrades, TotalVolumeTrade, TotalValueTrade);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        mem_head_ = mem_tail_ = mem_addr;
        return load_from_memory(mem_tail_, mem_size, OrigTime,
            ChannelNo, MDStreamID, SecurityID, SecurityIDSource,
            TradingPhaseCode, PrevClosePx, NumTrades, TotalVolumeTrade,
            TotalValueTrade);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        mem_head_ = mem_tail_ = mem_addr;
        return write_into_memory((char*&)mem_tail_, mem_size, OrigTime, ChannelNo,
            MDStreamID, SecurityID, SecurityIDSource, TradingPhaseCode,
            PrevClosePx, NumTrades, TotalVolumeTrade, TotalValueTrade);
    }
protected:
    // 记录首部和尾部指针，在Load和Write时更新，为方便子类执行后续的Load和Write过程
    // 若在此之前执行了Load，则head和tail指向读取内存的首部和内存的尾部
    // 若在此之前执行了Write，则指向的是写入内存的首部和尾部
    const char* mem_head_;
    const char* mem_tail_;
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
    static const uint32_t TypeID = 300111;
public:
    // Extend Fields 业务扩展字段
    TypeNumInGroup          NoMDEntries;            // 行情条目个数
    struct SecurityEntry : public Field<b>
    {
        TypeString<2>           MDEntryType;     // 行情条目类别
        TypeInt<int64_t>        MDEntryPx;       // 价格，乘数1000000
        TypeQty                 MDEntrySize;     // 数量
        TypeInt<uint16_t>       MDPriceLevel;    // 买卖盘档位
        TypeInt<int64_t>        NumberOfOrders;  // 价位总委托笔数，为 0 表示不揭示
        TypeNumInGroup          NoOrders;        // 价位揭示委托笔数，为 0 表示不揭示
        struct OrderQty : public Field<b>
        {
            TypeQty             Qty;
            virtual uint32_t Size() const override { return TypeQty::mem_size(); }
            virtual bool Load(const char* mem_addr, size_t mem_size) override
            {
                return load_from_memory(mem_addr, mem_size, Qty);
            }
            virtual bool Write(char* mem_addr, size_t mem_size) override
            {
                return write_into_memory(mem_addr, mem_size, Qty);
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
            return load_from_memory(mem_addr, mem_size, MDEntryType,
                MDEntryPx, MDEntrySize, MDPriceLevel, NumberOfOrders,
                NoOrders)
                && OrderQtyArray.Load(mem_addr, mem_size, NoOrders.get_value());
        }
        bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(mem_addr, mem_size, MDEntryType,
                MDEntryPx, MDEntrySize, MDPriceLevel, NumberOfOrders,
                NoOrders) && OrderQtyArray.Write(mem_addr, mem_size);
        }
    };
    TypeFieldArray<SecurityEntry> SecurityEntryArray;     // 集中竞价交易业务行情快照扩展字段
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + NoMDEntries.mem_size() + SecurityEntryArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size, NoMDEntries)
                && SecurityEntryArray.Load(mem_addr, mem_size, NoMDEntries.get_value());
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, NoMDEntries)
                && SecurityEntryArray.Write(mem_addr, mem_size);
        }
        return false;
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
    static const uint32_t TypeID = 300611;
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
            return load_from_memory(mem_addr, mem_size,
                MDEntryType, MDEntryPx, MDEntrySize);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(mem_addr, mem_size,
                MDEntryType, MDEntryPx, MDEntrySize);
        }
    };
    TypeFieldArray<SecurityEntry> SecurityEntryArray;     // 集中竞价交易业务行情快照扩展字段
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + NoMDEntries.mem_size() + SecurityEntryArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size, NoMDEntries)
                && SecurityEntryArray.Load(mem_addr, mem_size, NoMDEntries.get_value());
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, NoMDEntries)
                && SecurityEntryArray.Write(mem_addr, mem_size);
        }
        return false;
    }
};

// 指数行情快照 MarketSnapshot_309011
// 900 指数快照行情
template <is_mutable b>
class MarketSnapshot_309011 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t TypeID = 309011;
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
            return load_from_memory(mem_addr, mem_size, MDEntryType, MDEntryPx);
        }
        virtual bool Write(char* mem_addr, size_t mem_size) override
        {
            return write_into_memory(mem_addr, mem_size, MDEntryType, MDEntryPx);
        }
    };
    TypeFieldArray<IndexEntry> IndexEntryArray;     // 集中竞价交易业务行情快照扩展字段
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + NoMDEntries.mem_size() + IndexEntryArray.Size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size, NoMDEntries)
                && IndexEntryArray.Load(mem_addr, mem_size, NoMDEntries.get_value());
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, NoMDEntries)
                && IndexEntryArray.Write(mem_addr, mem_size);
        }
        return false;
    }
};

// 成交量统计指标行情快照 MarketSnapshot_309111
// 910 成交量统计指标快照行情
template <is_mutable b>
class MarketSnapshot_309111 : public MarketSnapshotBase<b>
{
    typedef MarketSnapshotBase<b> base_type;
public:
    static const uint32_t TypeID = 309111;
public:
    // Extend Fields 各业务扩展字段
    TypeInt<uint32_t>   StockNum;               // 统计量指标样本个数
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + StockNum.mem_size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size, StockNum);
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, StockNum);
        }
        return false;
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
    // 011 现货（股票，基金，债券等）集中竞价交易逐笔行情
    // 021 质押式回购交易逐笔行情
    // 041 期权集中竞价交易逐笔行情
    // 051 协议交易逐笔意向行情
    // 052 协议交易逐笔定价行情
    // 071 转融通证券出借逐笔行情
    TypeSecurityID       SecurityID;         // 证券代码
    TypeString<4>           SecurityIDSource;   // 证券代码源
    TypePrice            Price;              // 委托价格
    TypeQty              OrderQty;           // 委托数量
    TypeString<1>           Side;               // 买卖方向：1=买；2 = 卖；G = 借入；F = 出借
    TypeLocalTimeStamp   OrderTime;          // 委托时间
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ChannelNo, ApplSeqNum, MDStreamID, SecurityID,
            SecurityIDSource, Price, OrderQty, Side, OrderTime);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        mem_head_ = mem_tail_ = mem_addr;
        return load_from_memory(mem_tail_, mem_size, ChannelNo, ApplSeqNum,
            MDStreamID, SecurityID, SecurityIDSource, Price,
            OrderQty, Side, OrderTime);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        mem_head_ = mem_tail_ = mem_addr;
        return write_into_memory((char*&)mem_tail_, mem_size, ChannelNo, ApplSeqNum,
            MDStreamID, SecurityID, SecurityIDSource, Price,
            OrderQty, Side, OrderTime);
    }
protected:
    const char* mem_head_;
    const char* mem_tail_;
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
    static const uint32_t TypeID = 300192;
public:
    // Extend Fields 各业务扩展字段
    TypeString<2>   OrdType;               // 订单类别：1 = 市价；2 = 限价；U = 本方最优
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + OrdType.mem_size();
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size, OrdType);
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, OrdType);
        }
        return false;
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
    static const uint32_t TypeID = 300592;
public:
    // Extend Fields 各业务扩展字段
    TypeString<8>   ConfirmID;          // 定价行情约定号，ConfirmID 为空表示是意向行情，否则为定价行情
    TypeString<12>  Contactor;          // 联系人
    TypeString<30>  ContactInfo;        // 联系方式
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + byte_size_sum(ConfirmID, Contactor, ContactInfo);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size,
                ConfirmID, Contactor, ContactInfo);
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, ConfirmID, Contactor, ContactInfo);
        }
        return false;
    }
};

// 转融通证券出借业务逐笔委托行情 OrderSnapshot_300792
// 071 转融通证券出借逐笔行情
template <is_mutable b>
class OrderSnapshot_300792 : public OrderSnapshotBase<b>
{
    typedef OrderSnapshotBase<b> base_type;
public:
    static const uint32_t TypeID = 300792;
public:
    // Extend Fields 各业务扩展字段
    TypeInt<uint16_t>   ExpirationDays;               // 期限，单位为天数
    TypeInt<uint8_t>    ExpirationType;               // 期限类型，1 = 固定期限
public:
    virtual uint32_t Type() const override { return TypeID; }
    virtual uint32_t Size() const override
    {
        return base_type::Size() + byte_size_sum(ExpirationDays, ExpirationType);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        if (base_type::Load(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return load_from_memory(mem_addr, mem_size, ExpirationDays, ExpirationType);
        }
        return false;
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        if (base_type::Write(mem_addr, mem_size))
        {
            size_t base_type_size = base_type::mem_tail_ - base_type::mem_head_;
            mem_addr += base_type_size;
            mem_size -= base_type_size;
            return write_into_memory(mem_addr, mem_size, ExpirationDays, ExpirationType);
        }
        return false;
    }
};

// 逐笔成交行情的公共字段
template <is_mutable b>
class TransactionSnapshotBase : public Field<b>
{
public:
    TypeInt<uint16_t>       ChannelNo;          // 频道代码
    TypeSeqNum           ApplSeqNum;         // 消息记录号，从 1 开始计数
    TypeString<3>           MDStreamID;         // 行情类别
    // 011 现货（股票，基金，债券等）集中竞价交易逐笔行情
    // 021 质押式回购交易逐笔行情
    // 041 期权集中竞价交易逐笔行情
    // 051 协议交易逐笔意向行情
    // 052 协议交易逐笔定价行情
    // 071 转融通证券出借逐笔行情
    TypeSeqNum           BidApplSeqNum;      // 买方委托索引，从 1 开始计数， 0 表示无对应委托
    TypeSeqNum           OfferApplSeqNum;    // 卖方委托索引，从 1 开始计数， 0 表示无对应委托
    TypeSecurityID       SecurityID;         // 证券代码
    TypeString<4>           SecurityIDSource;   // 证券代码源
    TypePrice            LastPx;             // 委托价格
    TypeQty              LastQty;            // 委托数量
    TypeString<1>           ExecType;           // 成交类别，4 = 撤销，F = 成交
    TypeLocalTimeStamp   TransactTime;       // 委托时间
    virtual uint32_t Size() const override
    {
        return byte_size_sum(ChannelNo, ApplSeqNum, MDStreamID, BidApplSeqNum,
            OfferApplSeqNum, SecurityID, SecurityIDSource, LastPx, LastQty,
            ExecType, TransactTime);
    }
    virtual bool Load(const char* mem_addr, size_t mem_size) override
    {
        return load_from_memory(mem_addr, mem_size, ChannelNo, ApplSeqNum,
            MDStreamID, BidApplSeqNum, OfferApplSeqNum, SecurityID,
            SecurityIDSource, LastPx, LastQty, ExecType, TransactTime);
    }
    virtual bool Write(char* mem_addr, size_t mem_size) override
    {
        return write_into_memory(mem_addr, mem_size, ChannelNo, ApplSeqNum,
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
    static const uint32_t TypeID = 300191;
public:
    // Extend Fields 各业务扩展字段
    // 无扩展字段
public:
    virtual uint32_t Type() const override { return TypeID; }
};

// 协议交易业务逐笔成交行情 TransactionSnapshot_300591
// 051 协议交易逐笔意向行情
// 052 协议交易逐笔定价行情
template <is_mutable b>
class TransactionSnapshot_300591 : public TransactionSnapshotBase<b>
{
    typedef TransactionSnapshotBase<b> base_type;
public:
    static const uint32_t TypeID = 300591;
public:
    // Extend Fields 各业务扩展字段
    // 无扩展字段
public:
    virtual uint32_t Type() const override { return TypeID; }
};

// 转融通证券出借业务逐笔成交行情 TransactionSnapshot_300791
// 071 转融通证券出借逐笔行情
template <is_mutable b>
class TransactionSnapshot_300791 : public TransactionSnapshotBase<b>
{
    typedef TransactionSnapshotBase<b> base_type;
public:
    static const uint32_t TypeID = 300791;
public:
    // Extend Fields 各业务扩展字段
    // 无扩展字段
public:
    virtual uint32_t Type() const override { return TypeID; }
};

namespace immutable_
{
typedef cn::szse::binary::MsgHeader<false> MsgHeader;
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
typedef cn::szse::binary::MsgHeader<true> MsgHeader;
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