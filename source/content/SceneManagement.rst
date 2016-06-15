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
