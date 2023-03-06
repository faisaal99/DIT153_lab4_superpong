#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "typedef.h"


/// <summary>
/// A struct to model a single point in two-dimensional space.
/// </summary>
typedef struct
{
    i8 x;
    i8 y;
} Point, *P_Point;


/// <summary>
/// A basic line, with a starting point and an end point.
/// </summary>
typedef struct
{
    Point p0;      // Point to draw from.
    Point p1;      // Point to draw to.
} Line, *P_Line;


/// <summary>
/// A rect, with an origin-point (top-left), and width + height.
/// </summary>
typedef struct
{
    Point origin;   // The top-left corner of the rect.
    Point dimen;    // The width and height of the rect.
} Rect, *P_Rect;


/// <summary>
/// A polypoint is a polygon-shape with linked polygons.
/// </summary>
typedef struct poly_t
{
    i8 x, y;
    struct poly_t *next;
} PolyPoint, *P_PolyPoint;


// The maximum number of pixels a Geometry can hold.
#define MAX_SIZE 32

/// <summary>
/// Models a geometric shape.
/// </summary>
typedef struct
{
    int   num_points;
    int   size_x;
    int   size_y;
    Point px[MAX_SIZE];
} Geometry, *P_Geometry;


/// <summary>
/// Models a movable object on the screen.
/// </summary>
typedef struct Obj_t
{
    P_Geometry geo;    // The pixel-data of this object.
    int        dir_x;  // The horizontal direction of this object.
    int        dir_y;  // The vertical direction of this object.
    int        pos_x;  // The x-position of this object.
    int        pos_y;  // The y-position of this object.

    // Render this object.
    void (*draw)      (struct Obj_t*);
    // Clear all pixels of this object.
    void (*clear)     (struct Obj_t*);
    // Move this object.
    void (*move)      (struct Obj_t*);
    // Set the delta vector of this object.
    void (*set_speed) (struct Obj_t*, int, int);
} Object, *P_Object;


/// <summary>
/// Draw an object to the screen.
/// </summary>
/// <param name="obj">
/// The object whose pixels will be drawn to the screen.
/// </param>
void draw_object(P_Object obj);


/// <summary>
/// Clear an object's off the screen.
/// </summary>
/// <param name="obj">The object whose pixels to delete.</param>
void clear_object(P_Object obj);


/// <summary>
/// Get the absolute value of a char.
/// </summary>
/// <param name="nr"></param>
/// <returns></returns>
i8 abs(i8 nr);


/// <summary>
/// Swap the values of two chars.
/// </summary>
void swap(i8 *a, i8 *b);


// Functions for drawing graphics.

int draw_line(P_Line      line);
int draw_rect(P_Rect      rect);
int draw_poly(P_PolyPoint poly);


#endif // __GRAPHICS_H__