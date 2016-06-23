游戏组成
========

游戏的规则的设置，就是gameplay. 例如胜与败的规定。

Actor,在我的理解，只要能进行图形的各种仿射变换的都可以作为actor.

Pawn, 可以是World的中任意agent,而Character则是一个人物化agent.
http://docs.unrealengine.com/latest/INT/Gameplay/Framework/QuickReference/index.html



Tutorial
========

.. csv-table:: 

  `epicgames video tutorial <http://udn.epicgames.com/Three/VideoTutorials.html#Accessing the Videos>`_ 
   pdf  , see sina share folder 
  `beyond unreal<http://www.beyondunreal.com/>`_ , open and free UDN 
   https://www.unrealengine.com/blog?category=Tutorials , 
   https://docs.unrealengine.com/latest/CHN/Engine/QuickStart/6/index.html ， "大量的中文在线文档了"

一些特殊的名词
==============

VT， View Target
Ipv, light propagation volumes
ds,  delay shader

How to Compile for Android
--------------------------

#. 打开unreal的工程，并且添加code,然后，在配置里选上,Tegra-Android就可以了，实际上就是原来工程里一个external-buildsystem的配置文件。这样就可以直接debugger.
#. 如何添加c++代码，File->Add Code to Project, 然后就可以生成sln的工程，并且自动engine工程给你加载上去。你就可以直接debug.
#. 当然你可以直接编译，File->Package for Android 在buildConfig的输出路径不能有空格不然会编译失败。然后就会生成apk文件，并且还有安装脚本。
#. 具体如何手工来配制，可以用pentak的在线文档来做。

VS只是用来编辑代码而调试而用。而编译既可以在VS中进行，也可以在unrealEd中直接来编译。整个unreal引擎就像一个OS,非常的庞大，各种启动的。不过按照操作系统的模式来看，同时再加入游戏本身的特征就很容易理解了。

启动选项的控制，可以通过命令行参数，或者.ini 文件来控制，这就就像linux的etc目录，那就是工程的config目录。

Core
====

Unreal 的核心分为几部分

#. Async 这样是为多线程提供了尽可能的支持。
#. Containers 那些基本的数据结构，array,list,hash等等，queue.
#. Delegates 回调，函数指针的实现
#. Features
#. GenericPlatform 各种跨平台的操作
#. HAL  主要是对文件系统抽象 
#. Internationalization 主要是多语言的支持
#. Logging 主要是log与debug信息的提示。
#. Math 基本数学库
#. Misc 
#. Modules 主要是Hotload 的实现。
#. ProfilingDebugging 这个是自己核心与重点。
#. Serialization 也就是对象如何保存的问题。
#. Stats 各种统计信息类似于 /proc/
#. Templates 模板系统
#. UObject 自身的对象系统。 树形对象系统 + 设计模式就实现各种各样的功能。并且避免单树的各种问题。 https://docs.unrealengine.com/latest/INT/API/Runtime/Core/index.html 并且自身的反射系统都是靠这个UOjbect来实现的，以及后面垃圾回收等等都是依靠这些。

ENGINE_API 这些宏是用于生成DLL的export/import.
https://docs.unrealengine.com/latest/INT/Programming/Modules/API/index.html
只是减少了你的编码工作量，而没有什么magic word.

Blueprints editor
-----------------

Gameplay Foundations
--------------------

UObject 提供了基本对象的生成，采用了has-a的模式来生对象，并且支持所有一些基本功能，例如ＲＴＴＩ,以及等等。
UCLASS 利用可变参数宏来实现来实现元编程。因为软件工程的大的时候，重复是不可避免的，这个时候我们就要元编程了，因为没有重复不存pattern，就不存在复用了。复用的基础也就是重复。出现重复可以通过一定的方式来简化处理，或者代码的重复工作可以通过设计模式或者元编程来解决这个问题，而haskwell以及scheme最适适直接把词法分析与语法分析直接结合在一起。因为不管是词法分析还是语法分析都是可以看做流数据，并当做列表来处理，也就是不同格式的列表而己。还是那个思维，不断的迭代来进行处理。事不过三。一生二，二生三，三生万物。

而Unreal也同样利用基于宏处理的元编程技术实现了editor与Ｃ＋＋可以复用代码。要想两者一致，就必须把宏写好了。如果只是为能工作，就可能没有这么严格了，对于编程来说就三种变量，函数，以及流控。
同样unreal也实现类似的机制，作为中间语言来控editor的引擎共同使用。并且实现了C语言中所有基本元素。

.. graphviz::
   
   digraph {
        
     Gameplay-> Functions;Properties;
     structs;
     Interfaces;
   }

对于这个基本上UOjbect就包含整个引擎的所有接口，例这个函数入口就是用来驱动来联接engine的时间片中断函数的。engine的FEngineLoop:Tick()会调用所有对象的tick函数，来实现每一个时间片的更新。
*LoopEngine->tick()*
entryPoint
所以每一个UWorld.tick()就会调用APlayerController::Ticks,TickActorComponents,USequence::updatedOP.
并且更新过tick之后，更新RedrawViewPorts,
每一个APlayerController:tick会调用AActor::ProcessEvent->UObject::ProcessInternal->UObject::execFinalFunction->UObject::CallFunction 就是相当于python的函数执行的实现。
并且unreal采用的是多线程并行的机制 可以看到BaseEngine.ini看到有这么多的线程。

正是这样的tick的树形更新机制，就实现了，只要动态把UObject attach 到其父对象上，然后就建立从属的更新关系。在unreal中就自然更新了其内容。

+GameThread=Async Loading Time
+GameThread=Audio Update Time
+GameThread=FrameTime
+GameThread=HUD Time
+GameThread=Input Time
+GameThread=Kismet Time
+GameThread=Move Actor Time
+GameThread=RHI Game Tick
+GameThread=RedrawViewports
+GameThread=Script time
+GameThread=Tick Time
+GameThread=Update Components Time
+GameThread=World Tick Time
+GameThread=Async Work Wait
+GameThread=PerFrameCapture
+GameThread=DynamicLightEnvComp Tick

并且通过这样的封装把对内存的管理，垃圾处理，对象的创建管理都封在里面了。在外面表现就像一般的Ｃ＋＋的ＯＯ编程一样，例如什么时候关卡的资源给扔，都这样巧无生息的给做了。


这个UOjbect主要解决了gc的回收与reference自动更新的机制这一部分主要涉及内存的管理，以及串行化的处理，以及状态的更新还有与editor的集成，以及网络化的复制等等。首先要知道有哪些东东要做。后面的事情就自然好办了。以及运行时的ＲＴＴＩ的处理。
Gameplay Timmer的处理
游戏的各种时间的同步，最小的同步时间片那就是frame率，其他处理速度都要此基础了，也就是一帻内要把系统的所有状态更新一次。如何实现扩散性更新，每一次都更新自己相关的部分，这样才接近人类社会本身的运作。

系统本身的时间，以及每一个人物的生长时间，以及离线时间如何来计算。不断的动态加入后的时间都如何计算呢。根据生活常识来呗，可以每一个物体都一个自己的生物钟，也每一个UObject都有一个timer,并且对应的callback函数更新各自的timmer,例如在每一个tick函数中更新自己的timmer.并且客观世界本身也采用分级的机制。


object 再往下分，actor,再往下pawn/character, 是不是多线程取决于内容本身是否支持joint-fork模式，而unreal自身是支持这些。不相关的事情完成可以并行。

Unreal Build System，这个把build与元编程都集成在一起了，现在所谓通用的gradle不正是提供了unreal build system这一套，并且还多一套元编程系统，这套编程系统把整个engine封装起来了，这样提供了最灵活的输入与输出的控制，并且给了最大的灵活度，因为生成代码与真实使用代码都是c++所以它们之间互通性不成问题。只需要少量接口定义。

各个领域的可视化例如simulink等等可以大大加快whatif的速度，并且tradering也专门有这样的语言。


`ThreadRendering <https://docs.unrealengine.com/latest/INT/Programming/Rendering/ThreadedRendering/index.html>`_ 一般情况下，render thread会比game thread慢一两个frame,不然的话，就停一下来等。另外这种循环的情况下，数据输出是怎样的，如何保持的呢，例中gameThread已经走到了四，但是reader才走到了2，那么状态3保存在哪里。是每对象自己保存吗。


并且自身也都实现进程的并行，直接用for all/any 来进行判断。每一个进程的结果。这些build环境找东西，要么是环境变量，要么是注册表。所以只要简单的搜索就可以相应的东东，并且找到解决方案。



World
=====

游戏中World,都包含哪些设置，并且与现实世界。其中一个就是生死的边界，例如离中心多远就算死亡，应该消失了。
还有死了多久，尸体要从画面中消失。 并且其大小坐标原点。

在unreal 尺寸大小uu,一般1uu = 1cm

https://www.youtube.com/watch?v=oBU4Qqf67k0
 Editor Preferences > General > Appearance: Display Units on Numerical Properties, which is set to True by default. This shows the cm, m, or km units on applicable values. If you enter 100 while it is set to cm, it will update to 1 m. If you set it to 0.01 while in m, it will convert to 1 cm. You can disable this property to remove units, and it values will remain in cm without auto-converting.

并且World的最大值为2097152 这个是在EngineDefines.h 中定义的。


游戏的map
=========

分为行走层，地表层，透明层，三张图表示一种地图。而unreal完成是用物体拼出来的，而不是二维的图形整出来的。

每一个物体的组成
================

#. transform 这里坐标信息，以及缩放，大小等信息。
   并且要指定是静态，还是动态的。

#. Static Mesh 那就是mesh数据了。
        mesh的数据结构顶点，与边的连线数据。在保存的时候，这两项也是可分开来保存的。各种建模工具输出也是围绕这个的。
#. Materials, 就是那些纹理贴图的数据了
#. Physics, 其物理属性，例如质量，阻力（静阻力，角摩擦力）。并且哪一个方向不变形等。
#. Collision, 产生击中事件，产生重叠事件等。
#. Lighting ， 采用静态光，还是动态光，以及光源的设置
#. Rendering，可见不可见，可以减少render的负担，
#. Tags, 主要用来分组，当需要批量处理的时候，很方便过滤。
#. Actor, 每一个物体都是actor, 这里指定actor类型会不会改变，是不是可以销毁，产生方法，以及初使
#. Navigation，是不是有引导网络可以
#. LOD 

同时在运行时可修改的粒度也应该是到此，对于动画细节等等的修改，就是游戏运行前期就应该完成的事情。


unreal的继承关系  UObject->AAtor.  
UObject 主要的属性是引用技术，主要是内存管理的一些属性。
AActor 最基本的属性那就是可以被放置与生成。(placed or spawned). 所以坐标信息应该在这一层。同时可以用API.chm 有一个像MFC的框架,所以在需要的时候直接查在整个树中哪一块。
同时Actor还可以有 component, 这个可以从documentation中Engine 中Components中看到，看到这种组件模式。 ScenceComponent可以变形与attachment,也就是join,但是没有渲染以及碰撞的功能。
各种刚体动画的传导就这样这里实现的。当然这里包含大量的设计模式。

资源的加载顺序，是要根据你所绑定的的对象来决定的，

Blueprint 
==========

相当于unreal的脚本，它支持unreal中大部分扩展工作。
例如对于mesh的操作，对于已经结构的mesh有大量可操作的api,例如计算法线，切线等等。只要输入输出的格式固定。
例如三角形像四边形的转化， 创建网络，创建 box mesh 等等。 也是从最基础点线面的操作来的。
https://docs.unrealengine.com/latest/INT/BlueprintAPI/Components/ProceduralMesh/index.html

engine 自身的API查询。可以用来扩展editor.
https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/index.html

同时Unreal强大的一点，那就是支持asset以及blueprint的版本控制，支持perforce,svn,git. 并且集成很好，还可以diff.
对于blueprint的diff就像自己的那个xmldiff一样，不过是可视化的。

一般版本控制化,Unreal 会把这些资源转化成text来存储。
https://www.unrealengine.com/blog/diffing-blueprints
https://www.unrealengine.com/blog/diffing-unreal-assets

How To rotate an actor

游戏的交互
==========

一种是来自于输入设备的，这一类的交互采用的observe模式就可以得的，在每一个ticket中自己去查询这些事件。
例如基于事件的事件，也应该这样的，

其中一部分是通过系统的碰撞检测来实现的。 那系统中一个物体如何操作其他物体呢，例如拾起的这样的动作。

应该是有一个全局的状态变量，叫做类似于context之类的名字，并且每一个actor,都有attachParent,以及Child
还有attachTo 的函数。应该就是通过这些来实现的吧。


一种简单的交互方式，就是 AMyChractor中直接有相关的类变量。例如Tarray<weaponList>等等。而是经过空间
管理查询。

另一种那就要空间的管理的查询了。

例如子弹的生成就是通过World->SpawnACtor<AmyFProjectFIle>(ProjecttileClass,SpawnLocation,SpawnRotation)
来生成子弹的。

World就是所谓的context, 可以通过 UWorld * const World = GetWorld()得到。
World里的所有东东信息，
例如 Add/DestroyActor,
TArray<TAutoWeakObjectPtr> <class AController> > ControllerList;
PlayerControllerList;
PawnList;
AutoCameraActorList;
NoneDefaultPhysicsVolumneList;
PhysicsScene;

每一个物体都有一个独立的属性，你交互操作只是要在World里添加与修改某一个操作，然后由系统来维护其更新。

还有各种damage,就需要用到网络管理了，相当于直接某个结点为中心来进行查找，并且设置其状态。
但是这样的行为还是有些假，每一次的damage都是一样的，如何才能保证一样，这样需要动态把一个物体拆分开来，
这就要用到physX 这样的引擎来做这样的事情。

其实这也是一个集合操作。 原来在AActor这里有一个TakeDamage()这个函数，所以你要实现这些，就要继承这个函数就行了。
https://www.unrealengine.com/blog/damage-in-ue4

原来每一个Actor怎么死是由自己决定的。

碰撞检测
========

ECB 

.. code-block::
   
   enum ECanBeCharacterBase {
      /* Charactor cannot step up onto this component*/
      ECB_NO,
      /* Cheractor can step onto this compoent*/
      ECB_YES<
      ECB_Owener,
      ECB_MAX,
   }



Brush
=====

就些类似PS中笔刷，形状什么的也是可以定制的，不过也需要特定的模式下边来进行编辑。

Precedual mesh
--------------

https://docs.unrealengine.com/latest/INT/BlueprintAPI/Components/ProceduralMesh/index.html


并且都在一个actor之内，不过actor之间又分为不同的类型，
static,SkeletalMesh,GeometryBrushActor,Volume Actor,Trigger Actor,Fog Effects,Target Point Actor,
Actor Mobility, Player Start,Camera Actor. 

特别是Volume虽然不可见但是可以产生效果。 并且为了容易扩展，可以直接每一个类型里加一个标记，一种作法
遍历一遍然后查其tag,另外一种高效的方法，在生成actor时，就直接加入各种队列。无非是在这个类里面深入队列信息。

.. code-block:: 
   global queue {
       header;
       current;
       previous;
   }
   struct TriggerActor {
       TriggerActor * current;
       TriggerActor * previous;
       TriggerActor * next;
   }
   class Actor {
       TriggerActor triggerQueueEvent;
       TriggerQueueEvent * ele; 
       ele->previous = globalqueue->current;
       ele->current = this ;
       globalqueue->previous = globalqueue->current;
   }


这样系统里就可以很方便的各种各样的队列了。遍历起来也很方便。只需要遍历队列自己的内容，而不需要全部遍历。

现在明白blender与Unreal之间的关系与区别了， Blender更多的是提供从顶点画出物体，当然也向上实现一些功能。例如动画，
以及粒子系统。 而Unreal更多的是提供现成的物件，进行组合大的场景。Blender更向从原料到砖的过程，而Unreal更是由砖及柱到房子的过程。


现在明白了，只要把现在gameworks 中sample 中mesh加载，以及光照参数都变成参数开放出来，其实就是unreal的一个类。并且再加物理数据。对于如何render一个画面，实现一个特定效果，然后注册，也就是直接继承，并编译之后，就可以在Unreal中直接使用了。就加载一个lib一样。



Module加载顺序
==============

内核实现是https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Modules/FModuleManager/LoadModule/index.html 这里。
https://answers.unrealengine.com/questions/257451/load-order.html?sort=oldest

动态的加载，也就是把linux ld.conf类似功能实现一遍。

路的实现
========

其实路与其他物体是一样，只是显示的不同。显示与物理属性是独立计算的。碰撞与寻径的都是独立计算的，并且更新状态，最后只是显示而己。

路的区别就在于自己特殊的物理属性，但是例如上坡的等的事情是如何实现的呢。

filesystem
==========

unreal 中数据是如何存放，那些resource 是一次性直接读与入内存，还是放在硬盘上，还是用到什么读什么。采用的注册式的，在第一次使用的时候加载。 下回使用的时候，新查有没有。 另外一个重要一方面，那就是流式与异步加载这样hiden latency的问题。但是默认的操作系统并不提供这样的支持。
https://docs.unrealengine.com/latest/INT/Programming/Assets/ReferencingAssets/index.html
FindObject<>(), if has already been loaded or created.  LoadObject(), if it not already loaded.

https://docs.unrealengine.com/latest/INT/Programming/Assets/Registry/index.html

有自己的文件系统，一个是可以FPaths,来与操作系统相互，并指定资源都在哪里。然后用FObjectFinder 模板类，来去内存寻找，如果没有加载加载，并生成对应的类。
物体包括自己mesh,自身的materials,其实每个物体都有自己一整套东东，然后用opengl把它给自己给画出来。

对于各个物体来说，引擎本身也会进行各种各样裁剪。 例如游戏中各个物体的属性，相互关系的改变与交互。不同的玩法都需要不同的交互方式与玩法，


对于unreal 来说，就是其自身content management,包括， copy/move/delete,以及相关链接的更新等。 以及路径的更新移动，以及资源的查找。以及路径的一些操作函数，都在FPath这个模块中。https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Misc/FPaths/index.html  把它看做是一个 user_Mode special linux,这样会更容易理解一些。
https://answers.unrealengine.com/questions/179149/please-i-beg-you.html 已经有人报怨这些了。修改多了，就fix 一把。利用
https://docs.unrealengine.com/latest/INT/Engine/Basics/Redirectors/index.html  就是如何解决引用记数的问题，另一方便那就是尽量的减少移动文件的开销。

在Unreal界面做的那些关联动作，其实也就是赋值过程，同时也大部分现在逻辑与图形模块的关联关系，很大部分那就是glUnifi那些参数如何传递，由谁来传递。另外是这些参数如何变化。 其实有些过程整个过程也就顺了。


Material
========

就是要解决光照问题，什么材质具有什么样效果，而texture是一种简化，或者法向texture以及切线texture是material的另一种表达，或者是光照计算的中间过程。
其实设置材质，也就是规定了shader怎么写的过程，其实也就是写shader的过程。

并且对于texture,Unreal是按照优先级排序放在内存池里，如果内存不够的情况下，就会被stream out 到硬盘上，可以通过:command:`stat STREAM` 来查看。
https://docs.unrealengine.com/latest/INT/Engine/Content/Types/Textures/Streaming/index.html
同时也可以把设置成发光源，但是真实光源的区别是不能照亮动态的物体，原因在于这些预先 cooking的。
https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/HowTo/EmissiveGlow/index.html


另外真实的那就是PBR了。https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/PhysicallyBased/index.html


See also
========

#. `NSight&#95;Tegra UE3&#95;Setup <https://wiki.nvidia.com/engwiki/index.php/NSight&#95;Tegra/UE3&#95;Setup>`_  study it how to setup
#. `游戏开发入门 <http://www.cnblogs.com/tuyile006/archive/2007/05/10/741444.html>`_  至少两年
#. `how-do-i-make-games-a-path-to-game-development <http://www.gamedev.net/page/resources/&#95;/technical/game-programming/how-do-i-make-games-a-path-to-game-development-r892>`_  
#. `ureal faq <http://www.gamefaqs.com/pc/39722-unreal/faqs/29537>`_  
#. `gamedev <http://www.gamedev.net/page/index.html>`_  
#. `BSP <http://blog.csdn.net/pizi0475/article/details/6269068>`_  
#. `GettingStartedOverview.html <http://udn.epicgames.com/Three/GettingStartedOverview.html>`_  把这个当做unreal的开始
#. `Getting Started with the Unreal Development Kit Part 1 - UDK Tutorial <http://www.youtube.com/watch?v&#61;OcJ3J2Yf&#95;Wg>`_  youtube video tutorial.k
#. `dmlally&#95;unrealSeries&#95;interfaceOverview <http://www.replay.drexel.edu/classes/DIGM361/UnrealLevels/dmlally&#95;unrealSeries&#95;interfaceOverview.pdf>`_  



-- Main.GangweiLi - 26 Jul 2013


对于一个系统的调试，需要信息，那就是应用程序本身，以及相关的符号库以及源码，并且能够有机能把这些东西关联起来，这就需要GDB了。并且其是如何可寻找符号与源码的。另外一个那就是应用程序自身的框架。哪是入口点，哪里是控制点。

-- Main.GangweiLi - 16 Aug 2013


*生成apk* 其实就像physX 一样只是它们打包到一块而己。自己调用c++的编译生成 .so文件，然后打包的时候把这个.so扔进去就可以了。也就是一个组装而己。现在自己的能力，已经可以随意的组装这些东东了。


unreal 内置的shell，直接在使用`就可以打开游戏的console，直接来动态调整游戏的参数。而这些参数的更新是在tick中不断的去更新的。


WinRTLaunch.cpp，GuardedMain这个函数整个系统的起动点，通过预处理来解决跨平台的问题，这个函数调用了EnginePreInit, 然后就是，EngineTick()的while循环了。
而在EnginePreInit 又向转向各个具体的eingine,并且加载初始化，例如editorEngine, standloneGameEngine,以及renderEngine这个是共用的。然后加加载各个模块。

并且unreal也采用的是多线程的全局队列的操作。
目前最至少知道两个renderThread以及GameThread,而tick的本身就是在GameThread中，并且render的部分在游戏的时候不变的，变化的也只有游戏gamelogic部分，这个部分是在GameThread中的ticket上更新的。 基本上每一个actor 都会有一个ticket函数。并且还有前后hook等等。

Render的过程，就是PreRender,Render,PostPrender,RenderList,你可以那一个draw放在一个frame中，到底frame中哪些drawcall,是由Engine通过空间管理的算法决定的。

而队列之间中同步，就需要大量的数据，哪些静态的数据是全局共享的，哪些是实时更新的。而这里是要在类里，进入队列就要进行深copy,与浅copy的区别了。大量的copy是费些时间的。如果能让一个类同时保持多个时间的值，就需要这样的大量的copy了，就只需要传递指针了。


UWorld.h
.. csv-table:: Gameworld
   item,content
   NavigationSystem,
   AutorityGameMode,
   BehaviorTreeManager,
   environmentQueryManager,
   AvoidanceManager,
   Levels, TARRAY,
   networkActors, TARRAY,
   

.. csv-table:: global structure
   
    TArray<TAutoWeakObjctPtry<class Acontrollder>> ControllerList;
    Tarray,PlayerControllerList;
    Tarray, PawnList;
    FPhysSence, PhysicsScence,
    Tset, ComponentsThatNeedEndOfFrameUpdate;
    Tset, ComponentsThatNeedEndOfFrameUpate_onGameThread;

    

FEngineLoop 


#. 游戏物体的update
#. Cull 阶段，这个阶段包括相机对特体的裁剪
#. 对可见物体的渲染分类
#. 那些可见并且依赖相机的更新物体进行update
#. 渲染

相互依赖的流程，那只能用ring buffer 来进行解决了。最简单的那就是double buffer,来进行切换，也就是最基本的生产消费者模型。但是这样做，就会延迟很多，如果流程太多的化。

关键是依赖关系的修改，特别是运动传递依赖的修改。

现在也终于明白，为什么C++ 一直在讲的类copy有什么用，并且深copy,与浅copy的区别。现在知道它们的速图了，在复杂的生产与消费者模型中就是直接用类。同时考虑如何设计类，哪些值是哪些索引。例如 运动矩阵就是值传递了，那些基本的mesh就是可以采用索引。可以节省空间，避免重复。
http://bbs.gameres.com/thread_202026_1_1.html
这里对unreal enter_queue有更深的认识。

整个方框采用是taskgraph的并行机制，主线程只是用分发任务。其实就类似于fork-join的功能，类似于 C#中system.thteading.tasks 的功能，只要跨平台，就得自己实现一个库了。

ENameedThreads, 采用命名thread 与匿名，命名有的，stat,GameThread,RenderingThred. 多个线程之间的同步工作，其中一个模式那就是生产消费者模式，两个队列，GameThread把需要数据放一个buffer,然后让Render来进行一步rendering,并且rendering本身为了提高效率。其实采用doublebuffer本身就是一种生产消费者模型。如果需要等待的话，就可直接采用sched_yield.来进行调度。

并且这个模型整个过程，过程 https://unionpaysecure.com/b2c/showCard.action?transNumber=201505142300359934972 ，为了进行优化rendering效率，哪些先画哪些后画，例如那些staticMesh可以画一次，然后就放在哪里。相当于文件系统的缓存了。 而unreal 采用的是宏定义的方式。进入队列与队列的方式了。 但是这两个buffer是如何保存的，采用与opengl类似的方式来进行的。只不过是不颗粒度的吗。 


Foliage 
https://docs.unrealengine.com/latest/INT/Engine/Foliage/index.html
默认的那些植物都是采用的instanced技术，也就是在硬件真正的画的时候都是用instanced技术，以及LOD一般都是采用tessellsation来实现。

人物的小动作，可以由前面physX以及就像Gameworks中computerShader来不断计算更新状态。并且类自己保存前状态，然后并计算更新。

http://www.cnblogs.com/dongbo/archive/2012/07/10/2585284.html
它几个callback接口，来决定pipline的操作，以及在哪一个队里的位置，例如render的函数，就在renderThread里调用，例如physicalcallback,就会被physical的callback所所用，并且这几个线程也有同步的机制，就像声音与图像要同步配套一样。这样与CUDA之间的同步是一样的，可以并行的并行，然后共在某一地方集结同步。


Log system
==========

https://wiki.unrealengine.com/Logs,_Printing_Messages_To_Yourself_During_Runtime.


平时可以用 Editor/Windows/Developer Tools>Message Log/Output log.
对于editor 中log 可以从，C:\Program Files\Epic Games\4.7\Engine\Programs\AutomationTool\Saved\Logs得到。

UE3 Build
=========

UE3 use UBT build.
#. change build options
#. build the UBT tools.
#. build Win7_64 release*mix platform.
#. build AndroidARM release mix platform.
#. build AndroidARM NonUnity Debug Tegra-Android.
#. Cooked the resource
#. Deploy and resource.
#. install the apk.


现在UE4中好的一点，c++代码，直接编译后，直接就可以加载，而不需要重起Editor。这一点很重要，省却很多事情。如果你想在Editor测试修改后内容，直接编译就可以游戏中看到效果了。PlayerStart,相当于pdb.set_step() 的功能一样，在你需要的地方来直接切面运行程序。 并且C++ 用来扩展这些UObject类的，发现engine本身或者UObject不能够满足你的要求时候，就需要继承新的类，然后新instance来都从你的类进一步继承。

File>New C++ Class 就可以添加对应的code,然后在添加注册信息在project settings中，例如生成什么时候用它。



DEBUG
=====

在起动的游戏的 添加 -LOG 选项就会打开一个console来显示log. 例如 :command:`myGame.exe -LOG`. 如果写入文件 :command:`myGame.exe -ABSLOG=F:\UE4.log` .  
https://docs.unrealengine.com/latest/INT/Programming/Basics/CommandLineArguments/index.html
当然也可以用 XXX.ini参数来启动。


在编译的时候，可以把symbolfile分开放的，在
或者直接进入console命令行，dumpconsolecommands就可以拿到命令。 
当然你可以在游戏中调用这些命令，那可以用 :command:`UWorld::Exec(..)` 来做，例如
:command:`GetWorld()->Exec(GetWorld(),TEXT("myAwesomeConsoleCommand X Y Z"))`

UObject
=======

Unreal 自己实现一套原语，例如 class->Uclass,Property->UProperty等等，通过实现封装这些基本的原语，有了blueprint与C++的通用，并且反馈机制，所以所有的管理都在这套原语实现的。

例如UClass::staticClass() 的定义https://forums.unrealengine.com/showthread.php?77052-UClass-StaticClass%28%29-definition

Uboject反射，以及圾圾回收，生命周期的管理，都是UObject带来的，并且采用元编程的模式，你只要设计元类，例如各种actor,但是何时生成instance,以及调用，都是gamemode利用spawn这样连接控制起来的。
现在明白了C++中内存泄漏，例如只在一个函数中有new,而没有delete,或者析构函数没有删除，或者已经在析构函数中，但是由于垃圾回收的机制，不适合也是出现这种问题。 对于Unreal中一般你是看到new/delete,而是封装在设计模式之下的,例如new->XX::StaticClass, 而delete 则由gc来决定了，你所决定的那就是选用哪一种引用技术更适合自己。
https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Objects/index.html
而unreal 中new都已经封装了设计模式在里面。例如。

.. code-block:: cpp
   
   XX::StaticClass,
   // Create a camera boom...
   CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));


虚幻的垃圾回收系统，基本上就是从Root开始，不断遍历所有的Property，标记其为使用中。最后再遍历一遍，确认哪些Object没有标使用中就给它删掉。基本上，你不需要管这个过程，因为反射的作用，所以相关的信息都是UE自动就帮我们处理好的。有几个要注意的：

"Singleton"，需要一直存在的，直接AddToRoot。

F类本身是不走垃圾回收的，但是F 类内部又有U类，这种情况下你需要注意AddReferencedObjects。把F类内部的U类给加入到GC树上。

另外的作用，那就是资源管理，所有类都是无类，相当于模板，所以动态加载，会容易一些。
是[类名']路径名/路径名/Asset名.[包内路径.]Object本名：[属性名][']（一般是Object所在类名+一个数字后缀）。

另外资源的move/copy/detele都要在content explor里进行，其就像  linux 的 /lib的依赖关系一样。
http://www.cnblogs.com/noslopforever/p/4058304.html。 每一个content自动记录依赖包地址，擅自改了之后就会出现找不到的。就需要重定向。

M——World-Actor、V——Player、C——Controller. 要尽可能遵守这个原则。


Trray
=====

array, size->num, capacity->Max, 即可以resize,也可以shrink array的大小。
https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/TArrays/
https://answers.unrealengine.com/questions/153801/tarray-question-about-push-pop-and-shrink.html

UE4 for PentaK
==============

#. build 

   :command:`"C:\Program Files\Epic Games\4.7\Engine\Build\BatchFiles\Build.bat" VehicleCPlus Android DebugGame "$(SolutionDir)$(ProjectName).uproject" -rocket`

#. rebuild

   :command:`"C:\Program Files\Epic Games\4.7\Engine\Build\BatchFiles\Rebuild.bat" VehicleCPlus Android DebugGame "$(SolutionDir)$(ProjectName).uproject" -rocket`

#. clean

   :command:`"C:\Program Files\Epic Games\4.7\Engine\Build\BatchFiles\Clean.bat" VehicleCPlus Android DebugGame "$(SolutionDir)$(ProjectName).uproject" -rocket`

#. Output

   :file:`..\..\Binaries\Android\VehicleCPlus-Android-DebugGame.so`


#. debug
  
    Override APK Path: F:\Unreal\Unreal_Projects\VehicleCPlus\Binaries\Android\VehicleCPlus-Android-DebugGame-armv7-es2.apk
    additional Library symbols Directories:
      F:\Unreal\Unreal_Projects\VehicleCPlus\Binaries\Android/../../Intermediate/Android/APK\obj\local\armeabi-v7a;
      F:\Unreal\Unreal_Projects\VehicleCPlus\Binaries\Android/../../Intermediate/Android/APK\obj\local\x86;
      $(AdditionalLibraryDirectories)
    GDB working directory: F:\Unreal\Unreal_Projects\VehicleCPlus\Binaries\Android/../../Intermediate/Android/APK

need add engine src:
   C:\Program Files\Epic Games\4.7\Engine\Source\Runtime\Launch\Private\Android
   C:\Program Files\Epic Games\4.7\Engine\Source\Runtime\Launch\Private
   C:\Program Files\Epic Games\4.7\Engine\Source\Runtime\CoreUObject\Private\UObject


这里只是build的apk,而resource的格式转换以及deploy都没有在这里，需要独立去做。 可以通过outlog, 分析得出。 unreal直接从内存中，保存文件
<project root>/Saved/Cooked/ANdroid_XDT/Engine/Content 中，同时完成格式的转换，这个也就叫cooking. 



Unral 的开发流程
================

#. 搭基本的、可玩的场景关卡，虚幻提供了非常强大的BSP编辑功能（Convex功能），可以用来进行关卡原型的制作。这个关卡只要可玩就好，一般由编关策划搭出来，基本都是些Box这样的基本物体，满足关卡的可玩性即可。

#. 替换一些主体模型，主体模型一般应该基于已经固化的玩法关卡来制作，不应改变玩法关卡的主体结构。

#. 打光、赋予主题场景的材质，把场景的大效果给弄出来。

#. 添加细节物体和细节表现。

Main Function init
==================

Config/.init

这里各种功能的使能，新类与老类名的mapping.


AndroidMain
-----------

#. GEngineLoop.PreInit;
#. InitHMDs() 头盔显示

UBT
===

它的方式是类似于Nsight integration的方式来做的。
整个编译都是C#来实现的，每一个包只要指令，其输入与输出，以及依赖哪些东东，以及编译的参数就可以了。每一个包下面都会一个build.cs 文件。

Android 就在 UBT Tool Chain/AndroidToolChain.cs.

UBT 要实现跨平台的操作。

#. 选择 toolchain.
#. build engine的使用
#. build的参数指定， input,output,options, dependencies.

#. buidConfiguration.cs 用来配置这些的。

disable incredibuild

.. code-block:: C#
   
   /* Whether XGE may be used.*/
   //public static bool bAllowXGE = Utils.GetEnvironmentVariable("ue3.bAllowXGE", true);


在UE4中 UBT是独立的program,是放在Engine/source/Program/UnrealBuildTool中，
对于各种功能生成放在 system/xxx.cs 下，例如 system/VCProjecct.cs
以及对各个平台的支持。
而对于 Android的支持而是放在 Android下，默认编译选项就都放在这里，如生成apk放在
UEDeployAndroid.cs/mkApk中。
采用做法是把  Engine/Android/Java 与Game/Build/Android的东东都copy到Intermediate/Android下面，然后调用android sdk的命令。
并且在copy的过程中会完成替换。
如果想修改Androidmanifest.xml 可以在这些地方放置好，也可以在Editor中Project Setting>Platforms-Android>Advanced APKPacking 设置各种属性。
一个标准的做法放在GameDir/Build/Android/AndroidManifest.xml 放里一个模板，editor会读取它，然后分析，并且可以Editor中添加与修改。

Rendering
=========

游戏引擎，对于硬件接口又做进一步的封装，为了扩平台，RHI(rendering hardware Interface). 例如opengl,以及DX等等。虽然的 rendering pipline是固定的流程，但是由于都是可编程管线，所以改变其定制流程了。并且可以vertex,gs,ts,pix等等阶段进行，并且标准的opengl提供的功能颗粒太小了。 所以有render engine 都会定制的pipline,例如 orge等，以及还有种离线的 rendering engine, 无非都是 vertex,gs,ts,T&L以及光栅化，后期处理，光照的计算是难点，是放在哪一个阶段实现呢。 并且加shader来进行计算。
https://docs.unrealengine.com/latest/INT/Programming/Rendering/ShaderDevelopment/index.html

同时自己想添加自己一个自己的插件，就像blender中那样，可以生成各种模型，并且加入到MODE中。 这就需要从前往后的之前。整个过程都是设计模式。你要在Vertex Factories,定义一个mesh type,并且指定一个shader type,然后定义一个scence proxy 与VertexFactory对应去调用相应的shader. 
https://forums.unrealengine.com/showthread.php?13131-Custom-shaders-and-editor-integration
具体实现方法
#. Extend FGLobalShader for VertexSahder and for your Pixel Shader
#. add DECLEARE_Shader_Type
#. IMPLEMENT_SHADE_TYPE
#. IMPLEMENT_SHADER_TYPE2 
https://answers.unrealengine.com/questions/40627/how-to-add-a-new-postprocessing-shader.html

并且为了保证线程安全，gamethread与 renderthread使用自己独立的数据结构。并且它们之间是有mapping关系，renderThread只需要mapping actor中那些vertex,mesh,matrial,light这些数据就行了，这个在进入队列的时候，就已经实现了复制交换了。 
gamethread与renderThread之间的传递是通过 ``ENQUEUE_RENDER_COMMAND`` 来实现的，也是通过生成类实现实现的。具体数据如何传递存放就像你的array 等等如何设置了。http://bbs.gameres.com/thread_202024_1_1.html 并且 Renderer Module是独立的，所以 EngineModule 与Renderer Module之间的对应关系在https://docs.unrealengine.com/latest/INT/Programming/Rendering/Overview/index.html.
例如
#. UWorld -> FScence
#. UPrimitiveComponent/FPrimitiveSceneProxy ->FPrimitiveScenceInfo
#. FScenceView -> FViewInfo
#. ULocalPlayer ->FScenceViewState
#. ULightComponent/FlightScenceProxy->FLightScenceInfo


并且光照特效在其支持api不能支持的话，就需要从底层直接实现支持了，这也正是公司帮游戏公司所实现的东东。

对于光照的处理，有三种基于顶点，前向rendering,以及deferred Rendering等等技术。 进行视角的投景就需要viewport,一旦这些固定了，后面的光照计算就可以固定的。

而常见的特效的实现方式只要gameworks 每一个sample都是一种特效的。

全动态，半动态，以及全静态。

Lit Translucency,bloom,shadow.

对于rendering线程还要FRenderResource::InitResource. 另外一个就是


如何把对象与mesh关联起来，并且在unreal中把任意的mesh都能够有class,当然手工制作很容易，它的各个属性，一个最简单的结构那就是通过骨骼动化，通过骨骼把与顶点关联起来。然后骨骼的参数就是对象一的部分。
同时在blender中一个物体的各个属性，mesh只是其中的一块属性而己。 通过组件组合的方式来进行。可以看看skinning中是如何实现的。


unreal 中数据是如何存放，那些resource 是一次性直接读与入内存，还是放在硬盘上，还是用到什么读什么。采用的注册式的，在第一次使用的时候加载。 下回使用的时候，新查有没有。
https://docs.unrealengine.com/latest/INT/Programming/Assets/ReferencingAssets/index.html
FindObject<>(), if has already been loaded or created.  LoadObject(), if it not already loaded.

https://docs.unrealengine.com/latest/INT/Programming/Assets/Registry/index.html

G-Buffer -> Geometry Buffer.

Object 交互
============

pickup object,从键盘的动作产生，然后判断距离，物体的重量，并且做出动作，然后更新graphic
https://wiki.unrealengine.com/Pick_Up_Physics_Object_Tutorial
每一个对象都是一个actor,只要拿到相邻关系，这些就都不是问题，如何快速判断各个actor之间的距离，这样那些destroy 也就都容易实现了。
这个过程也就是设置actor本身的一些属性。以及各个属性之间的依赖关系。
http://unrealtutorials.com/unreal-engine-tutorials/unreal-engine-4-tutorials/pickups-items-weapons/

RealBoxing_2015_05_14
=====================

#. Use QuadD to see the flow
#. Use Battle to the specific frame


Analysis report
---------------

#. find out most busy thread

   - 4494 54.14%
       FRunnableThreadAndroid::Run()
          
   - 4469 38.02%
   - 4436  3.82%
   - 4489  0.77%

#. they always use Thumb to save the space in memory.
#. 


Vehicle
=======

Vehicle 发动机的扭距是用曲线拟合，https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Curves/index.html. 
简单的用数组存关键点，然后拟合就行了。 现在知道了key frame如何实现了，只要用一个index value,就行了。然后就可以根据index最近
的几个然后曲线拟合。然后把这些参数传给bone更新函数，然后再更新mesh,可以把bone当做constant来进去了，然后利用shader来进行。
对于shader 取决于下层的硬件，到底是vertex shader还是geometry shader. 当然我们可以直接ogl,或者dx来做这些。
所谓的动画，也无非是指定了bone,并且其中的运动关系,以及mesh中顶点与这些bone的关系。我们可以为不同顶点采用不同的shader。
当然还有光照等等的计算与渲染，这些都是动画的一部分。这些都有标准的描述这样，所有这些动画才能通用，例如3d max, maya 等等
工具制作的动画才能让unreal等等进行通用。 至于前面的碰撞分析，也都是上层实现的，以及一些关系。 actor 相当于一个人吧，而component
就相当于器官，而vertex相当于细胞吧。 物理的检测一般是基于actor的。

其实那些基本的功能在gameworks里都有实现例子。只要去研究一下就明白了，除了shadow还不是完全明白之外，那个是需要光照的模型的。


FBodyInstance 用来构成object. 
每一个actor都会有一个rootComponent. 用来实现动画的传递功能。


控制的代码基本都在 pawn里，HUD，vehicle这些都是构造出来的。并且还有一些静态的信息，并没有在c++代码里体现出来的，那就是引擎里
直接坐掉，例如环境以及路，并没有相当于初始化代码。 现在也终于明白了在界面上点与代码的关系。并且开始理解 blueprint与C++的关系。 blueprint也是被unreal 修订化的C++,并且blueprint 是直接继承c++代码，所以你可以直接c++实现，还是blueprint实现都是可以的。就非常方便，就是继承的一个好处。只需要修改你需要的地方。

现在发现 class是最灵活的，然后把这个东东都串联起来。

这个例子，已经把发动机用曲线拟合给做了，还有方向盘也是，并且还是采用四驱模型。


HUD
===

还有塔台游戏中，各种菜单，以及游戏中各种信息的显示与交互。
并且也是虚拟交互一个重要手段。
unreal 有HUD与HID。 同时有一个slate UI framework. 
例如选择音效，等等都是需要的。
Slate 用的是Unreal Editor的framework, 并且可以用游戏中，在画的时候，要以GEngine->GameViewport为参考。
https://docs.unrealengine.com/latest/INT/Programming/Slate/index.html 在这里。 它是由描述性语言来生成界面，有点类似于UML。

原理很简单，就像Gameworks中更新设置是一样的。把变量保存在全局变量，或者独立的类的中。就行。每一次更新完，最后画上这些，然后
再swapbuffer. 

一个最简单的做法，那就是可以用菜单与console命令的关联。Unreal中内置一个console里，里面还有很多可以操作空间。
另外一种那是 UMG，可以参考https://docs.unrealengine.com/latest/INT/Engine/UMG/UserGuide/index.html。
Unreal Motion Graphics UI Designer. 数据的更新最简单的做法，那就是Property Binding来这样会自动更新。 三种做法，另外两种是Function binding,或者event driven.

简单的用法，那就是UMG来实现，还可以加动画。就像MFC,Qt这样的控制介面的开发。
https://docs.unrealengine.com/latest/INT/Engine/UMG/UserGuide/WidgetTypeReference/index.html
比较的强的，Unreal控件比较强的一点，实现了一个浏览器控制，这个还很方便的，这样可以在游戏上网，办空的功能。

如果每天带着一个面罩，这样实现实时或取，这样不就解决了人虚拟中问题。



testing
=======

`Unreal Engine 游戏架构及测试方案介绍 <http://gad.qq.com/article/detail/7337>`_ 



Sample
======

TopDown
-------

就是采用做了第三视角的camera, CameraBoom 摄像机的吊杆。
寻径的代码也很简单，简单的设置一个距离就行了。


.. code-block:: cpp

   void AMyTopDownPlayerController::SetNewMoveDestination(const FVector DestLocation)
   {
   	APawn* const Pawn = GetPawn();
   	if (Pawn)
   	{
   		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
   		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());
   
   		// We need to issue move command only if far enough in order for walk animation to play correctly
   		if (NavSys && (Distance > 120.0f))
   		{
   			NavSys->SimpleMoveToLocation(this, DestLocation);
   		}
   	}
   }

并且在SetupInputComponent()中建立输入控制矩阵，主要是利用UInputComponent实现了一个回调函数的机制。实现原理也就是重载了()操作符。

.. code-block:: cpp

   D:\Epic Games\4.10\Engine\Source\Runtime\Core\Public\Delegates
   template <typename T, typename MemFunPtrType>
   class TMemberFunctionCaller
   {
   	T*            Obj;
   	MemFunPtrType MemFunPtr;
   
   public:
   	TMemberFunctionCaller(T* InObj, MemFunPtrType InMemFunPtr)
   		: Obj      (InObj)
   		, MemFunPtr(InMemFunPtr)
   	{
   	}
   
   	template <typename... ArgTypes>
   	auto operator()(ArgTypes&&... Args) -> decltype((Obj->*MemFunPtr)(Forward<ArgTypes>(Args)...))
   	{
   		return (Obj->*MemFunPtr)(Forward<ArgTypes>(Args)...);
   	}
   };


   void AMyTopDownPlayerController::SetupInputComponent()
   {
   	// set up gameplay key bindings
   	Super::SetupInputComponent();
   
   	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMyTopDownPlayerController::OnSetDestinationPressed);
   	InputComponent->BindAction("SetDestination", IE_Released, this, &AMyTopDownPlayerController::OnSetDestinationReleased);
   
   	// support touch devices 
   	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMyTopDownPlayerController::MoveToTouchLocation);
   	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMyTopDownPlayerController::MoveToTouchLocation);
   }

Flying
======

static 来实现只编译一次。只有一个APAWN添加一个 mesh,再添加一个固定的camera就行了。
只用重载NotifyHit,这个是AActor中碰撞检测，当检测到调用父类，并且把当前的速度为0.
并且还是用SetupPlayerInputComponent中输入传进来，并mapping callback.


Blueprints
===========

主要分为Games,Players,Cameras,Input,Items,Environments.


通过对blueprint对于可视化编程有了更深的认识，例如函数，输入，输出。中间的计算依赖，执行依赖。谁来调用它，它来调用谁。同时对于多个输入使能时，之间或还是与关系。默认是或比较合适。

blueprint 是在高level之下的面向对象，而是之前的简单原始基本结构组成，例如数字，字符，数组等。 而是由各种actor组成的结构。每一个基本元素本身也都有物理，属性，以及形状的属性。 并且在blueprint中，几何形体，内部结构，内部行为是在一起的。
而原来C++编程只是数据与行为binding,而现在物理属性的，内部结构，内部行为的binding.
你为一个形体，添加blueprint时，其实生成代码，就会有create static mesh component,在构造函数里。

其实blueprint就是C++的一种可视化编程，有interface,以及类，函数等等的定义。以及各种变量产生，唯一的区别，我不用关心变量真实的名字，只要知道alias就行。另外就是要知道你的邻居是什么，你可以如何操作它，一般来都是利用cast to XXXXX 来把 other actor变成想要的，然后进行访问。


把UI转换成三维，只需要用widget然后再用actor中addcomponent把其添加进来，就可完成三维的形态。unreal好处，自身的UI与游戏里的是通用的。具体做法见https://docs.unrealengine.com/latest/INT/Engine/UMG/HowTo/Create3DWidgets/ActorSetup/index.html

Particles
=========

粒子系统的基本思路是采用大量的，具有一定生命和属性的微小粒子图元作为基本元素来描述不规则的模糊物体。所以每一个例子都要经历 "产生","活动","消亡" 三个阶段。
至于每一种粒子生命周期是什么样，如何驱动更新都是不一样的。
http://blog.csdn.net/blues1021/article/details/46808357
http://eastmodelsoft.com/blog/2016/particlesystem/ParticleSystem.html
各种特效的本质都是已经写好的shader,只是指定了参数而己
Unreal的particle系统是独立的模块包，这样放置在任意的位置，主要分为三部分，particleSystem,emitter,module,并且采用合成的方式，同时也可以有多个基本粒子系统来合成。粒子的形状可是点，也可以是mesh。同时对于粒子系统生成控制，可以用参数是曲线控制还是外部输入控制都是要有定义的，最多的当然是曲线控制了。
一个particleSystem可以有多个emitter,并且每个emitter的的参数可以些module控制，这些module本身是可以用distribution来控制。

对于粒子系统的来说，另一个重要的事情那就是力场的效应的模拟，这里可以用volume来实现，那些背后都是PhysX来实现的。

#. How to make Snow https://www.youtube.com/watch?v=G9vKSqRzhGQ

当然对每一个粒子系统都有一个actor wraper 这样方便程序控制。

.uasset
=======

这种文件结构，就是类似于zip这样的，ar这样的，打包进行，各自还有自己的结构，例如GTL 的.gar文件一样，还有.tar的文件结构。 都属于类似于的如果做可以参考 UnrealPak这个工程。以及相关serialize/deserialize来做。写一个unreal command的import/export直接来实现。
http://www.gildor.org/smf/index.php?topic=2613.0
http://gamedev.stackexchange.com/questions/77741/how-does-game-asset-file-encryption-work


引擎的核心
==========

http://www.zhihu.com/question/30954492

真正核心在于效率，没有渲染质量这一说法，同一张显卡，同样的shadingmodule,如果还用同样的模型，同样的算法，有什么质量不质量的，难道unreal4 算的1+1=2 比 Unity 算的1 +1 =2 质量要高，那个2要更好看。
现代引擎的瓶颈一个很重要的点就在于渲染批次的提交上面，一些大场景DP数量达到5000-10000. 引擎用到的所有算法都是公开的，没有什么magic code关键是你的取舍，你花多少资源在哪一个部分，省了哪个地方的东本。 而这些功力是源自于游戏开发本身的，源自于积累，源自TA。


游戏的AI
========

那个NavMeshVolume,是不是就相当于画圈，相当于做一个集合，也就有边界，也就知道在哪位里去找，并且实时精确的模拟。
现在明白那个这个导航网络了，就是建立一个图结构，采用8叉树，这样来建立邻接图，然后就方便计算最知路径了。这个过程就图论中最短路径。
另一个那行为树，行为树=决策树+ action.
如果只是if,else,就会产生组合爆炸，计算不过来。一种就是剪枝算法，就像现在BT一样，首先是大的决策树，然后你在决定每一个节点里填什么。
关键是什么trigger这个，blackboard是用来BT的状态机。Dectorator相当于剪枝器。
当然还要一些动作，集合。

另外重要一点，那就是目标的分解，NPC要能够自动的把目标分解，与聚类，通过自动的分解聚类，来实现抽象的过程。
http://www.zhihu.com/question/29486474 行为树

另一个重要那就是模糊判决。关键是模糊函数的定义。

EQS这样非常的有用，通过它才能知道周围环境的情况。主要也是模糊判决与动态寻径问题。

另外的问题，那就是避让问题，这个在自动驾驶也会很重要，谷歌这次撞车也是因为这个问题。如何解决避让问题。一种那就是自动编组。
最简单的速度velocity Obstacle.
http://blog.csdn.net/natsu1211/article/details/37774547

对于路径的寻找，两个重要的问题，那就是最终的目标在哪里，另一个下一步如何何走。也就有了SetFinalDestination(),GetNextMoveLocation()这样两个函数。

https://udn.epicgames.com/Three/NavigationMeshTechnicalGuideCH.html
