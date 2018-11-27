# Chapter 3 The Graphics Processing Unit

>* Historically, graphics acceleration started with interpolating colors on each pixel scanline
overlapping a triangle and then displaying these values. Including the ability to
access image data allowed textures to be applied to surfaces. Adding hardware for
interpolating and testing z-depths provided built-in visibility checking. Because of
their frequent use, such processes were committed to dedicated hardware to increase
performance. More parts of the rendering pipeline, and much more functionality for
each, were added in successive generations. Dedicated graphics hardware’s only computational
advantage over the CPU is speed, but speed is critical.
---
从历史上来看，图形加速从对每个像素扫描线的三角形中上进行颜色插值，和显示这些值。

包括将纹理数据应用到表面的能力。

在硬件方面做插值和深度测试来检测是否可见。

因为这些的经常使用，这些流程需要GPU进行加速。

在渲染管道的大部分，更多的功能是几代人的努力加入的。

GPU相比CPU的优点就是速度。

>* Over the past two decades, graphics hardware has undergone an incredible transformation.
The first consumer graphics chip to include hardware vertex processing
(NVIDIA’s GeForce256) shipped in 1999. NVIDIA coined the term graphics processing
unit (GPU) to differentiate the GeForce 256 from the previously available rasterizationonly
chips, and it stuck. During the next few years, the GPU evolved from configurable
implementations of a complex fixed-function pipeline to highly programmable blank
slates where developers could implement their own algorithms. Programmable shaders
of various kinds are the primary means by which the GPU is controlled. For efficiency,
some parts of the pipeline remain configurable, not programmable, but the trend is
toward programmability and flexibility [175].
---
在过去20年，图形硬件经历了非常大的转变。

一个款消费型gpu，包含了顶点处理，G256在1999年发售。

NVIDIA创造了GPU这个术语。

接下来的几年，GPU从可配置发展而来实现复杂功能到高度的可编程。

可编程的着色器是控制GPU的主要手段。

为了效率考虑，有些部分还是可配置不可编程的类型，而趋势是可编程性和灵活性。

>* GPUs gain their great speed from a focus on a narrow set of highly parallelizable
tasks. They have custom silicon dedicated to implementing the z-buffer, to rapidly
accessing texture images and other buffers, and to finding which pixels are covered
by a triangle, for example. How these elements perform their functions is covered in
Chapter 23. More important to know early on is how the GPU achieves parallelism
for its programmable shaders.
---
GPU因为专注于密集集合的高并发，而有着极快的速度。

他们有专门为各种流程去定制芯片，比如z缓冲的实现，快速访问纹理图片和其他缓冲，快速寻找三角形内的顶点。

这些在23章有描述。

更重要的是，我们要知道GPU的并行结果是如何实现它的可编程实现。

>* Section 3.3 explains how shaders function. For now, what you need to know is
that a shader core is a small processor that does some relatively isolated task, such
as transforming a vertex from its location in the world to a screen coordinate, or
computing the color of a pixel covered by a triangle. With thousands or millions of
triangles being sent to the screen each frame, every second there can be billions of
shader invocations, that is, separate instances where shader programs are run.
---
在3.3章中，解释了着色器的功能。

现在，你需要知道，着色器的核心是一个小型的处理器（它能完成一些相对孤立的任务），比如将顶点从相对坐标转变世界坐标，

或者被三角形覆盖的像素颜色。

因为有着成千上百万个三角形会在每帧传送到屏幕，每一秒可能有数十亿的着色器被调用，即，运行着色器的实例。

>* To begin with, latency is a concern that all processors face. Accessing data takes
some amount of time. A basic way to think about latency is that the farther away
the information is from the processor, the longer the wait. Section 23.3 covers latency
in more detail. Information stored in memory chips will take longer to access than
that in local registers. Section 18.4.1 discusses memory access in more depth. The
key point is that waiting for data to be retrieved means the processor is stalled, which
reduces performance.
---
首先，延迟是所有处理器面对的问题。

访问数据的任务需要一定的时间。

考虑延迟的基本方法就是，消息距离处理器越远，等待的时间就越长。

在23.3章会谈到更多的有关延迟的细节。

信息保存在存储芯片中，会比放在本地寄存器中花费更多的时间。

18.4.1中讨论了内存访问。

最关键的是，等待数据检索意味着处理器停止，意味着性能降低。

## 3.1 Data-Parallel Architectures

>* Various strategies are used by different processor architectures to avoid stalls. A CPU
is optimized to handle a wide variety of data structures and large code bases. CPUs
can have multiple processors, but each runs code in a mostly serial fashion, limited
SIMD vector processing being the minor exception. To minimize the effect of latency,
much of a CPU’s chip consists of fast local caches, memory that is filled with data likely
to be needed next. CPUs also avoid stalls by using clever techniques such as branch
prediction, instruction reordering, register renaming, and cache prefetching [715]
---
不同的处理器使用各种不同的策略来避免处理器等待。

一个CPU经过优化，可以处理各种数据结构，和大型的代码库。

CPU有多个核心，但是每个都会以串联的方式运行代码，除了 单指令流多数据流 这个处理方式是个例外。

为了最小化延迟的影响，大部分的CPU芯片都有快速缓存，一块可能被接下来访问到的内存。

CPU还有一些避免延迟的技术，比如分支预测，指令重新排序，寄存器重命名，缓存预取等。

>* GPUs take a different approach. Much of a GPU’s chip area is dedicated to a large
set of processors, called shader cores, often numbering in the thousands. The GPU is a
stream processor, in which ordered sets of similar data are processed in turn. Because
of this similarity—a set of vertices or pixels, for example—the GPU can process these
data in a massively parallel fashion. One other important element is that these invocations
are as independent as possible, such that they have no need for information
from neighboring invocations and do not share writable memory locations. This rule
is sometimes broken to allow new and useful functionality, but such exceptions come
at a price of potential delays, as one processor may wait on another processor to finish
its work.
---
GPU 采用了不一样的方法。

GPU的大部分芯片区域是一个处理器的极大集合，被称为着色器核心，一般有上千个。

GPU是一个流处理器，依次处理有序的类似的数据。

因为他们都有相似性，都是顶点或者像素的几何。

比如，CPU就能处理这种大规模的并行数据流。

另一个很关键的因素是，这些调用都尽可能的独立，因为他们没有必要去获取相邻调用的信息，也不用去共享可写的内存位置。

这个规则有时会破坏了新的和方便调用的功能，（比如做高斯模糊，就只能用帧缓冲来做，而不能直接在像素中做一次着色器)

但这些例外的功能会带来潜在的延迟代价，因为这意味着一个处理器需要等待另一个处理器来完成它的工作。

>* The GPU is optimized for throughput, defined as the maximum rate at which data
can be processed. However, this rapid processing has a cost. With less chip area
dedicated to cache memory and control logic, latency for each shader core is generally
considerably higher than what a CPU processor encounters [462].
---
GPU针对吞吐量进行了优化，定义为最快的数据处理效率。

然而，这种速度的优化是有开销的。

随着为缓存和逻辑控制的芯片区域减少，每个着色器核心的延迟会升高，而且会比CPU处理要高。

>* Say a mesh is rasterized and two thousand pixels have fragments to be processed;
a pixel shader program is to be invoked two thousand times. Imagine there is only
a single shader processor, the world’s weakest GPU. It starts to execute the shader
program for the first fragment of the two thousand. The shader processor performs
a few arithmetic operations on values in registers. Registers are local and quick to
access, so no stall occurs. The shader processor then comes to an instruction such as
a texture access; e.g., for a given surface location the program needs to know the pixel
color of the image applied to the mesh. A texture is an entirely separate resource,
not a part of the pixel program’s local memory, and texture access can be somewhat
involved. A memory fetch can take hundreds to thousands of clock cycles, during
which time the GPU processor is doing nothing. At this point the shader processor
would stall, waiting for the texture’s color value to be returned.
---
假设一个网格被光栅化，有2000个像素有相关的片段要处理。

一个像素着色器的程序将被调用2000次。

想象下，如果只有一个像素处理器，这可以说是世界上最弱的GPU了。

这个处理器将开始处理2000个片段的第一个片段。

该着色器处理器会对寄存器上的值做一些算数操作。

因为寄存器是本地的，而且访问非常快，所以不会有延迟（stall）发生。

接着，这个处理器需要读一张贴图，比如给定一个表面的位置，来读取图片上的颜色（这个例子举得不好）。

一张贴图是一种完全不同的资源，不是像素程序的本地内存中，并且纹理的访问会部分涉及。

内存的访问需要花几百上千个时钟周期，这段时间GPU处理器是做不了任何事情的。

在这里，shader处理器会延迟，等待正确的纹理颜色值返回。

>* To make this terrible GPU into something considerably better, give each fragment
a little storage space for its local registers. Now, instead of stalling on a texture fetch,
the shader processor is allowed to switch and execute another fragment, number two
of two thousand. This switch is extremely fast, nothing in the first or second fragment
is affected other than noting which instruction was executing on the first. Now the
second fragment is executed. Same as with the first, a few arithmetic functions are
performed, then a texture fetch is again encountered. The shader core now switches to
another fragment, number three. Eventually all two thousand fragments are processed
in this way. At this point the shader processor returns to fragment number one. By this
time the texture color has been fetched and is available for use, so the shader program
can then continue executing. The processor proceeds in the same fashion until another
instruction that is known to stall execution is encountered, or the program completes.
A single fragment will take longer to execute than if the shader processor stayed
focused on it, but overall execution time for the fragments as a whole is dramatically
reduced.
---
为了让这个GPU变得好一些，给每个片段在他的本地寄存器上提供一点存储空间。

现在，代替等待纹理返回，shader处理器会切换去执行另一个片段，2000个中的另一个。

这种切换是非常快的，没有内容在第一个片元或第二个片元中被影响。

现在，第二个片元开始执行。

就像第一个一样，执行了一段算数过程后，需要一张贴图。

这时着色器核心切换到第3个片元。

最终，我们用这种方式处理完了2000个片元。

在这时，处理器回到了第一个片元。

这个时候，纹理已经load好了，并可以被使用，因此程序继续往下执行。

处理器以相同的方法处理需要停止执行的指令，直到程序跑完。

单个片元的处理时间可能会变长，但整体的时间会有明显的降低。

>* In this architecture, latency is hidden by having the GPU stay busy by switching
to another fragment. GPUs take this design a step further by separating the
instruction execution logic from the data. Called single instruction, multiple data
(SIMD), this arrangement executes the same command in lock-step on a fixed number
of shader programs. The advantage of SIMD is that considerably less silicon (and
power) needs to be dedicated to processing data and switching, compared to using an
individual logic and dispatch unit to run each program. Translating our two-thousandfragment
example into modern GPU terms, each pixel shader invocation for a fragment
is called a thread. This type of thread is unlike a CPU thread. It consists of a bit
of memory for the input values to the shader, along with any register space needed
for the shader’s execution. Threads that use the same shader program are bundled
into groups, called warps by NVIDIA and wavefronts by AMD. A warp/wavefront is
scheduled for execution by some number GPU shader cores, anywhere from 8 to 64,
using SIMD-processing. Each thread is mapped to a SIMD lane.
---
在这种架构中，延迟会在GPU的频繁切换中被隐藏。

GPU将这种设计更进了一步，通过分离数据中的指令逻辑，这个称为 单指令流多数据流。

这种安排方式，能够在锁定步骤中，对固定数量的着色器程序执行相同的指令。

SIMD的优势在于硅含量更低（耗电更低）需要被设计为处理数据和切换，对比于使用独立的逻辑并分发每个运行单位的情况。

将我们的2000个片段转换为现代的GPU语言，为每个片元调用的顶点着色器的称为线程。

这种线程跟CPU的线程不一样。

它包含了一小块为渲染准备的内存，以及一部分为渲染部分执行的寄存器。

使用相同着色程序的线程被捆绑在一起称为组，在NVIDIA中称为 wrap，AMD中称为 waveronts.

一个组被计划给一定数量的GPU渲染核心执行，从8到64.

使用SIMD模式，每个线程到映射一条 SIMD 通道。

>* Say we have two thousand threads to be executed. Warps on NVIDIA GPUs
contain 32 threads. This yields 2000/32 = 62.5 warps, which means that 63 warps are
allocated, one warp being half empty. A warp’s execution is similar to our single GPU
processor example. The shader program is executed in lock-step on all 32 processors.
When a memory fetch is encountered, all threads encounter it at the same time,
because the same instruction is executed for all. The fetch signals that this warp
of threads will stall, all waiting for their (different) results. Instead of stalling, the
warp is swapped out for a different warp of 32 threads, which is then executed by
the 32 cores. This swapping is just as fast as with our single processor system, as no
data within each thread is touched when a warp is swapped in or out. Each thread
has its own registers, and each warp keeps track of which instruction it is executing.
Swapping in a new warp is just a matter of pointing the set of cores at a different set
of threads to execute; there is no other overhead. Warps execute or swap out until all
are completed. See Figure 3.1.
---
假设我们有2000个线程需要被执行。

NVIDA上的组包含了32个线程。

这意味着需要 63 个组，一个组的一半是空的。

一个组的执行，类似于我们的单个GPU处理器的处理例子。

渲染程序将以锁定步骤在这32个处理器上执行。

当遇到一次内存提取时，所有的线程都会遇到它，因为大家执行的是相同的指令。

线程们将停止，等待他们的结果。

代替等待的是，这个组会和另一个组做交换，这个组一样包含了32个处理器。


