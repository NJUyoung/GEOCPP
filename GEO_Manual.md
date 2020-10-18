# GEO Manual

#### 代码部分

ORIGIN_DATA: fits文件未经处理过的像素值

DATA2: 在处理过程中的掩码矩阵

DATA：处理后的输出矩阵

#### 文件读取

value：读取fits文件原本的像素值
	/x_label:横坐标
	/y_label:纵坐标

newvalue:  读取预处理后的fits文件的像素值
	/x_label:横坐标
	/y_label:纵坐标

naxis1: x轴长度

naxis2: y轴长度

#### 文件预处理可选步骤

addnew: 延展目标恢复，会使得目标变大，一般仅用于连通域的标记中

twovalue: 可将处理的矩阵变为0-1矩阵，方便进行连通域标记