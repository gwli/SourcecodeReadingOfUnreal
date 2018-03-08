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

.. code-block:: bash

   C:\UnrealEngine-4.10\Engine\Shaders\VectorFieldCompositeShaders.usf
   C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Private\VectorField.cpp
   c:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Classes\Components\VectorFieldComponent.h
   
   
    	[0x56268844] UVectorField::UVectorField(UVectorField * this, const FObjectInitializer & ObjectInitializer) Line 95	C++
   	[0x574F7160] UVectorField::__DefaultConstructor(const FObjectInitializer & X) Line 14	C++
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

debug
=====

entrypoint: *UParticleSystemComponent::TickComponent@C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Private\Particles\ParticleComponents.cpp:3577*

.. code-block:: bash

   [0x935CB71C] UParticleSystemComponent::TickComponent(UParticleSystemComponent * this, float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) Line 3574	C++
   [0x935E9400] UParticleSystemComponent::ActivateSystem(UParticleSystemComponent * this, bool bFlagAsJustAttached) Line 4414	C++
   [0x935EA160] UParticleSystemComponent::Activate(UParticleSystemComponent * this, bool bReset) Line 4515	C++
   [0x930C6ECC] UActorComponent::OnRegister(UActorComponent * this) Line 612	C++
   [0x9326CB60] USceneComponent::OnRegister(USceneComponent * this) Line 372	C++
   [0x931BEDC4] UPrimitiveComponent::OnRegister(UPrimitiveComponent * this) Line 386	C++
   [0x935BBDCC] UParticleSystemComponent::OnRegister(UParticleSystemComponent * this) Line 2826	C++
   [0x930CCA44] UActorComponent::ExecuteRegisterEvents(UActorComponent * this) Line 1041	C++
   [0x930CC710] UActorComponent::RegisterComponentWithWorld(UActorComponent * this, UWorld * InWorld) Line 826	C++
   [0x91B4ABA4] AActor::IncrementalRegisterComponents(AActor * this, int32 NumComponentsToRegister) Line 3640	C++
   [0x9218A120] ULevel::IncrementalUpdateComponents(ULevel * this, int32 NumComponentsToUpdate, bool bRerunConstructionScripts) Line 751	C++
   [0x92189DF4] ULevel::UpdateLevelComponents(ULevel * this, bool bRerunConstructionScripts) Line 695	C++
   [0x92C16EFC] UWorld::UpdateWorldComponents(UWorld * this, bool bRerunConstructionScripts, bool bCurrentLevelOnly) Line 1319	C++
   [0x92C37520] UWorld::InitializeActorsForPlay(UWorld * this, const FURL & InURL, bool bResetTime) Line 2968	C++
   [0x92B51558] UEngine::LoadMap(UEngine * this, FWorldContext & WorldContext, FURL URL, UPendingNetGame * Pending, FString & Error) Line 9549	C++
   [0x92B40E2C] UEngine::Browse(UEngine * this, FWorldContext & WorldContext, FURL URL, FString & Error) Line 8684	C++
   [0x91E54374] UGameInstance::StartGameInstance(UGameInstance * this) Line 325	C++
   [0x91E40334] UGameEngine::Init(UGameEngine * this, IEngineLoop * InEngineLoop) Line 535	C++
   [0x8E9E3F30] FEngineLoop::Init(FEngineLoop * this) Line 2092	C++
   [0x8EA1167C] AndroidMain(android_app * state) Line 292	C++
   [0x8EA136A0] android_main(android_app * state) Line 425	C++
   [0x8EA47AC0] android_app_entry(void * param) Line 233	C++
   [0xB0AEA544] libc.so!__pthread_start(void*)()	C++
   [0xB0ABD116] libc.so!__start_thread()	C++
   ??()	C++

UParticleSystemComponent 
========================

有6个成员，主要与materitals,ParticleSystem,RenderSystem. 

.. code-block:: bash

   UParticleSystemComponent 
      Base Types UPrimitiveComponent 
      Derived Types UCascadeParticleSystemComponent 
      UParticleSystemComponent::EForceAsyncWorkCompletion
      UParticleSystemComponent::FFXSystemInterface 
      UParticleSystemComponent::FRenderCommandFence 
      UParticleSystemComponent::UMaterialInstanceDynamic 
      UParticleSystemComponent::UMaterialInterface 
      UParticleSystemComponent::UParticleSystem

   #method 34 method
   UparticleSystemComponent::Activate
   UParticleSystemComponent::ActivateSystem
   UParticleSystemComponent::AdditionalStatObject
   UParticleSystemComponent::ApplyWorldOffset
   UParticleSystemComponent::AutoPopulateInstanceProperties
   UParticleSystemComponent::BeginDestroy
   UParticleSystemComponent::BeginTrails
   UParticleSystemComponent::CacheViewRelevanceFlags
   UParticleSystemComponent::CalcBounds
   UParticleSystemComponent::CanBeOccluded
   UParticleSystemComponent::CancelAutoAttachment
   UParticleSystemComponent::CanConsiderInvisible
   UParticleSystemComponent::CanTickInAnyThread
   UParticleSystemComponent::CheckForErrors
   UParticleSystemComponent::ClearDynamicData
   UParticleSystemComponent::ClearParameter
   UParticleSystemComponent::Complete
   UParticleSystemComponent::ComputeCanTickInAnyThread
   UParticleSystemComponent::ComputeTickComponent_Concurrent
   UParticleSystemComponent::ConditionalCacheViewRelevanceFlags
   UParticleSystemComponent::CreateDynamicData
   UParticleSystemComponent::CreateDynamicDataFromReplay
   UParticleSystemComponent::CreateNamedDynamicMaterialInstance
   UParticleSystemComponent::CreateRenderState_Concurrent
   UParticleSystemComponent::CreateSceneProxy
   UParticleSystemComponent::Deactivate
   UParticleSystemComponent::DeactivateSystem
   UParticleSystemComponent::DestroyRenderState_Concurrent
   UParticleSystemComponent::DetermineLODLevelForLocation
   UParticleSystemComponent::EndTrails
   UParticleSystemComponent::FinalizeTickComponent
   UParticleSystemComponent::FindReplayClipForIDNumber
   UParticleSystemComponent::FinishDestroy
   UParticleSystemystemComponent::TrailEmitterArrayeSystemComponent::WaitForAsyncAndFinalize

   #variable 377-197=180
   UParticleSystemComponent::AccumLODDistanceCheckTime
   UParticleSystemComponent::AccumTickTime
   UParticleSystemComponent::AsyncBounds
   UParticleSystemComponent::AsyncComponentToWorld
   UParticleSystemComponent::AsyncInstanceParameters
   UParticleSystemComponent::AsyncPartSysVelocity
   UParticleSystemComponent::AsyncWork
   UParticleSystemComponent::AutoAttachLocationRule
   UParticleSystemComponent::AutoAttachLocationType_DEPRECATED
   UParticleSystemComponent::AutoAttachParent
   UParticleSystemComponent::AutoAttachRotationRule
   UParticleSystemComponent::AutoAttachScaleRule
   UParticleSystemComponent::AutoAttachSocketName
   UParticleSystemComponent::bAllowRecycling
   UParticleSystemComponent::bAsyncDataCopyIsValid
   UParticleSystemComponent::bAsyncWorkOutstanding
   UParticleSystemComponent::bAutoDestroy
   UParticleSystemComponent::bAutoManageAttachment
   UParticleSystemComponent::bDidAutoAttach
   UParticleSystemComponent::bForcedInActive
   UParticleSystemComponent::bForceLODUpdateFromRenderer
   UParticleSystemComponent::bHasBeenActivated
   UParticleSystemComponent::bIsElligibleForAsyncTick
   UParticleSystemComponent::bIsElligibleForAsyncTickComputed
   UParticleSystemComponent::bIsManagingSignificance
   UParticleSystemComponent::bIsTransformDirty
   UParticleSystemComponent::bIsViewRelevanceDirty
   UParticleSystemComponent::bJustRegistered
   UParticleSystemComponent::bNeedsFinalize
   UParticleSystemComponent::bOverrideLODMethod
   UParticleSystemComponent::bParallelRenderThreadUpdate
   UParticleSystemComponent::bResetOnDetach
   UParticleSystemComponent::bResetTriggered
   UParticleSystemComponent::bSkipUpdateDynamicDataDuringTick
   UParticleSystemComponent::bSuppressSpawning
   UParticleSystemComponent::bUpdateOnDedicatedServer
   UParticleSystemComponent::BurstEvents
   UParticleSystemComponent::bWarmingUp
   UParticleSystemComponent::bWasActive
   UParticleSystemComponent::bWasCompleted
   UParticleSystemComponent::bWasDeactivated
   UParticleSystemComponent::bWasManagingSignificance
   UParticleSystemComponent::CachedViewRelevanceFlags
   UParticleSystemComponent::CollisionEvents
   UParticleSystemComponent::CustomTimeDilation
   UParticleSystemComponent::DeathEvents
   UParticleSystemComponent::DeltaTimeTick
   UParticleSystemComponent::EditorDetailMode
   UParticleSystemComponent::EditorLODLevel
   UParticleSystemComponent::EmitterDelay
   UParticleSystemComponent::EmitterInstances
   UParticleSystemComponent::EmitterMaterials
   UParticleSystemComponent::FXSystem
   UParticleSystemComponent::InstanceParameters
   UParticleSystemComponent::KismetEvents
   UParticleSystemComponent::LastCheckedDetailMode
   UParticleSystemComponent::LastSignificantTime
   UParticleSystemComponent::LODLevel
   UParticleSystemComponent::LODMethod
   UParticleSystemComponent::MaxTimeBeforeForceUpdateTransform
   UParticleSystemComponent::NumSignificantEmitters
   UParticleSystemComponent::OldPosition
   UParticleSystemComponent::OnParticleBurst
   UParticleSystemComponent::OnParticleCollide
   UParticleSystemComponent::OnParticleDeath
   UParticleSystemComponent::OnParticleSpawn
   UParticleSystemComponent::OnSystemFinished
   UParticleSystemComponent::OnSystemPreActivationChange
   UParticleSystemComponent::PartSysVelocity
   UParticleSystemComponent::PlayerLocations
   UParticleSystemComponent::PlayerLODDistanceFactor
   UParticleSystemComponent::ReleaseResourcesFence
   UParticleSystemComponent::ReplayClipIDNumber
   UParticleSystemComponent::ReplayClips
   UParticleSystemComponent::ReplayFrameIndex
   UParticleSystemComponent::ReplayState
   UParticleSystemComponent::RequiredSignificance
   UParticleSystemComponent::SavedAutoAttachRelativeLocation
   UParticleSystemComponent::SavedAutoAttachRelativeRotation
   UParticleSystemComponent::SavedAutoAttachRelativeScale3D
   UParticleSystemComponent::SecondsBeforeInactive
   UParticleSystemComponent::SkelMeshComponents
   UParticleSystemComponent::SpawnEvents
   UParticleSystemComponent::Template
   UParticleSystemComponent::TimeSinceLastForceUpdateTransform
   UParticleSystemComponent::TimeSinceLastTick
   UParticleSystemComponent::TotalActiveParticles
   UParticleSystemComponent::WarmupTickRate
   UParticleSystemComponent::WarmupTime
   UParticleSystemComponent::TrailEmitterArrayeSystemComponent::WaitForAsyncAndFinalize
   UParticleSystemComponent::AccumLODDistanceCheckTime
   UParticleSystemComponent::AccumTickTime
   UParticleSystemComponent::AsyncBounds
   UParticleSystemComponent::AsyncComponentToWorld
   UParticleSystemComponent::AsyncInstanceParameters
   UParticleSystemComponent::AsyncPartSysVelocity
   UParticleSystemComponent::AsyncWork
   UParticleSystemComponent::AutoAttachLocationRule
   UParticleSystemComponent::AutoAttachLocationType_DEPRECATED
   UParticleSystemComponent::AutoAttachParent
   UParticleSystemComponent::AutoAttachRotationRule
   UParticleSystemComponent::AutoAttachScaleRule
   UParticleSystemComponent::AutoAttachSocketName
   UParticleSystemComponent::bAllowRecycling
   UParticleSystemComponent::bAsyncDataCopyIsValid
   UParticleSystemComponent::bAsyncWorkOutstanding
   UParticleSystemComponent::bAutoDestroy
   UParticleSystemComponent::bAutoManageAttachment
   UParticleSystemComponent::bDidAutoAttach
   UParticleSystemComponent::bForcedInActive
   UParticleSystemComponent::bForceLODUpdateFromRenderer
   UParticleSystemComponent::bHasBeenActivated
   UParticleSystemComponent::bIsElligibleForAsyncTick
   UParticleSystemComponent::bIsElligibleForAsyncTickComputed
   UParticleSystemComponent::bIsManagingSignificance
   UParticleSystemComponent::bIsTransformDirty
   UParticleSystemComponent::bIsViewRelevanceDirty
   UParticleSystemComponent::bJustRegistered
   UParticleSystemComponent::bNeedsFinalize
   UParticleSystemComponent::bOverrideLODMethod
   UParticleSystemComponent::bParallelRenderThreadUpdate
   UParticleSystemComponent::bResetOnDetach
   UParticleSystemComponent::bResetTriggered
   UParticleSystemComponent::bSkipUpdateDynamicDataDuringTick
   UParticleSystemComponent::bSuppressSpawning
   UParticleSystemComponent::bUpdateOnDedicatedServer
   UParticleSystemComponent::BurstEvents
   UParticleSystemComponent::bWarmingUp
   UParticleSystemComponent::bWasActive
   UParticleSystemComponent::bWasCompleted
   UParticleSystemComponent::bWasDeactivated
   UParticleSystemComponent::bWasManagingSignificance
   UParticleSystemComponent::CachedViewRelevanceFlags
   UParticleSystemComponent::CollisionEvents
   UParticleSystemComponent::CustomTimeDilation
   UParticleSystemComponent::DeathEvents
   UParticleSystemComponent::DeltaTimeTick
   UParticleSystemComponent::EditorDetailMode
   UParticleSystemComponent::EditorLODLevel
   UParticleSystemComponent::EmitterDelay
   UParticleSystemComponent::EmitterInstances
   UParticleSystemComponent::EmitterMaterials
   UParticleSystemComponent::FXSystem
   UParticleSystemComponent::InstanceParameters
   UParticleSystemComponent::KismetEvents
   UParticleSystemComponent::LastCheckedDetailMode
   UParticleSystemComponent::LastSignificantTime
   UParticleSystemComponent::LODLevel
   UParticleSystemComponent::LODMethod
   UParticleSystemComponent::MaxTimeBeforeForceUpdateTransform
   UParticleSystemComponent::NumSignificantEmitters
   UParticleSystemComponent::OldPosition
   UParticleSystemComponent::OnParticleBurst
   UParticleSystemComponent::OnParticleCollide
   UParticleSystemComponent::OnParticleDeath
   UParticleSystemComponent::OnParticleSpawn
   UParticleSystemComponent::OnSystemFinished
   UParticleSystemComponent::OnSystemPreActivationChange
   UParticleSystemComponent::PartSysVelocity
   UParticleSystemComponent::PlayerLocations
   UParticleSystemComponent::PlayerLODDistanceFactor
   UParticleSystemComponent::ReleaseResourcesFence
   UParticleSystemComponent::ReplayClipIDNumber
   UParticleSystemComponent::ReplayClips
   UParticleSystemComponent::ReplayFrameIndex
   UParticleSystemComponent::ReplayState
   UParticleSystemComponent::RequiredSignificance
   UParticleSystemComponent::SavedAutoAttachRelativeLocation
   UParticleSystemComponent::SavedAutoAttachRelativeRotation
   UParticleSystemComponent::SavedAutoAttachRelativeScale3D
   UParticleSystemComponent::SecondsBeforeInactive
   UParticleSystemComponent::SkelMeshComponents
   UParticleSystemComponent::SpawnEvents
   UParticleSystemComponent::Template
   UParticleSystemComponent::TimeSinceLastForceUpdateTransform
   UParticleSystemComponent::TimeSinceLastTick
   UParticleSystemComponent::TotalActiveParticles
   UParticleSystemComponent::WarmupTickRate
   UParticleSystemComponent::WarmupTime
   UParticleSystemComponent::TrailEmitterArray.

   #property
