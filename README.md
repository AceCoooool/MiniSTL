# MiniSTL
本仓库原是《STL源码剖析》一书的代码合集，并在其基础上运用部分C++17(主要是C++11) 特性进行了代码改写。

**说明** ：代码基本上来自[zsmj2017/MiniSTL](https://github.com/zsmj2017/MiniSTL)

### 编译环境

- Ubuntu16.04
- gcc-8.1，g++-8.1

> 注：个人采用CLion运行的

### 完成进度

- [x] Allocator (空间适配器)
- [x] Iterator (迭代器与traits编程技法)
- [x] Sequence (序列式容器)
- [x] Associative (关联式容器)
- [x] Algorithm (算法)
- [x] Functor (仿函数)


### TODO

- [ ] 将绝大多数std更改为MiniSTL实现的版本

- [ ] 替换try...catch

  ```cpp
  #   define __STL_NOTHROW throw()
  #   define __STL_UNWIND(action) catch(...) { action; throw; }
  # else
  #   define __STL_TRY 
  // 上述可以知道具体对应关系
  ```

### Reference

1. [zsmj2017/MiniSTL](https://github.com/zsmj2017/MiniSTL)
2. [steveLauwh/SGI-STL](https://github.com/steveLauwh/SGI-STL)