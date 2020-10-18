# GEO

## 1. FITS文件读入

#### 1.1 头文件的读取

使用getline函数获取头文件的信息后，主要借助Find函数找到各个关键词，在从中提取我们需要的信息。

##### BITPIX

<img src="/Users/young/Library/Application Support/typora-user-images/截屏2020-07-01上午10.03.29.png" alt="截屏2020-07-01上午10.03.29" style="zoom:50%;" />

[1]

使用getline函数获取头文件字符后用Find寻找BITPIX关键词，获取其数值可确定FITS文件的数据类型用以进行计算。

对应函数为

```C++
int HEAD::TYPE
```

##### NAXIS

代表数据的维数，此项目处理的一般为二维图像，所以此值一般为2，不作特殊考虑。

##### NAXIS1/NAXIS2

NAXIS1和NAXIS2分别代表图像的长和宽，以此作为数据单元矩阵的大小。

对应函数为

```C++
void HEAD::NAXIS
```

##### BSCALE/BZERO

BSCALE与BZERO一般成对出现以转换数据单元的大小。

$physicalvalue = \bold{BZERO} + \bold{BSCALE} × arrayvalue$ [1]

BSCALE的值一般为1，BZERO可正可负，要注意符号

对应函数为
```C++
void HEAD::BVALUE
```

#### 1.2 数据单元读取

读取array内的数据大小，主要分为确定位置和分类型读取。

##### 数据位置确定

头文件一般由一或几个header block组成，一个header block由2880 bytes组成，所以数据的开头一定在一个header block后。

以header block为单位寻找END字节所在的位置，其所在的header block即为HDU的最后一个header block。

对应函数为
```C++
int HEAD::DATA_POSITION
```

##### 分类型读取

根据前面BITPIX所读出的值确定数据的类型

如BITPIX = 16时，对应函数为

```C++
void DATAPART::READING16
```



## 2. 图像预处理

#### 2.1 图像分区

##### 2.1.1 分界线的确定

以Sobel算子为核心，现可转换为比较每一行上下两行像素相加大小的绝对值，相邻两行相加像素的绝对值越大，说明此行两边的背景差异越大，列的分割同理。

因为有可能出现分列的位置过于相近，所以设置判断机制，保证分块的大小不会过小。

##### 2.1.2 逐块背景抑制[2]

求每一块分区中的中位数，新的亮度值设置为

<img src="/Users/young/Library/Application Support/typora-user-images/image-20200701112233904.png" alt="image-20200701112233904" style="zoom:67%;" />

#### 2.2 全局阈值过滤[2]

过滤图像偶发噪点，低于阈值亮度设为0，高于阈值保留亮度值。

阈值由全图均值和标准差给出

<img src="/Users/young/Library/Application Support/typora-user-images/image-20200701112424219.png" alt="image-20200701112424219" style="zoom:80%;" />

k的取值使用经验表达式

<img src="/Users/young/Library/Application Support/typora-user-images/image-20200701112533676.png" alt="image-20200701112533676" style="zoom:80%;" />

#### 2.3 离散噪点抑制[2]

在每个像素周围宽高半径各为2个像素的范围内搜索，也就是搜索面积为5*5的区域；认定有效信号的亮像素比例阈值为20%，也就是5个。

若区域内的有效信号的个数少于五个，则认定为噪声。

## 3. 图像信号提取

#### 3.1 连通域标记

连通域分为四连通域和八连通域

二维矩阵res标记连通域的位置，

#### 3.2 形状判断

##### 3.2.1 坐标确定

##### 3.2.2 最小外接矩形

由于所处理的大部分FITS文件恒星都不水平，所以在形状判断的时候需要进行角度的确定，考虑到恒星图像的特性，我们决定采用最小外接矩形的方法来确定对象的形状特征和倾斜角度。



## 4.试验记录

#### 4.1 主要试验对象

##### 1.fit

BITPIX = 16

1024*1024

恒星不水平，有波浪形噪声

##### 2.fit

BITPIX = 16

3056*3056

恒星基本水平，图像中心大块部分较亮

##### 4.fits

BITPIX = 16

4160*4136

恒星不水平，背景有明显的深浅分块

#### 4.2 试验记录

##### 2020.07.01

不进行划分分块，进行到连通域的标记与描述

连通域大小阈值：5

1.fit

<img src="/Users/young/Library/Application Support/typora-user-images/截屏2020-07-01上午11.00.30.png" alt="截屏2020-07-01上午11.00.30" style="zoom:50%;" />

2.fit

<img src="/Users/young/Library/Application Support/typora-user-images/截屏2020-07-01上午11.01.31.png" alt="截屏2020-07-01上午11.01.31" style="zoom:50%;" />

4.fits

<img src="/Users/young/Library/Application Support/typora-user-images/截屏2020-07-01上午11.02.06.png" alt="截屏2020-07-01上午11.02.06" style="zoom:50%;" />

在包含根据背景深度进行分块时，所用时间基本为0.6s, 6s, 10s, 可见此步所占时间过大，需要使用其他步骤修改或代替。

##### 2020.07.03

最小外接矩形（MBR）的方法选择

外接矩形计算通常有两种基本方法，一种为通过坐标的最大最小值直接确定矩形，也就是我们一开始所使用的方法，但这种方法误差较大；另一种是等间隔旋转法，将外接矩形的角度等间隔旋转，寻找最佳的外接矩形，此种方法的问题在于耗时较长。

最小外接矩形也分为最小面积外接矩形和最小周长外接矩形，两者一般差距不大，为了计算方便，我们使用最小面积外接矩形。

根据文献的查阅，采用基于链码和离散格林的主轴法[3]效果比较出众。

##### 2020.07.05

首先需要获取连通域的链码（chain code）

思路：使用连通域标记的坐标寻找起始点，在原图中描绘出图像的边界，顺序从上到下，从左到右，逆时针旋转。

一共有八个方向，在上述原则下，每一种方向后的方向都会有固定的可能点，如前一个方向是0（水平向右），下一个方向只可能是1（右上），0（水平向右），7（左下）。用这种原则可以大大减小运算量，减少寻找的复杂度。

##### 2020.07.06

起始点定为连通域中最上面的最左边点，寻找每个连通域的起始点

1.fit 

<img src="/Users/young/Library/Application Support/typora-user-images/截屏2020-07-06上午10.26.27.png" alt="截屏2020-07-06上午10.26.27" style="zoom:40%;" />

x为574与576的两个起始点应是同一个连通域中出现断点分出的两部分，或出现了八连通部分但没有四连通部分。

2.fit

## ![截屏2020-07-06上午10.29.48](/Users/young/Library/Application Support/typora-user-images/截屏2020-07-06上午10.29.48.png)

4.fits

![截屏2020-07-06上午10.30.54](/Users/young/Library/Application Support/typora-user-images/截屏2020-07-06上午10.30.54.png)

## 5. 参考文献

1. Definition of the Flexible Image Transport System
2. 图像预处理和目标提取
3. 一种提取目标图像最小外接矩形的快速算法

