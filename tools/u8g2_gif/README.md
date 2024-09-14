U8G2 Gif Tool
=============

This is a helper to create a gif from a U8G2 app.

Instructions
------------

1. Symlink the u8g2 app source .c files into the `app` folder.
2. Run `make`.
3. Run the resulting `./u8g2app` executable. This needs to run for as long as
   you want the .gif to be.
4. Run `make gif RATE=10` where rate is the framerate of the .gif.
5. Copy `app.gif` where you want.

