Damage
=======

https://www.unrealengine.com/zh-CN/blog/damage-in-ue4
每一个actor都可以继承一个TakeDamage的函数，而由AppDamage,ApplyPointDamage,ApplyRadicalDamage 来形成一个damage call chain.  而这些都是由
c:\UnrealEngine-4.10\Engine\Source\ThirdParty\PhysX\APEX-1.3\module\destructible\public\NxDestructibleActor.h

各种各样的damage都是从
C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Classes\Components\DestructibleComponent.h 
C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Private\Components\DestructibleComponent.cpp
得到的。并且后台使用的APEX来实现的。每一个actor都可以自身响应这个destructible event.
Apply Radial Damage
https://docs.unrealengine.com/latest/INT/BlueprintAPI/Game/Damage/ApplyRadialDamage/index.html
并且可以添加channel来进行过滤，哪些毁，哪些不毁。

同时爆炸也利用这个类型来实现。同时再加上一些粒子系统来实现。
https://answers.unrealengine.com/questions/224372/how-to-apply-radial-damage-to-all-actors-in-the-ar.html

Damage type
https://docs.unrealengine.com/latest/INT/BlueprintAPI/Game/Damage/index.html


https://answers.unrealengine.com/questions/224372/how-to-apply-radial-damage-to-all-actors-in-the-ar.html



Pickup
======

都是通过固定的事件序列来产生，如果将来VR中的事件就比较难定义了。
http://unrealtutorials.com/unreal-engine-tutorials/unreal-engine-4-tutorials/pickups-items-weapons/

同时还得设定一个Colllision属性为了得到正确的行为。
https://www.newbiehack.com/ShowVideoClip.aspx?id=3000

同时还得进行一些特效的处理，这样就得添加AppPostProcessing函数的内容，以及Focus的处理。以及主从关系的处理。
http://expletive-deleted.com/2015/06/17/item-pickup-system-in-unreal-engine-using-c/
同时用一些debug函数划线实现一些特效。

把其放在tick中，并且:command:`InputCompoent->BindAction` 来绑定事件。
https://docs.unrealengine.com/latest/INT/Gameplay/Input/index.html
非累积式用action,累积式用axis.
https://www.unrealengine.com/blog/input-action-and-axis-mappings-in-ue4

各种物理运动
============

可以采用其Physics constraint,或者直接力学仿真，就像汽车的轮子一样。 需要复杂的方程计算。

例如轨道旋转 https://forums.unrealengine.com/showthread.php?92190-How-to-rotate-an-actor-around-another-actor

或者简单的不需要物理计算，只是简单的动画KR之类的依赖。 直接使用仿射变换方程就行了。

一种是动化的， 一种物理状态，这样那就是物理模型的计算，并且也还有一个树形的依赖，就像那个IK运动模型一样。
在游戏里一般物理运算简单模拟，而不是很深级别的有限元计算方法。


Componment
==========

SpringArm  最常见用法那就是用于Camera的连接，用途那就是保持连接依赖，又不需要碰撞检测。
https://forums.unrealengine.com/showthread.php?893-Component-SpringArm



finding actor
==============

游戏中要尽可能避免那样操作，因为比较耗时，一般都采用方法，那就是保存reference,
而实在不行就遍历了。
https://wiki.unrealengine.com/Iterators:_Object_%26_Actor_Iterators,_Optional_Class_Scope_For_Faster_Search

在Unreal中，搜索基本上都是基于TArray中，World存有Level的link
每一个World里，会有当前各种资源的列表，以前当激活的列表。
对于level也是样的。

例如某一个actor,或者一类actor都是调用的Level

一种是打tag在actor上，这样就可以直接搜索到。
http://docs.unrealengine.com/latest/INT/Gameplay/HowTo/FindingActors/Blueprints/index.html



CreateNavigationSystem 是在InitWorld时创建的。
	VehicleAdvanced_C-Win64-Debug.exe!UNavigationSystem::CreateNavigationSystem(UWorld * WorldOwner) Line 2150	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UWorld::InitWorld(const UWorld::InitializationValues IVS) Line 913	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UEngine::LoadMap(FWorldContext & WorldContext, FURL URL, UPendingNetGame * Pending, FString & Error) Line 9753	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UEngine::Browse(FWorldContext & WorldContext, FURL URL, FString & Error) Line 8945	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UGameInstance::StartGameInstance() Line 339	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UGameEngine::Init(IEngineLoop * InEngineLoop) Line 529	C++
 	VehicleAdvanced_C-Win64-Debug.exe!FEngineLoop::Init() Line 2185	C++
 	VehicleAdvanced_C-Win64-Debug.exe!EngineInit() Line 41	C++
 	VehicleAdvanced_C-Win64-Debug.exe!GuardedMain(const wchar_t * CmdLine, HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, int nCmdShow) Line 136	C++
 	VehicleAdvanced_C-Win64-Debug.exe!WinMain(HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, char * __formal, int nCmdShow) Line 189	C++
 	[External Code]	

spawn and destroy
=================

在游戏中采用的设计模式的方法，WorldSetting里可以指定默认的，也可以使用特定的。
http://docs.unrealengine.com/latest/INT/Gameplay/HowTo/SpawnAndDestroyActors/Blueprints/index.html

同时UWorld的是对应方法的

.. code-block:: cpp

   [0x583CDF78] UWorld::ContainsLevel(const UWorld * this, ULevel * InLevel) Line 5179	C++
   [0x583CDEF4] UWorld::AddNetworkActor(UWorld * this, AActor * Actor) Line 3386	C++
   [0x57042B48] UWorld::SpawnActor(UWorld * this, UClass * Class, const FTransform * UserTransformPtr, const FActorSpawnParameters & SpawnParameters) Line 426	C++
   [0x57043EC8] UWorld::SpawnActor(UWorld * this, UClass * Class, const FVector * Location, const FRotator * Rotation, const FActorSpawnParameters & SpawnParameters) Line 266	C++
   [0x583A8084] UWorld::InitializeNewWorld(UWorld * this, const UWorld::InitializationValues IVS) Line 1092	C++
   [0x583AAC60] UWorld::CreateWorld(const EWorldType::Type InWorldType, bool bInformEngineOfWorld, FName WorldName, UPackage * InWorldPackage, bool bAddToRoot, ERHIFeatureLevel::Type InFeatureLevel) Line 1174	C++
   [0x56C97C3C] UGameInstance::InitializeStandalone(UGameInstance * this) Line 104	C++
   [0x56C96A74] UGameEngine::Init(UGameEngine * this, IEngineLoop * InEngineLoop) Line 471	C++
   [0x52A7F448] FEngineLoop::Init(FEngineLoop * this) Line 2183	C++
   [0x52A76FE0] AndroidMain(android_app * state) Line 292	C++
   [0x52AA6EF0] android_main(android_app * state) Line 433	C++
   [0x52B07E84] android_app_entry(void * param) Line 233	C++
   [0x409F7C28] libc.so!__pthread_start(void*)()	C++
   [0x409D2294] libc.so!__start_thread()	C++
 	??()	C++


TickActor
=========

输入事件轮询是在这里。 PlayerInput.cpp:980

.. code-block:: cpp

   static TArray<FAxisDelegateDetails> AxisDelegates;
   static TArray<FVectorAxisDelegateDetails> VectorAxisDelegates;
   static TArray<FDelegateDispatchDetails> NonAxisDelegates;
   static TArray<FKey> KeysToConsume;
   static TArray<FDelegateDispatchDetails> FoundChords;


点的轨迹 trace
==============

#. 想到某个character前面10unit放一个东东

.. code-block:: cpp
   
   FVector statLoc = GetActorLocation();
   FVector endLoc  = startLoc + GetViewRotation().Vector()* 10.0f;

#. 例如跟踪你手在空中滑过的轨迹
可以用这些API

.. code-block:: cpp
   
   DrawDebugPoint
   DrawDebugLine
   DrawDebugSphere

https://wiki.unrealengine.com/Draw_3D_Debug_Points,_Lines,_and_Spheres:_Visualize_Your_Algorithm_in_Action
更全面API在 d:\UE4_11\Engine\Source\Runtime\Engine\Private\DrawDebugHelpers.cpp
https://forums.unrealengine.com/showthread.php?87012-Draw-a-line-between-two-points-IN-GAME&highlight=draw+line+between

#. 刀砍到人检测
可以用，ActorLineTraceSingle,或者 GetWorld->LineTraceSingle来判断中间是不是
遇到阻碍了，遇到就相当于是刀砍到人了。相当于一个三维的线检测。
以及TraceComponent,TraceActor来实现，主要是基于数据结构是 :command:`FCollisonQueryParams` 
以及 :command:`FhitResult` 来实现。

这样就可以在步进中着检测。 
https://wiki.unrealengine.com/Trace_Functions
https://answers.unrealengine.com/questions/3446/how-would-i-use-line-trace-in-c.html
