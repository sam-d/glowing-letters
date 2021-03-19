# Glowing letters, controlled by an Arduino and activated with a PIR motion sensor

This project shows how to combine 3D printing and electronics to create
illuminated letters that fade through a 'sunrise' color palette. If you happen
to have the same initials, feel free to print the STL files from the 'models'
directory. I used 58 LEDS from a 30 LEDS per meter strip.

If you have different initials, the OpenSCAD source in folder 'openscad' should
provide inspiration and should be easy to adapt.

# BOM

* WS811 LED strips, 30 LEDS per meter, 2 meters
* PIR motion sensor (I used a module of type HC-SR501
* Arduino nano 
* cables (I used 0,16 mm2 in 3 colors black, red, white)
* 5V power adapter with wall mart. It should be rated above 3 ampere. I used
  5V/3A, and the LEDs are never white and not run at full brightness. Beware to
calculate power draw yourself when adapting this project
* barrel jack to fit the 5V power adapter
* soldering iron and solder

# Build Log

Build log can be found on [my blog](http://sam-d.com/blog/...)
