Unreal用到一些编程技巧
======================

一上来看到Unreal的代码，会有各种不适应。 如果是这样，说明你的编程功力还不是很足。

首先那就是各种宏定义，Unreal没有使用第三方的高级元编程技巧，而是采用的宏来实现的元编程。C/C++的宏定义是用M4来实现的。但是基本语法，那就是替换，#，与##。 
估计Unreal 用UBT来扩展宏，并且达到能编译。然后再调试中间代码。最后回来对应宏的定义。 采用宏来实现远程，还有MFC的消息传递也是这么做的。

另外，Unreal还用宏封装一些接口定义，来实现自己内部的统一。

例如

IMPLEMENT_PRIMARY_GAME_MODULE， 这种会生成原类。

弱指针
======

就是对指针加入自动回收的功能，一般是基于引用计数。引用计数模型不一样，指针的类型就不一样。


Profiling
=========

可以把nvtx放进去，替换原来profiling格式。
例如 C:\UnrealEngine-4.10\Engine\Source\Runtime\Core\Private\Stats\Stats2.cpp 中那样，直接在原来函数中实现替换成nvtx.
采用方法，采用的是package模式，单独起一线程，来来接收profiling data package.



设计模式
========

大量的singleton模式，主要用在资源的加载与共享上，这样能保证只加载了一次。
同时采用大量工厂模式，这样实例化才方便，有python的味道，一切都是UObject,先是基本数据，后面再是添加数据以及修改。这就必然模块是可以伸缩的。


FName
=====

所有字符串，以及类名都是存在hash表中，就像obj中string的做法一样。 具体可以看
FName::InitInternal_FindOrAddNameEntry这个方法，D:\UE4_11\Engine\Source\Runtime\Core\Private\UObject\UnrealNames.cpp


构造函数传参
============

https://forums.unrealengine.com/showthread.php?60042-4-7-How-do-I-actually-pass-parameter-s-in-the-new-constructor-format
在UE4中你是不会直接调用构造函数的，并且都是通过工厂模式来实现的，并且参数分为不变部分与可以变部分。其通用的参数是:command:`FObjectInitializer`. 由它来控制是调用哪些回调。d:\UE4_11\Engine\Source\Runtime\CoreUObject\Public\UObject\UObjectGlobals.h:698 FObjectInitializer. 想要传参数只要把CreateDefaultSubobject等等几个函数搞明白就行了。
所以其构造函数就设计成了static，如果动态调整其参数就只能用回调函数来实现，例如
Init函数，或者Pre/Post等等来实现。AActor::InitializeDefaults().


Console Manager: Console Variables in C++
=========================================

相当于自定义的环境变量的实现
https://docs.unrealengine.com/latest/INT/Programming/Development/Tools/ConsoleManager/index.html

内存管理
========

FMallocBinned2::FMallocBinned2(uint32 InPageSize, uint64 AddressLimit)
d:\UE4_11\Engine\Source\Runtime\Core\Private\HAL\MallocBinned2.cpp

引用计数
========

所有垃圾回收机制都是基于引用计数的，但是什么时候算加，什么时候算减，是要根据情况来定的。
对于具体的系统，它的垃圾回收机制都会定义引用计数的加减操作。


profiling
=========

Unreal 本身已经有了大量的counter计数了，可以查看stat2.h

例如 

.. code-block:: cpp

   class FStartupMessages
   {
   	friend class FStatsThread;
   
   	TArray<FStatMessage> DelayedMessages;
   	FCriticalSection CriticalSection;
   
   public:
   	/** Adds a thread metadata. */
   	CORE_API void AddThreadMetadata( const FName InThreadFName, uint32 InThreadID );
   
   	/** Adds a regular metadata. */
   	CORE_API void AddMetadata( FName InStatName, const TCHAR* InStatDesc, const char* InGroupName, const char* InGroupCategory, const TCHAR* InGroupDesc, bool bShouldClearEveryFrame, EStatDataType::Type InStatType, bool bCycleStat, FPlatformMemory::EMemoryCounterRegion InMemoryRegion = FPlatformMemory::MCR_Invalid );
   
   	/** Access the singleton. */
   	CORE_API static FStartupMessages& Get();
   };
   


