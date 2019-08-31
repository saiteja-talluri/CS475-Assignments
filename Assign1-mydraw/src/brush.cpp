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

#include "brush.hpp"
#include "canvas.hpp"

namespace mydraw 
{

/**
 * \brief Point brush - Extended the stroke function to draw all pixels within a circle of radius equal to brush size. 
 * 
 */
 void point_brush_t::stroke (const point_t &pt, canvas_t &canvas)
		{	
			int radius = get_size();
			for(int i = -1*radius; i <= radius; i++)
			{
				for(int j = -1*radius; j <= radius; j++)
				{
					if(i*i + j*j <= radius*radius)
						canvas.set_pixel(pt.x + i,pt.y + j);
				}
			}
		}
 void point_brush_t::stroke (unsigned int xpos, unsigned int ypos, canvas_t *canvas)
 {
    int radius = get_size();
	for(int i = -1*radius; i <= radius; i++)
	{
		for(int j = -1*radius; j <= radius; j++)
		{
			if(i*i + j*j <= radius*radius)
				canvas->set_pixel(xpos + i,ypos + j);
		}
	}
 }
  
/**
 * \brief Eraser brush - Extended the stroke function to erase all pixels within a circle of radius equal to brush size. 
 * 
 */
 void eraser_point_brush_t::stroke (const point_t &pt, canvas_t &canvas)
		{
			int radius = get_size();
			for(int i = -1*radius; i <= radius; i++)
			{
				for(int j = -1*radius; j <= radius; j++)
				{
					if(i*i + j*j <= radius*radius)
						canvas.erase_pixel(pt.x + i,pt.y + j);
				}
			}
		}
 void eraser_point_brush_t::stroke (unsigned int xpos, unsigned int ypos, canvas_t *canvas)
 {
    int radius = get_size();
	for(int i = -1*radius; i <= radius; i++)
	{
		for(int j = -1*radius; j <= radius; j++)
		{
			if(i*i + j*j <= radius*radius)
				canvas->erase_pixel(xpos + i,ypos + j);
		}
	}
 }

/**
 * \brief Smooth brush - Extended the stroke function to draw all pixels within a circle of radius equal to brush size. 
 * 
 */
 void smooth_brush_t::stroke (const point_t &pt, canvas_t &canvas)
		{	
			int radius = get_size();
			for(int i = -1*radius; i <= radius; i++)
			{
				for(int j = -1*radius; j <= radius; j++)
				{
					if(i*i + j*j <= radius*radius)
						canvas.set_pixel(pt.x + i,pt.y + j);
				}
			}
		}
 void smooth_brush_t::stroke (unsigned int xpos, unsigned int ypos, canvas_t *canvas)
 {
    int radius = get_size();
	for(int i = -1*radius; i <= radius; i++)
	{
		for(int j = -1*radius; j <= radius; j++)
		{
			if(i*i + j*j <= radius*radius)
				canvas->set_pixel(xpos + i,ypos + j);
		}
	}
 }

  
}
