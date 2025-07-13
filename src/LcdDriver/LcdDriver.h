#ifndef GRAPHICS_DISPLAY_H_
#define GRAPHICS_DISPLAY_H_

#include <ti/grlib/grlib.h>

/* Global graphics context */
extern Graphics_Context g_sContext;

/* Initializes the Crystalfontz display and the graphics context */
void graphics_init(void);

void clear_lcd(Graphics_Context context);

#endif /* GRAPHICS_DISPLAY_H_ */
