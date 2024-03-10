## 五子棋项目 前端运行说明

### 文件内容
1、templates文件夹，包含html模板文件“index.html”
2、static文件夹，包含jQuery文件“jquery-3.6.0.min.js”、棋盘图片“board.png”、背景音乐"bgm.mp3"（因为使用audio标签导入bgm，所以可能会因为浏览器原因播放失败）
3、run.py Python交互文件

### 使用指令
1、使用 ’cd ..‘命令回到根文件夹”张倬珩“
2、使用 ’python3 front-end/run.py‘ 运行python文件
3、点击链接http://127.0.0.1:5000即可到达前端界面
（注：因为这是在macOS系统下的指令，AI文件为Unix可执行文件，所以若是Windows系统使用者，需先将baseline.cpp运行生成baseline.exe文件后再进行以上操作）
### 使用说明
在下棋前，需先选择模式，human-human为人人对战，human-AI为与AI人机对战，AI可在run.py 第64行通过更改路径改变AI，flip按键为三手交换按键（目前暂未设置仅在白棋方第2手时使用的功能）

### 优化
7月17日 将背景大小调整为棋盘大小，并添加了背景音乐