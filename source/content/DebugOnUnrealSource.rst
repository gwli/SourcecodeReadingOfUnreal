debugOnMetaMacro
================

Unreal中代码中有大量的GENERATED_UCLASS_BODY
如何查看其断点。

可以用命令:command:`-symbol-list-lines C:\UnrealEngine-4.10\Engine\Source\Runtime\Classes\Particles\EmitterCameraLensEffectBase.h`.
可以看到对应的 PC值与line号。

然后在Diassembly窗口，直接输入PC值，就知道对应的什么函数了。



Symbol Debugger
================

Unreal支持从服务上下载symbols,如果自己编译source的话，这个工具默认是不编译的，自己手工编译。
https://docs.unrealengine.com/latest/INT/Programming/Development/Tools/SymbolDebugger/index.html#methodselection。 

相当于帮你做初步的分析。


如何精确控制场景呢
===================

可以用skookumscript 来进行. 或者自己扩展一个shell这样来加Unreal 的console窗口.
