/******************************************************************************
 
 Copyright (c) 2015, Focusrite Audio Engineering Ltd.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of Focusrite Audio Engineering Ltd., nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
    Performance-optimized Launchpad Pro Firmware modification by mat1jaczyyy
 
 ******************************************************************************/

/*   Includes and debugging   */
/*----------------------------*/

#include <string.h> // memcpy
#include "app.h"    // HAL interface

u8 math_pow(u8 x, u8 e) {
	u8 y = 1;
	for (u8 j = 0; j < e; j++) {
		y *= x;
	}
	return y;
}

/* Palette and LED management */
/*----------------------------*/

void clear_led() {
	for (u8 i = 1; i < 99; i++) {
		hal_plot_led(TYPEPAD, i, 0, 0, 0);
	}
	hal_plot_led(TYPESETUP, 0, 0, 0, 0);
}

u8 palette[4][3][128] = {
	{}, // User-defined flash-backed palette
	{ // Novation's default palette
		{0, 7, 31, 63, 63, 63, 21, 6, 63, 63, 21, 9, 63, 63, 21, 6, 33, 20, 7, 4, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 33, 20, 6, 3, 63, 63, 21, 6, 63, 63, 21, 8, 63, 37, 29, 16, 0, 0, 0, 0, 0, 9, 31, 7, 63, 46, 43, 24, 3, 0, 0, 0, 15, 30, 43, 15, 63, 33, 28, 0, 14, 21, 13, 22, 12, 33, 52, 63, 63, 45, 35, 32, 14, 4, 3, 5, 5, 25, 41, 54, 53, 63, 39, 25, 7, 54, 31, 38, 35, 15, 28, 55, 39, 13, 6, 1, 45, 15, 44, 18},
		{0, 7, 31, 63, 18, 0, 0, 0, 46, 20, 7, 6, 63, 63, 21, 6, 63, 63, 21, 10, 63, 63, 21, 6, 63, 63, 21, 6, 63, 63, 21, 7, 63, 63, 21, 6, 48, 41, 16, 3, 33, 21, 7, 1, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 5, 13, 20, 24, 14, 21, 20, 0, 17, 0, 31, 7, 0, 63, 58, 63, 34, 63, 41, 10, 0, 0, 6, 8, 18, 55, 63, 63, 63, 63, 63, 34, 20, 31, 7, 0, 31, 43, 63, 22, 10, 18, 19, 5, 7, 14, 0, 20, 26, 55, 55, 44, 7, 63, 63, 37, 25, 15, 28, 63, 0, 0, 51, 16, 43, 12, 23, 5},
		{0, 7, 31, 63, 18, 0, 0, 0, 26, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 23, 6, 3, 0, 33, 21, 7, 4, 45, 37, 13, 4, 63, 63, 20, 6, 63, 63, 21, 6, 63, 63, 21, 6, 63, 63, 24, 11, 63, 63, 21, 6, 33, 20, 7, 4, 0, 0, 0, 0, 0, 13, 31, 63, 19, 50, 31, 7, 0, 11, 1, 2, 0, 33, 63, 63, 63, 63, 30, 0, 0, 1, 5, 0, 9, 27, 50, 63, 48, 57, 63, 22, 0, 0, 0, 1, 0, 3, 13, 10, 22, 6, 2, 15, 6, 9, 11, 3, 11, 26, 46, 63, 63, 15, 28, 63, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{ // mat1jaczyyy's custom palette
		{0, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 11, 23, 35, 51, 47, 7, 15, 23, 39, 31, 3, 7, 11, 27, 15, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 3, 7, 11, 27, 15, 7, 15, 23, 39, 31, 11, 23, 35, 51, 47, 15, 31, 47, 62, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 9, 18, 27, 36, 45, 54, 63},
		{0, 0, 0, 0, 15, 0, 3, 7, 11, 27, 15, 7, 15, 23, 39, 31, 11, 23, 35, 51, 47, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 62, 63, 11, 23, 35, 51, 47, 7, 15, 23, 39, 31, 3, 7, 11, 27, 15, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 9, 18, 27, 36, 45, 54, 63},
		{0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 3, 7, 11, 27, 15, 7, 15, 23, 39, 31, 11, 23, 35, 51, 47, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 62, 63, 11, 23, 35, 51, 47, 7, 15, 23, 39, 31, 3, 7, 11, 27, 15, 9, 18, 27, 36, 45, 54, 63}
	},
	{ // Launchpad S legacy palette
		{0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	}
};
u8 palette_selected = 0;

void palette_led(u8 p, u8 v) {
	if (p == 99) {
		hal_plot_led(TYPESETUP, 0, palette[palette_selected][0][v], palette[palette_selected][1][v], palette[palette_selected][2][v]);
	} else {
		hal_plot_led(TYPEPAD, p, palette[palette_selected][0][v], palette[palette_selected][1][v], palette[palette_selected][2][v]);
	}
}

void display_u8(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < 8; i++) {
		u8 w = ((v & math_pow(2, i)) >> i);
		hal_plot_led(TYPEPAD, (d == 0)? (8 - i + x * 10) : (i * 10 + x), r * w, g * w, b * w);
	}
}

void display_u6(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < 6; i++) {
		u8 w = ((v & math_pow(2, i)) >> i);
		hal_plot_led(TYPEPAD, (d == 0)? (7 - i + x * 10) : ((i + 2) * 10 + x), r * w, g * w, b * w);
	}
}

u8 vel_sensitive = 0;

/*  Flash storage management  */
/*----------------------------*/

u8 flash[1024] = {}; // [0, 383) = palette[0], {384} = palette_selected, {385} = vel_sensitive
u8 dirty = 0;

void flash_read() {
	hal_read_flash(0, &flash[0], 1024);
	
	memcpy(&palette[0][0][0], &flash[0], 384);
	palette_selected = flash[384];
	vel_sensitive = flash[385];
}

void flash_write() {
	if (dirty) {
		memcpy(&flash[0], &palette[0][0][0], 384);
		flash[384] = palette_selected;
		flash[385] = vel_sensitive;
		
		hal_write_flash(0, &flash[0], 1024);
		
		dirty = 0;
	}
}

/*       Palette editor       */
/*----------------------------*/

u8 editor_selected = 1;

u8 xy_v(u8 xy) {
	return 64 - (xy / 10 * 8) + (xy - 1) % 10 + (editor_selected >> 6) * 64;
}

u8 v_xy(u8 v) {
	return 81 - 10 * ((v % 64) >> 3) + v % 8;
}

void editor_refresh() {
	palette_led(v_xy(editor_selected), editor_selected);
	
	display_u8(editor_selected, 0, 9, 63, 63, 63);
	
	display_u6(palette[0][0][editor_selected], 1, 0, 63, 0, 0);
	
	
	display_u6(palette[0][1][editor_selected], 0, 0, 0, 63, 0);
	
	
	display_u6(palette[0][2][editor_selected], 1, 9, 0, 0, 63);
	
}

void editor_draw() {
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 1; y < 9; y++) {
			u8 xy = x * 10 + y;
			palette_led(xy, xy_v(xy));
		}
	}
	editor_refresh();
}

void editor_select_xy(u8 xy) {
	if (xy != 81 || (editor_selected >> 6) != 0) {
		editor_selected = xy_v(xy);
		editor_refresh();
	}
}

void editor_select_v(u8 v) {
	if (v != 0) {
		editor_selected = v;
		editor_refresh();
	}
}

void editor_select_flip(u8 i) {
	u8 v = editor_selected ^ math_pow(2, i);
	
	if (v != 0) {
		editor_selected = v;
		if (i == 6) {
			editor_draw();
		} else {
			editor_refresh();
		}
	}
}

/*  Modes (Launchpad states)  */
/*----------------------------*/

u8 mode = 0;

void mode_refresh() {
	clear_led();
	
	switch (mode) {
		case 0: // Performance mode
			hal_plot_led(TYPEPAD, 98, 63, 0, 10); // User LED
			break;
		
		case 1: // Setup mode
			hal_plot_led(TYPEPAD, 25, 4, 4, 7); // Select flash palette
			hal_plot_led(TYPEPAD, 26, 7, 7, 7); // Select Novation palette
			hal_plot_led(TYPEPAD, 27, 7, 7, 7); // Select mat1 palette
			hal_plot_led(TYPEPAD, 28, 7, 7, 7); // Select LP S palette
			
			if (palette_selected == 0) {
				hal_plot_led(TYPEPAD, 25, 19, 19, 31); // Draw selected palette
			} else {
				hal_plot_led(TYPEPAD, palette_selected + 25, 31, 31, 31); // Enter palette editor
				hal_plot_led(TYPEPAD, 15, 0, 7, 0); // Apply palette to custom
			}
			
			if (vel_sensitive) {
				hal_plot_led(TYPEPAD, 21, 19, 31, 31); // Velocity sensitivity enabled
			} else {
				hal_plot_led(TYPEPAD, 21, 4, 7, 7); // Velocity sensitivity disabled
			}
			break;
		
		case 2: // Palette editor mode
			editor_selected = 1;
			editor_draw();
			break;
	}
}

void mode_update(u8 x) {
	flash_write();
	
	mode = x;
	mode_refresh();
}

/*     Timer-based events     */
/*----------------------------*/

#define setup_tick 33
u8 setup_elapsed = setup_tick;
u8 setup_mode_led = 0;
u8 setup_editor_button = 0;
u8 setup_rainbow[48][3] = {{63, 0, 0}, {63, 7, 0}, {63, 15, 0}, {63, 23, 0}, {63, 31, 0}, {63, 39, 0}, {63, 47, 0}, {63, 55, 0}, {63, 63, 0}, {55, 63, 0}, {47, 63, 0}, {39, 63, 0}, {31, 63, 0}, {23, 63, 0}, {15, 63, 0}, {7, 63, 0}, {0, 63, 0}, {0, 63, 7}, {0, 63, 15}, {0, 63, 23}, {0, 63, 31}, {0, 63, 39}, {0, 63, 47}, {0, 63, 55}, {0, 63, 63}, {0, 55, 63}, {0, 47, 63}, {0, 39, 63}, {0, 31, 63}, {0, 23, 63}, {0, 15, 63}, {0, 7, 63}, {0, 0, 63}, {7, 0, 63}, {15, 0, 63}, {23, 0, 63}, {31, 0, 63}, {39, 0, 63}, {47, 0, 63}, {55, 0, 63}, {63, 0, 63}, {63, 0, 55}, {63, 0, 47}, {63, 0, 39}, {63, 0, 31}, {63, 0, 23}, {63, 0, 15}, {63, 0, 7}};

void app_timer_event() {
	switch (mode) {
		case 0: // Performance mode
			// Anything timer-based for the Performance mode, likely unnecessary
			break;
			
		case 1: // Setup mode
			if (++setup_elapsed >= setup_tick) {
				hal_plot_led(TYPESETUP, 0, setup_rainbow[setup_mode_led][0], setup_rainbow[setup_mode_led][1], setup_rainbow[setup_mode_led][2]); // Mode LED indicator animation
				setup_mode_led++; setup_mode_led %= 48;
				
				if (palette_selected == 0) {
					hal_plot_led(TYPEPAD, 15, setup_rainbow[setup_editor_button][0], setup_rainbow[setup_editor_button][1], setup_rainbow[setup_editor_button][2]);  // Enter palette editor button animation
					setup_editor_button++; setup_editor_button %= 48;
				}
				
				setup_elapsed = 0;
			}
			break;
			
		case 2: // Palette editor mode
			// Anything timer-based for the Palette editor mode, likely unnecessary
			break;
	}
}

/* Input and output interface */
/*----------------------------*/

u8 xy_dr[128] = {0, 116, 117, 118, 119, 120, 121, 122, 123, 0, 115, 36, 37, 38, 39, 68, 69, 70, 71, 107, 114, 40, 41, 42, 43, 72, 73, 74, 75, 106, 113, 44, 45, 46, 47, 76, 77, 78, 79, 105, 112, 48, 49, 50, 51, 80, 81, 82, 83, 104, 111, 52, 53, 54, 55, 84, 85, 86, 87, 103, 110, 56, 57, 58, 59, 88, 89, 90, 91, 102, 109, 60, 61, 62, 63, 92, 93, 94, 95, 101, 108, 64, 65, 66, 67, 96, 97, 98, 99, 100, 0, 28, 29, 30, 31, 32, 33, 34, 35, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 dr_xy[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28, 35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88, 89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0};

void app_surface_event(u8 t, u8 p, u8 v) {
	u8 av = (v == 0)? 0 : 127;
	v = (vel_sensitive)? v : av;
	
	u8 x = p / 10;
	u8 y = p % 10;
	
	switch (mode) {
		case 0: // Performance mode
			if (p == 0) { // Enter Setup mode
				if (av == 127) mode_update(1);
				
			} else { // Send input to DAW
				hal_send_midi(USBMIDI, 0x90, xy_dr[p], v);
			}
			break;
		
		case 1: // Setup mode
			if (p == 0 && av == 127) { // Enter Performance mode
				mode_update(0);
			
			} else if (25 <= p && p <= 28 && av == 127) { // Palette switch
				palette_selected = p - 25;
				dirty = 1;
				mode_refresh();
			
			} else if (p == 15 && av == 127) {
				if (palette_selected == 0) { // Enter Palette editor mode
					mode_update(2);
				
				} else { // Save current preset as custom palette
					memcpy(&palette[0][0][0], &palette[palette_selected][0][0], 384);
					palette_selected = 0;
					dirty = 1;
					mode_refresh();
				}
			
			} else if (p == 21 && av == 127) { // Toggle velocity sensitivity
				vel_sensitive = (vel_sensitive)? 0 : 1;
				dirty = 1;
				mode_refresh();
			}
			break;
		
		case 2: // Palette editor mode

			if (p == 0 && av == 127) { // Enter Setup mode
				mode_update(1);
			
			} else if (2 <= x && x <= 7 && y == 0 && av == 127) { // Modify red bit
				palette[0][0][editor_selected] ^= math_pow(2, x - 2);
				editor_refresh();
				dirty = 1;
			
			} else if (2 <= p && p <= 7 && av == 127) { // Modify green bit
				palette[0][1][editor_selected] ^= math_pow(2, 7 - p);
				editor_refresh();
				dirty = 1;
			
			} else if (2 <= x && x <= 7 && y == 9 && av == 127) { // Modify blue bit
				palette[0][2][editor_selected] ^= math_pow(2, x - 2);
				editor_refresh();
				dirty = 1;
			
			} else if (92 <= p && p <= 98 && av == 127) { // Modify velocity bit
				editor_select_flip(98 - p);
			
			} else if (p != 1 && p != 8 && p != 10 && p != 19 && p != 80 && p != 89 && p != 91 && av == 127) { // Select velocity
				editor_select_xy(p);
			}
			break;
	}
}

void app_midi_event(u8 port, u8 t, u8 p, u8 v) {
	if (mode == 0 && port == USBMIDI) {
		switch (t) {
			case 0x95: // Note on, Ch. 6
				palette_led(dr_xy[p], v);
				break;
			
			case 0x85: // Note off, Ch. 6
				palette_led(dr_xy[p], 0);
				break;
		}
	}
}

/*      Unused HAL calls      */
/*----------------------------*/

void app_sysex_event(u8 port, u8 * d, u16 l) {}
void app_aftertouch_event(u8 p, u8 v) {}
void app_cable_event(u8 t, u8 v) {}

/*  Initialize the Launchpad  */
/*----------------------------*/

void app_init(const u16 *adc_raw) {
	flash_read();
	
	// TODO: Implement boot animation
	// mode_update(255);
	
	mode_update(0);
}