

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


当你真正使用时,也还会有各种各样的问题,例如生成生成以及reference的更新等等.
https://forums.unrealengine.com/showthread.php?49057-UE-4-Python-Tools-from-Tragnarion

当你有各种各样的需求时,看看https://wiki.unrealengine.com/Category:Plug-ins 有没有现成插件可以用的.
或者论坛里看看,有什么合适的例如下面,有人做一个blueprint 共享库.
https://forums.unrealengine.com/showthread.php?3851-%2839%29-Rama-s-Extra-Blueprint-Nodes-for-You-as-a-Plugin-No-C-Required

以及各种常用尺寸，一般情况是1 unreal unit = 1cm,以及X,Y,Z等等的尺寸。
https://wiki.unrealengine.com/User_Submitted_Art_Specifications


