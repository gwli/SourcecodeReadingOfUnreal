

Features
=========

常见技术参数:

#. API 一般有DX 11/12, OGL,OGLES,vulkan.

#. Texture Resolution 材质解析度
#. Quadcore-Multithread 多核心多线程
#. Tesselation 曲面线分
#. Deferred shading 延迟渲染
#. Live Create 编辑器动态创建
#. Full-Dynamic Realtime shading 全动态实时阴影
#. Direct illumination (直接光照)
#. indirect illumination 
#. Global illumination
#. Local illumination 局光映射
#. Photon mapping 光子映射贴图
#. Raytracing 光线追踪
#. Radiosity 实时辐射照明，计算发光用
#. bouncing light  
#. sub-surface scatterring
#. Translucency shader
#. Normal map
#. parallax occlusion 视差映射贴图
#. Displacement map 硬件位移置换图
#. megatexture 顶点贴图
#. Multi-sample 多重采样
#. Alphato coverage
#. shadow map
#. instancing 点缓存系统
#. Gizmo particle 容积粒子
#. Volume Shader 
#. Vertex Animation
#. Character dynmaics



4.12 现在支持 VR editor,以及sequencer用法。sequencer用法，保存不是静态二维数据，而一个三维场景。



如何制作一个夜晚明月的场景 
==========================

​https://www.youtube.com/watch?v=0YpZlUzkQVk



CalcAABB
=========

>	VehicleAdvanced_C-Win64-Debug.exe!UPhysicsAsset::CalcAABB(const USkinnedMeshComponent * MeshComp, const FTransform & LocalToWorld) Line 120	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkinnedMeshComponent::CalcMeshBound(const FVector & RootOffset, bool UsePhysicsAsset, const FTransform & LocalToWorld) Line 731	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkeletalMeshComponent::CalcBounds(const FTransform & LocalToWorld) Line 1394	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USceneComponent::UpdateBounds() Line 913	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkeletalMeshComponent::PostBlendPhysics() Line 438	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkeletalMeshComponent::PostAnimEvaluation(FAnimationEvaluationContext & EvaluationContext) Line 1337	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkeletalMeshComponent::RefreshBoneTransforms(FActorComponentTickFunction * TickFunction) Line 1262	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkeletalMeshComponent::InitAnim(bool bForceReinit) Line 370	C++
 	VehicleAdvanced_C-Win64-Debug.exe!USkeletalMeshComponent::OnRegister() Line 309	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UActorComponent::ExecuteRegisterEvents() Line 1094	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UActorComponent::RegisterComponentWithWorld(UWorld * InWorld) Line 871	C++
 	VehicleAdvanced_C-Win64-Debug.exe!AActor::IncrementalRegisterComponents(int NumComponentsToRegister) Line 3828	C++
 	VehicleAdvanced_C-Win64-Debug.exe!ULevel::IncrementalUpdateComponents(int NumComponentsToUpdate, bool bRerunConstructionScripts) Line 806	C++
 	VehicleAdvanced_C-Win64-Debug.exe!ULevel::UpdateLevelComponents(bool bRerunConstructionScripts) Line 739	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UWorld::UpdateWorldComponents(bool bRerunConstructionScripts, bool bCurrentLevelOnly) Line 1328	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UWorld::InitializeActorsForPlay(const FURL & InURL, bool bResetTime) Line 3048	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UEngine::LoadMap(FWorldContext & WorldContext, FURL URL, UPendingNetGame * Pending, FString & Error) Line 9809	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UEngine::Browse(FWorldContext & WorldContext, FURL URL, FString & Error) Line 8945	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UGameInstance::StartGameInstance() Line 339	C++
 	VehicleAdvanced_C-Win64-Debug.exe!UGameEngine::Init(IEngineLoop * InEngineLoop) Line 529	C++
 	VehicleAdvanced_C-Win64-Debug.exe!FEngineLoop::Init() Line 2185	C++
 	VehicleAdvanced_C-Win64-Debug.exe!EngineInit() Line 41	C++
 	VehicleAdvanced_C-Win64-Debug.exe!GuardedMain(const wchar_t * CmdLine, HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, int nCmdShow) Line 136	C++
 	VehicleAdvanced_C-Win64-Debug.exe!WinMain(HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, char * __formal, int nCmdShow) Line 189	C++
 	[External Code]	


Physical Simulation
===================

Unreal的物理引擎用的是nvidia的Physix,并且各种动画的连接，也就是Joint,不过在Unreal中叫Physical Contraint. Unreal只是做了一个转接，
把这些都交给了物理引擎。但是为了减少计算量，没有用原来那个mesh网格来做仿真的计算，如果直接那些mesh网格，就成有限元分析了。运算量是巨量。
重新用一些简单长方体，球体，圆柱体来进行近似计算了。 

一般Physical 都是建立在skeletal mesh 上的。



Precomputed 
===========

在设置属性的，可用通过PrecomputedVisualVolumes来设置哪些东东是可以提前计算好的，也就是放在cooking阶段就计算的。
同时也可以通过:command:`start initviews` 来查看。 

当然也可以通过 :command:`start openglrhi` 来查看。


Editor技巧
==========

keyboardshortcut设置在https://docs.unrealengine.com/latest/INT/Engine/UI/KeyBindings/index.html，并且应该可以不同theme,例如使用3Dmax的习惯，或者blender的习惯。
blueeditor的一张表在https://cdn2.unrealengine.com/blog/BlueprintCheatSheet-1989117414.pdf
https://docs.unrealengine.com/latest/INT/Engine/Blueprints/UserGuide/CheatSheet/index.html
关键是Ctrl+F 的搜索功能。


plugin
=======

用法
#. put into /<Engine>/Plgins/Runtime or <YourProjects>/Plugins/
#. build 
#. Run editor 
#. check it in puglin list

当你真正使用时,也还会有各种各样的问题,例如生成生成以及reference的更新等等.
https://forums.unrealengine.com/showthread.php?49057-UE-4-Python-Tools-from-Tragnarion

当你有各种各样的需求时,看看https://wiki.unrealengine.com/Category:Plug-ins 有没有现成插件可以用的.
或者论坛里看看,有什么合适的例如下面,有人做一个blueprint 共享库.
https://forums.unrealengine.com/showthread.php?3851-%2839%29-Rama-s-Extra-Blueprint-Nodes-for-You-as-a-Plugin-No-C-Required

BLUI 插件
https://forums.unrealengine.com/showthread.php?58192-PLUGIN-BLUI-Open-Source-HTML5-JS-CSS-HUD-UI-Release-1-0!
以及各种常用尺寸，一般情况是1 unreal unit = 1cm,以及X,Y,Z等等的尺寸。
https://wiki.unrealengine.com/User_Submitted_Art_Specifications

TextureMoviePlugin
https://github.com/Ehamloptiran/TextureMoviePlugin 实现就如http://www.jianshu.com/p/291ff6ddc164,用于VR 非常方便

当然你如果添加自己的脚本,可以考参scriptplugin,它已经生成公共的接口,让你很方便嵌入任何脚本语言.例如
Lua.
https://forums.unrealengine.com/showthread.php?3958-Scripting-Language-extensions-via-plugins

https://github.com/enlight/klawr,用C#来做gameplay logic scripts.

一个更强的组件,那是runtime mesh generate,当然这个性能可能是问题,但是这个就为内容的动态演化提供了可能,尤其在虚拟现实中
真实会更强.
https://forums.unrealengine.com/showthread.php?113432-Runtime-Mesh-Component-Rendering-high-performance-runtime-generated-meshes!


AI in Game
==========

Unreal 中一个专门的AI插件SkookumScript. 可以直接操作人物.
https://forums.unrealengine.com/showthread.php?25379-SkookumScript-Plug-in

其中一个重要的項目那就是nvmesh的生成,如果是自己做游戏,可以用recastvaigation来做.
https://github.com/gwli/recastnavigation
或者使用这些工具来生成,例如Kynapse | Autodesk Gameware

导航网格的生成主要用是计算几何以及A*算法.

AI 的sourcecode 在c:\UE4_12\Engine\Source\Runtime\AIModule\Private\Navigation\PathFollowingComponent.cpp

在项目中有四处,AI System,Collision,Navigation Mesh,Navigation System.
navmesh要动态生成要在这里配置,默认是静态生成的.

>	[0x55449F4C] UPathFollowingComponent::UPathFollowingComponent(UPathFollowingComponent * this, const FObjectInitializer & ObjectInitializer) Line 36	C++
 	[0x55545E64] UPathFollowingComponent::__DefaultConstructor(const FObjectInitializer & X) Line 115	C++
 	[0x555310E0] InternalConstructor<UPathFollowingComponent>(const FObjectInitializer & X) Line 2543	C++
 	[0x53B2E72C] UClass::CreateDefaultObject(UClass * this) Line 3340	C++
 	[0x53698658] UClass::GetDefaultObject(UClass * this, bool bCreateIfNeeded) Line 2196	C++
 	[0x53C54338] UObjectLoadAllCompiledInDefaultProperties() Line 728	C++
 	[0x53C53CC0] ProcessNewlyLoadedUObjects() Line 818	C++
 	[0x53687E40] FEngineLoop::PreInit(FEngineLoop * this, const TCHAR * CmdLine) Line 1498	C++
 	[0x53681C20] FEngineLoop::PreInit(FEngineLoop * this, int32 ArgC, TCHAR ** ArgV, const TCHAR * AdditionalCommandline) Line 694	C++
 	[0x53681188] AndroidMain(android_app * state) Line 286	C++
 	[0x53683CCC] android_main(android_app * state) Line 453	C++
 	[0x536BF0DC] android_app_entry(void * param) Line 233	C++
 	[0x4070DC28] libc.so!__pthread_start(void*)()	C++
 	[0x406E8294] libc.so!__start_thread()	C++
 	??()	C++


跳起的处理
>	[0x55A554E4] UCharacterMovementComponent::SetMovementMode(UCharacterMovementComponent * this, EMovementMode NewMovementMode, uint8 NewCustomMode) Line 804	C++
 	[0x55A68BA0] UCharacterMovementComponent::SetPostLandedPhysics(UCharacterMovementComponent * this, const FHitResult & Hit) Line 4777	C++
 	[0x55A68970] UCharacterMovementComponent::ProcessLanded(UCharacterMovementComponent * this, const FHitResult & Hit, float remainingTime, int32 Iterations) Line 4750	C++
 	[0x55A6243C] UCharacterMovementComponent::PhysFalling(UCharacterMovementComponent * this, float deltaTime, int32 Iterations) Line 3595	C++
 	[0x55A5CB5C] UCharacterMovementComponent::StartNewPhysics(UCharacterMovementComponent * this, float deltaTime, int32 Iterations) Line 2355	C++
 	[0x55A5AD24] UCharacterMovementComponent::PerformMovement(UCharacterMovementComponent * this, float DeltaSeconds) Line 1931	C++
 	[0x55A56BC8] UCharacterMovementComponent::TickComponent(UCharacterMovementComponent * this, float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) Line 1076	C++
 	[0x55A24E80] FActorComponentTickFunction::ExecuteTick(float, ELevelTick, ENamedThreads::Type, TRefCountPtr<FGraphEvent> const&)::$_12::operator()(float) const(const class {...} * this, float DilatedTime) Line 700	C++
 	[0x55A14B6C] FActorComponentTickFunction::ExecuteTickHelper<FActorComponentTickFunction::ExecuteTick(float, ELevelTick, ENamedThreads::Type, TRefCountPtr<FGraphEvent> const&)::$_12>(UActorComponent*, bool, float, ELevelTick, FActorComponentTickFunction::ExecuteTick(float, ELevelTick, ENamedThreads::Type, TRefCountPtr<FGraphEvent> const&)::$_12 const&)(UActorComponent * Target, bool bTickInEditor, float DeltaTime, ELevelTick TickType, const class {...} & ExecuteTickFunc) Line 2888	C++
 	[0x55A14A24] FActorComponentTickFunction::ExecuteTick(FActorComponentTickFunction * this, float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef & MyCompletionGraphEvent) Line 698	C++
 	[0x564C0984] FTickFunctionTask::DoTask(FTickFunctionTask * this, ENamedThreads::Type CurrentThread, const FGraphEventRef & MyCompletionGraphEvent) Line 261	C++
 	[0x564C05CC] TGraphTask<FTickFunctionTask>::ExecuteTask(TGraphTask<FTickFunctionTask> * this, TArray<FBaseGraphTask*, FDefaultAllocator> & NewTasks, ENamedThreads::Type CurrentThread) Line 999	C++
 	[0x53718A70] FBaseGraphTask::Execute(FBaseGraphTask * this, TArray<FBaseGraphTask*, FDefaultAllocator> & NewTasks, ENamedThreads::Type CurrentThread) Line 472	C++
 	[0x53719CA4] FNamedTaskThread::ProcessTasksNamedThread(FNamedTaskThread * this, int32 QueueIndex, bool bAllowStall) Line 930	C++
 	[0x53718DF0] FNamedTaskThread::ProcessTasksUntilQuit(FNamedTaskThread * this, int32 QueueIndex) Line 677	C++
 	[0x53716FB8] FTaskGraphImplementation::ProcessThreadUntilRequestReturn(FTaskGraphImplementation * this, ENamedThreads::Type CurrentThread) Line 1725	C++
 	[0x53717394] FTaskGraphImplementation::WaitUntilTasksComplete(FTaskGraphImplementation * this, const FGraphEventArray & Tasks, ENamedThreads::Type CurrentThreadIfKnown) Line 1774	C++
 	[0x564BBB94] FTickTaskSequencer::ReleaseTickGroup(FTickTaskSequencer * this, ETickingGroup WorldTickGroup, bool bBlockTillComplete) Line 529	C++
 	[0x564B5750] FTickTaskManager::RunTickGroup(FTickTaskManager * this, ETickingGroup Group, bool bBlockTillComplete) Line 1434	C++
 	[0x55DEEE5C] UWorld::RunTickGroup(UWorld * this, ETickingGroup Group, bool bBlockTillComplete) Line 703	C++
 	[0x55DF0184] UWorld::Tick(UWorld * this, ELevelTick TickType, float DeltaSeconds) Line 1197	C++
 	[0x55C3F9E0] UGameEngine::Tick(UGameEngine * this, float DeltaSeconds, bool bIdleMode) Line 1040	C++
 	[0x53682E84] FEngineLoop::Tick(FEngineLoop * this) Line 2769	C++
 	[0x536812F4] AndroidMain(android_app * state) Line 326	C++
 	[0x53683CCC] android_main(android_app * state) Line 453	C++
 	[0x536BF0DC] android_app_entry(void * param) Line 233	C++
 	[0x4070DC28] libc.so!__pthread_start(void*)()	C++
 	[0x406E8294] libc.so!__start_thread()	C++
 	??()	C++


解决碰撞回避两种,一种是Unreal实现简单的办法(bUseRVOAvoidance)void UCharacterMovementComponent::SetAvoidanceEnabled(bool bEnable来使用,那一种Recast的修改来实现的.可以通过继承 
UCLASS(BlueprintType)
class AIMODULE_API UCrowdFollowingComponent : public UPathFollowingComponent, public ICrowdAgentInterfac
来使用. 具体用法可以参考https://wiki.unrealengine.com/Unreal_Engine_AI_Tutorial_-_2_-_Avoidance

具体动作都是这里进行的
=======================
void UCharacterMovementComponent::StartNewPhysics(float deltaTime, int32 Iterations)
{
	if ((deltaTime < MIN_TICK_TIME) || (Iterations >= MaxSimulationIterations) || !HasValidData())
	{
		return;
	}

	if (UpdatedComponent->IsSimulatingPhysics())
	{
		UE_LOG(LogCharacterMovement, Log, TEXT("UCharacterMovementComponent::StartNewPhysics: UpdateComponent (%s) is simulating physics - aborting."), *UpdatedComponent->GetPathName());
		return;
	}

	const bool bSavedMovementInProgress = bMovementInProgress;
	bMovementInProgress = true;

	switch ( MovementMode )
	{
	case MOVE_None:
		break;
	case MOVE_Walking:
		PhysWalking(deltaTime, Iterations);
		break;
	case MOVE_NavWalking:
		PhysNavWalking(deltaTime, Iterations);
		break;
	case MOVE_Falling:
		PhysFalling(deltaTime, Iterations);
		break;
	case MOVE_Flying:
		PhysFlying(deltaTime, Iterations);
		break;
	case MOVE_Swimming:
		PhysSwimming(deltaTime, Iterations);
		break;
	case MOVE_Custom:
		PhysCustom(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogCharacterMovement, Warning, TEXT("%s has unsupported movement mode %d"), *CharacterOwner->GetName(), int32(MovementMode));
		SetMovementMode(MOVE_None);
		break;
	}

	bMovementInProgress = bSavedMovementInProgress;
	if ( bDeferUpdateMoveComponent )
	{
		SetUpdatedComponent(DeferredUpdatedMoveComponent);
	}
}
背后采用的Apex来实现,从void UCharacterMovementComponent::NotifyJumpApex()是可以看到的.



如何建模
========

#.　使用建模仿真软件生成高模.　然后导出.
#.　再blender等变成低模,并且模块化.　例如选一模导出一下.　然后删除一些再导出一些组件.
#.　再导入Unreal.　进行进一步的开发.
例如https://www.youtube.com/watch?annotation_id=annotation_1089853839&feature=iv&src_vid=bero-JBTAX8&v=Ux_zJ4WJbZg
