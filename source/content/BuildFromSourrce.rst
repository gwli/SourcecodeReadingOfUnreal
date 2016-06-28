准备工作
========

#.  硬盘空间至好在100G以上，编译之后的中间文件以及binary，还有sample是很吃硬盘的。
#.  VS2013 与VS2015，最好都要装
#.  如果要android支持，最好 CodeWorks把这些安装好，如果自己手工搭android的环境。要手工设置这些环境变量
    *ANDROID_HOME,JAVA_HOME,ANT_HOME,NDKROOT*. 
    可以参考 `Unreal android Development Reference <https://docs.unrealengine.com/latest/INT/Platforms/Android/Reference/>`_


配置说明
========

#. Debug 调试引擎与以及游戏代码
#. DebugGame 调试游戏
#. Development 该配置等同于发布
#. Shipping 发行，该配置在设置后可达到最佳性能，并且并能发布，剥离了控制台命令行，统计数据和分析工具。
#. Test 该配置就是启动一些控制台命令，统计数据和分析工具后shipping的配置。

文件命名规则   UE4Editor-Platform-configuration.extension与UE4-Platform-Configuration.extension

下载源码
========

#. 从https://github.com/EpicGames/UnrealEngine/tree/4.12 下载代码。 建议直接下哉zip包要比git clone快很多。


Android Launch/Deploy
=====================
如果每一次用都editor太浪费时间。
#. 安装apk
#. push 文件

   .. code-block:: python
   adb.exe -s TNS0000002194000590a shell "echo $EXTERNAL_STORAGE"
   adb.exe -s TNS0000002194000590a uninstall com.YourCompany.VehicleAdvanced_C
   adb.exe -s TNS0000002194000590a install "D:\UE4_11\Unreal_Projects\VehicleAdvanced_C\Binaries/Android\VehicleAdvanced_C-Android-Debug-armv7-es2.apk"
   adb.exe -s TNS0000002194000590a shell rm -r /sdcard/UE4Game/VehicleAdvanced_C
   adb.exe -s TNS0000002194000590a push "D:\UE4_11\Unreal_Projects\VehicleAdvanced_C\Saved\StagedBuilds\Android_ASTC" "/sdcard/UE4Game/VehicleAdvanced_C"
   adb.exe -s TNS0000002194000590a shell rm /sdcard/obb/com.YourCompany.VehicleAdvanced_C/main.1.com.YourCompany.VehicleAdvanced_C.obb
   adb.exe -s TNS0000002194000590a shell "echo 'APK: 5/13/2016 10:13:32 AM' > /sdcard/UE4Game/VehicleAdvanced_C/APKFileStamp.txt"
   adb.exe -s TNS0000002194000590a shell am start -n com.YourCompany.VehicleAdvanced_C/com.epicgames.ue4.SplashActivity
   
    

如果只要生成的生成工程只要在LaunchPreparation Commandline 加入。

$(SdkAdbPath) -s $(AndroidDeviceID) shell rm -r /sdcard/UE4Game/MyFirstPerson_4_10_1
$(SdkAdbPath) -s $(AndroidDeviceID) push "F:\Unreal\Unreal_Project\MyFirstPerson_4_10_1\Saved\StagedBuilds\Android_ASTC" "/sdcard/UE4Game/MyFirstPerson_4_10_1"
del /Q "C:\UnrealEngine-4.10\NVIDIA_Android_Lab_GDC2016\Binaries\Android\libUE4.so"
mklink /H  "C:\UnrealEngine-4.10\NVIDIA_Android_Lab_GDC2016\Binaries\Android\libUE4.so" "C:\UnrealEngine-4.10\Engine\Binaries\Android\UE4Game-Android-Debug-armv7-es31.so"


Edtor deploy APK steps
======================

只要有log与callstack就很容易找到问题的原因。

Program.Main: ERROR: Exception in mscorlib: Could not find a part of the path 'D:\NVPACK\android-ndk-r11c/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/libgnustl_shared.so'.
Stacktrace:    at System.IO.__Error.WinIOError(Int32 errorCode, String maybeFullPath)
   at System.IO.File.InternalCopy(String sourceFileName, String destFileName, Boolean overwrite, Boolean checkHost)
   at UnrealBuildTool.Android.UEDeployAndroid.CopySTL(String UE4BuildPath, String UE4Arch, String NDKArch, Boolean bForDistribution) in C:\UnrealEngine-4.10\Engine\Source\Programs\UnrealBuildTool\Android\UEDeployAndroid.cs:line 597
   at UnrealBuildTool.Android.UEDeployAndroid.MakeApk(String ProjectName, String ProjectDirectory, String OutputPath, String EngineDirectory, Boolean bForDistribution, String CookFlavor, Boolean bMakeSeparateApks, Boolean bIncrementalPackage, Boolean bDisallowPackagingDataInApk) in C:\UnrealEngine-4.10\Engine\Source\Programs\UnrealBuildTool\Android\UEDeployAndroid.cs:line 1795
   at UnrealBuildTool.Android.UEDeployAndroid.PrepForUATPackageOrDeploy(String ProjectName, String ProjectDirectory, String ExecutablePath, String EngineDirectory, Boolean bForDistribution, String CookFlavor, Boolean bIsDataDeploy) in C:\UnrealEngine-4.10\Engine\Source\Programs\UnrealBuildTool\Android\UEDeployAndroid.cs:line 1940
   at AndroidPlatform.Deploy(ProjectParams Params, DeploymentContext SC) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\Android\AndroidPlatform.Automation.cs:line 548
   at Project.Deploy(ProjectParams Params) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\Scripts\DeployCommand.Automation.cs:line 27
   at BuildCookRun.DoBuildCookRun(ProjectParams Params) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\Scripts\BuildCookRun.Automation.cs:line 214
   at BuildCommand.Execute() in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\AutomationUtils\BuildCommand.cs:line 35
   at AutomationTool.Automation.Execute(List`1 CommandsToExecute, CaselessDictionary`1 Commands) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\AutomationUtils\Automation.cs:line 395
   at AutomationTool.Automation.Process(String[] CommandLine) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\AutomationUtils\Automation.cs:line 369
   at AutomationTool.Program.MainProc(Object Param) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\Program.cs:line 134
   at AutomationTool.InternalUtils.RunSingleInstance(Action`1 Main, Object Param) in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\AutomationUtils\Utils.cs:line 708
   at AutomationTool.Program.Main() in C:\UnrealEngine-4.10\Engine\Source\Programs\AutomationTool\Program.cs:line 53
ProcessManager.KillAll: Trying to kill 0 spawned processes.



如何添加自己的库
================

例如GOOGLEPLAY Service,这个是在主要是修改在UEDeployAndroid.cs这个文件，相当库都给cp到 Intermediates中，然后调用 ndk,ant来进行编译。
其本质过程，构造对应的结构，来调用相应的命令来就行了。
https://forums.unrealengine.com/showthread.php?3504-Android-Java-Libraries-in-UE4-Game-%28OUYA-SDK-Google-Play-Game-Services-etc
例如java的库放在，Engine/Build/Android/Java/Libs
其实现在做法是放在，Engine>Extra下面，然后去hack Deploy过程去东西copy过去，不想改engine代码，直接在自己一.cs里实现一下copy就行了。


另外添加一些地方库，也就是添加头文件与库路径的问题，修改编译选项。
https://wiki.unrealengine.com/Linking_Static_Libraries_Using_The_Build_System 同时能够添加把脚本语言给加进来，已经有人把Javascript给
放进来了，https://forums.unrealengine.com/showthread.php?254-Linking-V8-(JavaScript)-to-UE4

缓存数据
========

正确的使用缓存数据可以大大地加快自己速度，因为Unreal中build 与cooking是很费时的。
如何正确的使用。https://docs.unrealengine.com/latest/CHN/Engine/Basics/DerivedDataCache/index.html

原则，能共享就共享，不能共享就重新生成，而是copy来copy去。


Content only Project
====================

应该指是那些纯blueprint的项目吧，而UE4game.exe 就像一个解析器一样。
