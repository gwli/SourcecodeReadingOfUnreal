debugOnMetaMacro
================

Unreal中代码中有大量的GENERATED_UCLASS_BODY
如何查看其断点。

可以用命令:command:`-symbol-list-lines C:\UnrealEngine-4.10\Engine\Source\Runtime\Classes\Particles\EmitterCameraLensEffectBase.h`.
可以看到对应的 PC值与line号。

然后在Diassembly窗口，直接输入PC值，就知道对应的什么函数了。
