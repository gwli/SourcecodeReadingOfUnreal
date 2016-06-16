Damage
=======
https://www.unrealengine.com/zh-CN/blog/damage-in-ue4
每一个actor都可以继承一个TakeDamage的函数，而由AppDamage,ApplyPointDamage,ApplyRadicalDamage 来形成一个damage call chain.  而这些都是由
c:\UnrealEngine-4.10\Engine\Source\ThirdParty\PhysX\APEX-1.3\module\destructible\public\NxDestructibleActor.h

各种各样的damage都是从
C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Classes\Components\DestructibleComponent.h 
C:\UnrealEngine-4.10\Engine\Source\Runtime\Engine\Private\Components\DestructibleComponent.cpp
得到的。并且后台使用的APEX来实现的。每一个actor都可以自身响应这个destructible event.
Apply Radial Damage
https://docs.unrealengine.com/latest/INT/BlueprintAPI/Game/Damage/ApplyRadialDamage/index.html
并且可以添加channel来进行过滤，哪些毁，哪些不毁。

同时爆炸也利用这个类型来实现。同时再加上一些粒子系统来实现。
https://answers.unrealengine.com/questions/224372/how-to-apply-radial-damage-to-all-actors-in-the-ar.html

Damage type
https://docs.unrealengine.com/latest/INT/BlueprintAPI/Game/Damage/index.html


https://answers.unrealengine.com/questions/224372/how-to-apply-radial-damage-to-all-actors-in-the-ar.html



Pickup
======

都是通过固定的事件序列来产生，如果将来VR中的事件就比较难定义了。
http://unrealtutorials.com/unreal-engine-tutorials/unreal-engine-4-tutorials/pickups-items-weapons/

同时还得设定一个Colllision属性为了得到正确的行为。
https://www.newbiehack.com/ShowVideoClip.aspx?id=3000

同时还得进行一些特效的处理，这样就得添加AppPostProcessing函数的内容，以及Focus的处理。以及主从关系的处理。
http://expletive-deleted.com/2015/06/17/item-pickup-system-in-unreal-engine-using-c/
同时用一些debug函数划线实现一些特效。

把其放在tick中，并且:command:`InputCompoent->BindAction` 来绑定事件。
https://docs.unrealengine.com/latest/INT/Gameplay/Input/index.html
非累积式用action,累积式用axis.
https://www.unrealengine.com/blog/input-action-and-axis-mappings-in-ue4

各种物理运动
============

可以采用其Physics constraint,或者直接力学仿真，就像汽车的轮子一样。 需要复杂的方程计算。

例如轨道旋转 https://forums.unrealengine.com/showthread.php?92190-How-to-rotate-an-actor-around-another-actor

或者简单的不需要物理计算，只是简单的动画KR之类的依赖。 直接使用仿射变换方程就行了。
