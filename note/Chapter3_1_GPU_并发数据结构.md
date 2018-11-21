# Chapter 3 The Graphics Processing Unit

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

## 2.5 Pixel Processing

>* At this point, all the pixels that are considered inside a triangle or other primitive
have been found as a consequence of the combination of all the previous stages. The
pixel processing stage is divided into pixel shading and merging, shown to the right in
Figure 2.8. Pixel processing is the stage where per-pixel or per-sample computations
and operations are performed on pixels or samples that are inside a primitive.
---
在这里，所有像素都被认为存在一个三角形中，或者其他的基元中，这是作为前面一系列流程的结果。

像素处理阶段，会分为像素的着色和合并，在2.8中可以看到。

像素处理是一个每像素（或每个采样）进行计算并操作。

### 2.5.1 Pixel Shading

>* Any per-pixel shading computations are performed here, using the interpolated shading
data as input. The end result is one or more colors to be passed on to the next
stage. Unlike the triangle setup and traversal stages, which are usually performed
by dedicated, hardwired silicon, the pixel shading stage is executed by programmable
GPU cores. To that end, the programmer supplies a program for the pixel shader (or
fragment shader, as it is known in OpenGL), which can contain any desired computations.
A large variety of techniques can be employed here, one of the most important
of which is texturing. Texturing is treated in more detail in Chapter 6. Simply put,
texturing an object means “gluing” one or more images onto that object, for a variety
of purposes. A simple example of this process is depicted in Figure 2.9. The image
may be one-, two-, or three-dimensional, with two-dimensional images being the most
common. At its simplest, the end product is a color value for each fragment, and these
are passed on to the next substage.
---
任何逐像素的渲染计算都会在这发生，使用的是插值而得来的渲染数据。

最终的结果会是一种或者多种的颜色，并传递给下一个阶段。

不像三角形的2个阶段，会被硬件直接写死，像素处理阶段是可编程的GPU核心。

为此，程序员们会提供一个像素着色器（在OpenGL里面被称为顶点着色器），可以包括任何需要的计算。

这里可以采用一大堆的技术，一种最重要的就是纹理化。

纹理化意味着对简单的输入有着更多的细节（6章），纹理化一个对象意味着 “粘合” 一张或者多张图片到这个物体上，可以应付各种各样的目的。

一个这个过程的简单例子在2.9中，图片可以是1,2,3维的都行，当然2维的图片是最常见的。

最简单的片段着色器，就是给每个顶点赋值一个固定的颜色，然后传递给下一个子阶段。

![2.9](pic/2/2.9.png)

### 2.5.2 Merging

>* The information for each pixel is stored in the color buffer, which is a rectangular array
of colors (a red, a green, and a blue component for each color). It is the responsibility
of the merging stage to combine the fragment color produced by the pixel shading stage
with the color currently stored in the buffer. This stage is also called ROP, standing
for “raster operations (pipeline)” or “render output unit,” depending on who you ask.
Unlike the shading stage, the GPU subunit that performs this stage is typically not
fully programmable. However, it is highly configurable, enabling various effects.
---
每个像素的信息被存储在颜色缓冲中，这是一个矩形阵列（每种颜色的rgb三个分量）。

合并阶段的责任就是，将由像素着色阶段并将颜色存储在缓冲中的片段的颜色组合起来。

这个阶段被称为ROP， 意味着，光栅操作（流水线） 或者 渲染输出单元。

不像渲染阶段，执行GPU的子单元是不可编程的。

然而，它可以配置各种属性，来实现各种效果（比如stencil state的配置）。

>* This stage is also responsible for resolving visibility. This means that when the
whole scene has been rendered, the color buffer should contain the colors of the primitives
in the scene that are visible from the point of view of the camera. For most
or even all graphics hardware, this is done with the z-buffer (also called depth buffer)
algorithm [238]. A z-buffer is the same size and shape as the color buffer, and for
each pixel it stores the z-value to the currently closest primitive. This means that
when a primitive is being rendered to a certain pixel, the z-value on that primitive
at that pixel is being computed and compared to the contents of the z-buffer at the
same pixel. If the new z-value is smaller than the z-value in the z-buffer, then the
primitive that is being rendered is closer to the camera than the primitive that was
previously closest to the camera at that pixel. Therefore, the z-value and the color
of that pixel are updated with the z-value and color from the primitive that is being
drawn. If the computed z-value is greater than the z-value in the z-buffer, then the
color buffer and the z-buffer are left untouched. The z-buffer algorithm is simple, has
O(n) convergence (where n is the number of primitives being rendered), and works
for any drawing primitive for which a z-value can be computed for each (relevant)
pixel. Also note that this algorithm allows most primitives to be rendered in any
order, which is another reason for its popularity. However, the z-buffer stores only a
single depth at each point on the screen, so it cannot be used for partially transparent
primitives. These must be rendered after all opaque primitives, and in back-to-front
order, or using a separate order-independent algorithm (Section 5.5). Transparency is
one of the major weaknesses of the basic z-buffer.
---
在这个阶段同样负责解决可见性的问题。

这意味着当场景被渲染时，颜色缓冲应该存储着对应基元的颜色（从这当前摄像机看来是可见的）。

对于大多数甚至所有的图形硬件，这个阶段用 z缓冲的算法来实现。

一个z缓冲于颜色缓冲有着相同的大小和形状，并且对于每个像素，它存储着一个最近基元的z值。

着意味着，当一个基元被渲染到一个确定的像素是，基元上在这个像素点上的z值会被计算，并和同样像素点为的在z缓冲上的z值比较。

如果新的z值比z缓冲上的z值要小，那么这个基元会被渲染到照相机上，来替换原来的基元。

因此，一个像素的z值和颜色会被该基元在绘制的过程中更新。

如果新的z值比原来的z值大，则保持不变。

z值的算法很简单，是 O(n) 收敛（n代表有多少个基元被渲染），并且会对每一个在当前像素有z值的基元进行计算。

同事，这个算法也运行大多数的基元按照任何的书序被渲染，这也是它很流行的原因。（可以确定顺序？）

然而，z值只会在屏幕上存储单个的深度，所以不能用于部分透明的基元。

这些透明的部分必须在所有不透明的基元渲染之后渲染，用一个后向前呈现的方式，或者是用一个单独的算法（章节5.5会详细谈到）。

透明的渲染是z缓冲算法的一个弱点。

>* We have mentioned that the color buffer is used to store colors and that the z-buffer
stores z-values for each pixel. However, there are other channels and buffers that can
be used to filter and capture fragment information. The alpha channel is associated
with the color buffer and stores a related opacity value for each pixel (Section 5.5).
In older APIs, the alpha channel was also used to discard pixels selectively via the
alpha test feature. Nowadays a discard operation can be inserted into the pixel shader
program and any type of computation can be used to trigger a discard. This type of
test can be used to ensure that fully transparent fragments do not affect the z-buffer
(Section 6.6).
---
我们提到过颜色缓冲为每个像素存储颜色值，z缓冲为每个像素存储z值。

然而还有其他通道和缓冲能够用于过滤和捕获片段信息。

alpha通道和颜色缓冲有关，存储了每个像素的不透明值（其实就是透明度）。

在旧的API中，alpha通道也会丢弃选择性的丢弃像素值（不通过alpha测试的）。

现在在像素着色器代码中，以及任何的计算都能触发丢弃。（就是discard语句）

这类测试能保证完全透明的内容不会影响到z缓冲。（章节6.6中有详细介绍）

>* The stencil buffer is an offscreen buffer used to record the locations of the rendered
primitive. It typically contains 8 bits per pixel. Primitives can be rendered into the
stencil buffer using various functions, and the buffer’s contents can then be used to
control rendering into the color buffer and z-buffer. As an example, assume that a filled
circle has been drawn into the stencil buffer. This can be combined with an operator
that allows rendering of subsequent primitives into the color buffer only where the
circle is present. The stencil buffer can be a powerful tool for generating some special
effects. All these functions at the end of the pipeline are called raster operations
(ROP) or blend operations. It is possible to mix the color currently in the color buffer
with the color of the pixel being processed inside a triangle. This can enable effects
such as transparency or the accumulation of color samples. As mentioned, blending
is typically configurable using the API and not fully programmable. However, some
APIs have support for raster order views, also called pixel shader ordering, which
enable programmable blending capabilities.
---
模板缓冲是一块跟屏幕外的缓冲区（不会出现在渲染的内容上），被用作记录渲染基于的位置。

一般来说，它包含了8个bit。

基元可以渲染到模板缓冲区上（通过各种函数），缓冲的内容可以被用作控制颜色缓冲和z缓冲。

比如，一个填充好的圆已经被渲染到了模板缓冲上。

这个以做一个与操作，让后续的渲染基元只能在圆内做一次渲染。

模板缓冲是一种强大的功能，能够产生特定的渲染效果。

他可以做到混合当前缓冲区的颜色与三角形像素内的颜色。

它能够做到透明度和颜色累加的例子。

如上所示，混合通常是通过API进行设置，而不知完全可编程的。

然而，有些API支持光栅化顺序视图，也叫像素着色顺序，能够启用可编程的混合功能。

>* The framebuffer generally consists of all the buffers on a system.
---
帧缓冲由系统上的所有缓冲组成

>* When the primitives have reached and passed the rasterizer stage, those that are
visible from the point of view of the camera are displayed on screen. The screen
displays the contents of the color buffer. To avoid allowing the human viewer to see
the primitives as they are being rasterized and sent to the screen, double buffering is
used. This means that the rendering of a scene takes place off screen, in a back buffer.
Once the scene has been rendered in the back buffer, the contents of the back buffer
are swapped with the contents of the front buffer that was previously displayed on
the screen. The swapping often occurs during vertical retrace, a time when it is safe
to do so.
---
当基元到达并通过光栅化阶段时，那些在摄像机上可见的点会被显示在屏幕上。

该屏幕上有内容的颜色缓冲。

为了让人们避免看到光栅化的过程，这里使用了双缓冲。

这意味着，渲染工作是在离屏的一张缓冲上进行的。

当渲染完成时，会发生交换。

交换的发生在 vertical retrace， 垂直回收期间，这个阶段是安全的（可能是线程安全吧）。

>* For more information on different buffers and buffering methods, see Sections 5.4.2,
23.6, and 23.7.
---
更多请看 5.4.2, 23.6, 以及 23.7. 等章节。

## 2.6 Through the Pipeline

>* Points, lines, and triangles are the rendering primitives from which a model or an
object is built. Imagine that the application is an interactive computer aided design
(CAD) application, and that the user is examining a design for a waffle maker. Here
we will follow this model through the entire graphics rendering pipeline, consisting of
the four major stages: application, geometry, rasterization, and pixel processing. The
scene is rendered with perspective into a window on the screen. In this simple example,
the waffle maker model includes both lines (to show the edges of parts) and triangles
(to show the surfaces). The waffle maker has a lid that can be opened. Some of the
triangles are textured by a two-dimensional image with the manufacturer’s logo. For
this example, surface shading is computed completely in the geometry stage, except
for application of the texture, which occurs in the rasterization stage.
---
点，线，三角形，这些渲染基元由模型或被建立的物体而来。

想象一下，这是一个交互式计算辅助设计的软件，而且用户正在使用它来检测一个华夫饼干制作机器的模型设计。

这里我们通过整个管线来最终这个模型，主要分为4个步骤。

应用程序，几何，光栅化，像素处理。

该场景用透视渲染最终渲染到屏幕上。

在这个简单的例子中，华夫饼干制作机器模型有线（它的边角），有三角形（它的表面）。

它还有一个可以打开的盖子。

某些三角形是由2D的纹理图片组成。

对于此例子，表面着色完全由几何阶段计算，除了纹理的应用，这部分发生在光栅化阶段。

### Application
>* CAD applications allow the user to select and move parts of the model. For example,
the user might select the lid and then move the mouse to open it. The application
stage must translate the mouse move to a corresponding rotation matrix, then see to
it that this matrix is properly applied to the lid when it is rendered. Another example:
An animation is played that moves the camera along a predefined path to show the
waffle maker from different views. The camera parameters, such as position and view
direction, must then be updated by the application, dependent upon time. For each
frame to be rendered, the application stage feeds the camera position, lighting, and
primitives of the model to the next major stage in the pipeline—the geometry stage.
---
CAD 程序允许用户选择和移动模型的各个部分。

比如用户可能会选择盖子，并打开它。

应用程序阶段需要将鼠标的移动转化为相应的旋转矩阵，然后再查看盖子的时候使用正确的矩阵。

另一个例子是播放动画。

摄像机的参数，比如位置和view向量，必须由应用阶段更新。

包括摄像机信息，光照和模型的基元，会被传递给下一个主要阶段，几何阶段。

### Geometry Processing
>* For perspective viewing, we assume here that the application has supplied a projection
matrix. Also, for each object, the application has computed a matrix that describes
both the view transform and the location and orientation of the object in itself. In
our example, the waffle maker’s base would have one matrix, the lid another. In
the geometry stage the vertices and normals of the object are transformed with this
matrix, putting the object into view space. Then shading or other calculations at
the vertices may be computed, using material and light source properties. Projection
is then performed using a separate user-supplied projection matrix, transforming the
object into a unit cube’s space that represents what the eye sees. All primitives outside
the cube are discarded. All primitives intersecting this unit cube are clipped against
the cube in order to obtain a set of primitives that lies entirely inside the unit cube.
The vertices then are mapped into the window on the screen. After all these pertriangle
and per-vertex operations have been performed, the resulting data are passed
on to the rasterization stage.
---
因为是透视视角，我们假设应用阶段提供了一个投影矩阵。

对于每个物体，应用阶段已经计算出来它自身的位置和方向。

在几何阶段，点和发小会被这个矩阵相乘，转换到视线空间。

然后着色或者其他的顶点的计算会被执行，这时会使用材质和光源的一些属性。

投影会使用一个单独的投影矩阵表现，把眼睛能看到的东西都变换到一个单位正方体内。

所有在正方体内的基元都会被丢弃。

所有相交的基元会被裁剪，以获得新的基元。

当这些操作被执行后，结果传递到光栅化阶段。

### Rasterization
>* All the primitives that survive clipping in the previous stage are then rasterized, which
means that all pixels that are inside a primitive are found and sent further down the
pipeline to pixel processing.
---
所有在裁剪阶段存活的基元，会进行光栅化。

这意味着，在一个基元内的所有像素会被记录，并送往像素处理阶段。

### Pixel Processing
>* The goal here is to compute the color of each pixel of each visible primitive. Those
triangles that have been associated with any textures (images) are rendered with these
images applied to them as desired. Visibility is resolved via the z-buffer algorithm,
along with optional discard and stencil tests. Each object is processed in turn, and
the final image is then displayed on the screen.
---
这个阶段的目标是，计算每个可见基元的每个像素的颜色。

这些三角形可能会和某些贴图相关联。

可见性通过z缓冲算法，以及可选择的丢弃，和模板测试。

当每个object被依次处理，最终的图像会显示在屏幕上。

### Conclusion
>* This pipeline resulted from decades of API and graphics hardware evolution targeted
to real-time rendering applications. It is important to note that this is not the only
possible rendering pipeline; offline rendering pipelines have undergone different evolutionary
paths. Rendering for film production was often done with micropolygon
pipelines [289, 1734], but ray tracing and path tracing have taken over lately. These
techniques, covered in Section 11.2.2, may also be used in architectural and design
previsualization.
---
这个管道技术，源于数十年前的API和图形硬件到如今的实时渲染应用。

它并不是唯一的可行的渲染管道。

离线渲染经过了不同的演变。

比如用于电影的渲染会使用 微多边形 管道，但是光线追踪和路径追踪技术，在最近被取代了。

这些技术在 11.2.2 中有详细提及。

>* For many years, the only way for application developers to use the process described
here was through a fixed-function pipeline defined by the graphics API in use. The
fixed-function pipeline is so named because the graphics hardware that implements it
consists of elements that cannot be programmed in a flexible way. The last example of
a major fixed-function machine is Nintendo’s Wii, introduced in 2006. Programmable
GPUs, on the other hand, make it possible to determine exactly what operations are
applied in various sub-stages throughout the pipeline. For the fourth edition of the
book, we assume that all development is done using programmable GPUs.
---
一直以来，应用程序人员都是使用图形API定义的固定管线。

这个 固定管线 的命名，就是因为它没办法灵活编程。

最后一个固定管线的例子就是 Nintendo 的 Wii（2006）年版。

可编程的管线，能够让GPU准确的操作整个管道的各个子阶段。

我们这本书，都是在这个假设上完成的。

### Further Reading and Resources
>* Blinn’s book A Trip Down the Graphics Pipeline [165] is an older book about writing
a software renderer from scratch. It is a good resource for learning about some of
the subtleties of implementing a rendering pipeline, explaining key algorithms such
as clipping and perspective interpolation. The venerable (yet frequently updated)
OpenGL Programming Guide (a.k.a. the “Red Book”) [885] provides a thorough description
of the graphics pipeline and algorithms related to its use. Our book’s website,
realtimerendering.com, gives links to a variety of pipeline diagrams, rendering engine
implementations, and more.
---
应该买本 OpenGL 编程指南看一看了。











