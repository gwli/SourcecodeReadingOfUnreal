
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
