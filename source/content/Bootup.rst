Unreal的起动模式，是并行化，先把所有的resource加载，然后设置，event事件。
PreInitModules
==============
#. basic
   - Engine,Render,AnimGraphRuntime
   - Landscape,ShaderCore
#. platformSpecic
    
StartupCoreModules
==================

#. Core,Networking,Messaging,SessionServices
#. Slate,SlateReflector,UMG,...

FEngineLoop::PreInit
====================

这里主要处理 命令行参数，以及environment 设定，例如Workingspace，游戏的名字 等等。

#. malloc profiler proxy 也是这里启动的。
#. LogConsole 
#. 把当前线程注册成GameThread
#. 初使化数学库的随机数中种子
#. 初使化TaskGraphSystem,以及线程的profiling,并在后面初始化线程池
#. loadCoreModules()
#. GThreadPool  GlobalThreadPool
#. InitGamePHys()
#. RHIInit

#.  游戏资源的加载是ProcessNewlyLoadedUobjects() 

FEngineLoop::Init
=================
#. 判断是editor还是game.
#. MovePlayer
#. InitTime
#. load AutomationWorker
#. GEngine::Init
#. PostEngineInit

UGameEngine::Init
==================
#. UEngine::Init
#. Load and apply user game settings
#. Create GameInstance
#. Initialize VIewport
#. StartGameInstance


AndroidMain
===========

#. wait for JVM thread
#. change the fd_limit
#. Setup Joystick support with dlopen("libandroid.so")
#. Setup Keymap
#. wait for java activity OnCreate to Finish
#. Initialize file system access
#. GEgnineLoop.PreInit
#. InitHMDs
#. EEngineLoop.Init()
#. deal while loop for tick until done.
   - FAppEvent Ticket
   - GEngineLoop.Tick
#. exit
#. 初使化RenderingEngineVarCache
#. loadPreInitMOdules()
#. 读配置文件/Scripts/Engine.xxSetting
#. 创建SlateApplcation
#. 初使化 RHIInit
#. InitGamePhys
#. RHIPostInit
#. GEngineInit
#. RunAutomation Smoketests



LoadMap
=======

#. SpawnAllLocalPlayer
#. SetPlayer中会来处理 键盘mapping,以及一些事件callback都是在这里设置的。每一个player都可以不一样。


FEngineLoop::Tick
=================

#. Profilier FrameSync
#. 更新ConsoleVariable
#. ENQEUE_UNIQUE_RENDER_COMMAND
#. UpdateTimeAndHandleMaxTickRate
#. TickPFSChart
#. Update memory allcator stats
#. FStats::AdvanceFrame
#. Handle some per-frame tasks on the rendering thread,主要是DeferredUpdateRendering
#. GEngine->Tick
#. ShaderCompiling
#. GDistanceFieldAsyncQueue->ProcessAsyncTasks
#. Tick Slate application
#. ClearPendingStatGroups
#. FModuleManager::GetModuleChecked<IAutomationWorkerModule>(AutomationWorkerModuleName).Tick()
   这就是Hot compilable里的实现
#. RHITick
#. Find the objects which need to be cleaned up the next frame
#. Cleanup
#. FTicker::GetCoreTicker().Tick
#. FSingleThreadManager::get().Tick()
#. GEngine->TickDeferredCommands();
#. ENQUEUE_UNIQUE_RENDER_COMMAND, ENDFrame
#. Check for async platform hardware survey results
#. Set CPU utilization stats
#. Set the UObject count stat


UGameEngine::Tick
=================

#. Tick the modulemanager  //HotReload->Tick()
#. updateViewPort
#. Update subsystems
#. Begin ticking worlds
#. Tick all travel and Pending NetGames(Seamless,server,client)
#. UpdateSkyCaptureContents
#. UpdateReflectionCaptureContents
#. Tick the world
#. Issue cause event 
#. Tick the Viewport
#. Render everything //RedrawViewports();
#. Update Audio
#. Rendering Thread commands 
   ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER
#. Tick animation recorder


UWorld::Tick
============

All child actors are ticked after their owners have been ticked.

TickGroup
#. UMETA Pre Physics
#. Start Physics
#. During Physics
#. End Physics
#. Post Physics
#. Fost Update Work
#. Last Demotable
#. Newly Spawned


Steps

#. UpdateWorld's subsystems (NavigationSystem for now)
#. BeginFrame
#. Doing ActorTicks
#. PrePhysics
#. CollisionTreeBuild
#. RunTickGroup(TG_startPhysics)
   - call TaskGraph to do the work.
#. TickAsyncWorks
#. WaitForAsyncWork
#. TickableObject->Tick
#. Update cameras and streams volumes
#. EndFrame
#. Update profiler data
#. UpdateSpeedTreeWind
#. FXSystem->Tick ??
#. garbageCollection

