/*
    This file is part of the mydraw.

    mydraw is a simple, interactive drawing program written using OpenGL. 
    
    This code base is meant to serve as a starter code for an assignment in a 
    Computer Graphics course.

    Copyright (c) 2018 by Parag Chaudhuri

    mydraw is free software; you can redistribute it and/or modify it under the 
    terms of the MIT License.

    Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <bits/stdc++.h>
#include "primitive.hpp"
#include "canvas.hpp"

namespace mydraw
{
	//! Swap Function 
	void swap (double &x, double &y)
	{
		double temp = x;
		x = y; y = temp;
	}

	//! Bresenham's Line Drawing Algorithm
	void draw_line(canvas_t *mycanvas,double x0, double y0, double x1, double y1){
		bool steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep)
		{
			swap(x0, y0);
			swap(x1, y1);
		}
		if (x0 > x1)
		{
			swap(x0, x1);
			swap(y0, y1);
		}
		int deltax = x1 - x0;
		int deltay = abs(y1 - y0);

		float error = 0.0;
		float deltaerr = (float)deltay / (float)deltax;

		int ystep;
		int y = y0;

		if (y0 < y1) ystep = 1; 
		else ystep = -1;
		for (int x=x0; x < x1; x++)
		{
			if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Brush){
				if (steep)
				mycanvas->get_context()->current_brush->stroke((unsigned int)y, (unsigned int)x, mycanvas); 
				else 
				mycanvas->get_context()->current_brush->stroke((unsigned int)x, (unsigned int) y, mycanvas);
			}
			else
			{
				if (steep)
				mycanvas->get_context()->current_eraser->stroke((unsigned int)y, (unsigned int)x, mycanvas); 
				else 
				mycanvas->get_context()->current_eraser->stroke((unsigned int)x, (unsigned int) y, mycanvas);
			}
			error = error + deltaerr;
			if (error >= 0.5) 
			{
				y = y + ystep;
				error = error - 1.0;
			}
		}
	}

	//! Mode Changing Function
	void mode_changes(canvas_t *mycanvas, double xpos, double ypos){
		if(!(mycanvas->x2 == -1 || mycanvas->y2 == -1) && mycanvas->get_current_pmode() == mydraw::primitive_mode_t::line)
            {
              draw_line(mycanvas,mycanvas->x2, mycanvas->y2,xpos,ypos);
            }
		else if(!(mycanvas->x2 == -1 || mycanvas->y2 == -1)&& mycanvas->x1 == -1 && mycanvas->y1 == -1
			&& mycanvas->get_current_pmode() == mydraw::primitive_mode_t::triangle)
		{
			draw_line(mycanvas,mycanvas->x2, mycanvas->y2,xpos,ypos);
		}
		else if(!(mycanvas->x2 == -1 || mycanvas->y2 == -1 || mycanvas->x1 == -1 || mycanvas->y1 == -1) 
			&& mycanvas->get_current_pmode() == mydraw::primitive_mode_t::triangle)
		{
			draw_line(mycanvas,mycanvas->x2, mycanvas->y2,xpos,ypos);
			draw_line(mycanvas,mycanvas->x1, mycanvas->y1,xpos,ypos);
		}
		else {}
		mycanvas->x1 = mycanvas->x2;
		mycanvas->y1 = mycanvas->y2;
		mycanvas->x2 = xpos;
		mycanvas->y2 = ypos;
	}

	//! Function to draw Smooth Curves
	void smooth_curve(canvas_t *mycanvas, double xpos, double ypos){
		if(!(mycanvas->x2 == -1 || mycanvas->y2 == -1))
        {
            draw_line(mycanvas,mycanvas->x2, mycanvas->y2,xpos,ypos);
        }
		mycanvas->x1 = mycanvas->x2;
		mycanvas->y1 = mycanvas->y2;
		mycanvas->x2 = xpos;
		mycanvas->y2 = ypos;
	}
}
