Title: Overview

# What is Lasem ?

Lasem is a C/Gobject based SVG/Mathml renderer. It uses cairo and pango as it's
rendering abstraction layer, and then support numerous output formats: xlib,
PNG, SVG, PDF, PS, EPS...

The API tries to follow the SVG and MathML DOM.

It currently includes two simple applications:

**lasem-render**, which takes a filename as argument and generates either a SVG, a
PDF or a PS file, depending on the output file extension. There's a debug mode
option (-d) that displays the bounding boxes of elements as blue rectangles, the
logical extents of text as red boxes and the ink extents of text as green boxes.
It accepts either MathML, Latex maths and SVG input. For the latex input,
lasemrender uses the embedded itex2mml library for the conversion to MathML.

**lsm-test**, which, if launched without argument, renders recursively all svg and
mml files in a directory, and generate an html file which displays the browser
rendering, lasem rendering, and if present the reference output. The html
filename is `lasemtest.xml`. It's main use is the rendering of the sample files
in the samples directory, by invoking `cd test && ./lsm-test data`.
