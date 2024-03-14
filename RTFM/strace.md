# 工具学习----```strace```

这是一个强有力的跟踪程序,展示一个程序内部发生了什么的工具, 这里是一些对manual和实际运用的总结

## ```strace -e trace=....```

这表明我只希望追踪特定的系统调用,比如:

```sh
 strace -e trace=execve,openat,close,read,write cat a
```

这会显示:

```sh
execve("/usr/bin/cat", ["cat", "a"], 0x7ffec69a0058 /* 40 vars */) = 0
···
openat(AT_FDCWD, "a", O_RDONLY)         = 3
···
read(3, "hello! makisa kurusi!\n", 131072) = 22
write(1, "hello! makisa kurusi!\n", 22) = 22
read(3, "", 131072)                     = 0
close(3)                                = 0
close(1)                                = 0
close(2)                                = 0
+++ exited with 0 +++
```

可以看到发生的过程如上所示(做了一些删改)

