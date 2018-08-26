/***********************************************************************
 *                              _ooOoo_
 *                             o8888888o
 *                             88" . "88
 *                             (| -_- |)
 *                              O\ = /O
 *                          ____/`---'\____
 *                        .   ' \\| |// `.
 *                         / \\||| : |||// \
 *                       / _||||| -:- |||||- \
 *                         | | \\\ - /// | |
 *                       | \_| ''\---/'' | |
 *                        \ .-\__ `-` ___/-. /
 *                     ___`. .' /--.--\ `. . __
 *                  ."" '< `.___\_<|>_/___.' >'"".
 *                 | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *                   \ \ `-. \_ __\ /__ _/ .-` / /
 *           ======`-.____`-.___\_____/___.-`____.-'======
 *                              `=---='
 *           .............................................
 *                    佛祖镇楼                  BUG辟易
 *            佛曰:
 *                    写字楼里写字间，写字间里程序员；
 *                    程序人员写程序，又拿程序换酒钱。
 *                    酒醒只在网上坐，酒醉还来网下眠；
 *                    酒醉酒醒日复日，网上网下年复年。
 *                    但愿老死电脑间，不愿鞠躬老板前；
 *                    奔驰宝马贵者趣，公交自行程序员。
 *                    别人笑我忒疯癫，我笑自己命太贱；
 *                    不见满街漂亮妹，哪个归得程序员？
 * ==========================================================================
 *
 * this software or lib may be copied only under the terms of the gnu general
 * public license v3, which may be found in the source kit.
 *
 *       Filename:  _spx_ret_no.h
 *        Created:  2018年08月15日 15时27分46秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef __SPX_RET_NO_H_
#define __SPX_RET_NO_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
     *   * SR Macro() data list
     *    * Format: Enum, Value, Text
     */
#define SpxRet                                                                                        \
    SR(SpxRet_OK, 0, "Success")                                                                       \
    /* 没有权限 */                                                                                \
    SR(SpxRet_OperationNotPermitted, 1, "Operation not permitted")                                    \
    /* 文件或者目录不存在 */                                                                 \
    SR(SpxRet_FileOrFolderNotExist, 2, " No such file or directory")                                  \
    /* 进程不存在 */                                                                             \
    SR(SpxRet_ProcessNotExist, 3, " No such process")                                                 \
    /* 系统调用被中断 */                                                                       \
    SR(SpxRet_SystemCallInterrupted, 4, " Interrupted system call")                                   \
    /* IO错误 */                                                                                    \
    SR(SpxRet_IOError, 5, " I/O error")                                                               \
    /* 设备或地址不存在 */                                                                    \
    SR(SpxRet_DeviceOrAddressNotExist, 6, " No such device or address")                               \
    /* 参数列表太长*/                                                                           \
    SR(SpxRet_ArgumentListTooLong, 7, " Argument list too long")                                      \
    /* 执行格式错误 */                                                                          \
    SR(SpxRet_FormatError, 8, " Exec format error")                                                   \
    /* 文件编号错误*/                                                                           \
    SR(SpxRet_FileNumberError, 9, " Bad file number")                                                 \
    /* 子进程不存在 */                                                                          \
    SR(SpxRet_ChildProcessNotExist, 10, " No child processes")                                        \
    /* 重试*/                                                                                       \
    SR(SpxRet_TryAgain, 11, " Try again 重试")                                                        \
    /* 内存不足 */                                                                                \
    SR(SpxRet_OutOfMemory, 12, " Out of memory")                                                      \
    /* 没有权限 */                                                                                \
    SR(SpxRet_NoPermissions, 13, " Permission denied")                                                \
    /* 错误的地址 */                                                                             \
    SR(SpxRet_AddressBad, 14, " Bad address")                                                         \
    /* 需要块设备 */                                                                             \
    SR(SpxRet_BlockDeviceRequired, 15, " Block device required")                                      \
    /* 设备或者资源正在忙 */                                                                 \
    SR(SpxRet_ResourceIsBusy, 16, " Device or resource busy")                                         \
    /* 文件已经存在 */                                                                          \
    SR(SpxRet_FileIsExist, 17, " File exists")                                                        \
    /* 跨设备链接 */                                                                             \
    SR(SpxRet_CrossLink, 18, " Cross-device link")                                                    \
    /* 设备不存在 */                                                                             \
    SR(SpxRet_DeviceNotExist, 19, " No such device")                                                  \
    /*  不是路径*/                                                                                \
    SR(SpxRet_NotFolder, 20, " Not a directory")                                                      \
    /*  是路径*/                                                                                   \
    SR(SpxRet_IsFolder, 21, " Is a directory")                                                        \
    /* 参数无效 */                                                                                \
    SR(SpxRet_ArgumentError, 22, " Invalid argument")                                                 \
    /*  文件表溢出*/                                                                             \
    SR(SpxRet_FileTableOverflow, 23, " File table overflow")                                          \
    /*  打开的文件过多*/                                                                       \
    SR(SpxRet_TooManyOpenFiles, 24, " Too many open files")                                           \
    /*  不是tty设备*/                                                                             \
    SR(SpxRet_NotTTY, 25, "not tty device")                                                           \
    /*  文本文件正在忙*/                                                                       \
    SR(SpxRet_TextFileBusy, 26, " Text file busy")                                                    \
    /*  文件太大*/                                                                                \
    SR(SpxRet_FileTooLarge, 27, " File too large")                                                    \
    /*  设备已经没有空间了*/                                                                 \
    SR(SpxRet_FreeSpaceIsTooSmall, 28, " No space left on device")                                    \
    /*  不合法的随机访问*/                                                                    \
    SR(SpxRet_IllegalRandomAccess, 29, " Illegal seek")                                               \
    /*  只读文件系统*/                                                                          \
    SR(SpxRet_FileSystemIsReadonly, 30, " Read-only file system")                                     \
    /*  链接太多*/                                                                                \
    SR(SpxRet_TooManyLinks, 31, " Too many links")                                                    \
    /*  管道已经断开*/                                                                          \
    SR(SpxRet_PipeIsBroken, 32, " Broken pipe")                                                       \
    /*  参数超域*/                                                                                \
    SR(SpxRet_ArgumentOutOfScope, 33, " Math argument out of domain of func")                         \
    /*  结果无法表示*/                                                                          \
    SR(SpxRet_ResultNotShow, 34, " Math result not representable")                                    \
    /*  资源死锁*/                                                                                \
    SR(SpxRet_Deadlock, 35, " Resource deadlock would occur")                                         \
    /*  文件名太长*/                                                                             \
    SR(SpxRet_FilenameTooLong, 36, " File name too long")                                             \
    /*  没有可用的锁，锁正在忙*/                                                           \
    SR(SpxRet_LocksBusy, 37, " No record locks available")                                            \
    /*  函数未实现*/                                                                             \
    SR(SpxRet_FunctionNotImplemented, 38, " Function not implemented")                                \
    /*  目录不为空*/                                                                             \
    SR(SpxRet_FolderNotEmpty, 39, " Directory not empty")                                             \
    /*  太多的符号链接*/                                                                       \
    SR(SpxRet_TooManySymbolicLinks, 40, " Too many symbolic links encountered")                       \
    /*  操作被阻塞，需要重试*/                                                              \
    SR(SpxRet_WouldBlock, 41, " Operation would block")                                               \
    /* 消息不符合类型 */                                                                       \
    SR(SpxRet_NoMessage, 42, " No message of desired type")                                           \
    /*  标志被移除*/                                                                             \
    SR(SpxRet_IdIsremoved, 43, " Identifier removed")                                                 \
    /*  通道标号超出范围*/                                                                    \
    SR(SpxRet_ChannelNumberOutOfRange, 44, " Channel number out of range")                            \
    /*  Level 2没有同步*/                                                                         \
    SR(SpxRet_Level2NotSynchronized, 45, " Level 2 not synchronized ")                                \
    /*  */                                                                                            \
    SR(SpxRet_Level3Halted, 46, " Level 3 halted ")                                                   \
    /*  */                                                                                            \
    SR(SpxRet_Level3Reset, 47, " Level 3 reset ")                                                     \
    /* 链接编号超出限制 */                                                                    \
    SR(SpxRet_LinkNumberOutOfRange, 48, " Link number out of range")                                  \
    /*  协议驱动没有连接*/                                                                    \
    SR(SpxRet_ProtocolNotAttached, 49, " Protocol driver not attached")                               \
    /*  没有可用的CSI结构*/                                                                    \
    SR(SpxRet_NoCSIStructure, 50, " No CSI structure available")                                      \
    /*  */                                                                                            \
    SR(SpxRet_Level2Halted, 51, " Level 2 halted ")                                                   \
    /*  无效交换*/                                                                                \
    SR(SpxRet_ExchangeBad, 52, " Invalid exchange")                                                   \
    /*  无效的请求描述*/                                                                       \
    SR(SpxRet_RequestBad, 53, " Invalid request descriptor")                                          \
    /*  交换完全*/                                                                                \
    SR(SpxRet_ExchangeFull, 54, " Exchange full")                                                     \
    /*  不是一个节点*/                                                                          \
    SR(SpxRet_NoAnode, 55, " No anode")                                                               \
    /*  不合法的请求码*/                                                                       \
    SR(SpxRet_RequestCodeBad, 56, " Invalid request code")                                            \
    /*  无效的插槽*/                                                                             \
    SR(SpxRet_SlotIsInvalid, 57, " Invalid slot")                                                     \
    /* 资源死锁 */                                                                                \
    SR(SpxRet_EDeadlock, 58, "dead lock")                                                             \
    /* 非法的字体文件格式 */                                                                 \
    SR(SpxRet_FileFormatBad, 59, " Bad font file format")                                             \
    /*  设备不是流媒体*/                                                                       \
    SR(SpxRet_NotStream, 60, " Device not a stream")                                                  \
    /* 无数据可用 */                                                                             \
    SR(SpxRet_NoDataAvailable, 61, " No data available")                                              \
    /*  计时器到期*/                                                                             \
    SR(SpxRet_TimerExpired, 62, " Timer expired")                                                     \
    /*  流资源不足*/                                                                             \
    SR(SpxRet_StreamResourceNotEnough, 63, " Out of streams resources")                               \
    /*  机器不在网络上*/                                                                       \
    SR(SpxRet_HostNotOnline, 64, " Machine is not on the network")                                    \
    /*  包未安装*/                                                                                \
    SR(SpxRet_PkgNotInstalled, 65, " Package not installed")                                          \
    /*  远端对象*/                                                                                \
    SR(SpxRet_ObjectIsRemote, 66, " Object is remote")                                                \
    /* 连接正在繁忙中*/                                                                        \
    SR(SpxRet_LinkBusy, 67, " Link has been severed")                                                 \
    /*  广播错误*/                                                                                \
    SR(SpxRet_AdvertiseError, 68, " Advertise error")                                                 \
    /*  */                                                                                            \
    SR(SpxRet_SrmountError, 69, " Srmount error ")                                                    \
    /* 发送过程中的通讯异常 */                                                              \
    SR(SpxRet_CommunicationErrorOnSend, 70, " Communication error on send")                           \
    /*  协议错误*/                                                                                \
    SR(SpxRet_ProtocolError, 71, " Protocol error")                                                   \
    /* 多跳尝试*/                                                                                 \
    SR(SpxRet_MultihopAttempted, 72, " Multihop attempted")                                           \
    /*  RFS特定错误*/                                                                             \
    SR(SpxRet_RFSError, 73, " RFS specific error RFS")                                                \
    /*  不是数据消息*/                                                                          \
    SR(SpxRet_MessageBad, 74, " Not a data message")                                                  \
    /*  对于指定类型的数据来说值太大了*/                                               \
    SR(SpxRet_ValueOverflowForDateType, 75, " Value too large for defined data type")                 \
    /*   网络上的名字不唯一*/                                                                \
    SR(SpxRet_NameNotUniqueOnNetwork, 76, " Name not unique on network")                              \
    /*  文件描述符状态错误*/                                                                 \
    SR(SpxRet_BadFileDescriptor, 77, " File descriptor in bad state")                                 \
    /*  远端的地址已经改变*/                                                                 \
    SR(SpxRet_RemoteAddressChanged, 78, " Remote address changed")                                    \
    /*  无法访问共享库*/                                                                       \
    SR(SpxRet_AccessSharedLibraryUnable, 79, " Can not access a needed shared library")               \
    /*  共享库已经损坏*/                                                                       \
    SR(SpxRet_SharedLibraryFailure, 80, " Accessing a corrupted shared library")                      \
    /*  库部分已经在a.out中被损坏*/                                                         \
    SR(SpxRet_LibSectionFail, 81, " .lib section in a.out corrupted")                                 \
    /*  试图链接过多的共享库*/                                                              \
    SR(SpxRet_LinkTooManySharedLibraries, 82, " Attempting to link in too many shared libraries")     \
    /*  不能直接运行共享库*/                                                                 \
    SR(SpxRet_UnableExecSharedLibrary, 83, " Cannot exec a shared library directly")                  \
    /*  非法的字节序*/                                                                          \
    SR(SpxRet_ByteSequenceBad, 84, " Illegal byte sequence")                                          \
    /*  因重启被中断的系统调用*/                                                           \
    SR(SpxRet_SystemCallRestartedWhenInterrupted, 85, " Interrupted system call should be restarted") \
    /*  流管道错误*/                                                                             \
    SR(SpxRet_StreamsPipeError, 86, " Streams pipe error")                                            \
    /*  用户太多*/                                                                                \
    SR(SpxRet_TooManyUsers, 87, " Too many users")                                                    \
    /*  非socket上进行socket操作*/                                                              \
    SR(SpxRet_BadSocket, 88, " Socket operation on non-socket")                                       \
    /* 需要制定目的地址 */                                                                    \
    SR(SpxRet_BadDestinationAddress, 89, " Destination address required")                             \
    /*  信息太长*/                                                                                \
    SR(SpxRet_MessageTooLong, 90, " Message too long")                                                \
    /*  协议错误*/                                                                                \
    SR(SpxRet_BadProtocol, 91, " Protocol wrong type for socket")                                     \
    /*  协议不可用*/                                                                             \
    SR(SpxRet_ProtocolUnable, 92, " Protocol not available")                                          \
    /*  协议不支持*/                                                                             \
    SR(SpxRet_ProtocolNotSupported, 93, " Protocol not supported")                                    \
    /*  套接字类型不支持*/                                                                    \
    SR(SpxRet_SocketTypeNotSupported, 94, " Socket type not supported")                               \
    /*  不支持操作的传输端点*/                                                              \
    SR(SpxRet_OperationNotSupportedOnEndpoint, 95, " Operation not supported on transport endpoint")  \
    /*  协议簇未被支持*/                                                                       \
    SR(SpxRet_ProtocolFamilyNotSupported, 96, " Protocol family not supported")                       \
    /*  当前协议不支持地址群*/                                                              \
    SR(SpxRet_AddressFamilyNotSupportedByProtocol, 97, " Address family not supported by protocol")   \
    /*  地址已被占用*/                                                                          \
    SR(SpxRet_AddressInUse, 98, " Address already in use")                                            \
    /*  不能分配请求的地址*/                                                                 \
    SR(SpxRet_RequestedAddressUnableAssign, 99, " Cannot assign requested address")                   \
    /*  网络已经关闭*/                                                                          \
    SR(SpxRet_NetworkIsDown, 100, " Network is down")                                                 \
    /*  网络不可达*/                                                                             \
    SR(SpxRet_NetworkIsUnreachable, 101, " Network is unreachable")                                   \
    /*  由于网络复位，网络断开*/                                                           \
    SR(SpxRet_DropConnectionByReset, 102, " Network dropped connection because of reset")             \
    /*  软件导致的链接失败*/                                                                 \
    SR(SpxRet_ConnectionAbortBySoftware, 103, " Software caused connection abort")                    \
    /*  链接的对方复位*/                                                                       \
    SR(SpxRet_ConnectionResetByPeer, 104, " Connection reset by peer")                                \
    /* 缓冲区被耗尽 */                                                                          \
    SR(SpxRet_BufferSpaceAllBusy, 105, " No buffer space availabl ")                                  \
    /*  传输端点已经连接*/                                                                    \
    SR(SpxRet_TransportEndpointAlreadyConnected, 106, " Transport endpoint is already connected")     \
    /*  传输端点未连接*/                                                                       \
    SR(SpxRet_TransportEndpointNotConnected, 107, " Transport endpoint is not connected")             \
    /*  传输端点已经被关闭*/                                                                 \
    SR(SpxRet_TransportEndpointShutdown, 108, " Cannot send after transport endpoint shutdown")       \
    /*  太多的引用，无法链接*/                                                              \
    SR(SpxRet_ReferencesTooMany, 109, " Too many references: cannot splice")                          \
    /*  链接超时*/                                                                                \
    SR(SpxRet_ConnectionTimeout, 110, " Connection timed out")                                        \
    /*  连接被拒绝*/                                                                             \
    SR(SpxRet_ConnectionRefused, 111, " Connection refused")                                          \
    /*  主机已经关闭*/                                                                          \
    SR(SpxRet_HostIsDown, 112, " Host is down")                                                       \
    /*  无法路由到主机*/                                                                       \
    SR(SpxRet_NoRouteToHost, 113, " No route to host")                                                \
    /*  操作已经在运行了*/                                                                    \
    SR(SpxRet_OperationRuned, 114, " Operation already in progress")                                  \
    /* 操作在运行 */                                                                             \
    SR(SpxRet_OperationIsRunning, 115, " Operation now in progress")                                  \
    /*  网络文件系统句柄错误*/                                                              \
    SR(SpxRet_NFSFileHandlerError, 116, " Stale NFS file handle")                                     \
    /*  结构体需要清空*/                                                                       \
    SR(SpxRet_StructureNeedClean, 117, " Structure needs cleaning")                                   \
    /*  不是一个xenix命名类型文件*/                                                         \
    SR(SpxRet_NotXENIXNameTypeFile, 118, " Not a XENIX named type file")                              \
    /*  */                                                                                            \
    SR(SpxRet_NoXENIXSemaphoresUnable, 119, " No XENIX semaphores available ")                        \
    /*  不可识别的文件类型*/                                                                 \
    SR(SpxRet_UnkownFileType, 120, " Is a named type file")                                           \
    /*  key不存在*/                                                                                \
    SR(SpxRet_KeyNotExist, 126, " Required key not available")                                        \
    /*  key已经过期*/                                                                             \
    SR(SpxRet_KeyHasExpired, 127, " Key has expired")                                                 \
    /*  key已经被删除*/                                                                          \
    SR(SpxRet_KeyHasRemoved, 128, " Key has been revoked")                                            \
    /*  key已经被server拒绝*/                                                                    \
    SR(SpxRet_KeyRejectedByService, 129, " Key was rejected by service")                              \
    /*  拥有者不存在*/                                                                          \
    SR(SpxRet_OwnerNotExist, 130, " Owner not exist")                                                 \
    /*  z状态不可恢复*/                                                                         \
    SR(SpxRet_StateNotRecoverable, 131, " State not recoverable")                                     \
    /*  由于rkill，操作无法完成*/                                                            \
    SR(SpxRet_OperationUndoneByRFkill, 132, " Operation not possible due to RF-kill")                 \
    /*  内存页错误*/                                                                             \
    SR(SpxRet_MemoryPageError, 133, "memory page error")                                              \
    /*  内存页错误*/                                                                             \
    SR(SpxRet_BindCpuFail, 134, "bind thraed or process to cpu error")                                \
    /* 可用的lock*/                                                                                \
    SR(SpxRet_LockFree, 135, "spinlock free error")                                                   \
    /*  内存页错误*/                                                                             \
    SR(SpxRet_PathFormatError, 137, "path name format is error")                                      \
    /*  内存页错误*/                                                                             \
    SR(SpxRet_FilenameError, 138, "filename is error,maybe no suffix")                                \
    /*  内存页错误*/                                                                             \
    SR(SpxRet_NotFoundPattern, 139, "not found pattern in source")                                    \
    /*  内存页错误*/                                                                             \
    SR(SpxRet_IndexOutOfCount = 140, "index out of count in array ,map,set")

#ifdef __cplusplus
}
#endif
#endif
