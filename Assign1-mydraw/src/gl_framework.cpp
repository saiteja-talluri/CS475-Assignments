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
#include "gl_framework.hpp"

namespace csX75
{
  //! Initialize GL State
  void init_gl(mydraw::canvas_t* mycanvas)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    
    //Enable depth testing
    glDisable(GL_DEPTH_TEST);

    GLuint tex;
    glGenTextures( 1, &tex);
    
    glBindTexture( GL_TEXTURE_2D, tex );
    mycanvas->set_canvas_texture(tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mycanvas->get_width(), mycanvas->get_height(), 0, GL_RGBA, GL_FLOAT, mycanvas->get_store());
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // Load shaders and use the resulting shader program
    std::string vertex_shader_file("shaders/vshader.glsl");
    std::string fragment_shader_file("shaders/fshader.glsl");

    std::vector<GLuint> shader_list;
    shader_list.push_back(csX75::load_shader(GL_VERTEX_SHADER, vertex_shader_file));
    shader_list.push_back(csX75::load_shader(GL_FRAGMENT_SHADER, fragment_shader_file));

    GLuint shaderProgram = csX75::create_program(shader_list);
    glUseProgram( shaderProgram );
    mycanvas->set_canvas_sp(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "screen_texture"), 0);

    //! Vertex attributes for a quad that fills the entire screen in normalized device coordinates.
    float screen_quad_vertices[] = 
     { 
          // positions   // texCoords
          -1.0f,  1.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f,

          -1.0f,  1.0f,  0.0f, 1.0f,
           1.0f, -1.0f,  1.0f, 0.0f,
           1.0f,  1.0f,  1.0f, 1.0f
      };

    unsigned int screen_quad_vao, screen_quad_vbo;  

    glGenVertexArrays(1, &screen_quad_vao);
    glGenBuffers(1, &screen_quad_vbo);
    glBindVertexArray(screen_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, screen_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen_quad_vertices), &screen_quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    mycanvas->set_canvas_vao(screen_quad_vao);
    
    
    
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    mydraw::canvas_t* mycanvas = reinterpret_cast<mydraw::canvas_t*>(glfwGetWindowUserPointer(window));
    
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    
    //!Change the brush color if the C key was pressed
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
      float r,g,b;
      std::string a;
      std::cout << "Enter the color (r) :  ";
      std::cin >> r;
      std::cout << "Enter the color (g) :  ";
      std::cin >> g;
      std::cout << "Enter the color (b) :  ";
      std::cin >> b;
      std::cout << "Enter the color (a) :  ";
      std::cin.ignore();
      std::getline(std::cin,a);
      if(a==""){
        a="1.0";
      }
      mycanvas->get_context()->brush_color.r = r;
      mycanvas->get_context()->brush_color.g = g;
      mycanvas->get_context()->brush_color.b = b;
      mycanvas->get_context()->brush_color.a = std::stof(a);
    }
    
    //!Change the background color if the D key was pressed
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
      float r,g,b;
      std::string a;
      std::cout << "Enter the background color (r) :  ";
      std::cin >> r;
      std::cout << "Enter the background color (g) :  ";
      std::cin >> g;
      std::cout << "Enter the background color (b) :  ";
      std::cin >> b;
      std::cout << "Enter the background color (a) :  ";
      std::cin.ignore();
      std::getline(std::cin,a);
      if(a==""){
        a="1.0";
      }
      mycanvas->get_context()->bg_color.r = r;
      mycanvas->get_context()->bg_color.g = g;
      mycanvas->get_context()->bg_color.b = b;
      mycanvas->get_context()->bg_color.a = std::stof(a);
    }

    //!Go to floodfill mode if the F key was pressed
    else if (key == GLFW_KEY_F && action == GLFW_PRESS){
      mycanvas->set_context_keymode(mydraw::key_mode::Flood_Fill);
      std::cout << "Current Key mode : Flood_Fill" << std::endl;
    }

    //!Change the brush size in that keymode if the W key was pressed
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
      int size;
      std::cout << "Enter the size of the brush : ";
      std::cin >> size;
      size = std::max(size,1);
      if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Brush)
        {      
          mycanvas->get_context()->current_brush->set_size(size);
          std::cout << "Current size of the point brush : " << size << std::endl;
        }

      else if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Eraser)
        {
          mycanvas->get_context()->current_eraser->set_size(size);
          std::cout << "Current size of the eraser brush : " << size << std::endl;
        }
    }

    //!Change the primitve mode if the P key was pressed
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
      if(mycanvas->get_current_pmode() == mydraw::primitive_mode_t::point)
      {
        mycanvas->set_current_pmode(mydraw::primitive_mode_t::line);
        mycanvas->x2 = -1;
        mycanvas->y2 = -1;
        mycanvas->x1 = -1;
        mycanvas->y1 = -1;
        std::cout << "Current Primitive mode : Line" << std::endl;
      }
      else if(mycanvas->get_current_pmode() == mydraw::primitive_mode_t::line)
      {  
        mycanvas->set_current_pmode(mydraw::primitive_mode_t::triangle);
        mycanvas->x2 = -1;
        mycanvas->y2 = -1;
        mycanvas->x1 = -1;
        mycanvas->y1 = -1;
        std::cout << "Current Primitive mode : Triangle" << std::endl;
      }
      else
      {
          mycanvas->set_current_pmode(mydraw::primitive_mode_t::point);
          std::cout << "Current Primitive mode : Point" << std::endl;
      } 
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        mycanvas->x2 = -1;
        mycanvas->y2 = -1;
        mycanvas->x1 = -1;
        mycanvas->y1 = -1;
      mycanvas->set_context_keymode(mydraw::key_mode::Point_Brush);
      std::cout << "Current Key mode : Point_Brush" << std::endl;
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        mycanvas->x2 = -1;
        mycanvas->y2 = -1;
        mycanvas->x1 = -1;
        mycanvas->y1 = -1;
      mycanvas->set_context_keymode(mydraw::key_mode::Point_Eraser);
      std::cout << "Current Key mode : Point_Eraser" << std::endl;
    }

    //!Save the current canvas if the S key was pressed
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
      mycanvas->save();
      std::cerr<<"Saving canvas."<<std::endl;
    }
  }  


  //!GLFW mouse callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    mydraw::canvas_t* mycanvas = reinterpret_cast<mydraw::canvas_t*>(glfwGetWindowUserPointer(window));

    switch(button)
    {
      case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS)
        {
          mycanvas->set_onPress(true);
          ypos=mycanvas->get_height()-ypos;
          
          if(mycanvas->get_context_keymode() == mydraw::key_mode::Flood_Fill){
            mycanvas->get_context()->current_fill->fill(mycanvas,xpos,ypos);
            
          }
          else if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Brush)
          {
            mycanvas->get_context()->current_brush->stroke((unsigned int)xpos, (unsigned int)ypos, mycanvas);
            mydraw::mode_changes(mycanvas, xpos, ypos);
          }
          else if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Eraser){
            mycanvas->get_context()->current_eraser->stroke((unsigned int)xpos, (unsigned int)ypos, mycanvas);
            mydraw::mode_changes(mycanvas, xpos, ypos);
          }
        }
        else if (action == GLFW_RELEASE)
        {
          mycanvas->set_onPress(false);
        }
          break;
      default:
          break;
    }
  }

  

  //!GLFW mouse position callback
  void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
  {

    mydraw::canvas_t* mycanvas = reinterpret_cast<mydraw::canvas_t*>(glfwGetWindowUserPointer(window));
    
    if(mycanvas->get_onPress())
    {
          ypos= mycanvas->get_height()-ypos;

          if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Brush)
          {
            mycanvas->get_context()->current_brush->stroke((unsigned int)xpos, (unsigned int)ypos, mycanvas);
            mydraw::smooth_curve(mycanvas, xpos, ypos);
          }
          else if(mycanvas->get_context_keymode() == mydraw::key_mode::Point_Eraser)
          {
            mycanvas->get_context()->current_eraser->stroke((unsigned int)xpos, (unsigned int)ypos, mycanvas);
            mydraw::smooth_curve(mycanvas, xpos, ypos);
          }
    }
  }
  
  void display_callback(mydraw::canvas_t* mycanvas)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram( mycanvas->get_canvas_sp() );

    glBindVertexArray(mycanvas->get_canvas_vao());

    glBindTexture(GL_TEXTURE_2D, mycanvas->get_canvas_texture());

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mycanvas->get_width(), mycanvas->get_height(), GL_RGBA, GL_FLOAT, mycanvas->get_store());

    

    glDrawArrays(GL_TRIANGLES, 0, 6);

  }

  int render_gl(int argc, char** argv, mydraw::canvas_t* mycanvas)
  {
    //! The pointer to the GLFW window
    GLFWwindow* window;

    //! Setting up the GLFW Error callback
    glfwSetErrorCallback(csX75::error_callback);

    //! Initialize GLFW
    if (!glfwInit())
      return -1;

    //We want OpenGL 4.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //This is for MacOSX - can be omitted otherwise
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    //We don't want the old OpenGL4
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    //! Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(mycanvas->get_width(), mycanvas->get_height(), "MyDraw Canvas", NULL, NULL);
    if (!window)
      {
        glfwTerminate();
        return -1;
      }
    
    //! Make the window's context current 
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    //Turn this on to get Shader based OpenGL
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
      {
        //Problem: glewInit failed, something is seriously wrong.
        std::cerr<<"GLEW Init Failed : %s"<<std::endl;
      }

    //Print and see what context got enabled
    std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
    std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
    std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
    std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl<<std::endl;

    //Keyboard Callback
    glfwSetKeyCallback(window, key_callback);
    //Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Ensure we can capture the escape key being pressed below
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwSetWindowUserPointer(window, (void *)mycanvas);
    //Initialize GL state
    init_gl(mycanvas);
    //initBuffersGL();

    // Loop until the user closes the window
    while (glfwWindowShouldClose(window) == 0)
      {
         
        // Render here
        display_callback(mycanvas);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
      }
    
    glfwTerminate();
    return 0;
  }


}

