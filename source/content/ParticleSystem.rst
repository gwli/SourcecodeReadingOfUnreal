粒子系统
========

Unreal也实现了一个类似于 OGL的流程，这样只关心什么时候画什么问题。
具体流程

#. Particle Simulatation Time(tick) 是在 GameThread中做。
#. Particle data finialzation(Packing Gemotry,Draw Calls) 是在Render Thread中做。
#. Particle Visuals 在GPU上做。

例子数量等等都是影响性能的。


开始
=====

是在UWorld.Tick中RunTickGroup的TG_DuringPhysics.


Unreal这种engine,在运行的中有很多都是预编译，并且是能提前就提前。


流程
====

#. Anim Trail Notify Time
#. Trail Tick Time
#. Trail Render Time
#. Particle SkelMeshSurf Time
#. Particle Collision Time
#. Mesh Tick Time
#. Mesh Render Time
#. Wait for ASync Time
#. Async Work Time
#. UpdateInstances Time
#. Update Bounds Time
#. Activate Time
#. Initialize Time
#. SetTemplate Time
#. Particle Packing Time
#. Pacrticle Render Time
#. Particle Tick Time
#. PSys Comp Tick Time
#. Sprite Update Time
#. Sprite Spawn Time
#. Sprite Tick Time
#. Sprite Render Time
#. Sort Time
#. Beam Tick Time
#. Beam Render Time
#. Beam Spawn Time
#. Particle Tool Time

#. Beam Particles
#. Beam Ptcls Spawned
#. Beam Ptcls killed
#. Beam Ptcls Tris
#. Particle Draw Calls
#. Sprite Particles
#. Sprite Ptcls Spawned
#. Sprite Ptcls Updated
#. Sprite Ptcls Killed
#. Mesh Particles
#. Trail Particles
#. Trail Ptcls Spawned
#. Trail Ptcls Killed
#. Trail Ptcls Tris


VectorField
===========

C:\UnrealEngine-4.10\Engine\Shaders\VectorFieldCompositeShaders.usf
C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Private\VectorField.cpp
c:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Classes\Components\VectorFieldComponent.h


 	[0x56268844] UVectorField::UVectorField(UVectorField * this, const FObjectInitializer & ObjectInitializer) Line 95	C++
>	[0x574F7160] UVectorField::__DefaultConstructor(const FObjectInitializer & X) Line 14	C++
 	[0x574F70E4] InternalConstructor<UVectorField>(const FObjectInitializer & X) Line 2357	C++
 	[0x52907BE4] UClass::CreateDefaultObject(UClass * this) Line 2657	C++
 	[0x52089438] UClass::GetDefaultObject(UClass * this, bool bCreateIfNeeded) Line 2073	C++
 	[0x52C2E698] UObjectLoadAllCompiledInDefaultProperties() Line 661	C++
 	[0x52C2CFC4] ProcessNewlyLoadedUObjects() Line 751	C++
 	[0x52039D98] FEngineLoop::PreInit(FEngineLoop * this, const TCHAR * CmdLine) Line 1388	C++
 	[0x5202F908] FEngineLoop::PreInit(FEngineLoop * this, int32 ArgC, TCHAR ** ArgV, const TCHAR * AdditionalCommandline) Line 662	C++
 	[0x5207A5B0] AndroidMain(android_app * state) Line 283	C++
 	[0x5207C6A0] android_main(android_app * state) Line 425	C++
 	[0x520B0AC0] android_app_entry(void * param) Line 233	C++
 	[0x403BAC28] libc.so!__pthread_start(void*)()	C++
 	[0x40395294] libc.so!__start_thread()	C++

粒子系统的各种特效都是可以用Vector Field来实现的，有全局的，也有局部的。这样来实现对粒子的控制。
https://docs.unrealengine.com/latest/INT/Engine/Rendering/ParticleSystems/VectorFields/index.html
而这些的变化相当于uniform的参数传给shader。从而实现动态的vector field. 通过vector field来带动粒子的
运动，在实际上的时候，就相当于各种volume的效果。

添加一个curve,相当于添加一个函数，只需要一个输入的timedelta,然后是输出以及要输出给shader的相关设置。
大部分是uniform参数，因为Unreal自己维护了每一个resource在显存中的位置以及状态。
游戏中大部分情况，所有资源都是事先放在GPU的显存中的。只是修改部分内容而己。尽可能把变化用参数或者计算来传递。

并且GPU粒子与CPU的粒子可能支持的feature可能有所不同。甚至不兼容。

例如特殊造型的可以用Init Location module来实现。

而对于particle系统优化原则与steps https://docs.unrealengine.com/latest/INT/Engine/Rendering/ParticleSystems/Optimization/Concepts/index.html
