## Chapter 3.2 GPU Pipeline Overview

>* The GPU implements the conceptual geometry processing, rasterization, and pixel
processing pipeline stages described in Chapter 2. These are divided into several
hardware stages with varying degrees of configurability or programmability. Figure 3.2
shows the various stages color coded according to how programmable or configurable
they are. Note that these physical stages are split up somewhat differently than the
functional stages presented in Chapter 2.
---
GPU实现了几何处理，光栅化和像素处理阶段（这些在第二章有详细描述）。

这些根据不同程度的可配置性和可编程性分为几个阶段。

图3.2用不同的颜色展现了这些阶段是可编程的还是可配置的。

这些阶段所代表的功能详细见第二章。

![3.2](pic/3/3.2.png)

>* GPU implementation of the rendering pipeline. The stages are color coded according
to the degree of user control over their operation. Green stages are fully programmable. Dashed
lines show optional stages. Yellow stages are configurable but not programmable, e.g., various blend
modes can be set for the merge stage. Blue stages are completely fixed in their function.
---
这张图是GPU对渲染管线的大致实现。

每个阶段的控制程度用颜色表示。

绿色表示完全可编程的。

虚线表示是可选阶段。

黄色表示可以配置，但是不可编程。

比如，在混合阶段，可以设置不同的混合参数。

蓝色阶段则是完全固定的功能。

>* We describe here the logical model of the GPU, the one that is exposed to you as
a programmer by an API. As Chapters 18 and 23 discuss, the implementation of this
logical pipeline, the physical model, is up to the hardware vendor. A stage that is
fixed-function in the logical model may be executed on the GPU by adding commands
to an adjacent programmable stage. A single program in the pipeline may be split into
elements executed by separate sub-units, or be executed by a separate pass entirely.
The logical model can help you reason about what affects performance, but it should
not be mistaken for the way the GPU actually implements the pipeline.
---
我们在这里描述GPU的逻辑模型，其中一个就是暴露给程序的接口。

18和23章描述了，逻辑管线的实现，物理模型，取决于硬件产商。

一个在逻辑模型中固定功能的阶段，可能在GPU中，通过一个相邻的可编程阶段加上命令来实现。

在管线中的单个程序，可能会被分离的子单元分为不同的元素，或者直接被分为单独的pass。

逻辑模型能够帮助你推测影响性能的因素。

但它不应该是真正GPU如何实现管线的误解。（就是，真实的GPU是怎么实现的，可能不会一样）

>* The vertex shader is a fully programmable stage that is used to implement the
geometry processing stage. The geometry shader is a fully programmable stage that
operates on the vertices of a primitive (point, line, or triangle). It can be used to
perform per-primitive shading operations, to destroy primitives, or to create new ones.
The tessellation stage and geometry shader are both optional, and not all GPUs
support them, especially on mobile devices.
---
顶点着色器是一个完全可编程阶段，用于实现几何处理阶段。

几何着色器也是一个完全可编程的阶段，它会对基元（点，线，或三角形）的顶点进行处理。

它可以用来执行每个基元的着色操作，销毁这个基元，或者创建一个新的。

曲面细分阶段和几何着色器都是可选的，并不是所有的GPU都会支持他们，特别是在手机硬件上。

>* The clipping, triangle setup, and triangle traversal stages are implemented by
fixed-function hardware. Screen mapping is affected by window and viewport settings,
internally forming a simple scale and repositioning. The pixel shader stage is
fully programmable. Although the merger stage is not programmable, it is highly configurable
and can be set to perform a wide variety of operations. It implements the
“merging” functional stage, in charge of modifying the color, z-buffer, blend, stencil,
and any other output-related buffers. The pixel shader execution together with the
merger stage form the conceptual pixel processing stage presented in Chapter 2.
---
裁剪，三角形设置，三角形遍历阶段都是在硬件上的固定功能。

屏幕映射会被窗口和视口的设置影响，内部形成了一个简答的比例和重新定位。

像素着色器阶段是完全可编程的。

虽然合并阶段是不可编程的，但是他是可以高度设置，并且可以执行各种操作。

它实现了“合并”的功能阶段，它负责了修改颜色缓冲，z缓冲，混合缓冲，模板缓冲，以及其他的输出相关的缓冲区。

像素着色器和合并阶段在第二章中被统一称为像素处理阶段的概念。

>* Over time, the GPU pipeline has evolved away from hard-coded operation and
toward increasing flexibility and control. The introduction of programmable shader
stages was the most important step in this evolution. The next section describes the
features common to the various programmable stages.
---
随着时间的推移，GPU管线由硬编码进化到如今的灵活性和可编程性。

可编程的着色器的引入是这个进化的最重要的一步。

下一节会介绍各种可编程阶段的功能。
