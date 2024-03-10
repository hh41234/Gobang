## 五子棋项目 AI情况说明

### 文件内容
1、需要提交的AI代码 "sample.cpp"
2、需要打败的AI代码 "baseline.cpp"(因为默认前端AI是baseline，所以将它放在了该文件夹中)
3、"judge.py" Python交互文件
4、AI使用的头文件 "AIController.h" 
5、两个AI的Unix可执行文件 "sample"、"baseline"

### AI棋力
已经能够打败baseline(无论先后手)，并且不会5s超时
（注：WIndows系统用户需先将baseline.cpp和sample.cpp两个cpp文件运行后生成对应的exe可执行文件后再进行AI对打）

### 算法优化
sample.cpp是基于极小极大搜索算法、alpha-beta剪枝算法和启发式搜索算法的五子棋AI，相较于baseline，sample.cpp除了对本方的进攻方式有评估，对对方的防守方式也有评估，这对棋力也有一定的提升
在实现这个AI的过程中，主要参考了github上的博客https://github.com/lihongxun945/myblog/issues/11，三种算法在wiki上的伪代码以及baseline.cpp

7月17日 对sample.cpp开了O2优化，在运行速度上有一定的提升