Drawing Application (FLTK) - README
(My first project with FLTK)

Project Description
This project is a simple drawing application built using the FLTK library and developed in VSCode. The application allows users to draw lines, rectangles, and circles with customizable pen settings.


Requirements
-->FLTK library (version 1.4 recommended)
-->MinGW (for Windows users)
-->VSCode (optional, but recommended)


How to Run the Program
   1-Open the terminal and navigate to the project directory.
   2-Compile the code using:
	mingw32-make
   3-Run the application with:
	.\Paint

There is an example png about the Project.


Features
1. Line Tool
Click two points with the mouse to draw a straight line between them.
2. Rectangle Tool
Select two points to define the diagonal of the rectangle.
3. Circle Tool
A default radius value is set.
Users can input a custom radius and click the Apply button.
Then, clicking twice on the canvas will set the first click as the center and draw a circle with the chosen radius.
4. Clear Button
Clears the drawing area.
5. Exit Button
Closes the current drawing window.
