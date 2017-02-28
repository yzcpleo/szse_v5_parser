# szse_v5_parser
深交所V5报文解析库

<p>目前实现了binary协议的行情数据解析</p>
<p>binary协议的交易数据可参考行情格式添加</p>

当前性能：
在本地环境：i7-6700@3.4GHz Win7 16GB内存，immutable_方式可达到1GB/s
（包括 Structure Packet，GetField）

两类报文
cn::szse::binary::immutable_::Packet ： 不可变报文
cn::szse::binary::mutable_::Packet ： 可变报文
前者只保存原始字节流的引用，因此是不可变的
后者会将原始自己流拷贝至报文内部，因此是可以修改的

结构化方法：
bool Structure(const char* mem_addr, size_t* mem_size)
输入：
    mem_addr即输入字节流
    mem_size即输入字节流的长度
输出：
    bool 结构化成功/失败
    mem_size 若成功，该值为消耗的字节长度；若失败，该值为需要的字节长度

数据域获取：
bool GetField(FieldType*)
输入：数据域对象
输入：是否成功

数据域也分为两种类型：可变和不可变，实现方式类似Packet
GetField函数可接收两种类型的数据域

数据域写入：
bool InsertField(FieldType*)
输入：待写入的数据域
输出：是否成功

要求：只能是将可变类型数据域写入到可变类型报文中

# 使用方法

<pre><code>
const char* mem_addr =  szse_binary_packet_stream;  // 接收到的报文字节流
size_t mem_size = szse_binary_packet_stream_size;   // 接收到的字节流尺寸

cn::szse::binary::immutable_::Packet packet;
if (packet.Structure(mem_addr, &mem_size))
{
    const cn::szse::binary::MsgHeader<false>* header = packet.GetHeader();
    switch (header->MsgType.get_value())
    {
        case cn::szse::binary::Logon<false>::TypeID:
        {
            cn::szse::binary::Logon<false> logon_field;
            if (packet.GetField(&logon_field))
            {
                // do something
            }
            {
                assert(false);
            }
            break;
        }
        default:
            break;
    }
}
</code></pre>