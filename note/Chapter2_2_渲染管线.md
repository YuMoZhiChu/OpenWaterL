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
在应用阶段的末尾，要被渲染的几何体提供给 几何处理阶段。

他们是渲染的基本单元，比如，点，线，三角形，最终都有可能出现在屏幕上。

这是应用阶段的最重要任务。

>* A consequence of the software-based implementation of this stage is that it is
not divided into substages, as are the geometry processing, rasterization, and pixel
processing stages.1 However, to increase performance, this stage is often executed
in parallel on several processor cores. In CPU design, this is called a superscalar
construction, since it is able to execute several processes at the same time in the same
stage. Section 18.5 presents various methods for using multiple processor cores.
---
如果是软渲实现，这部分就不会分为 几何处理，光栅化，像素，。

但为了提高性能，多个流水线可以并行。

详细参考 18.5

>* One process commonly implemented in this stage is collision detection. After a
collision is detected between two objects, a response may be generated and sent back
to the colliding objects, as well as to a force feedback device. The application stage
is also the place to take care of input from other sources, such as the keyboard, the
mouse, or a head-mounted display. Depending on this input, several different kinds of
actions may be taken. Acceleration algorithms, such as particular culling algorithms
(Chapter 19), are also implemented here, along with whatever else the rest of the
pipeline cannot handle.
---
在这个阶段，通常实现的是 碰撞检测 这个功能。

在检测到2个物体的碰撞后，可以生成响应，并把响应通知物体，以及一些物理控件。

应用阶段同时会处理输入，比如鼠标，键盘，VR。

根据输入，产生效果。

加速算法（详见第19章），也在这里处理。

## 2.3 Geometry Processing

>* The geometry processing stage on the GPU is responsible for most of the per-triangle
and per-vertex operations. This stage is further divided into the following functional
stages: vertex shading, projection, clipping, and screen mapping (Figure 2.3).
---
几何处理阶段，在GPU中主要负责 三角形和顶点 的预处理操作。

可以分为4个部分，顶点着色，投影，裁剪 以及 屏幕映射。

![几何处理管线](pic/2/几何处理管线.png)

### 2.3.1 Vertex Shading

>* There are two main tasks of vertex shading, namely, to compute the position for a
vertex and to evaluate whatever the programmer may like to have as vertex output
data, such as a normal and texture coordinates. Traditionally much of the shade of
an object was computed by applying lights to each vertex’s location and normal and
storing only the resulting color at the vertex. These colors were then interpolated
across the triangle. For this reason, this programmable vertex processing unit was
named the vertex shader [1049]. With the advent of the modern GPU, along with some
or all of the shading taking place per pixel, this vertex shading stage is more general
and may not evaluate any shading equations at all, depending on the programmer’s
intent. The vertex shader is now a more general unit dedicated to setting up the data
associated with each vertex. As an example, the vertex shader can animate an object
using the methods in Sections 4.4 and 4.5.
---
顶点着色器有 2 个主要的任务， 顾名思义，计算一个顶点的位置

与求出任何在顶点输出阶段我们需要的值，比如 法线 和 纹理坐标。

一般来说，会将通过灯光，点的位置，法线计算出的阴影存在 vertex 的颜色中。

在三角形中，将这些值做插值。

因此，被称为顶点着色器。

对于现代的GPU，渲染是逐像素的，可能顶点着色器已经用不到渲染方程了（这还是取决于写程序的人）。

现在而言，顶点着色器是一个更加通用的单元，专门处理每个顶点及相关的数据。

在4.4和4.5 可以看到通过顶点着色器设置动画。

>* We start by describing how the vertex position is computed, a set of coordinates
that is always required. On its way to the screen, a model is transformed into several
different spaces or coordinate systems. Originally, a model resides in its own model
space, which simply means that it has not been transformed at all. Each model can
be associated with a model transform so that it can be positioned and oriented. It
is possible to have several model transforms associated with a single model. This
allows several copies (called instances) of the same model to have different locations,
orientations, and sizes in the same scene, without requiring replication of the basic
geometry.
---
我们来谈谈顶点位置是如何计算的，首先，需要一组坐标。

在绘制到屏幕上之前，一个模型会被转换到不同的空间和坐标系。

最初，一个模型存储在自己的模型空间中，这说明它还没有经过变换。

每个模型都会做一个 Model 变换，因此它有了位置和朝向。

Model 变换可以有多个。

这允许对于一个模型来说，可以有多份拷贝（称为实例），在一个场景中不同的位置，朝向，大小

而且这不需要去拷贝基础的几何体信息（即点，线，三角形）

>* It is the vertices and the normals of the model that are transformed by the model
transform. The coordinates of an object are called model coordinates, and after the
model transform has been applied to these coordinates, the model is said to be located
in world coordinates or in world space. The world space is unique, and after the models
have been transformed with their respective model transforms, all models exist in this
same space.
---
模型的顶点和法线会经过 Model 变换。

对应的坐标点由 模型坐标 变为 世界坐标。

世界空间是唯一的。

当模型经过各自的 Model 变换后，他们都会出现在世界空间中。

>* As mentioned previously, only the models that the camera (or observer) sees are
rendered. The camera has a location in world space and a direction, which are used to
place and aim the camera. To facilitate projection and clipping, the camera and all the
models are transformed with the view transform. The purpose of the view transform
is to place the camera at the origin and aim it, to make it look in the direction of the
negative z-axis, with the y-axis pointing upward and the x-axis pointing to the right.
We use the −z-axis convention; some texts prefer looking down the +z-axis. The
difference is mostly semantic, as transform between one and the other is simple. The
actual position and direction after the view transform has been applied are dependent
on the underlying application programming interface (API). The space thus delineated
is called camera space, or more commonly, view space or eye space. An example of
the way in which the view transform affects the camera and the models is shown in
Figure 2.4. Both the model transform and the view transform may be implemented as
4×4 matrices, which is the topic of Chapter 4. However, it is important to realize that
the position and normal of a vertex can be computed in whatever way the programmer
prefers.
---
就像之前提到的，只有被摄像机看到的模型才会被渲染。

摄像机在世界空间中有一个位置和方向，方向用来描述摄像机的朝向。

为了方便投影和裁剪，摄像机和所有的模型都做 View 变换。

View 变换的目的是，将摄像机挪到原点，朝向 -z 轴，+y 为 up 向量，+x 为right向量。

我们约定使用 -z 轴。

有些地方会写 +z 轴，不过问题不大，2者之间的转换很简单。

经过 View 变换的确定的位置和方向，依赖于 API。

这个空间被称为摄像机空间，或者，视角空间。

图2.4 阐述了， View 变换是怎么影响摄像机和模型的显示的。

Model 变换 和 View 变换 都是 4x4 的矩阵，在第四章会详细谈到。

但是，请意识到一点，一个顶点的位置和法线，可以按照程序想要的方式，来任意计算。

![2.4](pic/2/2.4.png)

>* Next, we describe the second type of output from vertex shading. To produce a
realistic scene, it is not sufficient to render the shape and position of objects, but their
appearance must be modeled as well. This description includes each object’s material,
as well as the effect of any light sources shining on the object. Materials and lights can
be modeled in any number of ways, from simple colors to elaborate representations of
physical descriptions.
---
接下来，我们会描述 顶点着色的 第二类输出。

为了产生实际的场景，仅仅是位置和形状是不够的，他们的表面也需要被建模。

这些描述包括了材质，光照的效果。

材质和光照可以用多种方式建模，从简单的颜色，到精心表现形式的物理描述。

>* This operation of determining the effect of a light on a material is known as shading.
It involves computing a shading equation at various points on the object. Typically,
some of these computations are performed during geometry processing on a model’s
vertices, and others may be performed during per-pixel processing. A variety of material
data can be stored at each vertex, such as the point’s location, a normal, a color,
or any other numerical information that is needed to evaluate the shading equation.
Vertex shading results (which can be colors, vectors, texture coordinates, along with
any other kind of shading data) are then sent to the rasterization and pixel processing
stages to be interpolated and used to compute the shading of the surface.
---
决定一种光在一种材质上的效果叫做渲染。

它涉及到了模型到各个点的渲染方程。

通常情况下，一些计算是在几何处理阶段完成的，而另一些在像素处理阶段完成。

每个顶点都能存下一系列的材质数据，就像点的位置，法线，和颜色一样，这些参数可能都会在渲染方程中用到。

顶点作色的结果，会被送去光栅化阶段和像素处理阶段，去做插值和计算表面的内容。

>* Vertex shading in the form of the GPU vertex shader is discussed in more depth
throughout this book and most specifically in Chapters 3 and 5.
---
在第3章和第5章，能看到更具体的顶点着色相关的内容。

>* As part of vertex shading, rendering systems perform projection and then clipping,
which transforms the view volume into a unit cube with its extreme points at
(−1,−1,−1) and (1, 1, 1). Different ranges defining the same volume can and are
used, for example, 0 ≤ z ≤ 1. The unit cube is called the canonical view volume.
Projection is done first, and on the GPU it is done by the vertex shader. There are
two commonly used projection methods, namely orthographic (also called parallel )
and perspective projection. See Figure 2.5. In truth, orthographic is just one type of
parallel projection. Several others find use, particularly in the field of architecture,
such as oblique and axonometric projections. The old arcade game Zaxxon is named
from the latter.
---
作为顶点着色的一部分，渲染系统会做 投影 和 裁剪。

会把view转化为从 (−1,−1,−1) 到 (1, 1, 1) 的单位正方体内。

不同的范围定义了不同的视线体积。

先会做投影，在GPU 上，通过 顶点着色器 完成。

这里是 2 种常见的投影方式，称为正交投影和透视投影。如图 2.5.

其他的几个用途，比如建筑学部分。

![2.5](pic/2/2.5.png)

>* Note that projection is expressed as a matrix (Section 4.7) and so it may sometimes
be concatenated with the rest of the geometry transform.
---
在4.7中，投影被表示为矩阵，所以它有时可以和剩下的几何变换结合。

>* The view volume of orthographic viewing is normally a rectangular box, and the
orthographic projection transforms this view volume into the unit cube. The main
characteristic of orthographic projection is that parallel lines remain parallel after the
transform. This transformation is a combination of a translation and a scaling.
---
正交投影的视图体积是一个矩形框。

>* The perspective projection is a bit more complex. In this type of projection, the
farther away an object lies from the camera, the smaller it appears after projection.
In addition, parallel lines may converge at the horizon. The perspective transform
thus mimics the way we perceive objects’ size. Geometrically, the view volume, called
a frustum, is a truncated pyramid with rectangular base. The frustum is transformed
into the unit cube as well. Both orthographic and perspective transforms can be
constructed with 4 × 4 matrices (Chapter 4), and after either transform, the models
are said to be in clip coordinates. These are in fact homogeneous coordinates, discussed
in Chapter 4, and so this occurs before division by w. The GPU’s vertex shader must
always output coordinates of this type in order for the next functional stage, clipping,
to work correctly.
---
透视投影会稍微复杂些。

这种投影中，物体离摄像机越远，它表现得越小。

另外，平行线可能会在地平线出汇聚。

透视投影变换，是在模仿我们感知物体的方式。

几何上，视线体积，被称为视椎体，是一个带有矩形底座的截头金字塔。

视椎体也要被转换为单位正方形。

2种变换都需要一个 4x4 的矩阵，经过变换后，模型就变成了裁剪坐标。

这些实际上是其次坐标，在第四章中讨论，会被除以 w。

GPU的顶点着色器必须输出坐标信息给下一个阶段。

>* Although these matrices transform one volume into another, they are called projections
because after display, the z-coordinate is not stored in the image generated but
is stored in a z-buffer, described in Section 2.5. In this way, the models are projected
from three to two dimensions.
---
最后，投影的 z-坐标 不会存在图片中，而是作为 z-buffer，以这种方式，模型从3维投影到2维。

### 2.3.2 Optional Vertex Processing

>* Every pipeline has the vertex processing just described. Once this processing is done,
there are a few optional stages that can take place on the GPU, in this order: tessellation,
geometry shading, and stream output. Their use depends both on the capabilities
of the hardware—not all GPUs have them—and the desires of the programmer. They
are independent of each other, and in general they are not commonly used. More will
be said about each in Chapter 3.
---
每个管线都会有上述所说的顶点处理过程。

一旦这个过程被完成，有一个可选择的流程会在GPU上发生。

这个过程可以完成： 曲面细分，几何着色，流输出。

他们取决于硬件能力（并不是所有的GPU可以完成），和程序员的想法。

他们通常独立，而且不会经常使用，会在第三章描述。

>* The first optional stage is tessellation. Imagine you have a bouncing ball object.
If you represent it with a single set of triangles, you can run into problems with
quality or performance. Your ball may look good from 5 meters away, but up close
the individual triangles, especially along the silhouette, become visible. If you make
the ball with more triangles to improve quality, you may waste considerable processing
time and memory when the ball is far away and covers only a few pixels on the screen.
With tessellation, a curved surface can be generated with an appropriate number of
triangles.
---
第一个可选阶段就是 曲面细分。

想象一下你有一个可以弹跳的球对象。

如果你用一组三角形来表示它，你可能会碰到一些性能上的问题。

你的球可以在5米远的时候看起来不错，但接近时，独立的一个个三角形，特别是沿着轮廓的部分，三角形会变得可见。

如果你将球的面数提高，那你就可能浪费了许多性能，当球距离很远的时候，只在屏幕上是几个像素。

通过 曲面细分，一个曲面 可以生成 近似个数的三角形。

```
这是写在管线里的 LOD 算法么？
```

>* We have talked a bit about triangles, but up to this point in the pipeline we have
just processed vertices. These could be used to represent points, lines, triangles, or
other objects. Vertices can be used to describe a curved surface, such as a ball. Such
surfaces can be specified by a set of patches, and each patch is made of a set of vertices.
The tessellation stage consists of a series of stages itself—hull shader, tessellator, and
domain shader—that converts these sets of patch vertices into (normally) larger sets
of vertices that are then used to make new sets of triangles. The camera for the scene
can be used to determine how many triangles are generated: many when the patch is
close, few when it is far away.
---
我们已经对三角形做了一些讨论，到目前为止，我们能获得在几何处理阶段过的顶点数据。

这些能用来表示点，线，三角形等。

顶点可以用来描述一个曲面，比如一个球。

这样的曲面可以被一组贴片描述，每个 patch 都是一组顶点。

曲面细分阶段包括了， hull 着色器，曲面细分单元，以及 域着色器，这是将 patch 的顶点组合转化为更大的 顶点几何。

这样就可以生成新的三角形几何。

对于场景而言，摄像机用于决定生成三角形的个数，近多远少。

```
这里写的应该是 曲面细分 阶段的实现方式，很可能是直接由 GPU 内部完成的，总之我品出的意思就是在
GPU 上做 LOD，算法大概是，将曲面分为 多个 patch，用 patch 来生成衍生的 patch，做缩小面或者放大面的技巧。
```
[曲面细分的Wiki](https://www.khronos.org/opengl/wiki/Tessellation)


>* The next optional stage is the geometry shader. This shader predates the tessellation
shader and so is more commonly found on GPUs. It is like the tessellation shader
in that it takes in primitives of various sorts and can produce new vertices. It is a
much simpler stage in that this creation is limited in scope and the types of output
primitives are much more limited. Geometry shaders have several uses, with one of
the most popular being particle generation. Imagine simulating a fireworks explosion.
Each fireball could be represented by a point, a single vertex. The geometry shader
can take each point and turn it into a square (made of two triangles) that faces the
viewer and covers several pixels, so providing a more convincing primitive for us to
shade.
---
下一个可选阶段是 几何着色器。

这个着色器在 曲面细分着色器 之前，在GPU上也很常见。

它跟曲面细分着色着色器类似，它获取各种各样的渲染单元，并能生成新的顶点。

它是一个更简单的阶段，因为在这个阶段的创造和输出的内容都会有限制。

几何着色器有多种用途，最常见的就是粒子生成。

想象一下烟花爆炸，每个火球就一个点，一个单独的顶点。

几何着色器可以把一个点转化为一个正方体（2个三角形），面向照相机，并且包含了几个像素。

这样就创造了更好看了粒子。

>* The last optional stage is called stream output. This stage lets us use the GPU
as a geometry engine. Instead of sending our processed vertices down the rest of the
pipeline to be rendered to the screen, at this point we can optionally output these to
an array for further processing. These data can be used by the CPU, or the GPU
itself, in a later pass. This stage is typically used for particle simulations, such as our
fireworks example.
---
最后一个可选阶段叫做 流输出。 这个阶段，我们可以把 GPU 当成一个几何引擎。

相比发送我们处理好的顶点数据到下一个流程，在这里我们可以选择性的把这些数据输出到一个数组中，做进一步处理。

这些数据可以存在 CPU，或者GPU中。

这个阶段通常用来做粒子模拟，比如我们的烟花的粒子。

```
这个阶段是说，可以把这段数据，再传递回 CPU 做操作？
```

>* These three stages are performed in this order—tessellation, geometry shading,
and stream output—and each is optional. Regardless of which (if any) options are
used, if we continue down the pipeline we have a set of vertices with homogeneous
coordinates that will be checked for whether the camera views them.
---
这三个阶段按照这个顺序： 曲面细分 - 几何着色 - 流输出。

每个阶段都是可选的。

无论哪个阶段会使用，如果我们进行管线，我们会得到一组 齐次的坐标数据（4元？）

这些数据会被检查，摄像机是否能看见他们。

```
这意味着，加入其中一个阶段，我们会做一次点是否在摄像机的视椎体内的判断。
```

### 2.3.3 Clipping

>* Only the primitives wholly or partially inside the view volume need to be passed on
to the rasterization stage (and the subsequent pixel processing stage), which then
draws them on the screen. A primitive that lies fully inside the view volume will
be passed on to the next stage as is. Primitives entirely outside the view volume
are not passed on further, since they are not rendered. It is the primitives that are
partially inside the view volume that require clipping. For example, a line that has
one vertex outside and one inside the view volume should be clipped against the view
volume, so that the vertex that is outside is replaced by a new vertex that is located
at the intersection between the line and the view volume. The use of a projection
matrix means that the transformed primitives are clipped against the unit cube. The
advantage of performing the view transformation and projection before clipping is that
it makes the clipping problem consistent; primitives are always clipped against the
unit cube.
---
只有在视椎体内的渲染单元（部分或者全部），会传递到光栅化阶段（以及之后像素处理阶段），然后渲染在平面上。

完全在视椎体内的单元会被完整的传递到下一个阶段。

完全不在的会被抛弃，因此他们也不会被渲染。

部分在视椎体内的渲染单元就需要裁剪了。

比如一条线AB，点A在视椎体内，点B在视椎体外，那它肯定要被裁剪了。

而在外的顶点B，会被一个新的顶点替代，这个顶点是直线和视椎体的焦点。

投影矩阵的使用，意味着被转换的几何单元已经被单位正方体裁剪。

在裁剪之前使用坐标变化和投影就是为了方便。

因为渲染单元往往在单位正方体内被裁剪

>* The clipping process is depicted in Figure 2.6. In addition to the six clipping
planes of the view volume, the user can define additional clipping planes to visibly
chop objects. An image showing this type of visualization, called sectioning, is shown
in Figure 19.1 on page 818.
---
在图2.6中，你可以看到大致的裁剪流程。

除了视体积的6个裁剪面，我们还可以自定义平面去裁剪物体，来确定他们的是否可见。

这种展示可视化结果的图片，被称为切片。

在818页的19.1图中有详细介绍。

![2.6](pic/2/2.6.png)

>* The clipping step uses the 4-value homogeneous coordinates produced by projection
to perform clipping. Values do not normally interpolate linearly across a triangle
in perspective space. The fourth coordinate is needed so that data are properly interpolated
and clipped when a perspective projection is used. Finally, perspective division
is performed, which places the resulting triangles’ positions into three-dimensional
normalized device coordinates. As mentioned earlier, this view volume ranges from
(−1,−1,−1) to (1, 1, 1). The last step in the geometry stage is to convert from this
space to window coordinates.
---
裁剪阶段使用的是投影阶段得到的4元齐次坐标。

值通常不会在透视空间中做三角形插值。

需要使用第4个坐标值，去保证数据被正确的插值，并在透视投影中被裁剪。

最后，透视除法被执行，将三角形的坐标放入 NDC 中。

就是之前说的，(−1,−1,−1) 到 (1, 1, 1)。

最后一个步骤就是把这个坐标转换为窗口坐标。

### 2.3.4 Screen Mapping

>* Only the (clipped) primitives inside the view volume are passed on to the screen mapping
stage, and the coordinates are still three-dimensional when entering this stage.
The x- and y-coordinates of each primitive are transformed to form screen coordinates.
Screen coordinates together with the z-coordinates are also called window coordinates.
Assume that the scene should be rendered into a window with the minimum corner
at (x1, y1) and the maximum corner at (x2, y2), where x1 < x2 and y1 < y2. Then the
screen mapping is a translation followed by a scaling operation. The new x- and ycoordinates
are said to be screen coordinates. The z-coordinate ([−1,+1] for OpenGL
and [0, 1] for DirectX) is also mapped to [z1, z2], with z1 = 0 and z2 = 1 as the default
values. These can be changed with the API, however. The window coordinates along
with this remapped z-value are passed on to the rasterizer stage. The screen mapping
process is depicted in Figure 2.7.
---
只有被裁剪的仍在视体积内的单元会被传递到屏幕映射阶段，这时候，这些坐标依然是3位的。

x，y被称为屏幕坐标。

屏幕坐标 + z 被称为窗口坐标。

假设场景要渲染一个从 (x1, y1) 到 (x2, y2) 的窗口。

那么屏幕映射，还要做一次缩放。

新的 x ,y 作为屏幕做包。

z （[-1, 1] OpenGL [0, 1] DX）也会投影到 z1-z2.

窗口坐标（带着做过映射的z），会传递给光栅化阶段。

![2.7](pic/2/2.7.png)

>* Next, we describe how integer and floating point values relate to pixels (and texture
coordinates). Given a horizontal array of pixels and using Cartesian coordinates, the
left edge of the leftmost pixel is 0.0 in floating point coordinates. OpenGL has always
used this scheme, and DirectX 10 and its successors use it. The center of this pixel is
at 0.5. So, a range of pixels [0, 9] cover a span from [0.0, 10.0). The conversions are
simply
---
接下来，我们谈谈，整数和浮点数的值，怎么和像素（以及贴图坐标）相关。

给定一个水平的像素列表，并使用笛卡尔坐标系，最左边的像素就是 0.0

OpenGL一直使用的是这个方案，DX10 以及后续的版本也用这个。

中心的像素是 0.5， 比如一个[0,9]的像素要对应 [0.0, 10.0), 他们的转换关系是：

![公式1](pic/2/公式1.png)

>* where d is the discrete (integer) index of the pixel and c is the continuous (floating
point) value within the pixel.

d 是离散的像素索引，c是连续的浮点数值，对应在一个像素内。

>* While all APIs have pixel location values that increase going from left to right, the
location of zero for the top and bottom edges is inconsistent in some cases between
OpenGL and DirectX.2 OpenGL favors the Cartesian system throughout, treating
the lower left corner as the lowest-valued element, while DirectX sometimes defines
the upper left corner as this element, depending on the context. There is a logic to
each, and no right answer exists where they differ. As an example, (0, 0) is located at
the lower left corner of an image in OpenGL, while it is upper left for DirectX. This
difference is important to take into account when moving from one API to the other.
---
虽然所有的API都是从左到右递增像素位置值，但从上到下，0的位置会在某些时候不一样。

opengl会把左下角定位最小的值，但DX有时会定在左上角，这取决于它的上下文环境。

这个各种情况不一样。

比如，（0，0）可能在 OpenGL的左下角，而在DX 的左上角，从一个api到另一个api时，这个转换非常重要。

## 2.4 Rasterization

>* Given the transformed and projected vertices with their associated shading data (all
from geometry processing), the goal of the next stage is to find all pixels—short for
picture elements—that are inside the primitive, e.g., a triangle, being rendered. We
call this process rasterization, and it is split up into two functional substages: triangle
setup (also called primitive assembly) and triangle traversal. These are shown to the
left in Figure 2.8. Note that these can handle points and lines as well, but since triangles
are most common, the substages have “triangle” in their names. Rasterization,
also called scan conversion, is thus the conversion from two-dimensional vertices in
screen space—each with a z-value (depth value) and various shading information associated
with each vertex—into pixels on the screen. Rasterization can also be thought
of as a synchronization point between geometry processing and pixel processing, since
it is here that triangles are formed from three vertices and eventually sent down to
pixel processing.
---
给定变化过并且投影过的顶点和他们的渲染数据（都进过了几何处理），下一个阶段的目标就是找到所有的像素（图元），在几何元素的内部。

比如，一个将要渲染的三角形。

我们把这个过程称为光栅化，并把它分为2个功能子阶段：三角形设置（也称为基元组装）和三角形遍历。

他们在图2.8中被表现。

注意到，这个过程能处理所有的点和线，但是最常处理的还是三角形。

光栅化，也被称为扫描转换，因为这是一个从一个拥有z值的屏幕空间的二维坐标表（和他对于的渲染数据），转换到屏幕上的像素点。、

光栅化，也能被视作一个同步点的过程（在几何处理和像素处理之间），因为它在这里把由三个顶点的三角形，最终给像素处理阶段处理。

![2.8](pic/2/2.8.png)

>* Whether the triangle is considered to overlap the pixel depends on how you have
set up the GPU’s pipeline. For example, you may use point sampling to determine
“insideness.” The simplest case uses a single point sample in the center of each
pixel, and so if that center point is inside the triangle then the corresponding pixel is
considered inside the triangle as well. You may also use more than one sample per
pixel using supersampling or multisampling antialiasing techniques (Section 5.4.2).
Yet another way is to use conservative rasterization, where the definition is that a
pixel is “inside” the triangle if at least part of the pixel overlaps with the triangle
(Section 23.1.2).
---
三角形是否与像素重叠，取决于你对GPU管线的设置。

比如，你可能使用了点采样来确定 “内心”。

最简单的情况就是，用单个在像素内的的点，如果这个点在三角形内，那么这个像素也在三角形内。

你还可以使用超采样或者多重采样技术来处理像素。

另一种方法是使用保守的光栅化，当像素的一部分和三角形重叠时，视为这个像素在三角形内。

### 2.4.1 Triangle Setup

>* In this stage the differentials, edge equations, and other data for the triangle are
computed. These data may be used for triangle traversal (Section 2.4.2), as well as
for interpolation of the various shading data produced by the geometry stage. Fixedfunction
hardware is used for this task.
---
在这个阶段，三角形的差分，边缘方程，以及三角形的其他数据也会被计算。

这些数据会被用于三角形遍历，以及用于对几何阶段产生的数据进行插值。

这些功能函数被固定在硬件上。

### 2.4.2 Triangle Traversal
>* Here is where each pixel that has its center (or a sample) covered by the triangle is
checked and a fragment generated for the part of the pixel that overlaps the triangle.
More elaborate sampling methods can be found in Section 5.4. Finding which
samples or pixels are inside a triangle is often called triangle traversal. Each triangle
fragment’s properties are generated using data interpolated among the three triangle
vertices (Chapter 5). These properties include the fragment’s depth, as well as any
shading data from the geometry stage. McCormack et al. [1162] offer more information
on triangle traversal. It is also here that perspective-correct interpolation over
the triangles is performed [694] (Section 23.1.1). All pixels or samples that are inside
a primitive are then sent to the pixel processing stage, described next.
---
在这里，每一个像素（它的中心或采样被三角形覆盖）都会被检查，并为这个覆盖了三角形的像素的部分生成片元。

更多详细的采样方法将在 5.4 中介绍。

找到哪一个采样，或者像素在三角形内被称为三角形遍历。

每个三角形片元（指的是三角形对应的片元）的属性，由3个三角形顶点数据插值而得到。

这些属性包括了，片元的深度，以及在几何阶段的任何渲染数据。

McCormack et al. 这个人在这方面做了很多工作。

这里也是用透视较真的插值方法来处理的（在 23.1.1 章中)

所有的像素或者采样都会在一个基元中，会被发送到下一个阶段，像素处理阶段。












