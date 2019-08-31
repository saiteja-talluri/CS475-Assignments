#include "gl_framework.hpp"
#include "hierarchy_node.hpp"

extern GLfloat c_xrot,c_yrot,c_zrot;
extern bool enable_perspective;
extern csX75::HNode* node[40], *curr_node , * bnode[10], * cnode[60], *hnode[30];
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
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
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
      curr_node = node[1];  
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
      curr_node = node[4]; 
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
      curr_node = node[6]; 
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
      curr_node = node[7]; 
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
      curr_node = node[10]; 
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
      curr_node = node[11]; 
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
      curr_node = node[15]; 
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
      curr_node = node[16]; 
    else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
      curr_node = node[19]; 
    else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
      curr_node = node[20];
    else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
      curr_node = node[5];
    else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
      curr_node = node[14]; 
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
      curr_node = hnode[1];
    else if (key == GLFW_KEY_X && action == GLFW_PRESS)
      curr_node = hnode[6];
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
      curr_node = node[23];
    else if (key == GLFW_KEY_V && action == GLFW_PRESS)
      curr_node = node[24];
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
      curr_node = bnode[1]; 
    else if (key == GLFW_KEY_O && action == GLFW_PRESS)
      curr_node = bnode[5]; 
    else if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
      curr_node = cnode[1]; 
    else if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
      curr_node = cnode[2]; 
    else if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
      curr_node = cnode[3]; 
    else if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
      curr_node = cnode[18]; 
    else if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
      curr_node = cnode[19];
    else if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
      curr_node = cnode[23]; 
    else if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
      curr_node = cnode[24]; 
    else if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
      curr_node = cnode[25];    
    else if (key == GLFW_KEY_F9 && action == GLFW_PRESS)
      curr_node = cnode[26]; 
    else if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
      curr_node = cnode[29];   
    else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
      curr_node = cnode[30];    
    else if (key == GLFW_KEY_F12 && action == GLFW_PRESS)
      curr_node = cnode[31]; 
    else if (key == GLFW_KEY_INSERT && action == GLFW_PRESS)
      curr_node = cnode[32];
    else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
      curr_node = cnode[22];
    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
      curr_node = cnode[41];
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
      curr_node->dec_ry();
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
      curr_node->inc_ry();
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
      curr_node->dec_rx();
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
      curr_node->inc_rx();
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
      curr_node->dec_rz();
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
      curr_node->inc_rz();
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
      enable_perspective = !enable_perspective;   
    else if (key == GLFW_KEY_A  && action == GLFW_PRESS)
      c_yrot -= 1.0;
    else if (key == GLFW_KEY_D  && action == GLFW_PRESS)
      c_yrot += 1.0;
    else if (key == GLFW_KEY_W  && action == GLFW_PRESS)
      c_xrot -= 1.0;
    else if (key == GLFW_KEY_S  && action == GLFW_PRESS)
      c_xrot += 1.0;        
    else if (key == GLFW_KEY_Q  && action == GLFW_PRESS)
      c_zrot -= 1.0;
    else if (key == GLFW_KEY_E  && action == GLFW_PRESS)
      c_zrot += 1.0; 
      
  }
};  
  


