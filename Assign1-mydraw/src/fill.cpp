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

#include "fill.hpp"
#include "canvas.hpp"
#include <bits/stdc++.h>
using namespace std;
typedef pair<int,int> Point;

namespace mydraw
{
	Point gp(int x, int y){
		Point p;
		p.first = x;
		p.second = y;
		return p;
	}

	bool check(int x0, int y0, int w, int h, float r, float g, float b, float a, mydraw::canvas_t* mycanvas){

		if(x0 < w  && y0 < h  && x0 > 0 && y0 > 0){
			float r1 = mycanvas->get_pixel(x0,y0).r;
			float g1 = mycanvas->get_pixel(x0,y0).g;
			float b1 = mycanvas->get_pixel(x0,y0).b;
			float a1 = mycanvas->get_pixel(x0,y0).a;
			if(r1 == r && g1 == g && b1 == b && a1 == a)return true;
		}
		return false;
	}


	void floodfill_t::fill(mydraw::canvas_t* mycanvas,int x0, int y0){
		queue<Point> q;
		queue<Point> q1;
		map<Point,int>visited;
		int x = mycanvas->get_width();
		int y = mycanvas->get_height();
		float r = mycanvas->get_pixel(x0,y0).r;
		float g = mycanvas->get_pixel(x0,y0).g;
		float b = mycanvas->get_pixel(x0,y0).b;
		float a = mycanvas->get_pixel(x0,y0).a;

		q.push(gp(x0,y0));
		q1.push(gp(x0,y0));
		visited[gp(x0,y0)] = 1;
		int i = 0;
		while(!q.empty()){
			Point p = q.front();
			Point p1 = gp(p.first + 1, p.second);
			if(check(p.first + 1, p.second, x, y, r, g, b, a, mycanvas) && visited.find(p1) == visited.end() ){
				visited[p1] = 1;
				q.push(p1);
				q1.push(p1);
				i++;
			}
			p1 = gp(p.first, p.second + 1);
			if(check(p.first, p.second + 1, x, y, r, g, b, a, mycanvas) && visited.find(p1) == visited.end() ){
				visited[p1] = 1;
				q.push(p1);
				q1.push(p1);
				i++;
			}
			p1 = gp(p.first - 1, p.second);
			if(check(p.first - 1, p.second, x, y, r, g, b, a, mycanvas) && visited.find(p1) == visited.end() ){
				visited[p1] = 1;
				q.push(p1);
				q1.push(p1);
				i++;
			}
			p1 = gp(p.first, p.second - 1);
			if(check(p.first, p.second - 1, x, y, r, g, b, a, mycanvas) && visited.find(p1) == visited.end() ){
				visited[p1] = 1;
				q.push(p1);
				q1.push(p1);
				i++;
			}
			q.pop();
		}
		while(!q1.empty()){
			Point p1 = q1.front();
			mycanvas->set_pixel((unsigned int)p1.first, (unsigned int)p1.second);
			q1.pop();
		}
		// printf("Filled %d pixels\n", i);
	}
}