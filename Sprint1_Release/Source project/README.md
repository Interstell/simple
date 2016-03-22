# How to compile Smart Candle v1.0

_**BEFORE YOU START**_: path to compiled program must **NOT** contain spaces(!) or cyrillic symbols. We'll try to repait it :).

**1a.** If you have MSVC 2013 x86: [download](http://rghost.ru/8jSLg7tRJ) OpenCV pre-compiled library

**1b.** If not, [download](http://opencv.org/) OpenCV 3.1 from official web-site and follow **[THIS](http://dogfeatherdesign.com/opencv-3-0-microsoft-visual-studio-2015-cmake-and-c/)** instruction (it includes compiling lib with _CMake_)

**2.** Now you have to tell the C/C++ compiler where the header files are, and you have to tell the Linker system where the compiled library files are located.

**For MSVC:**
* In Solution Explorer, right click on the project title, open up Properties.
* On the Configuration Properties, C/C++, General settings for Additional Include Directories, enter path to ..\OpenCV\include
* On the Configuration Properties, Linker, General settings for Additional Library Directories, enter ..\OpenCV\lib
* On the Configuration Properties, Linker, Input, Additional Dependencies, add the following libraries:

**2.1.** FOR **DEBUG** MODE:
  * opencv_calib3d310d.lib
  * opencv_core310d.lib
  * opencv_features2d310d.lib
  * opencv_flann310d.lib
  * opencv_highgui310d.lib
  * opencv_imgcodecs310d.lib
  * opencv_imgproc310d.lib
  * opencv_ml310d.lib
  * opencv_objdetect310d.lib
  * opencv_photo310d.lib
  * opencv_shape310d.lib
  * opencv_stitching310d.lib
  * opencv_superres310d.lib
  * opencv_ts310d.lib
  * opencv_video310d.lib
  * opencv_videoio310d.lib
  * opencv_videostab310d.lib

**NOTE**: There are SEPARATE Property entries, depending on if you are in Debug or Release mode. For **Release** config you should add **SAME** libs, but **WITHOUT** "d" in the end, for example _opencv_core310.lib_

**2.2.** Next, you have to add the .dll library locations to the Environment Variables / System Variables PATH. In this case I've added both _..\OpenCV\x86\vc12\bin_ and _..\OpenCV\x86\vc12\lib_ to my Path entry.
Reboot the Operating System to engage the new Path.

**3.** Jansson library (JSON parsing): on the Configuration Properties, Linker, Input, Additional Dependencies, add the path to **jansson_d.lib** (in folder with main.cpp)

**NOTE:** You can do the same operations with your favorite IDE (e.g. Codeblocks), but We strongly recommend using MSVC
