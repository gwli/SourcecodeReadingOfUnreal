Taskgraph 是Unreal的一个多线程系统。基本介绍可以参考
https://wiki.unrealengine.com/Multi-Threading:_Task_Graph_System
https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4


线程池
======

不同于一般的线程那就是Task的改变，直白一些那就是如何切换执行代码。一般线程在一开始create的时候，来指定函数体。
而线程池要能够动态改这个函数体，线程结构中也修改对应的代码段，也就是Runnable对象。
这个是如何实现的，线程体函数是改不了，线程自己维护一个 函数指针队列.

.. code-block:: cpp
   
   typedef struct {
       void *(*function)(void*);
       void *args;
   } task;


Unreal中多线程采用的 fork/join模式。 并且也就是一个线程池的作用。

一个事务，只要设计好哪里存放数据，每一个原子操作如何操作，以及事务结束时event的Handle就行了。

FGraphEventArray 来存放event.
写一个函数的处理TasksAreComplete。
以及一个干原子操作function. 
再也就是分启动初使用 function,也就是要多少thread来给你做事。但是真正由多少thread同时做，是由TaskGraph来分配的。 相当于软件开发中50个 人/月。 
   FGraphEventArray.Add(TgraphTask<TaskThread>::CreateTask(NULL,ENameThreads::GameThread).ConstructAndDispathWhenRead());
   从哪里开始。

然后封装一个Task Thread.

.. code-block::
   class TaskThread {
       public:
          TaskThread() {};
          static const TCHAR*GetTaskName(){
               return TEXT("YourTaskName");
          };
          FORCEINLINE static TStatId GetStatId() {
                RETURN_QUICK_DCLEAR_CYCLESTAT(TaskThread,STATGROUP_TaskGraphTasks);
          }
          ....
          void DoTask(ENameThreads::Type CurrentThread,const GraphEventRef& myCompletionGraphEvent) {
               // do your real job
          }
   }

AsyncWork.h 这里很多samples.
Core/Private/AsyncTaskGraph.cpp 

#. Questions
#. CreateTask
#. 



源码分析 
=========

#. 启动在是 FEngine::PreInit 中

.. code-block:: cpp

   // initialize task graph sub-system with potential multiple threads
   FTaskGraphInterface::Startup( FPlatformMisc::NumberOfCores() );
   FTaskGraphInterface::Get().AttachToThread( ENamedThreads::GameThread );
   
   #if STATS
   FThreadStats::StartThread();
   #endif

   // Statics in FTaskGraphInterface
   void FTaskGraphInterface::Startup(int32 NumThreads)
   {
   	// TaskGraphImplementationSingleton is actually set in the constructor because find work will be called before this returns.
   	new FTaskGraphImplementation(NumThreads); 
   }
         

实现FTaskGraphImplementation(NumThreads); 
==========================================

#. 根据CPU核数，来建立线程。
#. 最多12个线程，这个是编译的指定的， TaskGraph.cpp:2044 MAX_THREADS=12;
NameThread 有四个
#. StatsThread
#. RHIThread
#. GameThread
#. ActualRenderThread

anythread, 有3个。
并且启动这三个线程，并指定其stackSize=256*1024,最终还是调用pthread_create来创建thread.
这个可以在PThreadRunnableThread.h看到。

.. code-block:: cpp

   /**
    * Wrapper for pthread_create that takes a name
    *
    * Allows a subclass to override this function to create a thread and give it a name, if
    * the platform supports it
    *
    * Takes the same arguments as pthread_create
    */
   virtual int CreateThreadWithName(pthread_t* HandlePtr, pthread_attr_t* AttrPtr, PthreadEntryPoint Proc, void* Arg, const ANSICHAR* /*Name*/)
   {
   	// by default, we ignore the name since it's not in the standard pthreads
   	return pthread_create(HandlePtr, AttrPtr, Proc, Arg);
   }
_ThreadProc 就是自己来实现 taskqueue了。    


NameThread
==========

自己还有自己有优先级队列
#. 两级队列
#. 第二级队列
    - PrivateQueueHiPri
    - OutstandingHiPriTasks
    - IncomingQueue

TGraphTask
==========

#. 产生与处理依赖。
SetupPrereqs

调度类型
========

#. CreateAndDispatchWhenReady
-  Create a task and dispatch it when the prerequisites are complete
#. ProcessThreadUntilRequestReturn 



各种snync_syncrhonize 最终实现都是基于机器自己的，于Unreal中 MemoryBarrier 都是用指 arm "dmb ish". 
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0204ic/CIHJFGFE.html
