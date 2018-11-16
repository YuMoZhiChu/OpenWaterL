# Chapter2 The Graphics Rendering Pipeline

>* This chapter presents the core component of real-time graphics, namely the graphics
rendering pipeline, also known simply as “the pipeline.” The main function of the
pipeline is to generate, or render, a two-dimensional image, given a virtual camera,
three-dimensional objects, light sources, and more. The rendering pipeline is thus the
underlying tool for real-time rendering. The process of using the pipeline is depicted
in Figure 2.1. The locations and shapes of the objects in the image are determined
by their geometry, the characteristics of the environment, and the placement of the
camera in that environment. The appearance of the objects is affected by material
properties, light sources, textures (images applied to surfaces), and shading equations.
---
这个章节介绍了实时渲染的核心，叫做图形渲染管线，或简称 管线。

渲染管线的主要功能就是在给定 一个虚拟相机，许多3维物体，光源和更多内容的情况下，生成并渲染出一张2维的图片。

因此渲染管线就是实时渲染的核心工具。

图2.1描述了管线的使用流程。

2位图片的位置和形状由他们的几何形状，环境的属性，和摄像机的位置决定。

他们的表现由材质，光照，贴图，以及着色方程决定。

![2.1](pic/2/2.1.png)

>* We will explain the different stages of the rendering pipeline, with a focus on
function rather than implementation. Relevant details for applying these stages will
be covered in later chapters.
---
我们将着重介绍管线的各个阶段的功能而不是实现。

相关的细节将在更后的章节。

## 2.1 Architecture

>* In the physical world, the pipeline concept manifests itself in many different forms,
from factory assembly lines to fast food kitchens. It also applies to graphics rendering.
A pipeline consists of several stages [715], each of which performs part of a larger task.
---
在现实中，管线有多种不同的表现形式，从工厂流水线到快餐厨房的工作线。

对于图形渲染也一样。

管线由若干阶段组成，每个阶段是一个很大的任务。

>* The pipeline stages execute in parallel, with each stage dependent upon the result
of the previous stage. Ideally, a nonpipelined system that is then divided into n
pipelined stages could give a speedup of a factor of n. This increase in performance
is the main reason to use pipelining. For example, a large number of sandwiches can
be prepared quickly by a series of people—one preparing the bread, another adding
meat, another adding toppings. Each passes the result to the next person in line and
immediately starts work on the next sandwich. If each person takes twenty seconds
to perform their task, a maximum rate of one sandwich every twenty seconds, three
a minute, is possible. The pipeline stages execute in parallel, but they are stalled
until the slowest stage has finished its task. For example, say the meat addition
stage becomes more involved, taking thirty seconds. Now the best rate that can be
achieved is two sandwiches a minute. For this particular pipeline, the meat stage is
the bottleneck, since it determines the speed of the entire production. The toppings
stage is said to be starved (and the customer, too) during the time it waits for the
meat stage to be done.
---
渲染管线的步骤是并行执行的，并且每一个阶段依赖于上一个阶段的结果。

理想情况下，一个非管线的系统分成n个管线阶段能将速度提高到 n 倍。

这种性能的提高是主要选择使用渲染管线的原因。

举个例子，一个巨大的三明治可以被一组人快速的完成，一个准备面包，一个准备肉，一个准备浇头（谷歌翻译~）。

工人会将每个流程的结果传递给下一个工人，并立即开始准备新的三明治的工作。

如果每个工人需要20秒来完成，平均20秒完成一个三明治，一分钟制作3个，是可以达到的。

管线的各个阶段并行执行，然而他们会卡在最慢的阶段。

比如，三明治的添加肉的阶段会很复杂，需要花30秒。

那么现在的速度就变成了 1分钟制作2个三明治。

对于这个三明治管线，肉的制作阶段成了整个管线的瓶颈，因为它决定了整个生产的速度。

添加浇头的阶段，也会被卡住，直到加肉的阶段完成。

>* This kind of pipeline construction is also found in the context of real-time computer
graphics. A coarse division of the real-time rendering pipeline into four main
stages—application, geometry processing, rasterization, and pixel processing—is shown
in Figure 2.2. This structure is the core—the engine of the rendering pipeline—which
is used in real-time computer graphics applications and is thus an essential base for
discussion in subsequent chapters. Each of these stages is usually a pipeline in itself,
which means that it consists of several substages. We differentiate between the functional
stages shown here and the structure of their implementation. A functional stage
has a certain task to perform but does not specify the way that task is executed in the
pipeline. A given implementation may combine two functional stages into one unit
or execute using programmable cores, while it divides another, more time-consuming,
functional stage into several hardware units.
---
这种管道结构也能在实时渲染计算机图形的上下文中被找到。

粗略的分，我们将 rtt 渲染管线分为 4个部分， 应用，几何处理，光栅化，像素处理。如图 2.2

这个结构是渲染管线的核心结构，也是本章讨论的核心内容。

每个阶段自身也是一个管线，意思是它由几个子阶段组成。

我们在这里区分这里显示的 功能性阶段 和 他们的实现结构。

一个 功能性阶段 有一个特定的任务要执行，但不会描述确切在管线中执行的任务。

一个给定的 实现 可能需要将 2个功能性阶段 组合为一个单元 甚至会执行 可编程内核。

当它分为多一个阶段时，会更加耗时，功能性阶段 分为几个硬件 单元。

>* 这段翻译起来奇奇怪怪的，后续再看

![Pipeline](pic/2/pipeline.png)
```
管线基本分为4个部分，应用，几何处理，光栅化，像素处理。
图中几何处理为例，它其实本身就是一个管线。
而一个阶段可能可以（部分）并行化，就像图中的像素处理过程。
在这个例子中，应用阶段可能是单个进程。
但这个阶段也能被流水线化和并行化。
注：光栅化会找到图元中的基本单位，比如三角形
```

>* The rendering speed may be expressed in frames per second (FPS), that is, the
number of images rendered per second. It can also be represented using Hertz (Hz),
which is simply the notation for 1/seconds, i.e., the frequency of update. It is also
common to just state the time, in milliseconds (ms), that it takes to render an image.
The time to generate an image usually varies, depending on the complexity of the
computations performed during each frame. Frames per second is used to express
either the rate for a particular frame, or the average performance over some duration
of use. Hertz is used for hardware, such as a display, which is set to a fixed rate.
---
渲染速度用FPS表示，表示每秒渲染多少帧。

它也能用Hz表示，这只是 1/秒的单位，即更新的频率。

它也描述了时间，多少毫秒渲染了一张图片。

这个生成一张图片的时间经常变化，取决于每一帧绘制图片的复杂程度。

FPS 用于表示平均性能，而 HZ 一般用于硬件。

>* As the name implies, the application stage is driven by the application and is
therefore typically implemented in software running on general-purpose CPUs. These
CPUs commonly include multiple cores that are capable of processing multiple threads
of execution in parallel. This enables the CPUs to efficiently run the large variety of
tasks that are the responsibility of the application stage. Some of the tasks traditionally
performed on the CPU include collision detection, global acceleration algorithms,
animation, physics simulation, and many others, depending on the type of application.
The next main stage is geometry processing, which deals with transforms, projections,
and all other types of geometry handling. This stage computes what is to be drawn,
how it should be drawn, and where it should be drawn. The geometry stage is typically
performed on a graphics processing unit (GPU) that contains many programmable
cores as well as fixed-operation hardware. The rasterization stage typically takes as
input three vertices, forming a triangle, and finds all pixels that are considered inside
that triangle, then forwards these to the next stage. Finally, the pixel processing stage
executes a program per pixel to determine its color and may perform depth testing
to see whether it is visible or not. It may also perform per-pixel operations such as
blending the newly computed color with a previous color. The rasterization and pixel
processing stages are also processed entirely on the GPU. All these stages and their
internal pipelines will be discussed in the next four sections. More details on how the
GPU processes these stages are given in Chapter 3.
---
顾名思义，应用部分一般在CPU上完成。

这些CPU可能支持多核与并行，支持应用阶段的各种任务。

一些常见的算法，比如碰撞检测，动画，物理模拟等，这些取决于应用阶段的类型。

下一个阶段是几何阶段，它处理了所有的变化，投影，和其他类型的几何处理。

这一部分主要是计算，什么内容需要被绘制，应该怎样绘制（一些法线的处理吧。。），和应该绘制在哪里。

几何阶段在GPU中处理。

光栅化阶段取3个输入的顶点，构成三角形，并寻找应该在三角形内的像素，并传递到下一个阶段。

最后像素处理阶段决定了像素的颜色，以及用深度测试决定它是否可见。

它还可以执行 逐像素操作，比如把当前计算的颜色和原来的颜色做混合。

光栅化和像素处理阶段也是全在 GPU 进行的。

其他的内容会在下面介绍。

## 2.2 The Application Stage

>* The developer has full control over what happens in the application stage, since it
usually executes on the CPU. Therefore, the developer can entirely determine the
implementation and can later modify it in order to improve performance. Changes
here can also affect the performance of subsequent stages. For example, an application
stage algorithm or setting could decrease the number of triangles to be rendered.
---
开发人员对应用阶段有完全的掌控，因为这些都在CPU上处理。

因此，我们可以完全确定它们的实现方式，并后续做改进。

这里也会影响后面的表现。

比如，我们应该尽可能的剪去三角形的数量。

>* All this said, some application work can be performed by the GPU, using a separate
mode called a compute shader. This mode treats the GPU as a highly parallel general
processor, ignoring its special functionality meant specifically for rendering graphics.
---
有一种说法是，有些应用可以由GPU执行，使用单独的 计算着色器。

这种模式，是将GPU视为一个高并发的处理器，而忽略了其渲染图形的功能（人工智能等）。

>* At the end of the application stage, the geometry to be rendered is fed to the
geometry processing stage. These are the rendering primitives, i.e., points, lines, and
triangles, that might eventually end up on the screen (or whatever output device is
being used). This is the most important task of the application stage.
---




