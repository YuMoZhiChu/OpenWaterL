## Chapter 3.6 The tessellation stage

参考

[曲面细分中文教程](http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial30.html)

[曲面细分中文教程2](https://www.cnblogs.com/zenny-chen/p/4280100.html)

[khronos-wiki](https://www.khronos.org/opengl/wiki/Tessellation)

>* The tessellation stage allows us to render curved surfaces. The GPU’s task is to take
each surface description and turn it into a representative set of triangles. This stage
is an optional GPU feature that first became available in (and is required by) DirectX
11. It is also supported in OpenGL 4.0 and OpenGL ES 3.2.
---
曲面细分阶段允许我们去渲染曲面。

GPU的任务就是获取每个曲面的描述，并把它转换为一组表现良好的三角形。

这是GPU的一个可选特性，最早在DX11被支持。

OpenGL 4.0 和 OpenGL ES 3.2  同样支持。

>* There are several advantages to using the tessellation stage. The curved surface description
is often more compact than providing the corresponding triangles themselves.
Beyond memory savings, this feature can keep the bus between CPU and GPU from
becoming the bottleneck for an animated character or object whose shape is changing
each frame. The surfaces can be rendered efficiently by having an appropriate number
of triangles generated for the given view. For examp  le, if a ball is far from the camera,
only a few triangles are needed. Up close, it may look best represented with thousands
of triangles. This ability to control the level of detail can also allow an application to
control its performance, e.g., using a lower-quality mesh on weaker GPUs in order to
maintain frame rate. Models normally represented by flat surfaces can be converted to
fine meshes of triangles and then warped as desired [1493], or they can be tessellated
in order to perform expensive shading computations less frequently [225].
---
使用曲面细分有几个优点。

曲面的描述，通常比他们本身对应的三角形更加紧凑。

除了减少内存，这个特点还能避免从CPU到GPU的动画人物或者物体数据传输每一帧变化的动画数据的过程，变为瓶颈。

表面能够快速的渲染，通过由给出的视图生成的恰当数量的三角形。

比如，如果一个球远离摄像机，则只需要少量的三角形。

这种控制LOD的技术也可以允许应用去控制它的性能，比如在弱的GPU上使用低质量的网格，来补偿帧率。

由平面表示的模型，能够被转换为恰当的三角形网格，并需要形变。

或者他们能够被曲面细分，为了不那么频繁的进行代价高的渲染计算。

>* The tessellation stage always consists of three elements. Using DirectX’s terminology,
these are the hull shader, tessellator, and domain shader. In OpenGL the hull
shader is the tessellation control shader and the domain shader the tessellation evaluation
shader, which are a bit more descriptive, though verbose. The fixed-function
tessellator is called the primitive generator in OpenGL, and as will be seen, that is
indeed what it does.
---
曲面细分阶段通常由3个元素组成。

用DX的术语，他们是 壳着色器, 曲面细分器，和域着色器。

在OpenGL中，称为 曲面控制着色器，曲面细分器，以及域着色器，他们有些区别，但是问题不大。

固定功能的曲面细分着色器在OpenGL就是 基元生产器，就是生成基元的流程。（就是，最简单的曲面细分，就是不变，基元照旧)

>* How to specify and tessellate curves and surfaces is discussed at length in Chapter
17. Here we give a brief summary of each tessellation stage’s purpose. To begin,
the input to the hull shader is a special patch primitive. This consists of several control
points defining a subdivision surface, B´ezier patch, or other type of curved element.
The hull shader has two functions. First, it tells the tessellator how many triangles
should be generated, and in what configuration. Second, it performs processing on
each of the control points. Also, optionally, the hull shader can modify the incoming
patch description, adding or removing control points as desired. The hull shader outputs
its set of control points, along with the tessellation control data, to the domain
shader. See Figure 3.9.
---
如何识别特定的曲面并细分为恰当的曲面会在17章详细了解。

在这里我们简单的说一下每一个阶段的功能。

首先，输入 hull shader 的是 一组特定的基元 patch。（patch即对曲面的一份描述）

这包括了几个控制点来定义 一小块的曲面，贝塞尔参数，或者是其他类型的曲面元素。（这不是顶点数据，而是patch数据，但最简单的patch直接就是曲面的控制点 就是 对应的 顶点）

hull shader 主要有2个功能。

第一，它告诉 细分器 要生成多少个三角形，以及以怎样的配置生成。

第二，它对每个控制点做了处理。

当然，hull shader能够修改输入的 patch 描述，在必要时增加或者删除控制点。

hull shader的输出是它的控制点几何，以及 细分器的 控制数据，给 domain shader. 如图 3.9

![3.9](pic/3/3.9.png)
>* The tessellation stage. The hull shader takes in a patch defined by control points. It
sends the tessellation factors (TFs) and type to the fixed-function tessellator. The control point
set is transformed as desired by the hull shader and sent on to the domain shader, along with TFs
and related patch constants. The tessellator creates the set of vertices along with their barycentric
coordinates. These are then processed by the domain shader, producing the triangle mesh (control
points shown for reference).
---
曲面细分阶段。 hull shader 接受定义控制点 patch.

它将 曲面细分因子(TFs)以及类型，传递给 固定功能 的 曲面细分器。

hull shader 会将控制点传给 domain shader，包括 TFs，以及相关的曲面常量。

细分器会在他们的质心坐标系中，创建一组顶点。

然后再交由 domian shader 中处理，生成三角形网格（控制显示的点)。

>* The tessellator is a fixed-function stage in the pipeline, only used with tessellation
shaders. It has the task of adding several new vertices for the domain shader to process.
The hull shader sends the tessellator information about what type of tessellation
surface is desired: triangle, quadrilateral, or isoline. Isolines are sets of line strips,
sometimes used for hair rendering [1954]. The other important values sent by the hull
shader are the tessellation factors (tessellation levels in OpenGL). These are of two
types: inner and outer edge. The two inner factors determine how much tessellation
occurs inside the triangle or quadrilateral. The outer factors determine how much
each exterior edge is split (Section 17.6). An example of increasing tessellation factors
is shown in Figure 3.10. By allowing separate controls, we can have adjacent curved
surfaces’ edges match in tessellation, regardless of how the interiors are tessellated.
Matching edges avoids cracks or other shading artifacts where patches meet. The vertices
are assigned barycentric coordinates (Section 22.8), which are values that specify
a relative location for each point on the desired surface.
---
曲面细分器是在管线中的 固定功能阶段，仅仅用于 曲面细分着色器。

它的任务是 为 domain shader 增加几个新的顶点。

hull shader 会将曲面细分的信息来决定需要什么类型的细分器，三角形，四边形，或者 等值线。

等值线，是一组线段的集合，有时会用于头发的渲染。

其他的重要信息（由 hull shader传来的）就是 曲面细分因子（在OpenGL中称为 曲面细分等级).

这里有2种类型，内边和外边。（从内边和外边这里，就看不懂了。）

2个内边因子决定了 曲面细分器在三角形或者四边形的内部，大致分为多少部分。

外因子决定了，每个外部的边会分为多少份 (17.6)

图3.10就是一个 增加 曲面细分 因子的例子。

通过单独的控制，无论内部是如何细分的，我们可以控制曲面的边界是如何分为多少份的。

外边的对应保证了在和其他 patch 接触的时候，导致的裂缝，或者是相互覆盖的阴影。

顶点是由质心坐标系赋值的（22.8），他们对应的点与表面上的顶点对应。

![3.10](pic/3/3.10.png)

>* The effect of varying the tessellation factors. The Utah teapot is made of 32 patches.
Inner and outer tessellation factors, from left to right, are 1, 2, 4, and 8. (Images generated by demo
from Rideout and Van Gelder [1493].)
---
不同的曲面细分参数的效果。

这个茶壶由 32 个patch组成。

内外因子，从左到右是  1,2,4,8

![3.10.1](pic/3/3.10.1.png)
这是 inner outer 的例子

>* The hull shader always outputs a patch, a set of control point locations. However, it
can signal that a patch is to be discarded by sending the tessellator an outer tessellation
level of zero or less (or not-a-number, NaN). Otherwise, the tessellator generates a
mesh and sends it to the domain shader. The control points for the curved surface
from the hull shader are used by each invocation of the domain shader to compute the
output values for each vertex. The domain shader has a data flow pattern like that
of a vertex shader, with each input vertex from the tessellator being processed and
generating a corresponding output vertex. The triangles formed are then passed on
down the pipeline.
---
hull shader往往会输出一个 patch, 一组控制点位置的集合。

然而，它能表示，如果一个 patch将被丢弃，会发送一个 小于等于 0 的外边因子。

否则，曲面细分器会生成 网格 并发送给 domain shader。

在 hull shader 中表示曲面的控制点，会被 domian shader 调用，并计算出每个顶点的 值。

domain 对每个曲面细分器传递的 质心坐标的值 进行处理，并输出一个对应的顶点值（不能丢弃)

然后生成的 三角形 被送往之后的管线。

>* While this system sounds complex, it is structured this way for efficiency, and each
shader can be fairly simple. The patch passed into a hull shader will often undergo
little or no modification. This shader may also use the patch’s estimated distance or
screen size to compute tessellation factors on the fly, as for terrain rendering [466].
Alternately, the hull shader may simply pass on a fixed set of values for all patches
that the application computes and provides. The tessellator performs an involved but
fixed-function process of generating the vertices, giving them positions, and specifying
what triangles or lines they form. This data amplification step is performed outside of
a shader for computational efficiency [530]. The domain shader takes the barycentric
coordinates generated for each point and uses these in the patch’s evaluation equation
to generate the position, normal, texture coordinates, and other vertex information
desired. See Figure 3.11 for an example.
---
这套系统听起来很复杂，但它的结构是为了提高效率，并且让每个着色器都相当的简单。

传入 hull shader 的 patch 经常会做很少的修改，甚至没有。

这个着色器可能会用 patch 的估算距离，或者屏幕的大小，去计算 曲面因子，比如在画地表渲染时。

或者，hull shader 可以就用一组简单的常数来对所有的 patch 做一次曲面因子设定，这些常数可以在应用阶段计算并由CPU传入。

曲面细分器 就是一次调用，但它是一个固定功能的过程，目的是生成顶点，需要知道，顶点的位置，由怎样的三角形或者线段组成。

这个数据的放大步骤会在 shader 之外进行，就是为了效率上的考虑。

domain shader 获取每个点的质心坐标，并在 patch 表达式中计算出每个顶点的 位置，法线，纹理坐标，或者其他的顶点信息。

3.11 就是一个例子。

![3.11](D:/OpenWaterL3/note/pic/3/3.11.png)

左边只有6000个三角形，右边是用 PN 三角形细分算法达到的效果。

