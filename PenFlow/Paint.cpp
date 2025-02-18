#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/fl_message.H>
#include <FL/fl_draw.H> 
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>


using namespace std;

//Prototype of buttons
void buttonRectangle_callback(Fl_Widget* widget, void *data1); 
void buttonCircle_callback(Fl_Widget* widget, void *data1);   
void buttonLine_callback(Fl_Widget* widget, void *data1);
void colorButtonCallback(Fl_Widget* widget, void* data); 
void label_callback(Fl_Widget* widget, void* data);
void colorfulButtonCallback(Fl_Widget* widget, void*);
void isBoldCallbackIncrease(Fl_Widget* widget, void*); //Increase the bold.
void isBoldCallbackDecrease(Fl_Widget* widget, void*); // Decrease the bold.
void buttonRadiusApply(Fl_Widget* widget, void* data);
void getColors(void* data); // Get the color buttons.
void button_exit(Fl_Widget* widget, void *data1);
void clearCanvas(Fl_Widget* widget, void* data);
void drawLineCallback(Fl_Widget*, void* data);

//You have to click a buton to activate.
bool buttonRectangleActivate = false; 
bool buttonCircleActivate = false;    
bool buttonLineActivate = false;      

Fl_Color currentColor = FL_BLACK; //Current color, you can change the color thanks to the color buttons.

int getRadius = 10; // Default radius.
const double M_PI = 3.141592653589793;

class Pen {
public:
    Fl_Color color;
    bool isBold;

public:
    int bold = 3;
    
    Pen(Fl_Color color = FL_BLACK, bool isBold = false) : color(color), isBold(isBold) {}

    void setColor(Fl_Color newColor) {
        color = newColor;
    }

    bool setBold(bool newBold) {
        isBold = newBold;
        return newBold;
    }

    virtual void drawLine(int x1, int y1, int x2, int y2) {
        fl_color(color);
        fl_line_style(FL_SOLID, bold);
        fl_line(x1, y1, x2, y2);
    }
};


class ColorfulPen : public Pen {
public:

    ColorfulPen(Fl_Color color = FL_RED, bool isBold = false)
        : Pen(color, isBold) {}

    void drawLine(int x1, int y1, int x2, int y2) override {
        // Change the color of each line.
        if (color == FL_BLUE) {
            color = FL_RED;
        } else if (color == FL_RED) {
            color = FL_GREEN;
        }
        else if(color == FL_GREEN){
            color = FL_MAGENTA;
        }else{
            color = FL_BLUE;
        }
        Pen::drawLine(x1, y1, x2, y2);
    }
};

Pen* currentPen = new Pen(FL_BLACK,1); // Default Pen.
class Shape{
    public:
    Pen* pen;

    Shape(Pen* pen) : pen(pen){}
    virtual void draw() const = 0;
    void setPen(Pen* newPen){pen = newPen;}
};
class Rectangle : public Shape{
    public:
    int x1,y1,width,height;
    Rectangle(int x1, int y1,int width, int height, Pen* pen) : Shape(pen),x1(x1),y1(y1),width(width),height(height){}
   void draw() const override {
        int newX = min(x1, x1 + width);
        int newY = min(y1, y1 + height);
        int newWidth = abs(width);
        int newHeight = abs(height);
        
        //Draw the rectangle line by line.
        pen->drawLine(newX, newY, newX + newWidth, newY);
        pen->drawLine(newX, newY + newHeight, newX + newWidth, newY + newHeight);
        pen->drawLine(newX, newY, newX, newY + newHeight);
        pen->drawLine(newX + newWidth, newY, newX + newWidth, newY + newHeight);
    
    }
};
class Circle : public Shape{
    public:
    int x1,y1,radius;
    Circle(int x1, int y1, int radius,Pen* pen) : Shape(pen), x1(x1),y1(y1),radius(radius){}
    void draw() const override {
        const int circleLines = 32; // Shape has created dividing by 32 lines to looks like a circle 
        float angleIncrement = 360.0 / circleLines;

        for (int i = 0; i < circleLines; ++i) {
            // Calculate the two points of the line segment
            float angle1 = angleIncrement * i;
            float angle2 = angleIncrement * (i + 1);

            int xStart = x1 + radius * cos(angle1 * M_PI / 180.0);
            int yStart = y1 + radius * sin(angle1 * M_PI / 180.0);

            int xEnd = x1 + radius * cos(angle2 * M_PI / 180.0);
            int yEnd = y1 + radius * sin(angle2 * M_PI / 180.0);

            pen->drawLine(xStart, yStart, xEnd, yEnd);
        }
    }
};

class Line{
    public:
    Line(int x1, int y1, int x2, int y2, int thickness, Fl_Color color)
    :x1(x1), y1(y1), x2(x2), y2(y2), thickness(thickness), color(color) {}
    int x1, y1, x2, y2;
    int thickness;
    Fl_Color color;

};

// Canvas class for drawing
class Canvas : public Fl_Widget {
public:
    std::vector<Line> lines; // Store all lines
    std::vector<Shape*> shapes; // Store the lines for rectangle and circle.
    int x1, y1, x2, y2;
    int lineThickness;
    bool firstClick = true;

public:
    Canvas(int X, int Y, int W, int H, const char* L = 0)
        : Fl_Widget(X, Y, W, H, L), lineThickness(lineThickness) {} //lineThickness -> başlangıç kalınlığı

        int handle(int event) override {
        int xCoordinate = 0, yCoordinate = 0;
        switch (event) {
            case FL_PUSH:
                xCoordinate = Fl::event_x();
                yCoordinate = Fl::event_y();

                if (firstClick) {
                    x1 = xCoordinate;
                    y1 = yCoordinate;
                    firstClick = false;
                    
                     
                } else {
                    x2 = xCoordinate;
                    y2 = yCoordinate;
                    if(buttonLineActivate){
                        
                        lines.push_back(Line(x1, y1, x2, y2, lineThickness, currentColor));
                    }
                    else if(buttonRectangleActivate){
                       
                         shapes.push_back(new Rectangle(min(x1,x2),min(y1,y2),abs(x2-x1),abs(y2-y1), currentPen));
                    }
                    else if(buttonCircleActivate){
                       
                         shapes.push_back(new Circle(x1, y1, getRadius, currentPen));
                    }
                            
                    
                    
                    firstClick = true;
                    redraw();
                }
                return 1;

            default:
                return Fl_Widget::handle(event);
        }
    }

    void draw() override {
        // Clear the canvas with a white background
        fl_color(FL_WHITE);
        fl_rectf(x(), y(), w(), h());

        // Draw all the stored lines
        for (const auto& line : lines) {
            currentPen->drawLine(line.x1,line.y1,line.x2,line.y2);
        }
         for (const auto& shape : shapes) {
            shape->draw();
          }

    }

    void clear_lines() {
        lines.clear();
        redraw();
    }
    void clear_shapes(){
        shapes.clear();
        redraw();
    }
};

int main() {
    // Create buttons for shapes
    Fl_Window* window = new Fl_Window(800, 600, "Shape Drawer");

    Fl_Button* buttonLine = new Fl_Button(250, 40, 300, 120, "Draw Line");
    Fl_Button* buttonRectangle = new Fl_Button(250, 230, 300, 120, "Draw Rectangle");
    Fl_Button* buttonCircle = new Fl_Button(250, 420, 300, 120, "Draw Circle");

    Fl_Button* isBold = new Fl_Button(650, 300, 80, 30, "Bold Pen");
    isBold->callback(isBoldCallbackIncrease, currentPen);

    Fl_Button* isBoldDec = new Fl_Button(650,250,80,30,"Slim Pen");
    isBoldDec->callback(isBoldCallbackDecrease,currentPen);


    buttonRectangle->callback(buttonRectangle_callback, window);
    buttonCircle->callback(buttonCircle_callback, window);
    buttonLine->callback(buttonLine_callback, window);

    getColors(window);

    window->end();
    window->show();

    return Fl::run();
}

void buttonLine_callback(Fl_Widget* widget, void* data) {
    Fl_Window* window1 = (Fl_Window*)data;
    buttonLineActivate = true;
    buttonRectangleActivate = false;
    buttonCircleActivate = false;
    

    // New window for drawing line
    Fl_Window* window = new Fl_Window(800, 600, "Persistent Line Drawer");

    Fl_Box* lblLine = new Fl_Box(285, 5, 200, 50, "DRAW LINE");
    lblLine->labelsize(20);

    Canvas* canvas = new Canvas(50, 50, 650, 500); // Canvas position (x, y, width, height)


    Fl_Button* clearButton = new Fl_Button(710, 270, 80, 30, "Clear");
    clearButton->callback(clearCanvas, canvas);

    Fl_Button* buttonExit = new Fl_Button(710, 330, 80, 30, "Exit");
    buttonExit->callback(button_exit, window);

    window->end();
    window->show();
}

void buttonRectangle_callback(Fl_Widget* widget, void* data) {
    buttonLineActivate = false;
    buttonRectangleActivate = true;
    buttonCircleActivate = false;
    Fl_Window* window1 = (Fl_Window*)data;

    // New window for drawing rectangle
    Fl_Window* window = new Fl_Window(800, 600, "Persistent Rectangle Drawer");

    Fl_Box* lblRectangle = new Fl_Box(290, 5, 200, 50, "RECTANGLE");
    lblRectangle->labelsize(20);


    Canvas* canvas = new Canvas(50, 50, 650, 500); // Canvas position (x, y, width, height)

    Fl_Button* clearButton = new Fl_Button(710, 240, 80, 30, "Clear");
    clearButton->callback(clearCanvas, canvas);

    Fl_Button* buttonExit = new Fl_Button(710, 290, 80, 30, "Exit");
    buttonExit->callback(button_exit, window);

    window->end();
    window->show();
}

void buttonCircle_callback(Fl_Widget* widget, void* data) {
    buttonLineActivate = false;
    buttonRectangleActivate = false;
    buttonCircleActivate = true;
    Fl_Window* window1 = (Fl_Window*)data; 

    // New window for drawing circle
    Fl_Window* window = new Fl_Window(800, 600, "Persistent Circle Drawer");

    Fl_Box* lblCircle = new Fl_Box(295, 5, 200, 50, "CIRCLE");
    lblCircle->labelsize(20);

    Canvas* canvas = new Canvas(50, 50, 650, 500); // Canvas position (x, y, width, height)

    //Label unit.What is the unit of each line?
    Fl_Box* lblUnit = new Fl_Box(720,80,60,30,"Radius");
    lblUnit->labelsize(15);


    //Get the unit from user.
    Fl_Input *inputCenter = new Fl_Input(710, 120, 80, 30); // value() çağrısı burada doğru           // Hata burada düzeltildi
    inputCenter->align(FL_ALIGN_RIGHT);
    
    Fl_Button* btnApply = new Fl_Button(720,160,60,30,"Apply");
    btnApply->labelsize(15);
    btnApply->callback(buttonRadiusApply,inputCenter);


    Fl_Button* clearButton = new Fl_Button(710, 330, 80, 30, "Clear");
    clearButton->callback(clearCanvas, canvas);

    Fl_Button* buttonExit = new Fl_Button(710, 390, 80, 30, "Exit");
    buttonExit->callback(button_exit, window);

    window->end();
    window->show();
}

void clearCanvas(Fl_Widget* widget, void* data) {
    Canvas* canvas = (Canvas*)data;
    canvas->clear_lines();
    canvas->clear_shapes();
}

void button_exit(Fl_Widget* widget, void* data) {
    Fl_Window* window1 = (Fl_Window*)data;
    window1->hide(); // Close the current window
}

void getColors(void* data) {
    Fl_Window* window = (Fl_Window*)data;

    Fl_Button* buttonRed = new Fl_Button(100, 560, 120, 30, "RED PEN");
    buttonRed->color(FL_RED);
    buttonRed->callback(colorButtonCallback);

    Fl_Button* buttonGreen = new Fl_Button(250, 560, 120, 30, "GREEN PEN");
    buttonGreen->color(FL_GREEN);
    buttonGreen->callback(colorButtonCallback);


    Fl_Button* buttonBlue = new Fl_Button(400, 560, 120, 30, "BLUE PEN");
    buttonBlue->color(FL_BLUE);
    buttonBlue->callback(colorButtonCallback);

    Fl_Button* buttonColorful = new Fl_Button(550, 560, 120, 30, "COLORFUL PEN");
    buttonColorful->color(FL_MAGENTA);
    buttonColorful->callback(colorfulButtonCallback);
    
    window->redraw();  // Update the window
}

void colorButtonCallback(Fl_Widget* widget, void*) {
    Fl_Button* btnColor = (Fl_Button*)widget;


    currentPen->setColor(btnColor->color());
}
 
void colorfulButtonCallback(Fl_Widget* widget, void*){
    currentPen = new ColorfulPen(FL_RED,1);
}

void isBoldCallbackIncrease(Fl_Widget* widget, void* data){
    Fl_Button* btnBold = (Fl_Button*)widget;
     Pen* cPen = (Pen*)data;
     currentPen->bold = 7; // Increase the bold of all pens.
     cPen->bold = 7;      
}
void isBoldCallbackDecrease(Fl_Widget* widget, void* data){
    Fl_Button* btnBold = (Fl_Button*)widget;
    Pen* cPen = (Pen*)data;
    cPen->bold = 3;  // Decrease the bold of all pens.
    currentPen->bold = 3;
}
void buttonRadiusApply(Fl_Widget* widget, void* data){
    Fl_Button* btnBold = (Fl_Button*)widget;
    Fl_Input* rds = (Fl_Input*)data;
    const char* userInput = rds->value();
    getRadius = atoi(userInput); 
}
// Button callback to add and draw a line on the canvas
void drawLineCallback(Fl_Widget*, void* data) {
    Canvas* canvas = static_cast<Canvas*>(data);
    canvas->clear_lines();
    canvas->redraw(); // Redraw the canvas to reflect the new line
}