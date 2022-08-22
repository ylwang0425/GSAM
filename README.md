# GSAM

基于广义后缀自动机拓展的泛基因图匹配算法

#### 需要的环境

`c++17`、`boost`

可以使用`sudo apt install libboost-all-dev`

或者使用conda但是作者研究了半天还是没有研究出来怎么include虚拟环境下的boost，救救孩子吧。

#### 运行

快速入门: `gsam -g test/chr4_GL000008v2_random.gfa -f test/wrong_read1000.fa -a res.gaf`

帮助可以使用`gsam --help`、`gsam -h`、`gsam -h`参看参数

#### 参数

`-g`输入的图gaf格式

`-f`read文件目前只开发了读fa文件,fq以及压缩包待开发中...

`-t`线程数

`-a`输出文件，目前支持json和gaf文件，可以输出多个文件-a out1 -a out2

`-p`惩罚系数，假入说惩罚系数为p
$
alignmentScore = 读段长度 - （1+p/(1-p)）*错误个数
$


`--tolerant-length=x`两个后缀自动机拓展段的最长距离默认为100

`-h` 查看帮助信息

`--include-cigar=true/false`gaf文件是否包括cigar字符串

`--beautiful-json`json格式是为压缩的还是展开的true为展开，false是压缩的

#### 大体思路

##### 建立后缀自动机

首先对所有sequence忽略掉图的结构建立广义后缀自动机，下图为广义后缀自动机的一个例子。并且在建立自动机的时候进行预处理。创建状态时令，<br/>
<p align="center">
$firstpos(cur) = len(cur)$<br/>
$id(cur) = getId(cur)$<br/>
</p>


复制状态时需要令<br/>
<p align="center">
$firstpos(clone) = firstpos(q)$<br/>
$id(clone) = id(q)$<br/>
</p>

![image](https://user-images.githubusercontent.com/56342176/185861357-3612df49-79a8-4e5e-9117-9d133463252c.png)<br/>
<p align="center">图1</p>

##### 后缀自动机拓展

在没有任何错误的情况下read匹配到图上序列的时候，对自动机进行左右方向的拓展。就如图2来说首先“T”匹配到3节点的第一个字符。由于序列图上没有“TA”所以第一段匹配失败了，对“T”这个匹配段向正向和反向拓展，发现没法匹配。所以第一次匹配结束匹配结果是>3[0]。然后从A开始重新匹配“ACGTACGT”这一段，结果为2[0-7]。然后对“ACGTACGT”这段进行正向和反向拓展，正向拓展了4号节点的“ACTT”，然后反向拓展1号节点的“T”。因此第二段匹配了1[7]>2[0-7]>4[0]。

![图2](https://user-images.githubusercontent.com/56342176/185865594-8861a06a-a3bc-474d-b5ff-ddd3fd4ad503.png)
<p align="center">图2</p>

