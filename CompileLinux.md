This HowTo has been tested on Ubuntu 10.04 and 10.10 and should work on all Debian-based systems.

  * First, install all necessary packages for building from SVN
```
sudo apt-get install libqt4-dev qt4-qmake libgphoto2-dev subversion
```
  * Afterwards, checkout the SVN-Repository
```
svn checkout http://qtgphotoinfo.googlecode.com/svn/trunk/ qtgphotoinfo
cd qtgphotoinfo
```
  * Since this is a default gphoto2 installation (from packages), we can delete the local configuration file
```
rm config/local.pro && touch config/local.pro
```
  * Now, we have to build the Makefile and build the whole project
```
qmake-qt4 QTGPhotoInfo.pro
make -s
```
  * After the compilation, you can start the program by typing
```
./QTGPhotoInfo
```