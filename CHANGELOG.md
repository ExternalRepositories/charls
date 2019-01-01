# Change Log

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/) and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]

### Changed

- Improved the validation of the JPEG stream during decoding
- Use #pragma once (supported by all modern C++ compilers)
- Updated referenced NuGet packages to their latest versions
- Update CMake build script to 3.9 (Modern CMake structure)
- charls_error is replaced by C++11 compatible jpegls_errc error code enum
- All types are now in the charls C++ namespace
- Support for .NET Code Contracts has been removed

### Fixed

- Fixes [#35](https://github.com/team-charls/charls/issues/35), Encoding will fail if the bit per sample is greater than 8, and a custom RESET value is used

## [2.0.0] - 2016-5-18

### Changed

- Updated the source code to C++14
- Refactored several APIs to make usage of the library easier

### Fixed

- Fixes [#10](https://github.com/team-charls/charls/issues/10), Fixed the problem that "output buffer to small" was not detected when writting encoded bytes to a fixed output buffer. This could cause memory corruption problems
- Fixes [11](https://github.com/team-charls/charls/issues/11), Update charlstest to return EXIT_SUCCESS/FAILURE
- Fixed the issue that DecodeToPnm would set params.colorTransform = ColorTransformation::BigEndian but the library didn’t support this option during decoding

## [1.1.0] - 2016-5-15

### Fixed

- Fixes [#9](https://github.com/team-charls/charls/issues/9) EncoderStrategy::AppendToBitStream method fails if buffer is full, 31 bits are added and xFF bytes are written

## [1.0.0] - 2010-11-18

First release of the CharLS JPEG-LS codec.
