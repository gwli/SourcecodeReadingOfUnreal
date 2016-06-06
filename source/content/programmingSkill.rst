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
   
