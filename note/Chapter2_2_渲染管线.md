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






