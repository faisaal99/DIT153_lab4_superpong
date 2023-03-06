#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__


void graphic_initialize   (void);
void graphic_clear_screen (void);
void graphic_pixel_set    (int, int);
void graphic_pixel_clear  (int, int);


#endif // __DISPLAY_DRIVER_H__