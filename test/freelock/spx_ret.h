#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spx_ctype.h"
#include "spx_mfunc.h"

/*
 * spx errno setting
 * howto :   if you donot found the errno in enum spx_ret_t,
 you can add the errno by youself in spx_ret_t by the mechanism：
 1. add your errno in the spx_ret_t,and the added-errno must is less 0 and one by one.
 2. you must add the errno-message(for added-errno) in the spx_ret_info,
 3. make sure  the function spx_ret_msg return the errno-message.
 */

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        SpxRet_BindCpuFail = -8,
        SpxRet_SpinLockFree = -7,
        SpxRet_SpinkLockBusy = -6 ,
        SpxRet_FREELOCK = -5,
        SpxRet_PathFormatError = -4, /* path name format is error */
        SpxRet_FilenameError = -3,   /* filename is error,maybe no suffix */
        SpxRet_NotFoundPattern = -2, /* not found pattern in source 匹配失败 */
        SpxRet_IndexOutOfCount = -1, /* index out of count in array ,map,set */
        SpxRet_OK = 0,
        SpxRet_OperationNotPermitted = 1,   /* Operation not permitted 操作不允许*/
        SpxRet_FileOrFolderNotExist = 2,    /* No such file or directory 文件或者目录不存在*/
        SpxRet_ProcessNotExist = 3,         /* No such process 进程不存在*/
        SpxRet_SystemCallInterrupted = 4,   /* Interrupted system call 被中断的系统调用*/
        SpxRet_IOError = 5,                 /* I/O error IO错误*/
        SpxRet_DeviceOrAddressNotExist = 6, /* No such device or address 设备或地址不存在*/
        SpxRet_ArgumentListTooLong = 7,     /* Argument list too long 参数列表太长*/
        SpxRet_FormatError = 8,             /* Exec format error 执行格式错误*/
        SpxRet_FileNumberError = 9,         /* Bad file number 错误的文件编号*/
        SpxRet_ChildProcessNotExist = 10,   /* No child processes 没有子进程*/
        SpxRet_TryAgain = 11,               /* Try again 重试*/
        SpxRet_OutOfMemory = 12,            /* Out of memory 内存不足*/
        SpxRet_NoPermissions = 13,          /* Permission denied 无权限*/
        SpxRet_AddressBad = 14,             /* Bad address 错误的地址*/
        SpxRet_BlockDeviceRequired = 15,    /* Block device required 需要块设备*/
        SpxRet_ResourceIsBusy = 16,         /* Device or resource busy 设备或资源正忙*/
        SpxRet_FileIsExist = 17,            /* File exists 文件已经存在*/
        SpxRet_CrossLink = 18,              /* Cross-device link 跨设备连接*/
        SpxRet_DeviceNotExist = 19,         /* No such device 设备不存在*/
        SpxRet_NotFolder = 20,              /* Not a directory 不是路径*/
        SpxRet_IsFolder = 21,               /* Is a directory 是路径*/
        SpxRet_ArgumentError = 22,          /* Invalid argument 无效参数*/
        SpxRet_FileTableOverflow = 23,      /* File table overflow 文件表溢出*/
        SpxRet_TooManyOpenFiles = 24,       /* Too many open files 打开的文件过多*/
        SpxRet_NotTTY = 25,
        SpxRet_TextFileBusy = 26,                        /* Text file busy 文本文件忙*/
        SpxRet_FileTooLarge = 27,                        /* File too large 文件太大*/
        SpxRet_FreeSpaceIsTooSmall = 28,                 /* No space left on device 设备已经没有空间了*/
        SpxRet_IllegalRandomAccess = 29,                 /* Illegal seek 不合法的随机访问*/
        SpxRet_FileSystemIsReadonly = 30,                /* Read-only file system 只读文件系统*/
        SpxRet_TooManyLinks = 31,                        /* Too many links 太多链接*/
        SpxRet_PipeIsBroken = 32,                        /* Broken pipe 断开的管道*/
        SpxRet_ArgumentOutOfScope = 33,                  /* Math argument out of domain of func 参数超出函数域*/
        SpxRet_ResultNotShow = 34,                       /* Math result not representable 结果无法表示*/
        SpxRet_Deadlock = 35,                            /* Resource deadlock would occur 资源死锁*/
        SpxRet_FilenameTooLong = 36,                     /* File name too long 文件名太长*/
        SpxRet_LocksBusy = 37,                           /* No record locks available 没有可用的记录锁*/
        SpxRet_FunctionNotImplemented = 38,              /* Function not implemented 函数未实现*/
        SpxRet_FolderNotEmpty = 39,                      /* Directory not empty 目录不为空*/
        SpxRet_TooManySymbolicLinks = 40,                /* Too many symbolic links encountered 太多的符号链接*/
        SpxRet_WouldBlock = SpxRet_TryAgain,             /* Operation would block 操作会被阻塞*/
        SpxRet_NoMessage = 42,                           /* No message of desired type 没有符合需求类型的消息*/
        SpxRet_IdIsremoved = 43,                         /* Identifier removed 标志被移除*/
        SpxRet_ChannelNumberOutOfRange = 44,             /* Channel number out of range 通道编号超出范围*/
        SpxRet_Level2NotSynchronized = 45,               /* Level 2 not synchronized */
        SpxRet_Level3Halted = 46,                        /* Level 3 halted */
        SpxRet_Level3Reset = 47,                         /* Level 3 reset */
        SpxRet_LinkNumberOutOfRange = 48,                /* Link number out of range 链接编号超出限制*/
        SpxRet_ProtocolNotAttached = 49,                 /* Protocol driver not attached 协议驱动没有连接*/
        SpxRet_NoCSIStructure = 50,                      /* No CSI structure available 没有可用的CSI结构*/
        SpxRet_Level2Halted = 51,                        /* Level 2 halted */
        SpxRet_ExchangeBad = 52,                         /* Invalid exchange 无效交换*/
        SpxRet_RequestBad = 53,                          /* Invalid request descriptor 无效的请求描述*/
        SpxRet_ExchangeFull = 54,                        /* Exchange full 交换完全*/
        SpxRet_NoAnode = 55,                             /* No anode 无阳极*/
        SpxRet_RequestCodeBad = 56,                      /* Invalid request code 不合法的请求码*/
        SpxRet_SlotIsInvalid = 57,                       /* Invalid slot 无效插槽*/
        SpxRet_EDeadlock = SpxRet_Deadlock,              /*资源死锁*/
        SpxRet_FileFormatBad = 59,                       /* Bad font file format 不合法的字体文件格式*/
        SpxRet_NotStream = 60,                           /* Device not a stream 设备不是流媒体*/
        SpxRet_NoDataAvailable = 61,                     /* No data available 无数据可用*/
        SpxRet_TimerExpired = 62,                        /* Timer expired 计时器到期*/
        SpxRet_StreamResourceNotEnough = 63,             /* Out of streams resources 流资源不足*/
        SpxRet_HostNotOnline = 64,                       /* Machine is not on the network 机器不在网络上*/
        SpxRet_PkgNotInstalled = 65,                     /* Package not installed 包未安装*/
        SpxRet_ObjectIsRemote = 66,                      /* Object is remote 远端对象*/
        SpxRet_LinkBusy = 67,                            /* Link has been severed 连接正在服务中*/
        SpxRet_AdvertiseError = 68,                      /* Advertise error 广告错误*/
        SpxRet_SrmountError = 69,                        /* Srmount error */
        SpxRet_CommunicationErrorOnSend = 70,            /* Communication error on send 发送过程中通讯异常*/
        SpxRet_ProtocolError = 71,                       /* Protocol error 协议错误*/
        SpxRet_MultihopAttempted = 72,                   /* Multihop attempted 多跳尝试*/
        SpxRet_RFSError = 73,                            /* RFS specific error RFS特定错误*/
        SpxRet_MessageBad = 74,                          /* Not a data message 不是数据消息*/
        SpxRet_ValueOverflowForDateType = 75,            /* Value too large for defined data type 对于指定的数据类型来说值太大了*/
        SpxRet_NameNotUniqueOnNetwork = 76,              /* Name not unique on network 网络上名字不唯一*/
        SpxRet_BadFileDescriptor = 77,                   /* File descriptor in bad state 文件描述符状态错误*/
        SpxRet_RemoteAddressChanged = 78,                /* Remote address changed 远端的地址已经改变*/
        SpxRet_AccessSharedLibraryUnable = 79,           /* Can not access a needed shared library 无法访问所需要的共享库*/
        SpxRet_SharedLibraryFailure = 80,                /* Accessing a corrupted shared library 访问了损坏的共享库*/
        SpxRet_LibSectionFail = 81,                      /* .lib section in a.out corrupted 库部分在a.out被损坏*/
        SpxRet_LinkTooManySharedLibraries = 82,          /* Attempting to link in too many shared libraries 试图连接过多的共享库*/
        SpxRet_UnableExecSharedLibrary = 83,             /* Cannot exec a shared library directly 不能直接运行共享库*/
        SpxRet_ByteSequenceBad = 84,                     /* Illegal byte sequence 非法的字节序*/
        SpxRet_SystemCallRestartedWhenInterrupted = 85,  /* Interrupted system call should be restarted 因重启被中断的系统调用*/
        SpxRet_StreamsPipeError = 86,                    /* Streams pipe error 流管道错误*/
        SpxRet_TooManyUsers = 87,                        /* Too many users 太多用户*/
        SpxRet_BadSocket = 88,                           /* Socket operation on non-socket 非套接字上进行套接字操作，操作了没有建立成功的套接字*/
        SpxRet_BadDestinationAddress = 89,               /* Destination address required 需要指定目的地址*/
        SpxRet_MessageTooLong = 90,                      /* Message too long 信息太长*/
        SpxRet_BadProtocol = 91,                         /* Protocol wrong type for socket 协议错误*/
        SpxRet_ProtocolUnable = 92,                      /* Protocol not available 协议不可用*/
        SpxRet_ProtocolNotSupported = 93,                /* Protocol not supported 协议不支持*/
        SpxRet_SocketTypeNotSupported = 94,              /* Socket type not supported 套接字类型不支持*/
        SpxRet_OperationNotSupportedOnEndpoint = 95,     /* Operation not supported on transport endpoint 不支持操作的传输端点*/
        SpxRet_ProtocolFamilyNotSupported = 96,          /* Protocol family not supported 协议蔟未被支持*/
        SpxRet_AddressFamilyNotSupportedByProtocol = 97, /* Address family not supported by protocol 当前协议蔟不支持地址群*/
        SpxRet_AddressInUse = 98,                        /* Address already in use 地址已被占用*/
        SpxRet_RequestedAddressUnableAssign = 99,        /* Cannot assign requested address 不能分配请求的地址*/
        SpxRet_NetworkIsDown = 100,                      /* Network is down 网络已经关闭*/
        SpxRet_NetworkIsUnreachable = 101,               /* Network is unreachable 网络不可达*/
        SpxRet_DropConnectionByReset = 102,              /* Network dropped connection because of reset 由于网络复位，网络断开连接*/
        SpxRet_ConnectionAbortBySoftware = 103,          /* Software caused connection abort 软件导致的连接失败*/
        SpxRet_ConnectionResetByPeer = 104,              /* Connection reset by peer 连接被对方复位，一般出现在接受请求失败的情况，与EPIPE类似*/
        SpxRet_BufferSpaceAllBusy = 105,                 /* No buffer space available 缓冲区空间耗尽*/
        SpxRet_TransportEndpointAlreadyConnected = 106,  /* Transport endpoint is already connected 传输端点已连接*/
        SpxRet_TransportEndpointNotConnected = 107,      /* Transport endpoint is not connected 传输端点未连接*/
        SpxRet_TransportEndpointShutdown = 108,          /* Cannot send after transport endpoint shutdown 传输端点关闭后不能再发送数据*/
        SpxRet_ReferencesTooMany = 109,                  /* Too many references: cannot splice 太多的引用：无法接合*/
        SpxRet_ConnectionTimeout = 110,                  /* Connection timed out 连接超时*/
        SpxRet_ConnectionRefused = 111,                  /* Connection refused 连接被拒绝*/
        SpxRet_HostIsDown = 112,                         /* Host is down 主机已经关闭*/
        SpxRet_NoRouteToHost = 113,                      /* No route to host 无法路由到主机*/
        SpxRet_OperationRuned = 114,                     /* Operation already in progress 操作已经在运行了*/
        SpxRet_OperationIsRunning = 115,                 /* Operation now in progress 操作在运行*/
        SpxRet_NFSFileHandlerError = 116,                /* Stale NFS file handle 网络文件系统句柄错误*/
        SpxRet_StructureNeedClean = 117,                 /* Structure needs cleaning  结构体需要清空*/
        SpxRet_NotXENIXNameTypeFile = 118,               /* Not a XENIX named type file 不是一个XENIX命名类型文件*/
        SpxRet_NoXENIXSemaphoresUnable = 119,            /* No XENIX semaphores available */
        SpxRet_UnkownFileType = 120,                     /* Is a named type file 不是一个可识别类型文件 */
        SpxRet_KeyNotExist = 126,                        /* Required key not available key不存在*/
        SpxRet_KeyHasExpired = 127,                      /* Key has expired key已经过期*/
        SpxRet_KeyHasRemoved = 128,                      /* Key has been revoked key已经被废除*/
        SpxRet_KeyRejectedByService = 129,               /* Key was rejected by service  key已经被server拒绝*/
        SpxRet_OwnerNotExist = 130,                      /* Owner died 拥有者不存在*/
        SpxRet_StateNotRecoverable = 131,                /* State not recoverable 状态不可恢复*/
        SpxRet_OperationUndoneByRFkill = 132,            /* Operation not possible due to RF-kill 由于RFKILL，操作无法完成*/
        SpxRet_MemoryPageError = 133,                    /* Memory page has hardware error 物理内存页错误*/
    } spx_ret_t;

#define SpxRetRetry(ret) ((SpxRet_TryAgin == (ret)) || (SpxRet_WouldBlock == (ret)) || (SpxRet_SystemCallInterrupted == (ret)))

    __SpxPrivate__ const char *spx_ret_info[] = {
        "success.",
        "Index out of collect.",
        "not found pattern in source.",
        "free locker is busy.",
        "spin lock is busy.",

    };

    __SpxPrivate__ const char *spx_ret_msg(spx_ret_t ret) {
        if (0 < ret) {
            return strerror(ret);
        }

        return spx_ret_info[ SpxAbs(ret) ];
    }

    /*
     *   * X Macro() data list
     *    * Format: Enum, Value, Text
     */
#define X_ERROR                          \
    X(ERROR_NONE, 1, "Success")          \
    X(ERROR_SYNTAX, 5, "Invalid syntax") \
    X(ERROR_RANGE, 8, "Out of range")

    /*
     *   * Build an array of error return values
     *    *   e.g. {0,5,8}
     *     */
    static int ErrorVal[]
        = {
#define X(Enum, Val, Text) Val,
            X_ERROR
#undef X
        };

    /*
     *   * Build an array of error enum names
     *    *   e.g. {"ERROR_NONE","ERROR_SYNTAX","ERROR_RANGE"}
     *     */

    static char *ErrorEnum[] = {
#define X(Enum, Val, Text) #Enum,
        X_ERROR
#undef X
    };

    //#define IDX_MAX 5

    /*
     *   * Build an array of error strings
     *    *   e.g. {"Success","Invalid syntax","Out of range"}
     *     */
    static char *ErrorText[] = {
#define X(Enum, Val, Text) Text,
        X_ERROR
#undef X
    };

    /*
     *   * Create an enumerated list of error indexes
     *    *   e.g. 0,1,2
     *     */
    enum {
#define X(Enum, Val, Text) IDX_##Enum,
        X_ERROR
#undef X
            IDX_MAX /*  Array size */
    };

    /* __SpxPrivate__ void showErrorInfo(void) {
       int i;
       */
    /*
     *       * Access the values
     *            */
    /*     for (i = 0; i < IDX_MAX; i++)
           printf(" %s == %d [%s]\n", ErrorEnum[ i ], ErrorVal[ i ], ErrorText[ i ]);
           } */

    /*
     * Test validity of an error value
     *      case ERROR_SUCCESS:
     *      case ERROR_SYNTAX:
     *      case ERROR_RANGE:
     */

    // switch (value) {
    // #define X(Enum, Val, Text) case Val:
    //     X_ERROR
    // #undef X
    //     printf("Error %d is ok\n", value);
    //     break;
    //     default:
    //         printf("Invalid error: %d\n", value);
    //         break;
    // }

#ifdef __cplusplus
}
#endif
