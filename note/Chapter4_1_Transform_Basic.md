# Chapter 4 Transforms

参考

[Wiki正交矩阵](https://zh.wikipedia.org/wiki/%E6%AD%A3%E4%BA%A4%E7%9F%A9%E9%98%B5)

## 序

>* A transform is an operation that takes entities such as points, vectors, or colors and
converts them in some way. For the computer graphics practitioner, it is extremely
important to master transforms. With them, you can position, reshape, and animate
objects, lights, and cameras. You can also ensure that all computations are carried
out in the same coordinate system, and project objects onto a plane in different ways.
These are only a few of the operations that can be performed with transforms, but
they are sufficient to demonstrate the importance of the transform’s role in real-time
graphics, or, for that matter, in any kind of computer graphics.
---
变换是一种将元素（比如顶点，向量，颜色）进行某种形式的转换的操作。

对于计算机图形学的从业者，掌握变换是非常重要的。

通过变换，你可以调整位置，形状，动画，光照，以及摄像机。

你同样能保证所有的计算都在一个坐标系内，所有的物体都投影在一个平面上。

其实只有少量的操作会通过变换执行，但是他们非常重要。

>* A linear transform is one that preserves vector addition and scalar multiplication.
Specifically,
----
线性变换值的是向量的加法和乘法。

![Formula 1](pic/4/formula_1.png)

>* As an example, f (x) = 5x is a transform that takes a vector and multiplies each
element by five. To prove that this is linear, the two conditions (Equations 4.1 and 4.2)
need to be fulfilled. The first condition holds since any two vectors multiplied by five
and then added will be the same as adding the vectors and then multiplying. The
scalar multiplication condition (Equation 4.2) is clearly fulfilled. This function is
called a scaling transform, as it changes the scale (size) of an object. The rotation
transform is another linear transform that rotates a vector about the origin. Scaling
and rotation transforms, in fact all linear transforms for three-element vectors, can be
represented using a 3 × 3 matrix.
----
举个例子，f (x) = 5x 是一个将向量的每一个元素都乘上5的变换。

为了证明这是线性的，4.1和4.2中的2条式子需要被满足。

第一个条件成立，因为任意2个向量乘上5再相加和相加再乘5是一样的。

第二个条件显然成立。

这个函数被称为缩放变换，因为它改变了物体的大小。

旋转变换是另一种新型变换，它是物体绕原点的变换。

旋转和缩放变换，事实上所有对3元向量的线性变换，都能表示成3X3的矩阵。

>* However, this size of matrix is usually not large enough. A function for a threeelement
vector x such as f (x) = x+(7, 3, 2) is not linear. Performing this function on
two separate vectors will add each value of (7, 3, 2) twice to form the result. Adding
a fixed vector to another vector performs a translation, e.g., it moves all locations by
the same amount. This is a useful type of transform, and we would like to combine
various transforms, e.g., scale an object to be half as large, then move it to a different
location. Keeping functions in the simple forms used so far makes it difficult to easily
combine them.
----
然而，矩阵往往不够大。

比如一个 f (x) = x+(7, 3, 2) 就是非线性的。

对2个不同的向量进行这个操作，会对结果加上2次 (7, 3, 2)。

对任意一个向量增加一个固定的向量值，就是对所有的位置移动相同的距离。

这是一类非常有用的变换，我们通常会将它和其他的变换结合。

比如放大一个物体，并移动到一个不同的位置。

这些简单的形式能组合成非常复杂的效果。

![Wiki Affine Transform](pic/4/wiki_affine_transform.png)

>* Combining linear transforms and translations can be done using an affine transform,
typically stored as a 4 × 4 matrix. An affine transform is one that performs
a linear transform and then a translation. To represent four-element vectors we use
homogeneous notation, denoting points and directions in the same way (using bold
lowercase letters). A direction vector is represented as v = (vx vy vz 0)T and
a point as v = (vx vy vz 1)T . Throughout the chapter, we will make extensive
use of the terminology and operations explained in the downloadable linear algebra
appendix, found on realtimerendering.com.
----
线性变换和平移的结合可以用仿射变换来表示，通常用4X4的矩阵。

一次仿射变换意味着一次线性变换，然后平移。

为了表示4元向量，我们使用齐次表示法，用同样的方法表示点和向量（使用小写粗体字母）

向量表示为  v = (vx vy vz 0)T ， 点表示为  v = (vx vy vz 1)T 。

在这一整章中，我们会对本书的线性代数附录中的内容进行广泛的使用。

>* All translation, rotation, scaling, reflection, and shearing matrices are affine. The
main characteristic of an affine matrix is that it preserves the parallelism of lines, but
not necessarily lengths and angles. An affine transform may also be any sequence of
concatenations of individual affine transforms.
----
所有的平移、旋转、缩放、反射和倾斜矩阵都是仿射的（4X4）。

仿射矩阵的主要特征是它保证了直线间的平行性（原来平行的直线依然是平行的），但不能保证长度和角度不变。

仿射矩阵也可以分解成一系列的单个仿射变换。

>* This chapter will begin with the most essential, basic affine transforms. This
section can be seen as a “reference manual” for simple transforms. More specialized
matrices are then described, followed by a discussion and description of quaternions,
a powerful transform tool. Then follows vertex blending and morphing, which are
two simple but effective ways of expressing animations of meshes. Finally, projection
matrices are described. Most of these transforms, their notations, functions, and
properties are summarized in Table 4.1, where an orthogonal matrix is one whose
inverse is the transpose.
----
这一章将会从最本质，基础的仿射变换开始。

这一节可以作为简单变换的参考手册。

之后，我们会讨论更加专业的矩阵，就是四元数，这是一个强大的变换工具。

然后是顶点的混合与形变，这是2种最简单也是最有效的网格动画的描述方式。

最后，我们会讨论投影矩阵。

大多数的矩阵，他们的符号，函数，性质在表4.1中，其中正交矩阵的逆就是它的转置。

![Table 4 1](pic/4/table_4_1.png)

>* Transforms are a basic tool for manipulating geometry. Most graphics application
programming interfaces let the user set arbitrary matrices, and sometimes a library
may be used with matrix operations that implement many of the transforms discussed
in this chapter. However, it is still worthwhile to understand the real matrices and
their interaction behind the function calls. Knowing what the matrix does after such
a function call is a start, but understanding the properties of the matrix itself will
take you further. For example, such an understanding enables you to discern when
you are dealing with an orthogonal matrix, whose inverse is its transpose, making for
faster matrix inversions. Knowledge like this can lead to accelerated code.
----
变换是处理几何体的基础工具。

大多数的图形API都会允许设置任意的矩阵，通常会有一个实现了本章讨论的大多数矩阵操作的库。

然而，它仍然值得去弄懂真正的矩阵以及在函数调用背后的互相影响。

知道一个函数调用后这个矩阵做了什么只是一个开始，理解矩阵本身的意义能让你更进一步。

比如当你处理正交矩阵时，因为正交矩阵的转置就是它的逆，这能帮助你更快的求逆。

这些知识能够让你的代码跑得更快。

## Basic Transforms
基础变换

>* This section describes the most basic transforms, such as translation, rotation, scaling,
shearing, transform concatenation, the rigid-body transform, normal transform (which
is not so normal), and computation of inverses. For the experienced reader, this can
be used as a reference manual for simple transforms, and for the novice, it can serve
as an introduction to the subject. This material is necessary background for the rest
of this chapter and for other chapters in this book. We start with the simplest of
transforms—the translation.
----
这一章节会讲述最基础的变换，比如平移，旋转，缩放，倾斜，变换串联，刚体变换，法线变换，以及逆运算。

如果你很有经验，这一章可以作为简单变换的参考手册。

对于新手，这是对这个主题的介绍。

这一部分对本书剩余的部分是不可缺少的。

我们先从最简单的开始 -- 平移。

### 4.1.1 Translation
平移

>* A change from one location to another is represented by a translation matrix, T.
This matrix translates an entity by a vector t = (tx, ty, tz). T is given below by
Equation 4.3:
----
从一个坐标移动到另一个坐标的变换能表示成一个平移矩阵。 T。

给定移动的向量 t = (tx, ty, tz)， T 如下：

![Formula 4 3](pic/4/formula_4_3.png)

>* An example of the effect of the translation transform is shown in Figure 4.1. It is
easily shown that the multiplication of a point p = (px, py, pz, 1) with T(t) yields a
new point p′ = (px+tx, py+ty, pz+tz, 1), which is clearly a translation. Notice that a
vector v = (vx, vy, vz, 0) is left unaffected by a multiplication by T, because a direction
vector cannot be translated. In contrast, both points and vectors are affected by the
rest of the affine transforms. The inverse of a translation matrix is T−1(t) = T(−t),
that is, the vector t is negated.
----
图4.1展示了平移变换的效果。

它表示了一个点从 p = (px, py, pz, 1) 进过 T(t) 得到一个新的点 p′ = (px+tx, py+ty, pz+tz, 1)

这明显是一次平移。

注意到向量  v = (vx, vy, vz, 0) 并不受 T 的影响，因为向量不能被平移。

相比之下，点和向量都会受到仿射变换剩余的部分的影响（不算平移）。

平移矩阵的逆是 T^(−1)(t) = T(−t)，也就是说向量 t 是负的。

![4.1](pic/4/4.1.png)

>* We should mention at this point that another valid notational scheme sometimes
seen in computer graphics uses matrices with translation vectors in the bottom row.
For example, DirectX uses this form. In this scheme, the order of matrices would
be reversed, i.e., the order of application would read from left to right. Vectors and
matrices in this notation are said to be in row-major form since the vectors are rows.
In this book, we use column-major form. Whichever is used, this is purely a notational
difference. When the matrix is stored in memory, the last four values of the sixteen
are the three translation values followed by a one.
----
我们需要提到的一点是，有时候会有另一种的表示方式，将平移的向量放在最底下的那一行中。

比如，DX就是这么做的。

在这种情况下，矩阵的顺序应该被反过来，就是矩阵的顺序应该从左往右。

向量和矩阵在这种表示下都被称为 行主矩阵，因为向量都是行。

在这本书中，我们使用 列主形式。

不管使用哪种，都只是一个表示方式上的区别。

当矩阵被存入内存时，16个数字的最后4个值时，3个平移值，加上一个 1。

### 4.1.2 Rotation
>* A rotation transform rotates a vector (position or direction) by a given angle around
a given axis passing through the origin. Like a translation matrix, it is a rigid-body
transform, i.e., it preserves the distances between points transformed, and preserves
handedness (i.e., it never causes left and right to swap sides). These two types of
transforms are clearly useful in computer graphics for positioning and orienting objects.
An orientation matrix is a rotation matrix associated with a camera view or
object that defines its orientation in space, i.e., its directions for up and forward.
----
旋转会对一个矢量（点或者向量），进行通过原点的给定轴，进行一定角度的旋转。

跟平移矩阵一样，它是一个刚体变换，就是说，它保证了变换后的点之间的距离，以及偏手性（即，它不会导致左右交换）

这两种类型的变换是经常使用的。

方向矩阵就是摄像机空间+旋转矩阵。

>* In two dimensions, the rotation matrix is simple to derive. Assume that we have
a vector, v = (vx, vy), which we parameterize as v = (vx, vy) = (r cos θ, r sin θ). If
we were to rotate that vector by φ radians (counterclockwise), then we would get
u = (r cos(θ + φ), r sin(θ + φ)). This can be rewritten as
----
在2维空间中，旋转矩阵的推导很简单。

假设我们有一个向量 v = (vx, vy)，我们将v写作 v = (vx, vy) = (r cos θ, r sin θ)。

我们假设旋转向量  φ （逆时针方向），我们会得到 u = (r cos(θ + φ), r sin(θ + φ))。

写作：

![Formula 4 4](pic/4/formula_4_4.png)

>* where we used the angle sum relation to expand cos(θ + φ) and sin(θ + φ). In three
dimensions, commonly used rotation matrices are Rx(φ), Ry(φ), and Rz(φ), which 
rotate an entity φ radians around the x-, y-, and z-axes, respectively. They are given
by Equations 4.5–4.7:
----
我们把2D的结果向3D拓展。

在3D中，常用的旋转矩阵是 Rx(φ), Ry(φ), 和 Rz(φ)

他们代表绕  x-, y-, 和 z-轴旋转，他们如下：

![Formula 4 5 4 7](pic/4/formula_4_5_4_7.png)

>* If the bottom row and rightmost column are deleted from a 4 × 4 matrix, a 3 × 3
matrix is obtained. For every 3×3 rotation matrix, R, that rotates φ radians around
any axis, the trace (which is the sum of the diagonal elements in a matrix) is constant
independent of the axis, and is computed as [997]:
----






