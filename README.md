VM167
=====

libusb based user-space driver for the Velleman VM167 USB card

Based on the source of the Windows DLL written in Delphi

Build on raspbian
-----------------

Dependencies:

libusb-1.0-0-dev cmake git

Building:

git clone https://github.com/rahlskog/VM167.git
cd VM167

edit CMakeLists.txt >> comment out subdirectory TestApp (QT dependency)

mkdir build
cd build
cmake ..
make

Testing that it works

(in build subdir)
cd test
sudo ./vm167test

Unless you have a testing board like the one I designed you will get failures. The important part is that device is opened and card FW is read.
