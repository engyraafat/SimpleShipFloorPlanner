# SimpleShipFloorPlanner

This Program implements a Chip Floor Planner that works with the following types of imputs:
I/O Pads ( Type 1 Rectangles)
Hard Modules ( Type 2 Rectangles)
Soft Modules ( Type 3 Rectangles)

It uses the Analytical Approach in order to Minimize the Area of the Chip. 
For the hard modules, it finds its location and whether it should be rotated. 
For Soft modules, it finds their heights, widths and locations
For I/O pads, it finds their locations

Input Format:
The input files follow the following format:
For Hard Modules:
.hard_module name width height 
For Soft Modules:
.soft_module  name  area
For I/O pads:
.pin  number_of_pins  width  height 

Output Format:
The output files follow the following format:
They report the total width, total height, total area, utilization , I/o Pad Locations.

Hard Modules are reported in the following format:
Name: x_position , y_position , rotated_or_not

Soft Modules are reported in the following format:
Name: x_position , y_position , width , height
