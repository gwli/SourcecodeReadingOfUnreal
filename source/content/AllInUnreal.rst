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
