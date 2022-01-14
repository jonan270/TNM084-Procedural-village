# TNM084-Procedural-village
This is a project by Jonathan Andersson in the course TNM084 - Procedural Methods for Images.
The project generates a 3d scene of a village in an environment containing trees and lakes.
The ground work is based on a lab in the course.

Seed values are taken from the current runtime with an added unseeded rand function call, 
but it can be changed to use predefined seedvalue by providing an integer value when constructing
the biomeMap member variable inside TerrainGrid.cpp.

# Controls
WASD EQ XC : Move the camera

\+ \- : Tilt camera

z : Regenerate procedural village
