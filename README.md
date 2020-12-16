# GLchess

基于 OpenGL 的国际象棋游戏 Demo

## Build

- Visual Studio on Windows
- 或直接运行 glchess.exe

## 操作方法

1. 模型相关
- SPACE：初始化对战棋盘
- 鼠标左键：选中某个模型（建议选中棋子）
- E/e：开关编辑模式
- wasdrt：
  - 非编辑模式下：移动视角
  - 编辑模式下： 在三个方向上拉伸/缩放目前选中的模型
- Q/q：（选中模型后）导出目前选中的模型，保留缩放效果
2. 光照相关
- 0-7：选择控制的光源对象
- JKL/jkl：选择控制的光照类型，依次 AMIENT/DIFFUSE/SPECULAR
- UIO/uio：加/减控制的光源对应类型数值
- WASDRT：移动控制的光源的位置
3. 其他
- 鼠标左键拖动：平移
- 鼠标右键拖动：旋转
- P/p：截图
- z-m/Z-M：（键盘最后一行）添加棋子
- 选中棋子->点击棋盘：移动棋子

## 效果演示

- 初始界面

<img src="https://github.com/chronoby/glchess/blob/master/image/init.png" width="700"  alt="初始界面"/><br/>

- 光照

<img src="https://github.com/chronoby/glchess/blob/master/image/light.png" width="700"  alt="光照"/><br/>

- 棋子移动

<img src="https://github.com/chronoby/glchess/blob/master/image/move.png" width="700"  alt="棋子移动"/><br/>
