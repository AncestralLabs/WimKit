# Introduction

WimKit is a fork of wimlib version 1.14.3 (September 2023).  wimlib is a C 
library for creating, modifying, extracting, and mounting files in the Windows 
Imaging Format (WIM files). 

For the release notes, see the [NEWS file](NEWS.md).

# Table of Contents

- [WIM files](#wim-files)
- [Compression](#compression)
- [Windows PE](#windows-pe)
- [Dependencies](#dependencies)
- [Configuration](#configuration)
- [Portability](#portability)
- [References](#references)
- [History](#history)
- [Notices](#notices)

# WIM files

A Windows Imaging (WIM) file is an archive designed primarily for archiving
Windows filesystems.  However, it can be used on other platforms as well, with
some limitations.  Like some other archive formats such as ZIP, files in WIM
archives may be compressed.  WIM archives support multiple compression formats,
including LZX, XPRESS, and LZMS.  All these formats are supported by wimlib.

A WIM archive contains one or more "images", each of which is a logically
independent directory tree.  Each image has a 1-based index and usually a name.

WIM archives provide data deduplication at the level of full file contents.  In
other words, each unique "file contents" is only stored once in the archive,
regardless of how many files have that contents across all images.

A WIM archive may be either stand-alone or split into multiple parts.

An update of the WIM format --- first added by Microsoft for Windows 8 ---
supports solid-mode compression.  This refers to files being compressed together
(e.g. as in a .tar.xz or .7z archive) rather than separately (e.g. as in a .zip
archive).  This usually produces a much better compression ratio.  Solid
archives are sometimes called "ESD files" by Microsoft and may have the ".esd"
file extension rather than ".wim".  They are supported in wimlib since v1.6.0.

# Windows PE

wimlib can also be used to create customized images of Windows PE on either
UNIX-like systems or Windows.  Windows PE (Preinstallation Environment) is a
lightweight version of Windows that runs entirely from memory and can be used to
perform maintenance or to install Windows.  It is the operating system that runs
when you boot from the Windows installation media.

A copy of Windows PE can be found on the installation media for Windows (Vista
or later) as the file `sources/boot.wim`, or in the Windows Automated
Installation Kit (WAIK), which is free to download from Microsoft.

A shell script `mkwinpeimg` is provided with wimlib on UNIX-like systems to
simplify the process of creating and customizing a bootable Windows PE image,
sourcing the needed files from the Windows installation media or from the WAIK.

# Dependencies

This section documents the dependencies of wimlib and the programs distributed
with it, when building for a UNIX-like system from source.

- `libfuse3` (optional but recommended)

  Unless configured `--without-fuse`, wimlib requires `libfuse3`, for mounting
  WIM images using [FUSE](https://github.com/libfuse/libfuse).  Most Linux
  distributions already include this, but make sure you have the `fuse3` package
  installed, and also `libfuse3-dev` if your distribution distributes
  development files separately.  FUSE also requires a kernel module.  If the
  kernel module is available it should automatically be loaded if you try to
  mount a WIM image.

- `cdrkit` (optional)
- `mtools` (optional)
- `syslinux` (optional)
- `cabextract` (optional)

  The `mkwinpeimg` shell script will look for several other programs depending
  on what options are given to it.  Depending on your Linux distribution, you
  may already have these programs installed, or they may be in the software
  repository.  Making an ISO filesystem requires `mkisofs` from
  [`cdrkit`](https://www.cdrkit.org).  Making a disk image requires
  [`mtools`](https://www.gnu.org/software/mtools) and
  [`syslinux`](https://www.syslinux.org).  Retrieving files from the Windows
  Automated Installation Kit requires
  [`cabextract`](https://www.cabextract.org.uk).

# Configuration

This section documents the most important options that may be passed to the
"configure" script when building from source:

- `--without-fuse`

  The `--without-fuse` option disables support for mounting WIM images.  This
  removes dependencies on libfuse and librt.  The `wimmount`, `wimmountrw`, and
  `wimunmount` commands will not work.

  The default is `--with-fuse` when building for Linux, and `--without-fuse`
  otherwise.

# Portability

WimKit works only on MacOS X for Apple Sillicon architecture.

It has been tested on the ARM architectures.

Currently, gcc and clang are the only supported compilers.  A few nonstandard
extensions are used in the code.

# References

The WIM file format is partially specified in a document that can be found in
the Microsoft Download Center.  However, this document really only provides an
overview of the format and is not a formal specification.  It also does not
cover later extensions of the format, such as solid resources.

With regards to the supported compression formats:

- Microsoft has official documentation for XPRESS that is of reasonable quality.
- Microsoft has official documentation for LZX, but in two different documents,
  neither of which is completely applicable to its use in the WIM format, and
  the first of which contains multiple errors.
- There does not seem to be any official documentation for LZMS, so my comments
  and code in `src/lzms_decompress.c` may in fact be the best documentation
  available for this particular compression format.

The algorithms used by WimKit's compression and decompression codecs are
inspired by a variety of sources, including open source projects and computer
science papers.

A limited number of other free programs can handle some parts of the WIM
file format:

- 7-Zip is able to extract and create WIMs (as well as files in many other
  archive formats).  However, WimKit is designed specifically to handle WIM
  files and provides features previously only available in Microsoft's
  implementation, such as the ability to mount WIMs read-write as well as
  read-only, the ability to create compressed WIMs, the correct handling of
  security descriptors and hard links, and support for LZMS compression.

If you are looking for an archive format that provides features similar to WIM
but was designed primarily for UNIX, you may want to consider
[SquashFS](https://docs.kernel.org/filesystems/squashfs.html).  However, you may
find that wimlib works surprisingly well on UNIX.  It will store hard links and
symbolic links, and it supports storing standard UNIX file permissions (owners,
groups, and modes); special files such as device nodes and FIFOs; and extended
attributes.  Actually, I use it to back up my own files on Linux!

# History

WimKit is a fork of wimlib adapted to the Swift Package format in order to be 
used on MacOS Xcode and Swift projects. 

wimlib was originally a project started by Carl Thijssen for use on Linux in the
[Ultimate Deployment Appliance](https://www.ultimatedeployment.org).  Since then
the code has been entirely rewritten and improved (main author: Eric Biggers).
Windows support has been available since version 1.3.0 (March 2013).  A list of
version-to-version changes can be found in the [NEWS file](NEWS.md).

# Notices

WimKit is free software that comes with NO WARRANTY, to the extent permitted by
law.

Bug reports, suggestions, and other contributions are appreciated and should be
posted to [the forums](https://wimlib.net/forums/).

WimKit is independently developed and does not contain any code, data, or files
copyrighted by Microsoft.  It is not known to be affected by any patents.

# Copying

WimKit (the Swift Package fork of the library portion of wimlib) may be 
redistributed and/or modified under the terms of the GNU Lesser General Public 
License; either version 3 of the License, or (at your option) any later version.
A copy of this license can be found in the file LICENSE.

There is NO WARRANTY, to the extent permitted by law.

Originally the libwim library can optionally be linked to the third-party library 
"libntfs-3g", which is licensed under the GPL. Usually the GPL is interpreted in 
a way that means that any binary that uses a GPL library must be licensed under 
the GPL as well, not (for example) the LGPL. Nevertheless, WimKit library use 
exclusively the LGPL option cause does not link at all to the "libntfs-3g" 
library.

