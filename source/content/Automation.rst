采用UnrealMessageBus来进行执行的。就像UNIT testing的接口一样。
D:\UE4_11\Engine\Plugins\Messaging\UdpMessaging\Source\UdpMessaging\Private\Transport\UdpMessageProcessor.cpp 
D:\UE4_11\Engine\Source\Runtime\AutomationWorker

并都是FAutomationTestBase为基础的。
这个结构那就是test name，与类型

在每一个类的Private\Tests

是一个独立的线程，并且也自己的tick函数，所以能够进行测试，只要游戏正在运行。
提交了之后就会在下一个tick里开始执行。 


对于test都是具有基本的结果检查的功能，FAutomationTestBase:TestTrue/False

console命令
===========

Unreal提供大量的console命令可以使用，例如暂停，以加载等等。还些统计数据，同时还可以这些信息dump到文件。
而这些命令的参考在这里。
http://blog.csdn.net/pizi0475/article/details/47323063
并且是可用ULocalPlayer::Exec()来调用的。

命令行执行
==========

:command:`UE4Editor.exe GAMENAME -Game -ExecCmds="Automation RunAll"`
http://stackoverflow.com/questions/29135227/how-do-you-run-tests-from-the-command-line


具体的场景可以用 editor的命令行参数来搞定。
https://docs.unrealengine.com/latest/INT/Programming/Basics/CommandLineArguments/

各种log的位置
=============

https://wiki.unrealengine.com/Locating_Project_Logs#Game_Logs


开始
====

SetForceSmokeTests 是FEngineLoop.AppInit中。
然后是初始化AutomationWorker.
FModuleManager::Get().LoadModule("AutomationController");
	FModuleManager::GetModuleChecked<IAutomationControllerModule>("AutomationController").Init();

然后就是tick它。

再然后是
开始于



helpUtils
=========

Latent Commands,是可以跨frame的，自己实现update,然后自己判断开始与结束。
