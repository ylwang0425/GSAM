# GSAM

基于广义后缀自动机拓展的泛基因图匹配算法

#### 需要的环境

`c++17`、`boost`

可以使用`sudo apt install libboost-all-dev`

或者使用conda但是作者研究了半天还是没有研究出来怎么include虚拟环境下的boost。

#### 运行

快速入门: `gsam -g test/chr4_GL000008v2_random.gfa -f test/wrong_read1000.fa -a res.gaf`

帮助可以使用`gsam --help`、`gsam -h`、`gsam -h`参看参数

会得到如下结果

```
read1   1000   0  999    +  <s85226>s108098>s108099>s145184<s85224 1000   135    1135   979    1003   60 NM:i:24    AS:f:929.412   dv:f:0.0239282 id:f:0.976072  cg:Z:129=1X60=1X15M1X43=1I5M1D4M1X42=1X27M1X11=1X37=1D144=1X10=1X38=1X3M1I55=1I5M1D32=1X10=1X19=1X18=1X43=1X60=1X19=1X128=1X23=
```

可以看到GSAM找到1000个序列中的24个错误，按照cigar可以匹配到图上的相应位置。结果不一定是错误个数最少的结果，一定是最接进最少的结果

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

![图2](https://user-images.githubusercontent.com/56342176/185865907-c9f84ea5-d7c7-419f-99f5-e0a3b07677b1.png)
<p align="center">图2</p>

##### 解决read里面有错误的问题

下图(a)、(b)为有一个错误的图上匹配

![图片3](https://user-images.githubusercontent.com/56342176/185888199-79fbc97e-f6ee-4d7e-a13b-ae4ca49ad06e.png)

<p align="center">图3</p>



按照上述自动机拓展的方法进行拓展匹配

1. “T”匹配到>3[0]
2. “ACGTA”匹配到>2[0-4]，然后进行拓展，“TACGTA”匹配到>1[7]>2[0-4]
3. “TGT”匹配到>4[5-7]
4. “ACTT”匹配到>4[0-3]，然后进行拓展，“GTACTT”匹配到>2[6-7]>4[0-3]

然后根据这四个匹配串然后推理出路径。这里暴力枚举每个匹配串为起点，然后如果两个匹配串在图上的距离小于d（这是一个参数，默认设置了100，图小的话改小一点）的时候，就将匹配串连起来。如果从某一个起点开始，连起来的路径超过read长度的一半的时候停止。

这么做理论上最坏时间复杂度是![wps8](https://user-images.githubusercontent.com/56342176/185888788-864c1a8c-a94c-4644-87af-b0e2ec2c6788.jpg)
但是，这和匈牙利算法差不多，理论是![wps8](https://user-images.githubusercontent.com/56342176/185888819-4af3fab8-8531-4dea-a728-871c0f41fc1d.jpg)实际上远小于![wps8](https://user-images.githubusercontent.com/56342176/185888832-aa6c409b-f509-4561-a115-503afb2162e3.jpg)，正常情况下，第一个或者第二个匹配串做为起点就能把路径给找出来了。所以实际运行中，这么干基本上是线性的。

##### dp获得cigar字符串

如果直接将read和按照路径拼起来的路径进行dp时间复杂度太高。所以对那些已经匹配的匹配串不管，记成匹配(Match)，只对没有的匹配的部分进行dp。按照最短编辑距离<a href="https://www.luogu.com.cn/problem/P2758">（https://www.luogu.com.cn/problem/P2758）<a/>进行dp。

![wps11](https://user-images.githubusercontent.com/56342176/185889106-7f80d0bd-ca99-4160-bcf6-22c08a2c37d3.jpg)

dp的时候记录状态的转移，然后进行回溯。就得到cigar字符串。这个时间复杂度理论上是远小于最坏时间复杂度<p align="center">![wps12](https://user-images.githubusercontent.com/56342176/185889125-bd656290-112e-4486-8bb3-6619d39c2a87.jpg)</p>d上面讲过默认设置了100所以最坏也是线性的。

