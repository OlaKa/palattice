# palattice

**a C++ particle accelerator lattice library**
	 
*Copyright (C) 2016 Jan Felix Schmidt <janschmidt@mailbox.org>, GNU General Public License*
********************************************************************

libpalattice is a C++ library for particle accelerator lattices. It includes import from
and export to Elegant and Mad-X. Additionally, accelerator data like Twiss parameters or
tracking results can be imported.
Lattices can also be exported to LaTeX to draw a map using tikz-palattice package.

Besides the library, some example programs are included in the programs/ subdirectory:

- "lattice" demonstrates the lattice data structures
- "interptwiss" & "fields" show import and usage of "accelerator data"

Additionally, "convertlattice" is included: a simple tool
for automatic lattice format conversion between Elegant and Mad-X.

Please report bugs and do not hesitate to contact me if you have any questions.


## Installation TL;DR

#### Dependencies:

- CMake
- Gnu Scientific Library (GSL)
- SDDSToolKit-devel (optional, see "Requirements: libSDDS1" below)
- zlib Compression Library (needed by SDDSToolKit-devel)

#### Installation:

```
 cd palattice/build
 cmake ..
 make
 sudo make install
```

#### Compile example programs (API explanations in source files):

```
 cd palattice/programs/build
 cmake ..
 make
```

#### Link your code to libpalattice:

```
 -lpalattice -lz -lgsl -lgslcblas
 (lz is needed only if libSDDS1 is available)
```



## Detailed Installation Guide

1. CMake is used to configure the project for your system and generate the build files.
   Go to subdirectory build/ and run CMake by the command:
   `cmake ..`
   - requires CMake, in Ubuntu it can be installed via `sudo apt-get install cmake`
   - palattice requires the Gnu Scientific Library GSL,
     in Ubuntu it can be installed via `sudo apt-get install libgsl-dev`
   - the SDDS library libSDDS1 is recommended, see section "Requirements: libSDDS" below.
2. compile the library and the convertlattice program:
   in palattice/build directory run
   `make`
3. install the library & convertlattice for all users of your system:
   in build/ directory type:
   `sudo make install`
   - the default install path is `/usr/local` and can be changed in
     `palattice/CMakeLists.txt` (look for `CMAKE_INSTALL_PREFIX`)
4. compile example programs:
   in palattice/programs/build directory type:
   `cmake ..`
   `make`
   Now the binaries can be found in `palattice/programs/build`.

4. Alternative:
   Alternatively, the example programs can be compiled directly from `palattice/build`:
   in `palattice/build` run
   `make programs`
   The binaries are now located in `palattice/build/programs/src/programs-build/`.


#### Remark: Operating Systems

So far, palattice has been used under Linux only (mainly Ubuntu & Debian).
Please tell me about tests with other platforms.
One issue will be configuring Mad-X/Elegant from C++, which currently
uses the program `sed` (see "automatic execution of Mad-X/Elegant"
and "TODOs" below).

#### If you used older versions of libpalattice (< 3.2.3) before:

It is recommended to delete the existing `libpal.ele`/`libpal.madx`
files in your projects to avoid confusion, because the library ignores the
old files and creates `libpalattice.ele`/`libpalattice.madx` instead
during first Mad-X/Elegant execution in this directory.
If you made any custom changes you should copy them to the new files.

If you used the unpublished statically linked convertlattice program before,
please first uninstall this by execution of the `uninstall.sh` script from
your convertlattice directory.


#### Uninstall

To remove all library files from the installation path (default /usr/local)
go to palattice/build directory and type
`sudo make uninstall`.
To remove all compiled files from the build directory type
`make clean`.
To delete all source files just delete the palattice directory.





## Recommended: libSDDS1

From version 3.7.2, libpalattice can read SDDS binary files directly, which
are the default output of Elegant. This requires libSDDS1 by the
SDDS/Elegant developers from Argonne National Lab.
A package called `SDDSToolKit-devel` can be [downloaded](http://www.aps.anl.gov/Accelerator_Systems_Division/Accelerator_Operations_Physics/software.shtml).
If you have any questions about it, I recommend asking in the [Elegant forum](http://www.aps.anl.gov/Accelerator_Systems_Division/Accelerator_Operations_Physics/phpBB3/)

cmake automatically detects, if libSDDS1 is installed on your system and deactivates
the SDDS support otherwise, so that you can still use libpalattice.
But I really recommend using SDDS :-)

Since version SDDSToolKit-devel-3.3.1-2 "Most of the important functions should
now be thread safe" (Elegant forum, "libSDDS and threads"). Using this, libpalattice
was successfully tested with a parallelized application.

libSDDS1 requires zlib, (in Ubuntu it is in the package `zlib1g`)
and liblzma (in Ubuntu it is in the package `liblzma-dev`).

If you installed or uninstalled `SDDSToolKit-devel` and cmake does not recognize,
try deleting `libpalattice/build/CMakeCache.txt` and then run `cmake ..` again.



## Requirements for automatic execution of Mad-X/Elegant by palattice

The commands to run Mad-X and Elegant can be set in `palattice/config.hpp`.
Default values are:
- command to run Mad-X: `madx`
- command to run Elegant: `elegant`





## Comment on automatic execution of Mad-X/Elegant by palattice

To allow for automatic execution of Mad-X/Elegant, the library:

1. copies file `libpalattice.madx`/`libpalattice.ele`
   from `/usr/local/lib/libpalattice_simTools/`
   to the path of your lattice file if it does not exist there.
2. writes the name of your lattice file (and a few other settings)
   to `libpalattice.madx`/`libpalattice.ele`.
3. executes `madx libpalattice.madx` or `elegant libpalattice.ele`.
4. if Elegant is used without libSDDS1: executes
   `/usr/local/bin/elegant2libpalattice` to convert SDDS files to ascii.

If execution fails for your lattice, you can edit `libpalattice.madx`/`libpalattice.ele`
in your lattice folder or edit the original template in
`/usr/local/lib/libpalattice_simTools/`.
E.g. for a beamline (no ring) MadX twiss module fails, because of missing
start values (`betx`, `bety`). Add them to the twiss command in libpalattice.madx
to run MadX successfully.

If you want to run Mad-X/Elegant with your own "run-File", use
`SimToolInstance::setRunFile(string file)`.

The manipulation of `libpalattice.madx`/`libpalattice.ele` is done really
quick-and-dirty so far (using `sed`...) and works only if `sed` is available!




## Documentation of palattice API

Unfortunately there is no dedicated documentation yet.
If you have some time left over please send it to me...

A good start are the example programs in `palattice/programs`:
- `lattice.cpp` for `pal::AccLattice` class
- `interptwiss.cpp` & `fields.cpp` for `pal::FunctionOfPos<T>` class
The source files have many comments and show the basic usage of the classes.

For the full API I can only refer to the header files (`.hpp`) in `palattice/`,
where nearly all classes and methods are described by short comments.
Start with `libpalattice.hpp`, which is a kind of table of contents.

Please do not hesitate to contact me if you have any further questions.




## TODOs

- replace quick-and-dirty implementation of configuring Mad-X/Elegant
  (replace system calls to `sed` by writing config files from c++)
- implement more element types & element parameters
- new implementation of `element_type` to improve looping types &
  type name string output
- ...
