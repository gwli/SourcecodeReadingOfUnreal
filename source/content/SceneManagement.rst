D:\UE4_11\Engine\Source\Runtime\RenderCore\Private\RenderingThread.cpp


#. 开始  RenderingThreadMain
#. 调度
FRenderCommandFence.Wait用来同步，要gamethread来等renderingthread.

#. 结束 
Unreal 用八叉树来进行场景管理。
#. 传输， 通过 CreateNewobject然后把这个NewObject扔给render,并且这个对象都有对应GameThread类与Rendering类。
#. ShaderRendering.cpp:1050 CompareDrawingPolicy.
在进入渲染之前，还得从场景管理中抽取哪一部分进行渲染。 所以整个World里进行完PhySicas的之后，进入Rendering之前就要进行过滤了。

一个方面那就是viewport开始。

https://forums.unrealengine.com/showthread.php?103012-UE4-the-scene-management-method


https://docs.unrealengine.com/latest/INT/Programming/Rendering/Overview/index.html

在Unreal中是FScene,
UWorld,能够交互Actor与components
ULevel,相关一关
USceneCompoent, lights,methes,fogs.
UPrimitiveComponent
ULightComponent
FScene

场景管理的 14 pass
==================

#. GameThread - The GameThread handles updating gameplay. This includes ticking Actors, ticking components, performing garbage collection, etc.
#. RenderThread - The RenderThread handles performing lighting calculations, shadowing calculations, translucenecy setup, updating scene captures, occlusion, etc.



这个可以从 :command:`stat SceneRendering`.得到

#. Base pass drawing
#. Begin OcclusionTests
#. Depth drawing
#. Dynamic Primitive drawing
#. Dynamic shadow setup
#. FinishRenderingViewTarget
#. InitViews
   这个主要是visibity culling 操作，主要static/dynamic meshes.
#. LightingDrawing
#. Proj Shadow drawing
#. RenderVelocities
#. StaticDrawList drawing
#. RenderViewFamily
#. Translucency drawing


三种counter
-----------

#. Dynamic path draw calls
#. mesh draw calls
#. Present time
#. lights scene
#. Static list draw calls.


:command:`Stat SCENEUPDATE` 能得到 ogl的资源的更新状态，添加什么，改动什么。


#. Run Stat unit
#. stat scendering
#. stat sceneupdate
#. set breakpoint at AddLight. to see why and who do this slow.

Game Threading profiling
=========================

#. Blueprint Time
#. Finish Async Trace Time
#. GC Sweep Time
#. MoveComponent Time
#. Nav Tick Time
#. Net Post BC Tick Time
#. Net Broadcast Tick Time
   - Transform or RenderData
   - Recreate
#. Post Tick Component update
#. Queue Ticks
#. Reset Async Trace Time
#. Runtime Movie Tick Time
#. TeleportTo Time
#. GT Tickable Time
#. Tick Time
#. Update Camera Time
#. World Tick Time

counters
---------
#. Ticks Queued


GPU profiling
=============

两大关键点，pixel shading,这个是随着screen的大小而变的，另一个transform/skinning就是 vertex shadering了。 一般光照复杂一般是在gemotry shadering.
可以通过 r.ProfileGPU..ShowUI来看到。 
另外那就是通过viewmode查看 shader 复杂度与还有光照的复杂度。https://udn.epicgames.com/Three/GPUProfilingHome.html

viewmode 就像是PHOTOSHOP中通道一样。方便你查看场景的方方面面。
https://udn.epicgames.com/Three/ViewModes.html

#. EarlyZPass
#. Base Pass
#. Shadow map rendering
#. Shadow Projection/filtering
#. Occlusion culling
#. Defferred lighting
#. Tiled defferred lighting
#. Environment reflection
#. Ambient occlusion
#. Post processing

这些都会影响性能。减少一些不必要计算。这样才能节省时间，在这里没有什么办法可以计算更快。而更多的是
裁减。
https://docs.unrealengine.com/latest/INT/Engine/Performance/GPU/index.html
bottleneck是会随着scale的变化而变化的，这个不是一个静态的值，而工具分析是提供一个静态的值，例如GPU的bottleneck是会谁着你的pixel,vertex的数量的多少而变化的，在复杂的场景下，一个frame可能会成千个draw.而不是自己所想的那些一个draw来搞定一切了。

并且关于renderingSetting可以查看这个类。
https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/URendererSettings/index.html


GL command 调用方式
====================

Graphic 的代码都是采用元语言的方式来生成的，而不是直接调用。



各种ENQUE_UNIQUEXXX 都是用宏继承于FRenderCommand. RenderingThread.h:190

而各种RHICommandList 而在d:\UE4_11\Engine\Source\Runtime\RHI\Private\RHICommandList.cpp

并且对于帧数会有一个计数，GFrameNumberRenderThread,这样会知道相差几帧。

全局的计数器还有这些，都在Core.cpp中。

Rendering与Game之间的同步，static void GameThreadWaitForTask(const FGraphEventRef& Task, bool bEmptyGameThreadTasks = false)
并且在FAppEventManager是时不是pauseRendering.
明白了，所有RHICommandList 就是所有OPENGL/Dx的wraper了。

想要添加一些新功能，就要通过RHI， APP->RHI->OPENGL/DX.

GL函数封装可以在,D:\UE4_11\Engine\Source\Runtime\Core\Private\Linux\LinuxPlatformSplash.cpp 里打到例如


//////////////////////////////////
// the below GL subset is a hack, as in general using GL for that

// subset of GL functions
/** List all OpenGL entry points used by Unreal. **/
#define ENUM_GL_ENTRYPOINTS(EnumMacro) \
	EnumMacro(PFNGLENABLEPROC, glEnable) \
	EnumMacro(PFNGLDISABLEPROC, glDisable) \
	EnumMacro(PFNGLCLEARPROC, glClear) \
	EnumMacro(PFNGLCLEARCOLORPROC, glClearColor) \
	EnumMacro(PFNGLDRAWARRAYSPROC, glDrawArrays) \

生成函数让TaskGraph来执行。


并且动态mesh与静态的mesh要分开的，最简单的原理，采用两个buffer来分别放，一个static,一个dynamic.
对于静态的只需要修改Indexbufffer来进行drawcall,而动态的则需要glbufferData来更新数据本身。

现在的做法把场景中mesh按类型进行分类，在画的时候，可以使用一个drawCall来画的尽可能多，来减少
Overhead. 同样DLL一样，GL的操作本身并不提供这些管理工作，而是需要APP自己来管理的。
Unreal是通过场景管理来进行分类管理。具体可以参考。
DyanmicPrimitiveDrawing.h 与 DyanmicPrimitiveDrawing.inl 这个文件，你就会看到大量的firstIndex,lastIndex.

ThreadRendering
================

开始于PreInit的，StartRenderingThread,然后放进ThreadGraph中。
所谓的Runnable也就是指的有线程在里面。


FRenderResource
===============

所有图片，纹理都会提前加载显存里，如何存放呢，在编译的时候会事先计算好需要多少，然后并区分是否变化。来决定如何存放的问题。
例如所有静态的图都可以放在一个buffer里，动态的放在另一个里面。
然后每一个FRenderResource里都会记载放在哪一个buffer里，并且位置在哪里，同时有什么属性。

对于OGL来说，有VBO，VAO。就是为复杂场景准备的。
具体可以看 d:\UE4_11\Engine\Source\Runtime\RenderCore\Public\RenderResource.h。
也是一个静态的全局变量的单向列表。也就是后插，只记其head. 但看代码，又像是用两级链表实现了一层单链。而是用双向实现了一个单向，把prevLink当做一个备份而己。

.. code-block:: cpp
   
    /* @return The global initialized resource list. */
    static TLinkedList<FRenderResource*>*& GetResourceList();


这里采用 GetResourceList则是返回链表的头。
另外局部是在 private:  TLinkedList<FRenderResource*> ResourceLink; at line:111  of d:\UE4_11\Engine\Source\Runtime\RenderCore\Public\RenderResource.h

而资源之间的相互依赖，而用引用来解决，例如

.. code-block:: cpp
    
   FTextureReferenceRHIRef TextureReferenceRHI;

并且由BeginInitResource提交到RenderThread来进行初始化。
如何查看unreal的宏展开呢。
首先是生成一个class EURCMacro_##TypeName : public FRenderCommand
然后再创建一个task来执行这个命令类。

.. code-block:: cpp

   #define TASK_FUNCTION(Code) \
   		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) \
   		{ \
   			FRHICommandListImmediate& RHICmdList = GetImmediateCommandList_ForRenderCommand(); \
   			Code; \
   		} 

资源的加载过程
==============

在构造函数中你只是提交你要加载的资源的通过BeginInitResource提交request的申请。
而由LoadPackageInternal来处理其依赖。
Unreal中所有资源的读取都是通过FArchive来实现的。所有资源都是UObject封装过的，所以还要先经过
BeginInit_gameThread,来初始对象，然后再处理图片资源。
所以你在其构造函数看这些部分。

并且Unreal所有资源的读取都是从FArchive中实现的，Unreal的中这个ar采用的是文件结构呢。

.. code-block:: cpp

   BeginInitResource(FRenderResource * Resource) Line 130	C++
   FTextureReference::BeginInit_GameThread() Line 178	C++
   UTexture::UTexture(const FObjectInitializer & ObjectInitializer) Line 70	C++
   UTexture2D::UTexture2D(const FObjectInitializer & ObjectInitializer) Line 23	C++
   UTexture2D::__DefaultConstructor(const FObjectInitializer & X) Line 11	C++
   InternalConstructor<UTexture2D>(const FObjectInitializer & X) Line 2513	C++
   StaticConstructObject_Internal(UClass * InClass, UObject * InOuter, FName InName, EObjectFlags InFlags, EInternalObjectFlags InternalSetFlags, UObject * InTemplate, bool bCopyTransientsFromClassDefaults, FObjectInstancingGraph * InInstanceGraph) Line 2794	C++
   FLinkerLoad::CreateExport(int Index) Line 3836	C++
   FLinkerLoad::CreateExportAndPreload(int ExportIndex, bool bForcePreload) Line 2781	C++
   FLinkerLoad::LoadAllObjects(bool bForcePreload) Line 2918	C++
   LoadPackageInternal(UPackage * InOuter, const wchar_t * InLongPackageName, unsigned int LoadFlags, FLinkerLoad * ImportLinker, TSet<FName,DefaultKeyFuncs<FName,0>,FDefaultSetAllocator> & InDependencyTracker, IAssetRegistryInterface * InAssetRegistry) Line 1135	C++
   LoadPackageInternal(UPackage * InOuter, const wchar_t * InLongPackageName, unsigned int LoadFlags, FLinkerLoad * ImportLinker) Line 1258	C++
   LoadPackage(UPackage * InOuter, const wchar_t * InLongPackageName, unsigned int LoadFlags) Line 1272	C++
   ResolveName(UObject * & InPackage, FString & InOutName, bool Create, bool Throw) Line 695	C++
   StaticLoadObjectInternal(UClass * ObjectClass, UObject * InOuter, const wchar_t * InName, const wchar_t * Filename, unsigned int LoadFlags, UPackageMap * Sandbox, bool bAllowObjectReconciliation) Line 782	C++
   StaticLoadObject(UClass * ObjectClass, UObject * InOuter, const wchar_t * InName, const wchar_t * Filename, unsigned int LoadFlags, UPackageMap * Sandbox, bool bAllowObjectReconciliation) Line 852	C++
   ConstructorHelpersInternal::FindOrLoadObject<UFont>(FString & PathName) Line 31	C++
   ConstructorHelpers::FObjectFinder<UFont>::FObjectFinder<UFont>(const wchar_t * ObjectToFind) Line 104	C++
   AVehicleAdvanced_CHud::AVehicleAdvanced_CHud() Line 19	C++
   InternalConstructor<AVehicleAdvanced_CHud>(const FObjectInitializer & X) Line 2512	C++
   UClass::CreateDefaultObject() Line 2763	C++
   UObjectLoadAllCompiledInDefaultProperties() Line 728	C++
   ProcessNewlyLoadedUObjects() Line 818	C++
   FEngineLoop::PreInit(const wchar_t * CmdLine) Line 1443	C++
   EnginePreInit(const wchar_t * CmdLine) Line 31	C++
   GuardedMain(const wchar_t * CmdLine, HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, int nCmdShow) Line 110	C++
   WinMain(HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, char * __formal, int nCmdShow) Line 189	C++
    [External Code]	

FObjectInstaningGraph
=====================


Cooking resource
================


shader compiling
----------------

VehicleAdvanced_C-Win64-Debug.exe!VerifyGlobalShaders(EShaderPlatform Platform, bool bLoadedFromCacheFile) Line 284	C++
VehicleAdvanced_C-Win64-Debug.exe!GetGlobalShaderMap(EShaderPlatform Platform, bool bRefreshShaderMap) Line 599	C++
VehicleAdvanced_C-Win64-Debug.exe!FEngineLoop::PreInit(const wchar_t * CmdLine) Line 1421	C++
VehicleAdvanced_C-Win64-Debug.exe!EnginePreInit(const wchar_t * CmdLine) Line 31	C++
VehicleAdvanced_C-Win64-Debug.exe!GuardedMain(const wchar_t * CmdLine, HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, int nCmdShow) Line 110	C++
VehicleAdvanced_C-Win64-Debug.exe!WinMain(HINSTANCE__ * hInInstance, HINSTANCE__ * hPrevInstance, char * __formal, int nCmdShow) Line 189	C++
[External Code]	
d:\UE4_11\Engine\Source\Runtime\Engine\Private\GlobalShader.cpp


#.
Level Profiling
===============

https://udn.epicgames.com/Three/LevelOptimization.html

对于一个东东，首先要搞清楚它是哪里计算的，例如lighting来说，找到哪些物体需要lighting,并且需要什么类型的lighting,这个是CPU的来计算的，真正的lighting本身的计算是放在GPU的shader中来计算的。

例外查看:command:`stat threading` 来看thread的IDEL time就知道，gamethread与renderingthread之间的速度匹配的程度。

而GPU本身，瓶颈一般是shader throughput,以及屏幕的分辨率。


Lightmaps 的使用可以减轻CPU与GPU的load. 是不是用lightmaps, 是在primtive 的属性上来设置的(UseDirectLightMap or bForceDirectLightMap).

所有特效都是以performance为基础的，例如dynamic shadow对性能影响还是很大的。


另一个性能的影响那就是static Meshes以及基本primtives.

Materials,采用viewmode来看， 越绿说明复杂度越低。
You can easily see this cost in the Shader Complexity Mode. Bright Red = 300 instructions, pink = 600 instructions, and white >= 900 instructions. Press Alt+8 to view shader complexity on PC.
https://docs.unrealengine.com/latest/INT/Engine/Rendering/ParticleSystems/Optimization/Results/index.html
并且对于level的profiling可以细化到每一个primitive，再细化就像修改原始primitive的设计了。


同时各种Cull Distance Volumnes的设置是为了优化计算。


ContentProfiling
================

https://udn.epicgames.com/Three/ContentProfilingHome.html

#. 删除那些不会用到 或者重复的资源
#. 替换掉很少用到资源 
#. 优化用到次数最多的资源，例如mesh的尽可减少其vertex.
#. 对于只用很少部分资源想办法拆分，只加载用到部分，尤其是对anim 资源有效。
