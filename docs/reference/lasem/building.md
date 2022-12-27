Title: Installation

# Installing Lasem

Lasem uses the meson build system ( http://mesonbuild.com/ ). After you have
downloaded the latest release from
[https://github.com/LasemProject/lasem/releases](https://github.com/LasemProject/lasem/releases),
you can build and install Lasem like [any other meson
project](http://mesonbuild.com/Quick-guide.html#compiling-a-meson-project):

```
meson setup build
cd build
ninja
ninja install
```

The build can be configured at any time using `meson configure` in the build
directory. `meson configure` invoked without any other argument will show the
configuration options.

On some platforms (like Ubuntu), you may have to configure the dynamic linker
(ld) to let it know where the aravis libraries are installed, and run ldconfig
as root in order to update ld cache.

## Requirements

gobject, glib, gio, gdk-pixbuf, gdk, cairo, pangocairo, libxml, bison, flex

For the runtime, the following ttf fonts should be installed: cmr10, cmmi10,
cmex10 and cmsy10.  They are provided by the lyx-fonts package in fedora, and
the fonts-lyx package in debian/ubuntu.
