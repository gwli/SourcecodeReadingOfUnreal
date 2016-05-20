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
$(SdkAdbPath) -s $(AndroidDeviceID) push "F:\Unreal\Unreal_Projects\MyFirstPerson_4_10_1\Saved\StagedBuilds\Android_ASTC" "/sdcard/UE4Game/MyFirstPerson_4_10_1"
del /Q "C:\UnrealEngine-4.10\NVIDIA_Android_Lab_GDC2016\Binaries\Android\libUE4.so"
mklink /H  "C:\UnrealEngine-4.10\NVIDIA_Android_Lab_GDC2016\Binaries\Android\libUE4.so" "C:\UnrealEngine-4.10\Engine\Binaries\Android\UE4Game-Android-Debug-armv7-es31.so"

