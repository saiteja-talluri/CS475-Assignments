#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include <queue>

/*// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;*/
// Camera position and rotation Parameters
GLfloat c_xpos = 200.0, c_ypos = 60.0, c_zpos = 200.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=0.0,c_yrot=0.0,c_zrot=0.0;

//Running variable to toggle culling on/off
bool enable_culling=false;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=true;
//Shader program attribs
GLuint vPosition,vColor,vNormal,vTexCoord,vType;

// Light On/Off
GLuint ON1,ON2;
int on1 = 1;
int on2 = 1;

//global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

csX75::HNode* root_node;
csX75::HNode* curr_node;
csX75::HNode* node[40];  // Humanoid model without hands
csX75::HNode* hnode[30]; // Adding hands to humnoid model
csX75::HNode* bnode[10]; // Box model
csX75::HNode* cnode[60]; // Robot model
csX75::HNode* dnode[2];
csX75::HNode* chnode[10]; //chair1
csX75::HNode* chnode1[10]; //chair2
csX75::HNode* tnode[10]; //table
csX75::HNode* beznode[50]; // bezier points
csX75::HNode* tnode1[10]; //table2
csX75::HNode* tempns[50]; //
csX75::HNode* quads[50]; //



std::vector<point>knots;
bool enable_animation=false;
bool enable_insertion=false;
std::queue<point>nodelist;




#endif
