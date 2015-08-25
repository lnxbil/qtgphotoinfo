You need the following packages (most recent versions):
  * libtool (using LGPL licensed libdl)
  * pkg-config (GPL, but not using to link against)
  * libusb (LGPL)
  * libusb-compat (LGPL)
  * jpeg-6b (some kind of royality and fee-free license)
  * libgphoto2 (LGPL)
  * QT 4.6 (LGPL Version available)

So using libgphoto2 is completly linked against LGPL libraries.

# Further Information / Other How-To's #

  * http://blog.dcclark.net/2009/05/how-to-gphoto-primer.html

# Example Output (Version 0.1) #

Compiling is very easy, if libgphoto2 is already installed:
> qmake -spec macx-g++ -config debug QTGPhotoInfo.pro
to build the Makefile and after his compile the program
> make
and to start the program
> open QTGPhotoInfo.app

![http://qtgphotoinfo.googlecode.com/svn/trunk/images/screenshots/mac.png](http://qtgphotoinfo.googlecode.com/svn/trunk/images/screenshots/mac.png)