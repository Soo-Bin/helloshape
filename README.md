# Hello Shape!

shapelib 사용법 정리

### 순서

- [print_information.cc](https://github.com/Soo-Bin/helloshape/blob/main/print_information.cc): dbf 파일 읽기
- [print_geometry.cc](https://github.com/Soo-Bin/helloshape/blob/main/print_geometry.cc): shp 파일 읽기
- [print_rewind.cc](https://github.com/Soo-Bin/helloshape/blob/main/print_rewind.cc): vertex rewind (결과 필요)
- [copy_information.cc](https://github.com/Soo-Bin/helloshape/blob/main/copy_information.cc): dbf 복사하기
- [copy_geometry.cc](https://github.com/Soo-Bin/helloshape/blob/main/copy_geometry.cc): shp 복사하기
- [modify_information.cc](https://github.com/Soo-Bin/helloshape/blob/main/modify_information.cc): dbf 속성 중 하나 수정하기

## Build

### Windows

```sh
$ mkdir build
$ cd build
$ cmake -G "Visual Studio 16 2019" -A x64 ..
```

### Linux

```sh

```
