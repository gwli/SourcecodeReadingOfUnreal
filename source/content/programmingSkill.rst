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


回调的实现
==========

在不同的语言有不同叫法。其本质就是一个函数指针，而在汇编层面，那就是子程序代码的首地址。 每一段都编译器先放一个占位符，最后放入实际的值。


而在底层，高级语言比汇编强的一点，实现一定的代码搜索功能。
主要是根据编译器特性来实现的，因为所有代码回到了汇编了之后，就像在汇编层面添加自己的应用，然后这种算法再转换成一个pattern,并且添加一个关键字让编译器能够生成对应的代码。
C++的面向对象，要求编译器要构造虚表，在执行的时候还要一些搜索操作。

profiling
=========

profiling看什么呢，这根据不同目标就会方法，对于特定的目标来说，那就是时间越短越好。然后那是不断的break down,然后找到瓶颈，并且想办法去解决。也可能不能解决。

原因就像编程一样，因为不可能从汇编开始，就不可避免会有指令的冗余。对于Unreal这样大的组件，冗余肯定不少，也就是找到他们。 因为现在算法，也不是万能的，不会智能去判断，造多余的计算。


另外那就是算法复杂度的问题，其实profiling问题是由算法复杂度问题引出来的，一开始大家只关注问题的能否解决，并不关心其性能。
所以问题的scale大之后，就发现了原来方法也不是那好用了。固为每一个问题的复杂度不是一个简单的随着scale的一样线性关系。
并且这个关系也比较复杂，所以大家只能通过实际的测试来发现问题。 所以同样的算法，在不同的平台不同scale情况下，问题也是不一样。
但是能不建立一个精确的算法度模型。

例如对于mobile平台的，这种低性能device上，要考虑的性能，那就是LDR,HdR等等。
https://docs.unrealengine.com/latest/INT/Platforms/Mobile/Lighting/HowTo/ModulatedShadows/index.html

#. 先看FPS，并且看时间花在哪里 :command:`start unit`
#. 或者直接用start/StopFPSChart得到数据。
#. 再加上 dumpFrame来得到更加详细的数据。
#. 然后再看 :command:`start SceneRendring` 等。
#. :command:`Show StaticMeshes`.
#. :command:`stat Particles`  以及 :command:`Show Particles`.

#. 程序代码执行时间
#. 程序函数或代码段（汇编指令)执行次数
#. 内存使用量

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
   
   	/** Access the singleton. **/
   	CORE_API static FStartupMessages& Get();
   };
   

要根据profiling添加自己的event与counter. 具体如何用。
http://docs.unrealengine.com/latest/INT/Engine/Performance/Profiler/index.html

当你看到大量的运行时间花在ProcessEvent,CallFunction时，就去看Unreal 的event profiling tool了。
而对于Cache等等问题解决，是要依赖 native profiling来解决的。各个硬件平台都有自己的profiling工具的。

#. UE4Game.exe --messaging
#. UnrealFrontend.exe --messaging

就可以看到这些counter值，以及各种图表了。


或者直接用start/StopFPSChart然后用excel来打开看看FPS的情况，虽然你能看到每frame的情况，但是还没有办法精确的定位是哪一个frame,然后再一步分析。
当然能够配合截图录制那就更好。


要有一个大体的方向，然后逐步的细化。


当然你可以打开各种各样的trace，就像nlog一样。

.. code-block:: bash
   
   Trace Render
   Trace Game

CPU profiling
=============

如果有大量的draw calls会花费大量时间，一个办法那就是合并draw call. 
例外减少object量，场景复杂度，都是减少cpu时间，因为scene management本身是由CPU来做。
例如各种光照的减裁。

另外一些那就是物理数值的计算。 同时注意scale的问题，一般来说分辨越高，计算量越大。

需要更多优化，每次都先看下手册https://docs.unrealengine.com/latest/INT/Engine/Performance/

Memory Profiling
================

https://udn.epicgames.com/Three/MemoryProfilingHome.html

在runtime报现内存足，一般会是下面三种原因
#. level有太多的static meshes.
#. AI 创建了太多projectiles and particles.
#. 在代码中分配了太多内存。

:command:`stat levels` .

Unreal会加载的所有的依赖，但是有些是不必要的。
(Pawn->Skeletalmesh->Animsets->Animations).
这个可以通过:command:`obj list or obj refs` 来查看。
