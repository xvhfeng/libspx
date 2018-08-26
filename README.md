# libspx
spx为c语言的一个基本组件，提供基本的c语言程序的功能。
# 此文件为spx3的设计及注意事项
1. spx_entry_t :
    SpxEntryBase  
        1. 单独变成一个宏定义是为了后续扩展entry的需要，所有的entry都必须包含SpxEntryBase，从而可以使用allocator分配器
        2. 使用SpxEntryBase的时候，必须作为struct的最后一个item。
        3. 在SpxEntryBase中，len为整个entry的长度，包含entry的隐藏头部的size，busylen为正在使用的buf长度，不包括隐藏头部。
            之所以这样的设计是为了解决更新问题，在单独更新busylen的时候，如果busylen包括隐藏的头部，则需要精确的得到entry的类型，然后通过sizeof进行计算，而如果只包括buf的使用长度则不需要。所以不加入隐藏头部可以减少一次flag的判断和指针的计算与类型转换，对于未知的客户自定义类型，还减少了相应的得到类型设计。只要更新busylen即可。
            而更新len的时候因为是重新分配内存，所以肯定是需要转换到具体的entry类型再申请内存的。
        4. 理论上，我们不建议直接使用SpxEntryBase作为基接口这样操作。而是应该隐藏entry的存在。

2. 将原来default allocator和memory搅和在一起的方式进行更改，分开来。使之形成一个总的allocator，其包括memory分配器和default分配器，让使用者自己选择使用那种类型的函数进行分配。但如果使用memory的分配函数时，memory的pooling为NULL，则直接报错。让开发者知道pooling没有比兼容性的使用normal分配器要好。至少在debug阶段就会出错。
3. memory的好处是什么？忘记free的内存可以在pool被reset的时候被free。所以这个和上面的不对，而且貌似这样的设计完全弱化了分配器的概念。
4. 运维第一，管理第一，debug第一
5. 经过对比和统一，根据smartptr的dtor函数特性，dtor函数主要做两件事情：
    1. 释放掉ptr的item的资源（如果有item的话）
    2. 释放掉自己
因为allocator在reset的时候会释放掉cleanup和entry，所以在申请内存的时候请区分使用cleanup和entry。对于有资源释放的entry，必须使用cleanup
6. 将智能指针由scopedptr和sharedptr缩小到smartptr，这是因为如果分为两种情况，看上去更好的区分了指针，但是其实增加了编程的难度，因为其实你不知道你的指针在一个函数中被申请，然后在调用函数中的存在情况，所以其实只要ref-count来区分就可以了，不需要从名字上加以区分


