现在体会科学与游戏的区别的，只要形似的那是游戏，仿真那就是科学了。当然随着计算能力的越来越高。
两者的区别越来越小。其本质还是分层的问题，在不同层面，可以不同的象而省去其节细。游戏中尽可能用一些
高层的数据学模型来近似真实的物理模型。

#
汽车的模拟最终是PhysX来实现的。可以参考
http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Vehicles.html
D:\UE4_11\Engine\Source\Runtime\Engine\Private\Vehicles\WheeledVehicleMovementComponent.cpp
把断点设置在1066行。

模型数据参考: http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Vehicles.html#tuning-guide

PTireShader at line 70
===========

PxVehicleComputeTireForceDefault at line 432
PhysLevel.cpp 
UWorld::SetupPHysicsTickFunctions at 75.


The Physics Tick function
==========================
void FStartPhysicsTickFunction::ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
void FEndPhysicsTickFunction::ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
用Taskgraph来执行，只有一个StartPhysicsSim(),

FPhysXVehicleManager.h
======================

所有的车的信息都会存放在
// All instanced vehicles
	TArray<TWeakObjectPtr<class UWheeledVehicleMovementComponent>>			Vehicles;
只是存放一个指针，并且是一个弱指针。

轮胎力量的计算
==============

PxVehicleComputeTireForceDefault， D:\UE4_11\Engine\Source\Runtime\Engine\Private\Vehicles\WheeledVehicleMovementComponent.cpp

是采用动画的这种从根到点计算过程吗，动画只是最终数据与显示的连接。 是利用物理模型来计算的，用physX来进行计算的。physX里集合不少物理计算模型。但是真正物理仿真还有一定的距离。特别是这那种有限元的仿真计算。

模型的准备
==========

#. 车头的朝向最好是X, 重心的位置就是原点。 例外尺寸最好cm,相当于unreal的1u.
Joints 的对好对齐Z轴向上。

所谓的Physics Asset Tool 其实就是建立有限元的过程，每一个面或者体都什么样的物理模型来代替，一般都是长方体或球体。

https://answers.unrealengine.com/questions/17904/how-to-use-uparticlesystemcomponent.html
继承关系
https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Particles/UParticleSystemComponent/index.html

建模方式
=========
一个mesh,sketch bone, physical Asset.
wheels and vichles.
https://docs.unrealengine.com/latest/INT/Engine/Physics/Vehicles/VehicleUserGuide/index.html


到目前为止，自己还没有设计大的OOP程序。自己目前的用法，也停留在简单的一些小的应用上，并且对于大的框架设计，自己还没有用过OOP来设计。

类的设计，实例的产生，以及静态变量与函数的实现。 静态变量是所的类共享的。这在Unreal的对象中会大量用到的。 一个对象可能包含mesh, asset,以及等等。

对于对象的设计，一个类可以是另一个类的成员。
每个类都会有staticClass来生成这公有方法。采用工厂的生成方尖。 并且所有实例的生成都是由引擎来生成的，我们只设计类。
