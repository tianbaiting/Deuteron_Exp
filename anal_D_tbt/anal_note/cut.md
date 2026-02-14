


## 博源的cut

![Alt text](%7B4B8B9F92-62C1-45BC-A986-ABA3536837DB%7D.png)


## cut原因

由于氘核自发裂解。

z方向取向的只有几乎只有z方向背对背出射。

（侧面反映了，其实ImQMD的计算里初始值几乎不会有偏离z方向的取向的。ImQMD高斯波包的弥散程度远比我们想象的小。）



### z张量极化的图

![Pz+-](../original_data_tbt/img/show_pz_+-.svg)


![pz](%7BF9DFDCA4-BB4C-40E4-9F99-D0CD8F196351%7D.png)




### y张量极化的图


![Alt text](%7B249DDE05-690F-4744-B7CB-23B166BC008A%7D.png)


### cut条件



| cut条件原因       | zpol       | ypol |
|------------|----------------|----------|
| 选择b较大的， （力的冲量较小，横向动量改变小）  | $p^p_x+p^n_x < 200 \mathrm{MeV}$     |$p^p_x+p^n_x < 200 \mathrm{MeV}$  |
|   去除背对背发射的,选择正常的物理事件  |  $p^p_{z'} - p^n_{z'} \in (-150,150) \mathrm{MeV}$     | $p^p_{y'} - p^n_{y'} \in (-150,150) \mathrm{MeV}$ |
| |  $p^p_{z'} + p^n_{z'} \in (1150,\infty) \mathrm{MeV}$    | |





## show

![未cut](../original_data_tbt/img/hist2d_sn124_pz_origin.svg)
ImQMD 原始数据 未按照b选择 $N \propto b$

![按b选择](../original_data_tbt/img/hist2d_sn124_pz_select_b.svg)

上图按照了b选择。

![Alt text](image-1.png)

![Alt text](image-3.png)


![Alt text](image-2.png)


## 加入ypol

![Alt text](image-4.png)


phi_fixed 下图 Sn124 ypol的

![Alt text](%7BA9AF1949-7B69-4106-B011-058DF59EDBC8%7D.png)


加入找旋转平面

![Alt text](%7B8B68BC35-289A-4C9A-A61B-8A3CD7ABA13A%7D.png)