# Chapter1 Introdution

## 1.1 Contents Overview
>* Real-time rendering is concerned with rapidly making images on the computer. It
is the most highly interactive area of computer graphics. An image appears on the
screen, the viewer acts or reacts, and this feedback affects what is generated next.
This cycle of reaction and rendering happens at a rapid enough rate that the viewer
does not see individual images, but rather becomes immersed in a dynamic process.
---
实时渲染专注于在计算机上快速的绘制图片。
它是计算机图形学中交互性最高的部分。
一张图片出现在屏幕上，观察者的反应，以及下一步发生的影响。
这种相互影响和渲染的循环会在一个急速的发生，以至于我们不会看到独立的图片，而是一个沉浸的动态过程。

>* The rate at which images are displayed is measured in frames per second (FPS)
or Hertz (Hz). At one frame per second, there is little sense of interactivity; the user
is painfully aware of the arrival of each new image. At around 6 FPS, a sense of
interactivity starts to grow. Video games aim for 30, 60, 72, or higher FPS; at these
speeds the user focuses on action and reaction.
---
图片显示的速率被记为 FPS 或 HZ。
如果是一秒一帧，几乎没有任何的交互感。
用户会对一张张到来的图片感到非常痛苦。
达到 6 FPS，就会稍稍好上一些。
电子游戏的目标一般在 30,60,72或者更高的帧率。
这些帧率能让玩家更加的沉浸。

>* Movie projectors show frames at 24 FPS but use a shutter system to display each
frame two to four times to avoid flicker. This refresh rate is separate from the display
rate and is expressed in Hertz (Hz). A shutter that illuminates the frame three times
has a 72 Hz refresh rate. LCD monitors also separate refresh rate from display rate.
---
电影的帧率是24帧，但是使用了 “shutter system” 使得每一帧出现 2~4 次以避免闪烁。
这种刷新速率区别于显示速率，被记为 赫兹(Hz)。
一次 "shutter" 点亮一帧3次被称做 72Hz。
LCD监视器 还会将刷新效率和显示效率分开。

>* Watching images appear on a screen at 24 FPS might be acceptable, but a higher
rate is important for minimizing response time. As little as 15 milliseconds of temporal
delay can slow and interfere with interaction [1849]. As an example, head-mounted
displays for virtual reality often require 90 FPS to minimize latency.
---
以24帧的速率观看图像还是可以接受的，但更高的帧率对最小化反应时间非常重要。
比如15毫秒的延迟能够减慢并影响交互过程。
比如，头戴式虚拟现实设备通常要求 90FPS。

>* There is more to real-time rendering than interactivity. If speed was the only
criterion, any application that rapidly responded to user commands and drew anything
on the screen would qualify. Rendering in real time normally means producing threedimensional
images.
---
实时渲染相比交互性更加重要。
如果速度一定，任何需要快速反馈并绘制的命令会被限制。
实时渲染通常以为着产生3维图片。
```
这一段翻起来怪怪的。
```
>* Interactivity and some sense of connection to three-dimensional space are sufficient
conditions for real-time rendering, but a third element has become a part of
its definition: graphics acceleration hardware. Many consider the introduction of the
3Dfx Voodoo 1 card in 1996 the real beginning of consumer-level three-dimensional
graphics [408]. With the rapid advances in this market, every computer, tablet, and
mobile phone now comes with a graphics processor built in. Some excellent examples
of the results of real-time rendering made possible by hardware acceleration are shown
in Figures 1.1 and 1.2.
---
交互性和三维空间的连接感构成了实时渲染的条件，但第三个元素也成为了其中的一个定义：
图形加速硬件。
许多人在1996年的第一代3D显卡就开始消费级的3D渲染。
随着市场的快速发展，每台电脑，平板电脑和手机现在配备了内置的图形处理器。
一些很好的例子就是示通过硬件加速实现的实时渲染的结果
在图1.1和1.2中。
>* 2张游戏截图，一张某赛车游戏，一张是巫师3血与酒的城市的内容。

>* Advances in graphics hardware have fueled an explosion of research in the field
of interactive computer graphics. We will focus on providing methods to increase
speed and improve image quality, while also describing the features and limitations of
acceleration algorithms and graphics APIs. We will not be able to cover every topic in
depth, so our goal is to present key concepts and terminology, explain the most robust
and practical algorithms in the field, and provide pointers to the best places to go for
more information. We hope our attempts to provide you with tools for understanding
this field prove to be worth the time and effort you spend with our book.
---
图形硬件的进步推动了该领域的研究爆炸式增长交互式计算机图形学。
我们将专注于提供增加的方法速度和提高图像质量，同时还描述了特征和局限性
加速算法和图形API。 
我们无法涵盖所有主题深度，所以我们的目标是提出关键概念和术语，解释最强大
该领域的实用算法，并提供指向最佳位置的指针更多信息。
我们希望我们能够为您提供理解工具
这个领域证明是值得你花在我们的书上的时间和精力。
```
这一段就是叫你好好努力的意思。
```





