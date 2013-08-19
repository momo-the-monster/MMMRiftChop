MMMRiftChop
===========

CPlusPlus CHOP to read data from Oculus Rift in TouchDesigner

Outputs:

+ Oculus Display Found
+ Oculus Sensor Found
+ Sensor yaw / pitch / roll values
+ Software Version (currently broken? outputs 0)
+ Horizontal Resolution
+ Vertical Resolution
+ Horizontal Screen Size
+ Vertical Screen Size
+ Vertical Screen Center
+ Eye to Screen Distance
+ Lens Separation Distance
+ Interpupillary Distance
+ Distortion Coefficients (x3)
+ Render Size
+ Shader Scaling Uniform
+ Left Eye Proejection Matrix

Note that many of these attributes may be unnecessary. Once I have a fully-working version, I can probably use most of the measurements, and just output a projection matrix.

Note that I only output the Left Eye Projection Matrix as the right is the same, just offset with a negative parameter for matrix.M[2][0].
