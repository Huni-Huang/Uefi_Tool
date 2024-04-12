EDK II UEFI驱动程序-空白驱动程序-不做任何事情。
这个UEFI驱动程序不做任何事情，只是提供了一个UEFI驱动程序模板来支持UEFI Spec 2.3。
它包括驱动绑定协议、组件名称协议和卸载功能。
使用方法:
1)将所有*.txt重命名为相应的扩展文件名(例如。c，。h，。inf)
2)添加BlankDrv。.dsc文件中平台组件部分的Inf workspace路径，用于构建

结果:
BlankDrv。Efi将在构建输出目录中构建。