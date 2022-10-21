/**
 * @file data_struct_def.h
 * @brief  this head file define the data struct and MARCOS using in lorastack
 * @author Fan Chuanlin, fanchuanlin@aliyun.com
 * @version 1.0
 * @date 2021-01-30
 * @attention
 * @brief History:
 * <table>
 * <tr><th> Date <th> Version <th> Author <th> Description
 * <tr><td> xxx-xx-xx <td> 1.0 <td> Fan Chuanlin <td> Create
 * </table>
 *
 */

#ifndef _DATA_STRUCT_DEF_H_
#define _DATA_STRUCT_DEF_H_
#if defined(_WIN32)
#include "../inc/normal_typedef.h"
#else
#include "../inc/normal_typedef.h"
#endif
#include <cstdint>

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Antenna Cmds Start~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~///
#define FRAME_FIELD_NUM_MAX 20
#define FRAME_FIELD_NAME_MAX_LEN 30
#define FRAME_FIELD_VALUE_MAX_LEN 8192

/**
 * @enum
 * @brief for network state
 */
typedef enum
{
    NETWORK_STATE_INIT = 0,       ///< 网络状态初始化
    NETWORK_STATE_CONNECTING = 1, ///< 连接
    NETWORK_STATE_DISCONNECT = 2, ///< 断开连接
    NETWORK_STATE_RUNNING = 3,    ///< 运行
    NETWORK_STATE_MAX
} NETWORK_STATE_E;

const char network_state_str[NETWORK_STATE_MAX][30] =
    {
        " NETWORK_STATE_INIT",
        " NETWORK_STATE_CONNECTING",
        " NETWORK_STATE_DISCONNECT",
        " NETWORK_STATE_RUNNING",
};

/**
 * @enum
 * @brief for timeout type
 */
typedef enum
{
    TIMEOUT_TYPE_TX = 0,
    TIMEOUT_TYPE_RX = 1,
    TIMEOUT_TYPE_STATE = 2,
    TIMEOUT_TYPE_LINK = 3,
    TIMEOUT_TYPE_MAX
} TIMEOUT_TYPE_E;

const char timeout_type_str[][30] =
    {
        "TIMEOUT_TYPE_TX ",
        "TIMEOUT_TYPE_RX ",
        "TIMEOUT_TYPE_STATE ",
        "TIMEOUT_TYPE_LINK ",
};
/**
 * @enum BYTEORDER
 * @brief 字端大小端
 */
typedef enum
{
    BIG_ENDIAN1 = 0,   //大端字序  网络字序
    LITTLE_ENDIAN1 = 1 //小端字序
} BYTEORDER;
/**
 * @enum OPERATION_XPROTOCOL
 * @brief 加减乘除操作
 */
typedef enum
{
    OPERA_NULL = 0,           ///<无操作
    OPERA_ADD_INT = 1,        ///<例如{"OPERA_ADD":10}表示加10  针对整数
    OPERA_SUBTRACT_INT = 2,   ///<例如{"OPERA_SUBTRACT":10}表示减10 针对整数
    OPERA_MULTIPLY_INT = 3,   ///<例如{"OPERA_MULTIPLY":10}表示乘10 针对整数
    OPERA_DIVIDE_INT = 4,     ///<例如{"OPERA_DIVIDE":10}表示除10 针对整数
    OPERA_MULTIPLY_FLOAT = 5, ///<例如{"OPERA_MULTIPLY":10}表示乘10 针对浮点数
    OPERA_DIVIDE_FLOAT = 6,   ///<例如{"OPERA_DIVIDE":10}表示除10 针对浮点数
    OPERA_ADD_FLOAT = 7,      ///<例如{"OPERA_ADD":10.0}表示加10.0  针对浮点数
    OPERA_SUBTRACT_FLOAT = 8  ///<例如{"OPERA_SUBTRACT":10.0}表示减10.0 针对浮点数
} OPERATION_XPROTOCOL;
/**
 * @enum OPERA_BYTEORDER
 * @brief 字节序控制
 */
typedef enum
{
    OPERA_BO_NULL = 0,    ///<不做操作
    OPERA_BO_AB = 1,      ///<单纯凑数
    OPERA_BO_BA = 2,      ///< 2个字节逆序
    OPERA_BO_CBA = 3,     ///< 3个字节逆序
    OPERA_BO_DCBA = 4,    ///< 4个字节逆序
    OPERA_BO_EDCBA = 5,   ///< 5个字节逆序
    OPERA_BO_FEDCBA = 6,  ///< 6个字节逆序
    OPERA_BO_GFEDCBA = 7, ///< 7个字节逆序
    OPERA_BO_HGFEDCBA = 8 ///< 8个字节逆序
} OPERA_BYTEORDER;
/**
 *@struct packet_field
 * @brief 对应json文件中健值对的字段，将JSON文件中各个字段的基本信息解析出来后存放到本结构体中
 *
 */
typedef struct packet_field
{
    UINT8 fieldIndex;                         ///< 字段序列号
    CHAR fieldName[FRAME_FIELD_NAME_MAX_LEN]; ///< 字段名称
    UINT32 fieldSize;                         ///< 字段大小
    CHAR value[FRAME_FIELD_VALUE_MAX_LEN];    ///< 字段的值
    CHAR minValue[FRAME_FIELD_VALUE_MAX_LEN]; ///< 最大值
    CHAR maxValue[FRAME_FIELD_VALUE_MAX_LEN]; ///< 最小值
    UINT32 offset;                            ///< 字段的偏移
    UINT32 dataType;                          ///< 字段的数据类型
    CHAR Lua_Script_in[1024];                 ///< Lua脚本名
    CHAR Lua_Script_out[1024];                ///< Lua脚本名
    UINT32 operation_in;                      ///<对于从设备读取到的数据需要进行的加减乘除的处理
    UINT32 operation_in_num;                  ///>对于从设备读取到的数据需要进行的加减乘除的处理的参数
    UINT32 operation_out;                     ///<对于从上层传下来的数据需要进行的加减乘除的处理
    UINT32 operation_out_num;                 ///>对于从上层传下来的数据需要进行的加减乘除的处理的参数
    UINT32 operaBO_in;                        ///>细节的字节序控制 类似modbus{ AB\BA\ABCD\DCBA等}
    UINT32 operaBO_out;                       ///>细节的字节序控制

    packet_field() ///< 结构体构造函数，复制默认的初始化为0
    {
        memset(fieldName, 0, FRAME_FIELD_NAME_MAX_LEN);
        memset(value, 0, FRAME_FIELD_VALUE_MAX_LEN);
        memset(minValue, 0, FRAME_FIELD_VALUE_MAX_LEN);
        memset(maxValue, 0, FRAME_FIELD_VALUE_MAX_LEN);
        fieldIndex = 0;
        fieldSize = 0;
        offset = 0;
        dataType = 0;
        memset(Lua_Script_in, 0, 1024);
        memset(Lua_Script_out, 0, 1024);
        operation_in = 0;
        operation_in_num = 0;
        operation_out = 0;
        operation_out_num = 0;
        operaBO_in = 0;
        operaBO_out = 0;
    }
} packet_field_s;

/**@enum DATATYPE_E
 * @brief 定义报文中不同字段的数据类型
 */
typedef enum
{
    DATATYPE_DEFAULT = 0,   ///< 默认数据类型。不做任何转换处理。直接copy
    DATATYPE_STR = 1,       ///< string类型数据。不做任何转换处理。直接copy
    DATATYPE_UINT_STR = 2,  ///< UINT_string类型数据。目前暂时没有遇到这种类型的数据，函数UpdatePacketField中没有定义对应的处理
    DATATYPE_INT_STR = 3,   ///< UINT_string类型数据。目前暂时没有遇到这种类型的数据，函数UpdatePacketField中没有定义对应的处理
    DATATYPE_FLOAT_STR = 4, ///< FLOAT_string类型数据。目前暂时没有遇到这种类型的数据，函数UpdatePacketField中没有定义对应的处理
    DATATYPE_END = 5,       ///< 协议的结束符，有的字符串协议比如end结束
    DATATYPE_HEX = 6,       ///< hex数据，16进制
    DATATYPE_HEX_STR = 7,   ///< hex数据，16进制
    DATATYPE_DOUBLE = 8,    ///< DOUBLE 数据
    DATATYPE_PAYLOAD = 9,   ///< 不定长的数据，一个协议报中只能有一个不定长的数据
    DATATYPE_INT8 = 10,     ///< INT8 类型数据
    DATATYPE_TIMESTR = 11,  ///< TIMESTR类型数据。不做任何转换处理。直接copy
    DATATYPE_MAX
} DATATYPE_E;

const char datatype_str[DATATYPE_MAX][30] =
    {
        " DATATYPE_DEFAULT ",
        " DATATYPE_STR ",
        " DATATYPE_UINT_STR ",
        " DATATYPE_INT_STR ",
        " DATATYPE_FLOAT_STR ",
        " DATATYPE_END ",
        " DATATYPE_HEX ",
        " DATATYPE_HEX_STR ",
        " DATATYPE_DOUBLE ",
        " DATATYPE_PAYLOAD ",
        " DATATYPE_INT8 ",
        " DATATYPE_TIMESTR ",
};

#define PACKET_TYPE_NUM 16

///  macro for buffer size of one packet type
#define PACKET_BUFF_SIZE 5

///  macro for max packet len
#define PACKET_MAX_LEN 9600

/**
 * @brief for store packet, the most size is PACKET_BUFF_ZIE
 * @{ */
/**  @} */
typedef struct packet_buffer
{
    UINT8 isover;               ///<  set for refresh flag
    UINT32 size;                ///<  packet size
    UINT8 buff[PACKET_MAX_LEN]; ///<  packet buffer
} packet_buffer_s;

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Antenna Cmds end~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~///
#endif
