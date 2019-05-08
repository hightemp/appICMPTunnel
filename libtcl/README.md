# libtcl
libtcl - Tempow Concurrency Library

[![Build Status](https://travis-ci.com/uael/libtcl.svg?token=x8y8gmMrerkmyNtJbqyy&branch=master)](https://travis-ci.com/uael/libtcl)

- [Introduction](#introduction)
- [License](#license)
- [Install](#install)
- [Usage](#usage)
- [Credits](#credits)
- [Bugs, vulnerabilities and contributions](#bugs-vulnerabilities-and-contributions)
- [Resources](#resources)
- [Badges and static analysis](#badges-and-static-analysis)
  - [Travis CI](#travis-ci)
  - [Clang's Static Analyzer](#clangs-static-analyzer)

## Introduction

libtcl is a multipurpose concurrency library in pure c99 without dependencies.
It provide coroutines, fibers, a reactor pattern and other concurrency utilities.

## License

See the [LICENSE](https://github.com/tempow/libtcl/blob/master/LICENSE) file.

## Install
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ make check
```

## Usage

See the [test](https://github.com/tempow/libtcl/blob/master/test) directory

## Credits

The stuff was originaly written by Google for the [fluorid](https://android.googlesource.com/platform/system/bt) project in the [osi](https://android.googlesource.com/platform/system/bt/+/android-7.1.2_r36/osi/) directory.

## Bugs, vulnerabilities and contributions

Bug  and vulnerability  reports are  appreciated, all  the vulnerability reports  are  public; register  them  using  the  Issue Tracker  at  the project's Github  site.  For  contributions and  patches please  use the Pull Requests feature at the project's Github site.

Reports about the original code must be registered at:
<https://github.com/tempow/libtcl/issues>

## Resources

Development of the original projects takes place at:
<https://github.com/tempow/libtcl/>

## Badges and static analysis

### Travis CI

Travis CI is  a hosted, distributed continuous  integration service used to build and test software projects  hosted at GitHub.  We can find this project's dashboards at:
<https://travis-ci.org/tempow/libtcl>

Usage of this service is configured through the file ".travis.yml".

### Clang's Static Analyzer

The Clang Static Analyzer is a source code analysis tool that finds bugs in C, C++, and Objective-C programs.  It is distributed along with Clang and we can find it at:
<http://clang-analyzer.llvm.org/>

Usage of this service is implemented with make rules; see the relevant section in the file "Makefile.am".
