## 基于Qt前端的mplayer播放器项目文档 v0.01
* AUTHOUR：zs_hust
* EMAIL: zhengsong2017@gmail.com


#### 引言
随着近几年嵌入式技术的飞速发展和广泛应用, 嵌入式设备与用户亲密接触,自然友好、简单便捷的人机交互界面成为了嵌入式设备的迫切需要，以GUI屏幕为中心的嵌入式图形界面给人们很大的亲和力。目前发展较为成熟的主要有以下几种：
* MiniGUI:图形设备层次过于抽象，图形功能不够完善;
* Microwindows:源码开放，但是其很多的图形引擎算法底下，控件或构建的实现还很不完备，系统整体不够完善;
* OpenGUI：内核采用汇编实现，不支持多进程，目前发展较慢;
* Qt/Embedded：面向嵌入式系统图形用户界面的应用程序开发框架，其库采用C++封装，完全面向对象，可移植性好;

Qt/Embedded开发过程即完整又独特，界面设计美观，拥有一套比较完整的嵌入式GUI解决方案，因此在众多的GUI里面发展较快。为了解决跨平台的程序运行问题，本次采用的是Qt/Embedded作为移植对象移植到目标机上。


#### Qt/Embedded概述
Qt/Embedded是以原始的Qt为基础，并作出了许多出色的调整以适用于嵌入式环境。他采用了轻量级窗口系统，在底层完全屏蔽了Xlib，仅使用Framebuffer作为图形接口，同时支持键盘、GMP鼠标、触摸屏以及用户自定义的设备等，并将外部输入设备抽象为键盘、鼠标和触摸屏的输入事件来实现事件驱动对外部事件的响应。

由于在嵌入式开发环境下开发和调试应用程序比较复杂，可以采用[NFS将文件系统挂载在宿主机][2]的方式对应用程序进行调试。调试成功之后再将程序加载到嵌入式linux的文件系统上。

Qt中最重要的是Qt的元对象系统。元对象系统有两个关键的技术：信号和槽，以及内省，其中内省是信号和槽的基础。由于Qt通过moc来解析Q_OBJECT类定义，所以Qt的元对象系统可以在任意的C++编译器上工作，这也是开发者觉得源程序的变异速度较慢的原因。

![Qt创建应用程序](http://upload-images.jianshu.io/upload_images/6128001-96e3d75a8d3081ac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

程序完成之后再使用qmake工具生成Makefile文件，在运行make即可生成可执行文件。



mplayer支持大量的多媒体文件格式，常见的音频文件如：MP3/wav/mid，常见的视频文件如：avi/vcd/dvd/MP4/rmvb等，各种视频编码/解码方式也是应有尽有。

我的目标是在linux下用Qt做一个MPlayer的播放器界面，能够实现播放器的常见功能，如：播放、停止、快进、快退、上/下一曲等。并把程序移植到ARM平台上。

### mplayer的安装

在ubuntu下可以直接使用`sudo apt install mplayer`安装，也可以自己使用源码编译安装[mplayer][1]。

mplayer可以工作在slave模式，这就为Qt的界面程序当中对其进行控制提供方便。

> mplayer的salve模式：即运行过程中能够接收用户的命令行输入，具体的信息可以在源码的slave.txt中查看，或者使用mplayer -input cmdlist查看

slave模式下工作的mplayer可以和系统的标准输入、输出进行信息交互。我们可以使用linux C编程来完成对slave模式工作的mplayer进行控制和信息的获取。如：

```
int main(int argc, char const *argv[])
{
	mkfifo("/tmp/fifo",0777);
	FILE *mp;
	mp = popen("mplayer /home/zs/qtBuild/video/test.mp4 -quiet -slave -gui -input file=/tmp/fifo","r");
	sleep(1);//等待mplayer启动，否则程序会直接退出而没启动mplayer。
	return 0;
}

```

程序启动之后就可以在命令行通过fifo文件给mplayer写控制信息，如：

`echo “mute 0\n” > /tmp/fifo `


### 界面文件UI的编写
Qt中提供了很好的方法来实现调用mplayer并对其进行控制。通过定义一个QProcess对象来调用mplayer。
```
mplayerProcess = new QProcess(this);
mplayerProcess>setProcessChannelMode(QProcess::MergedChannels);
mplayerProcess->start(programs, args);

```

#### 定位视频窗口
为了将视频的窗口定位在我们指定的widget的位置，而不是新弹出一个窗口进行播放，我们需要对其进行定位 。mplayer中可以指定参数-wid 来指定在我们设定好的控件当中播放视频，通常设置为：`args << "-wid"<<QString::number(this->winId());` this是你需要用来播放视频的控件的名字。**-wid 参数只有在X11、directX和openGL当中适用**。



[1]:http://www.mplayerhq.hu/design7/news.html
[2]:http://blog.csdn.net/Wind4study/article/details/53366379