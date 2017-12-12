[TOC]

#本代码用于操作framebuffer



## framebuffer 

Framebuffer在Linux中是作为设备来实现的，它是对图形硬件的一种抽象，代表着显卡中的帧缓冲区.通过Framebuffer设备，上层软件可以通过一个良好定义的软件接口访问图形硬件，而不需要关心底层图形硬件是如何工作的，比如，上层软件不用关心应该如何读写显卡寄存器，也不需要知道显卡中的帧缓冲区从什么地址开始，所有这些工作都由Framebuffer去处理，上层软件只需要集中精力在自己要做的事情上就是了。

由于对Framebuffer设备的读写应该是不缓冲的，但是标准IO库默认是要进行缓冲的，因此通常不使用标准IO库读写Framebuffer设备，而是直接通过read()、write()或者mmap()等系统调用来完成与Framebuffer有关的IO操作。又由于mmap()能够大大降低IO的开销，因此与Framebuffer设备有关的IO通常都是通过mmap()系统调用来完成的。如果mmap()调用成功，就可以在程序中对得到的映射内存空间进行读写操作了。所有的读写都将由操作系统内核转换成IO操作。

> 系统调用mmap()完成的是内存映射IO,内存映射IO是指将一个磁盘文件的内容与内存中的一个空间相映射,当从这个映射内存空间中取数据时,就相当于从文件中读取相应的内容,而当向这个映射内存空间写入数据的时候,就相当于向磁盘文件中写入数据,这就是内存映射IO的含义.

应用程序需要通过**ioctl()系统调用来获取** framebuffer设备的相关参数,**在头文件<linux/fb.h>中定义了所有的ioctl命令字**,常用的命令字是:**FBIOGET_FSCREENINFO,FBIOGET_VSCREENINFO,FBIOPUT_VSCREENINFO**

- **固定信息**有图形硬件上实际的帧缓存空间的大小,能都硬件加速等信息.放入`struct fb_fix_screeninfo`结构体当中,----这个结构在显卡被设定模式之后创建,他描述的是显示卡的属性,并且在系统运行时不能够被修改;比如framebuffer的起始地址.他依赖于被设定的模式,当一个模式设定之后,内存的信息由显示卡的硬件给出,内存的位置等信息就不可修改.
- **可变信息**有长度,宽度,颜色深度等信息.同样的图形硬件,可以工作在不同的模式下,如一个支持1024x768x24图形模式的硬件通常也可以工作在800x600x16的图形模式之下.该信息放入`struct fb_var_screeninfo`结构体当中.

应用程序中常使用的是`fb_var_screeninfo`结构体当中的`xres,yres,bits_per_pixel`,分别表示x轴的分辨率,y轴的分辨率以及每像素的颜色深度(单位为bit/pixel).其类型定义都是无符号32位整型数。







### PC上打不开framebuffer设备?

要在PC平台上启用Framebuffer，首先必须要内核支持，这通常需要重新编译内核。另外，还需要修改内核启动参数。在Linux PC机上可能不会加载framebuffer的驱动,需要你手动的加载它或者将其加入启动列表当中,例如在我的ubuntu 17.04的`/etc/modprobe.d/blacklist-framebuffer.conf`文件当中有这么一段话:

>  Framebuffer drivers are generally buggy and poorly-supported, and cause suspend failures, kernel panics and general mayhem.For this reason we never load them automatically.

于是我们可以开启framebuffer:

1. 在`/etc/initramfs-tools/modules`文件当中加入两行:`fbcon`和`vesafb`.

> ***[/etc/initramfs-tools/modules文件的作用](http://blog.csdn.net/lixiangminghate/article/details/50061251)***
>
> 

2. 在`/etc/modprobe.d/blacklist-framebuffer.conf`文件当中找到含有vesafb的行并注释掉.
3. 更新initramfs文件.`sudo update-initramfs -u`

> ***initramfs文件的作用***
>
> 通过脚本文件update-initramfs(位于/usr/sbin/update-initramfs)来更新initramfs文件,

4. 重启系统



可以用命令: #dd if=/dev/zero of=/dev/fb0 清空屏幕.

如果显示模式是 1024x768-8 位色，用命令：$ dd if=/dev/zero of=/dev/fb0 bs=1024 count=768 清空屏幕.

用命令: #dd if=/dev/fb0 of=fbfile  可以将fb中的内容保存下来；

可以重新写回屏幕: #dd if=fbfile of=/dev/fb0