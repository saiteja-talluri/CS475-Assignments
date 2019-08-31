#include "hierarchical_modelling.hpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;

GLuint uModelViewMatrix;

/* <--------- Number of vertices ------------------> */

const int num_vertices_frustum = 360*3*2+ 360*6 + 360*3*2; // independent of frustum radius and height
const int num_vertices_ellipsoid = 360*180*6;              // ellipsoid independent of radius
const int num_vertices_glasses = 360*3*2+6*3;              // cooling glasses
const int num_vertices_cuboid = 36;                        // cuboid
const int num_vertices_square = 30;                        // box
const int num_vertices_hand = 18;                          // hand
const int num_vertices_feet = 42+12;                       // feet

/*  Definition of vertex position arrays and vertex color arrays */

double fingerscale = 1;
int tri_idx=0;
glm::vec4 v_frustum[num_vertices_frustum];       // frustum
glm::vec4 v_cfrustum[num_vertices_frustum];      // frustum
glm::vec4 v_ellipsoid[num_vertices_ellipsoid];   // ellipsoid
glm::vec4 v_cellipsoid[num_vertices_ellipsoid];  // ellipsoid
glm::vec4 v_hat[num_vertices_frustum];           // hat
glm::vec4 v_chat[num_vertices_frustum];          // hat
glm::vec4 v_specs[num_vertices_glasses];         // cooling glasses
glm::vec4 v_cspecs[num_vertices_glasses];        // cooling glasses
glm::vec4 v_bsquare[num_vertices_square];        //square
glm::vec4 v_cbsquare[num_vertices_square];       // square
glm::vec4 v_cuboid[num_vertices_cuboid];         // cuboid
glm::vec4 v_ccuboid[num_vertices_cuboid];        // cuboid
glm::vec4 v_finger[num_vertices_hand];           //finger
glm::vec4 v_cfinger[num_vertices_hand];          // finger
glm::vec4 v_feet[num_vertices_feet];             //feet
glm::vec4 v_cfeet[num_vertices_feet];            // feet

/* <--------- Colors used for models -------------> */
glm::vec4 bottom_color;
glm::vec4 lid_color;
glm::vec4 frustum_color;
glm::vec4 cycle_color_frustum_top;
glm::vec4 cycle_color_frustum_bottom;
glm::vec4 ellipsoid_color;
glm::vec4 hat_color;
glm::vec4 specs_color;
glm::vec4 cycle_color_specs;
glm::vec4 cycle_color;
glm::vec4 cuboid_color;


/* <-------- Sample colors used for music box  ------->  */

glm::vec4 colors[7] = {
  glm::vec4(0.8,  0.65, 0.45, 0.5),
  glm::vec4(1.0,  0.0,  0.0, 1.0), 
  glm::vec4(1.0 , 1.0 , 0.0, 1.0),
  glm::vec4(0.0 , 0.2 , 1.0, 0.5),
  glm::vec4(1.0 , 1.0 , 1.0, 1),
  glm::vec4(0.8,  0.65, 0.45, 1),
  glm::vec4(0.88, 0.36, 0.04, 1.0)
};

void cint(){
  tri_idx = 0;
}

/* <--------- Function to create a cuboid  --------> */
/*  To change cuboid color, you need to change cuboid_color */

void cuboid(double a, double b, double c) {

  glm::vec4 pos[8] = {
    glm::vec4(-a, -b ,  c , 1.0),   //0
    glm::vec4(-a,  b ,  c , 1.0),   //1
    glm::vec4( a,  b ,  c , 1.0),   //2
    glm::vec4( a, -b,   c , 1.0),   //3
    glm::vec4(-a, -b , -c , 1.0),   //4
    glm::vec4(-a,  b , -c , 1.0),   //5
    glm::vec4( a,  b,  -c , 1.0),   //6
    glm::vec4( a, -b , -c , 1.0)    //7
  };
  
  int x, y, z, w;
  
  for (int m = 0; m < 6; m++)
  {
    if(m == 0)        {x = 1; y = 0; z = 3, w = 2;}
    else if (m == 1)  {x = 2; y = 3; z = 7, w = 6;}
    else if (m == 2)  {x = 3; y = 0; z = 4, w = 7;}
    else if (m == 3)  {x = 6; y = 5; z = 1, w = 2;}
    else if (m == 4)  {x = 4; y = 5; z = 6, w = 7;}
    else if (m == 5)  {x = 5; y = 4; z = 0, w = 1;}
    else;
    v_ccuboid[tri_idx] = cuboid_color; v_cuboid[tri_idx] = pos[x]; tri_idx++;
    v_ccuboid[tri_idx] = cuboid_color; v_cuboid[tri_idx] = pos[y]; tri_idx++;
    v_ccuboid[tri_idx] = cuboid_color; v_cuboid[tri_idx] = pos[z]; tri_idx++;
    v_ccuboid[tri_idx] = cuboid_color; v_cuboid[tri_idx] = pos[x]; tri_idx++;
    v_ccuboid[tri_idx] = cuboid_color; v_cuboid[tri_idx] = pos[z]; tri_idx++;
    v_ccuboid[tri_idx] = cuboid_color; v_cuboid[tri_idx] = pos[w]; tri_idx++;
  }
}

double torad(double deg)
{
  return deg * M_PI / 180.0;
}

/* <--------- Function to create a horizontal disc--------> */
/*  To change horizontal disc color, you need to change cycle_color */

void cycle(glm::vec4 *v_cylin,glm::vec4 *v_ccylin,double y,double radius){
  for(int i = 0;i < 360; i++){
    v_cylin[tri_idx] = glm::vec4(0,y,0,1);v_ccylin[tri_idx] = cycle_color;tri_idx++;
    v_cylin[tri_idx] = glm::vec4(radius*cos(torad(i)),y,radius*sin(torad(i)),1);v_ccylin[tri_idx] = cycle_color;tri_idx++;
    v_cylin[tri_idx] = glm::vec4(radius*cos(torad(i+1)),y,radius*sin(torad(i+1)),1);v_ccylin[tri_idx] = cycle_color;tri_idx++;
  }
}

/* <--------- Function to create a vertical disc  --------> */
/*  To change vertical disc color, you need to change cycle_color */

void vcycle(glm::vec4 *v_cylin,glm::vec4 *v_ccylin,double x,double radius){
  for(int i = 0;i < 360; i++){
    v_cylin[tri_idx] = glm::vec4(x,0,0,1);v_ccylin[tri_idx] = cycle_color;tri_idx++;
    v_cylin[tri_idx] = glm::vec4(x+radius*cos(torad(i)),radius*sin(torad(i)),0,1);v_ccylin[tri_idx] = cycle_color;tri_idx++;
    v_cylin[tri_idx] = glm::vec4(x+radius*cos(torad(i+1)),radius*sin(torad(i+1)),0,1);v_ccylin[tri_idx] = cycle_color;tri_idx++;
  }
}

/* <--------- Function to create a Frustum  --------> */
/* To change the top and bottom disc colors of frustum, you need to change cycle_color_frustum_top and cycle_color_frustum_bottom */

void frustum(double ny,double py,double r_ny, double r_py){
  cycle_color = cycle_color_frustum_top;
  cycle(v_frustum,v_cfrustum,ny,r_ny);
  cycle_color = cycle_color_frustum_bottom;
  cycle(v_frustum,v_cfrustum,py,r_py);
  for(int i = 0;i < 360; i++){
    v_frustum[tri_idx] = glm::vec4(r_ny*cos(torad(i)),ny,r_ny*sin(torad(i)),1);v_cfrustum[tri_idx] = frustum_color;tri_idx++;
    v_frustum[tri_idx] = glm::vec4(r_py*cos(torad(i)),py,r_py*sin(torad(i)),1);v_cfrustum[tri_idx] = frustum_color;tri_idx++;
    v_frustum[tri_idx] = glm::vec4(r_py*cos(torad(i+1)),py,r_py*sin(torad(i+1)),1);v_cfrustum[tri_idx] = frustum_color;tri_idx++;
    v_frustum[tri_idx] = glm::vec4(r_py*cos(torad(i+1)),py,r_py*sin(torad(i+1)),1);v_cfrustum[tri_idx] = frustum_color;tri_idx++;
    v_frustum[tri_idx] = glm::vec4(r_ny*cos(torad(i)),ny,r_ny*sin(torad(i)),1);v_cfrustum[tri_idx] = frustum_color;tri_idx++;
    v_frustum[tri_idx] = glm::vec4(r_ny*cos(torad(i+1)),ny,r_ny*sin(torad(i+1)),1);v_cfrustum[tri_idx] = frustum_color;tri_idx++;
  }
  tri_idx = 0;
}

/* <--------- Function to create a hat  --------> */
/* To change the hat color, you need to change cycle_color and hat_color */

void hat(double ny,double py,double radius,double outradius){
  cycle_color = glm::vec4(1.0,0.5,0.0, 1.0);
  cycle(v_hat,v_chat,ny,outradius);
  cycle_color = hat_color;
  cycle(v_hat,v_chat,py,radius);
  for(int i = 0;i < 360;i++){
    v_hat[tri_idx] = glm::vec4(radius*cos(torad(i)),ny,radius*sin(torad(i)),1);v_chat[tri_idx] = hat_color;tri_idx++;
    v_hat[tri_idx] = glm::vec4(radius*cos(torad(i)),py,radius*sin(torad(i)),1);v_chat[tri_idx] = hat_color;tri_idx++;
    v_hat[tri_idx] = glm::vec4(radius*cos(torad(i+1)),py,radius*sin(torad(i+1)),1);v_chat[tri_idx] = hat_color;tri_idx++;
    v_hat[tri_idx] = glm::vec4(radius*cos(torad(i+1)),py,radius*sin(torad(i+1)),1);v_chat[tri_idx] = hat_color;tri_idx++;
    v_hat[tri_idx] = glm::vec4(radius*cos(torad(i)),ny,radius*sin(torad(i)),1);v_chat[tri_idx] = hat_color;tri_idx++;
    v_hat[tri_idx] = glm::vec4(radius*cos(torad(i+1)),ny,radius*sin(torad(i+1)),1);v_chat[tri_idx] = hat_color;tri_idx++;
  }
  tri_idx = 0;
}

/* <--------- Function to create an ellipsoid  --------> */
/* To change the ellipsoid color, you need to change ellipsoid_color */

void ellipsoid(double a, double b, double c){
  for(int i=0;i<360;i++){
    for(int j=-90;j<90;j++){
      v_ellipsoid[tri_idx] = glm::vec4(a*cos(torad(j))*cos(torad(i)),b*sin(torad(j)),c*cos(torad(j))*sin(torad(i)),1);
      v_cellipsoid[tri_idx]= ellipsoid_color;
      tri_idx++;
      v_ellipsoid[tri_idx] = glm::vec4(a*cos(torad(j))*cos(torad(i+1)),b*sin(torad(j)),c*cos(torad(j))*sin(torad(i+1)),1);
      v_cellipsoid[tri_idx]=ellipsoid_color;
      tri_idx++;
      v_ellipsoid[tri_idx] = glm::vec4(a*cos(torad(j+1))*cos(torad(i)),b*sin(torad(j+1)),c*cos(torad(j+1))*sin(torad(i)),1);
      v_cellipsoid[tri_idx]= ellipsoid_color;
      tri_idx++;
      v_ellipsoid[tri_idx] = glm::vec4(a*cos(torad(j))*cos(torad(i+1)),b*sin(torad(j)),c*cos(torad(j))*sin(torad(i+1)),1);
      v_cellipsoid[tri_idx]= ellipsoid_color;
      tri_idx++;
      v_ellipsoid[tri_idx] = glm::vec4(a*cos(torad(j+1))*cos(torad(i)),b*sin(torad(j+1)),c*cos(torad(j+1))*sin(torad(i)),1);
      v_cellipsoid[tri_idx]= ellipsoid_color;
      tri_idx++;
      v_ellipsoid[tri_idx] = glm::vec4(a*cos(torad(j+1))*cos(torad(i+1)),b*sin(torad(j+1)),c*cos(torad(j+1))*sin(torad(i+1)),1);
      v_cellipsoid[tri_idx]= ellipsoid_color;
      tri_idx++;
    }
  }
}

/* <--------- Helper function to create spectacle boxes  --------> */

void specsboxes(double scale,double scale1,double scale2,double pos){

  // Front Box
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale,pos+scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale , pos+-scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale ,pos+scale1  , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale , pos+scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale,pos+-scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale,pos+-scale1 , 0.0 , 1.0); tri_idx++;

  // Right Box
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale, pos+scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale , pos+-scale1 , 0.0  , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale , pos+scale1  ,-scale2 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale ,pos+scale1 , -scale2 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale, pos+-scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(scale, pos+-scale1 ,-scale2 , 1.0); tri_idx++;

  // Left Box
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale, pos+scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale ,pos+-scale1 , 0.0  , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale , pos+scale1  ,- scale2 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale , pos+scale1 ,-scale2 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale, pos+-scale1 , 0.0 , 1.0); tri_idx++;
  v_cspecs[tri_idx] = specs_color; v_specs[tri_idx] = glm::vec4(-scale, pos+-scale1 , -scale2 , 1.0); tri_idx++;

}

/* <--------- Helper function to create spectacle  --------> */
/* To change the spectacke color, you need to change both cycle_color_specs and specs_color */

void aspecs(double scale,double radius,double arg1,double arg2,double arg3,double arg4){
  cycle_color = cycle_color_specs;
  vcycle(v_specs,v_cspecs,-0.5*scale,radius*scale);
  vcycle(v_specs,v_cspecs,0.5*scale,radius*scale);
  specsboxes(arg1*scale,arg2*scale,arg3*scale,arg4*scale);
}

/* <--------- Function to create bottom of musical box  --------> */
/* <----- To change the colors look at colors[0] and colors[1] ----> */

void vbsquare(double x,double ny,double y,double delta){
  tri_idx = 0;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[1];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[1];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,ny+delta,0,1);v_cbsquare[tri_idx] = colors[1];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[1];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,-ny+delta,0,1);v_cbsquare[tri_idx] = colors[1];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,-ny+delta,0,1);v_cbsquare[tri_idx] = colors[1];tri_idx++;

  v_bsquare[tri_idx] = glm::vec4(-x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;

  v_bsquare[tri_idx] = glm::vec4(-x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;

  v_bsquare[tri_idx] = glm::vec4(x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,y-delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x,y,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;


  v_bsquare[tri_idx] = glm::vec4(-x,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,-ny+delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x,-ny,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(x-delta,-ny+delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;
  v_bsquare[tri_idx] = glm::vec4(-x+delta,-ny+delta,0,1);v_cbsquare[tri_idx] = colors[0];tri_idx++;

}

/* <--------- Function to create fingers  --------> */
/* <----- To change the colors look at colors[0] and colors[5] ----> */

void finger(double x,double y,double z){
  tri_idx = 0;
  x = x * fingerscale;
  y = y * fingerscale;
  z = z * fingerscale;
  v_finger[tri_idx] = glm::vec4(0,-y,0,1);v_cfinger[tri_idx] = colors[5];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(0,-y,0,1);v_cfinger[tri_idx] = colors[5];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(0,-y,0,1);v_cfinger[tri_idx] = colors[5];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(0,-y,0,1);v_cfinger[tri_idx] = colors[5];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(x,0,z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
  v_finger[tri_idx] = glm::vec4(-x,0,-z,1);v_cfinger[tri_idx] = colors[0];tri_idx++;
}


void quadad(glm::vec4* v_b,glm::vec4* v_c,glm::vec4* pos,int a, int b, int c, int d, int co)
{
  v_c[tri_idx] = colors[co]; v_b[tri_idx] = pos[a]; tri_idx++;
  v_c[tri_idx] = colors[co]; v_b[tri_idx] = pos[b]; tri_idx++;
  v_c[tri_idx] = colors[co]; v_b[tri_idx] = pos[c]; tri_idx++;
  v_c[tri_idx] = colors[co]; v_b[tri_idx] = pos[a]; tri_idx++;
  v_c[tri_idx] = colors[co]; v_b[tri_idx] = pos[c]; tri_idx++;
  v_c[tri_idx] = colors[co]; v_b[tri_idx] = pos[d]; tri_idx++;
 }


/* <--------- Function to create feet  --------> */
/* <----- To change the colors look at colors[0] and colors[6] ----> */

void feet(double nx,double px, double nz , double pz1 , double pz2, double ny, double py,double hy){
  glm::vec4 pos[10] = {
    glm::vec4(nx, ny,  pz2 ,1.0),   //0
    glm::vec4(nx, py , pz1 ,1.0),   //1
    glm::vec4(px, py , pz1 ,1.0),   //2
    glm::vec4(px, ny,  pz2 ,1.0),   //3
    glm::vec4(nx, ny , nz , 1.0),   //4
    glm::vec4(nx, py , nz , 1.0),   //5
    glm::vec4(px, py , nz , 1.0),   //6
    glm::vec4(px ,ny , nz , 1.0),   //7
    glm::vec4(nx, hy,  pz2, 1.0),   //8
    glm::vec4(px, hy,  pz2, 1.0),   //9
  }; 

    quadad( v_feet,v_cfeet,pos,0, 8, 9, 3 ,6);
    quadad( v_feet,v_cfeet,pos,8, 1, 2, 9 ,6);
    quadad( v_feet,v_cfeet,pos,2, 3, 7, 6 ,0);
    quadad( v_feet,v_cfeet,pos,3, 0, 4, 7 ,0);
    quadad( v_feet,v_cfeet,pos,4, 5, 6, 7 ,0);
    quadad( v_feet,v_cfeet,pos,5, 4, 0, 1 ,0);
    quadad( v_feet,v_cfeet,pos,1, 2, 6, 5 ,0);
    v_feet[tri_idx] = pos[2];v_cfeet[tri_idx] = colors[0];tri_idx++;
    v_feet[tri_idx] = pos[3];v_cfeet[tri_idx] = colors[0];tri_idx++;
    v_feet[tri_idx] = pos[9];v_cfeet[tri_idx] = colors[0];tri_idx++;
    v_feet[tri_idx] = pos[0];v_cfeet[tri_idx] = colors[0];tri_idx++;
    v_feet[tri_idx] = pos[8];v_cfeet[tri_idx] = colors[0];tri_idx++;
    v_feet[tri_idx] = pos[1];v_cfeet[tri_idx] = colors[0];tri_idx++;
    tri_idx = 0;
}

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("shaders/vshader.glsl");
  std::string fragment_shader_file("shaders/fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");


  /* <---------------- START OF HUMANOID MODEL ----------------> */
  double scale = 0.5;

  /* Lower section of torso 1*/
  frustum_color = glm::vec4(0.34,0.88,0.04, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.0*scale, 1.1*scale, 1.3*scale);
  cint();
  node[1] = new csX75::HNode(NULL,num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[1]->change_parameters(0,-1.2,0,0,0,0);
  
  /* Upper section of torso 1 */
  frustum_color = glm::vec4(0.34,0.88,0.04, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.9*scale, 1.3*scale, 1.5*scale);
  cint();
  node[2] = new csX75::HNode(node[1],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[2]->change_parameters(0,1.9*scale,0,0,0,0);
  node[2]->change_cons(-90,90,-60,60,-60,60,0,0,0);

  /* Section below neck */
  frustum_color = glm::vec4(0.34,0.38,0.94, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.8*scale, 1.5*scale, 0.4*scale);
  cint();
  node[3] = new csX75::HNode(node[2],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[3]->change_parameters(0,0.9*scale,0,0,0,0);
  node[3]->change_cons(0,0,0,0,0,0,0,0,0);

  /* Neck */
  ellipsoid_color = glm::vec4(0.88,0.36,0.04, 1.0);
  ellipsoid(0.5*scale,0.4*scale,0.5*scale);
  cint();
  node[4] = new csX75::HNode(node[3],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[4]->change_parameters(0,0.7*scale,0,20,0,0);
  node[4]->change_cons(-45,45,-45,45,-45,45,0,0,0);

  /* Head */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.9*scale,1.0*scale,1.0*scale);
  cint();
  node[5] = new csX75::HNode(node[4],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[5]->change_parameters(0,1.3*scale,0,0,0,0);
  node[5]->change_cons(-45,45,-45,45,-45,45,0,0,0);

  /* Right shoulder */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.35*scale,0.35*scale,0.45*scale);
  cint();
  node[6] = new csX75::HNode(node[2],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[6]->change_parameters(1.65*scale,0.70*scale,0,-110,0,60);

  /* Left Shoulder */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.35*scale,0.35*scale,0.45*scale);
  cint();
  node[7] = new csX75::HNode(node[2],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[7]->change_parameters(-1.65*scale,0.70*scale,0,-110,10,60);

  /* Right hand shoulder to elbow */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.75*scale, 0.25*scale, 0.22*scale);
  cint();
  node[8] = new csX75::HNode(node[6],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[8]->change_parameters(0.15*scale,0,0,0,0,-160);

  /* Left hand shoulder to elbow */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.75*scale, 0.25*scale, 0.22*scale);
  cint();
  node[9] = new csX75::HNode(node[7],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[9]->change_parameters(-0.15*scale,0,0,0,0,160);

  /* Right hand elbow */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.30*scale,0.30*scale,0.3*scale);
  cint();
  node[10] = new csX75::HNode(node[8],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[10]->change_parameters(0,2.0*scale,0,0,0,0);
  node[10]->change_cons(-15,180,0,0,0,0,0,0,0);

  /* Left hand elbow */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.30*scale,0.30*scale,0.3*scale);
  cint();
  node[11] = new csX75::HNode(node[9],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[11]->change_parameters(0,2.0*scale,0,-10,0,0);
  node[11]->change_cons(-15,180,0,0,0,0,0,0,0);

  /* Right hand elbow to wrist */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.0*scale, 0.25*scale, 0.15*scale);
  cint();
  node[12] = new csX75::HNode(node[10],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[12]->change_parameters(0,0.10*scale,0,0,0,-0);
  node[12]->change_cons(0,0,0,0,0,0,0,0,0);

  /* Left hand elbow to wrist */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.0*scale, 0.25*scale, 0.15*scale);
  cint();
  node[13] = new csX75::HNode(node[11],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[13]->change_parameters(0,0.10*scale,0,0,0,0);
  node[13]->change_cons(0,0,0,0,0,0,0,0,0);

  /* Torso 2 (just below Torso 1) */
  frustum_color = glm::vec4(0.88,0.36,0.04, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.5*scale, 1.1*scale, 1.1*scale);
  cint();
  node[14] = new csX75::HNode(node[1],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[14]->change_parameters(0,0,0,0,0,180);
  node[14]->change_cons(-90,90,-90,90,-90,90,0,0,1);
  /* From here the comments are wrt the body and not the viewer */
  /* Left hip (below torso 2) */
  ellipsoid_color = glm::vec4(0.9,0.74,0.44, 0.05);
  ellipsoid(0.5*scale,0.40*scale,0.50*scale);
  cint();
  node[15] = new csX75::HNode(node[14],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[15]->change_parameters(-0.6*scale,0.70*scale,0,0,0,0);

  /* Right hip (below torso 2) */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.5*scale,0.40*scale,0.50*scale);
  cint();
  node[16] = new csX75::HNode(node[14],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[16]->change_parameters(0.6*scale,0.70*scale,0,0,0,0);

  /* Left thigh */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.25*scale, 0.4*scale, 0.3*scale);
  cint();
  node[17] = new csX75::HNode(node[15],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[17]->change_parameters(0,0.10*scale,0,0,0,0);

  /* Right Thigh */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.25*scale, 0.4*scale, 0.3*scale);
  cint();
  node[18] = new csX75::HNode(node[16],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[18]->change_parameters(0,0.10*scale,0,0,0,0);

  /* Left knee */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.30*scale,0.30*scale,0.3*scale);
  cint();
  node[19] = new csX75::HNode(node[17],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[19]->change_parameters(0,2.4*scale,0,0,0,0);
  node[19]->change_cons(-165,165,0,0,0,0,0,0,0);

  /* Right knee */
  ellipsoid_color = glm::vec4(0.99,0.74,0.44, 1.0);
  ellipsoid(0.30*scale,0.30*scale,0.3*scale);
  cint();
  node[20] = new csX75::HNode(node[18],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[20]->change_parameters(0,2.4*scale,0,40,0,0);
  node[20]->change_cons(-165,165,0,0,0,0,0,0,0);
  
  /* Left leg */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.75*scale, 0.3*scale, 0.25*scale);
  cint();
  node[21] = new csX75::HNode(node[19],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[21]->change_parameters(0,0.20*scale,0,0,0,0);
  node[21]->change_cons(0,0,0,0,0,0,0,0,0);

  /* Right Leg */
  frustum_color = glm::vec4(0.06,0.32,0.77, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.75*scale, 0.3*scale, 0.25*scale);
  cint();
  node[22] = new csX75::HNode(node[20],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  node[22]->change_parameters(0,0.20*scale,0,0,0,0);
  node[22]->change_cons(0,0,0,0,0,0,0,0,0);
  
  /* Right foot */
  feet(-0.25*scale,0.25*scale,-0.2*scale , 0.3*scale , 1.2*scale, -0.5*scale, 0*scale,-0.1*scale);
  node[23] = new csX75::HNode(node[22],num_vertices_feet,v_feet,v_cfeet,sizeof(v_feet),sizeof(v_cfeet));
  node[23]->change_parameters(0,1.75*scale,0,0,40,180);
  
  /* Left foot */
  cint();
  node[24] = new csX75::HNode(node[21],num_vertices_feet,v_feet,v_cfeet,sizeof(v_feet),sizeof(v_cfeet));
  node[24]->change_parameters(0,1.75*scale,0,0,-40,180);

  /* Hat */
  hat_color = glm::vec4(0.71,0.09,0.43, 1.0);
  hat(-0.2*scale,0.6*scale,0.7*scale,1.4*scale);
  cint();
  node[30] = new csX75::HNode(node[5],num_vertices_frustum,v_hat,v_chat,sizeof(v_hat),sizeof(v_chat));
  node[30]->change_parameters(0,1.1*scale,0,8,0,0);

  /* Spectacles */
  cycle_color_specs = hat_color;
  specs_color = hat_color;
  aspecs(scale,0.4,0.95,0.075,1.2,0.1);
  node[25] = new csX75::HNode(node[5],num_vertices_glasses,v_specs,v_cspecs,sizeof(v_specs),sizeof(v_cspecs));
  node[25]->change_parameters(0,0,1.0*scale,0,0,0);

  /* Right ear */
  cint();
  ellipsoid_color = glm::vec4(0.76,0.56,0.28, 0.56);
  ellipsoid(0.3*scale,0.20*scale,0.20*scale);
  node[26] = new csX75::HNode(node[5],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[26]->change_parameters(0.8*scale,-0.1*scale,0,0,0,0);
  
  /* Nose */
  cint();
  ellipsoid(0.08*scale,0.2*scale,0.2*scale);
  node[27] = new csX75::HNode(node[5],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[27]->change_parameters(0,-0.35*scale,0.9*scale,-20,0,0);
  
  /* Left ear */
  cint();
  ellipsoid(0.3*scale,0.20*scale,0.20*scale);
  node[28] = new csX75::HNode(node[5],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[28]->change_parameters(-0.8*scale,-0.1*scale,0,0,0,0);
  
  /* Mouth */
  cint();
  ellipsoid_color = glm::vec4(0.71,0.32,0.32, 0.7);
  ellipsoid(0.3*scale,0.15*scale,0.1*scale);
  node[29] = new csX75::HNode(node[5],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  node[29]->change_parameters(0,-0.7*scale,0.65*scale,0,0,0);

  /* Hands for the humanoid model */
  fingerscale = 0.8*scale;
  finger(0.12,1.1,0.15);
  
  hnode[1] = new csX75::HNode(node[13],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  finger(0.12,1,0.15);
  
  hnode[2] = new csX75::HNode(hnode[1],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[2]->change_parameters(0.06*fingerscale,0,0,0,0,10);
  
  finger(0.12,0.8,0.15);
  hnode[3] = new csX75::HNode(hnode[1],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[3]->change_parameters(0.06*2*fingerscale,0,0,0,0,20);
  
  finger(0.12,1,0.15);
  hnode[4] = new csX75::HNode(hnode[1],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[4]->change_parameters(-0.06*fingerscale,0,0,0,0,-10);
  
  finger(0.12,0.7,0.15);
  hnode[5] = new csX75::HNode(hnode[1],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[5]->change_parameters(-0.06*2*fingerscale,0,0,0,0,-25);
  
  finger(0.12,1.1,0.15);
  hnode[6] = new csX75::HNode(node[12],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  
  finger(0.12,1,0.15);
  hnode[7] = new csX75::HNode(hnode[6],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[7]->change_parameters(-0.06*fingerscale,0,0,0,0,-10);
  
  finger(0.12,0.8,0.15);
  hnode[8] = new csX75::HNode(hnode[6],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[8]->change_parameters(-0.06*2*fingerscale,0,0,0,0,-20);
  
  finger(0.12,1,0.15);
  hnode[9] = new csX75::HNode(hnode[6],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[9]->change_parameters(0.06*fingerscale,0,0,0,0,10);
  
  finger(0.12,0.7,0.15);
  hnode[10] = new csX75::HNode(hnode[6],num_vertices_hand,v_finger,v_cfinger,sizeof(v_finger),sizeof(v_cfinger));
  hnode[10]->change_parameters(0.06*2*fingerscale,0,0,0,0,25);

  cint();
  feet(-1,1,-1 , 1 , 1.5, -2, 0,-1);
  hnode[11] = new csX75::HNode(NULL,num_vertices_feet,v_feet,v_cfeet,sizeof(v_feet),sizeof(v_cfeet));

  /* <---------------- END OF HUMANOID MODEL ----------------> */
  
  /* <---------------- START OF ROBOT MODEL ----------------> */  

  double cscale = 0.50;

  /* Center white disc */
  cint();
  frustum_color = glm::vec4(1.0,1.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.5*cscale, 0.8*cscale, 0.8*cscale);
  cint();
  cnode[1] = new csX75::HNode(NULL,num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[1]->change_parameters(0,0,0,0,0,0);

  /* Right small green sphere */
  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.15*cscale,0.15*cscale,0.10*cscale);
  cint();
  cnode[2] = new csX75::HNode(cnode[1],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[2]->change_parameters(0.95*cscale,0.25*cscale,0,0,0,0);

  /* Left small green sphere */
  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.15*cscale,0.15*cscale,0.10*cscale);
  cint();
  cnode[3] = new csX75::HNode(cnode[1],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[3]->change_parameters(-0.95*cscale,0.25*cscale,0,0,0,0);

  /* Right yellow thigh */
  frustum_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.9*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[4] = new csX75::HNode(cnode[2],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[4]->change_parameters(0,0.0*scale,0,0,0,180);

  /* Left yellow thigh */
  frustum_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.9*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[5] = new csX75::HNode(cnode[3],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[5]->change_parameters(0,0.0*scale,0,0,0,180);

  /* Triple ellipsoids on right side */
  ellipsoid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  ellipsoid(0.25*cscale,0.10*cscale,0.25*cscale);
  cint();
  cnode[6] = new csX75::HNode(cnode[4],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[6]->change_parameters(0,1.0*cscale,0,0,0,0);

  ellipsoid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  ellipsoid(0.25*cscale,0.10*cscale,0.25*cscale);
  cint();
  cnode[8] = new csX75::HNode(cnode[6],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[8]->change_parameters(0,0.20*cscale,0,0,0,0);

  ellipsoid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  ellipsoid(0.25*cscale,0.10*cscale,0.25*cscale);
  cint();
  cnode[10] = new csX75::HNode(cnode[8],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[10]->change_parameters(0,0.20*cscale,0,0,0,0);

  /* Triple ellipsoids on left side */
  ellipsoid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  ellipsoid(0.25*cscale,0.10*cscale,0.25*cscale);
  cint();
  cnode[7] = new csX75::HNode(cnode[5],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[7]->change_parameters(0,1.0*cscale,0,0,0,0);

  ellipsoid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  ellipsoid(0.25*cscale,0.10*cscale,0.25*cscale);
  cint();
  cnode[9] = new csX75::HNode(cnode[7],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[9]->change_parameters(0,0.20*cscale,0,0,0,0);

  ellipsoid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  ellipsoid(0.25*cscale,0.10*cscale,0.25*cscale);
  cint();
  cnode[11] = new csX75::HNode(cnode[9],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[11]->change_parameters(0,0.20*cscale,0,0,0,0);

  /* Right yellow cuboid below ellipsoid triplet */
  cuboid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cuboid(0.5*cscale,0.1*cscale,0.5*cscale);
  cint();
  cnode[12] = new csX75::HNode(cnode[10],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[12]->change_parameters(0,0.2*cscale,0,0,0,0);
  
  /* Left yellow cuboid below ellipsoid triplet */
  cuboid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cuboid(0.5*cscale,0.1*cscale,0.5*cscale);
  cint();
  cnode[13] = new csX75::HNode(cnode[11],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[13]->change_parameters(0,0.2*cscale,0,0,0,0);

  /* Right green cuboid below yellow cube */
  cuboid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cuboid(0.25*cscale,0.5*cscale,0.25*cscale);
  cint();
  cnode[14] = new csX75::HNode(cnode[12],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[14]->change_parameters(0,0.6*cscale,0,0,0,0);

  /* Left green cuboid below yellow cube */
  cuboid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cuboid(0.25*cscale,0.5*cscale,0.25*cscale);
  cint();
  cnode[15] = new csX75::HNode(cnode[13],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[15]->change_parameters(0,0.6*cscale,0,0,0,0);

  /* Right green cuboid below green length cuboid */
  cuboid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cuboid(0.5*cscale,0.2*cscale,0.5*cscale);
  cint();
  cnode[16] = new csX75::HNode(cnode[14],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[16]->change_parameters(0,0.7*cscale,0,0,0,0);
  
  /* Left green cuboid below green length cuboid */
  cuboid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cuboid(0.5*cscale,0.2*cscale,0.5*cscale);
  cint();
  cnode[17] = new csX75::HNode(cnode[15],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[17]->change_parameters(0,0.7*cscale,0,0,0,0);

  /* Right red cylinder below the green cuboid */ 
  frustum_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(-0.5*cscale, 0.5*cscale, 0.30*cscale, 0.30*cscale);
  cint();
  cnode[18] = new csX75::HNode(cnode[16],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[18]->change_parameters(0,0.5*cscale,0,90,0,0);

  /* Left red cylinder below the green cuboid */ 
  frustum_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(-0.5*cscale, 0.5*cscale, 0.30*cscale, 0.30*cscale);
  cint();
  cnode[19] = new csX75::HNode(cnode[17],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[19]->change_parameters(0,0.5*cscale,0,90,0,0);

  /* Right cuboid below right red cylinder */
  cuboid_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cuboid(0.5*cscale,0.5*cscale,0.2*cscale);
  cint();
  cnode[20] = new csX75::HNode(cnode[18],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[20]->change_parameters(0,0,-0.5*cscale,0,0,0);
  
  /* Left cuboid below left red cylinder */
  cuboid_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cuboid(0.5*cscale,0.5*cscale,0.2*cscale);
  cint();
  cnode[21] = new csX75::HNode(cnode[19],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[21]->change_parameters(0,0,-0.5*cscale,0,0,0);

  /* Red Cuboid ABOVE the root node white frustum */
  cuboid_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cuboid(0.6*cscale,0.6*cscale,0.6*cscale);
  cint();
  cnode[22] = new csX75::HNode(cnode[1],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[22]->change_parameters(0,1.1*cscale,0,0,0,0);

  /* Cylinder to the right of the red cuboid */
  frustum_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.6*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[23] = new csX75::HNode(cnode[22],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[23]->change_parameters(0.6*cscale,0,0,0,0,-90);

  /* Cylinder to the left of the red cuboid */
  frustum_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.6*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[24] = new csX75::HNode(cnode[22],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[24]->change_parameters(-0.6*cscale,0,0,0,0,90);

  /* Right green sphere at top */
  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.6*cscale,0.6*cscale,0.6*cscale);
  cint();
  cnode[25] = new csX75::HNode(cnode[23],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[25]->change_parameters(0,2.0*cscale,0,0,0,0);

  /* Left green sphere at top */
  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.6*cscale,0.6*cscale,0.6*cscale);
  cint();
  cnode[26] = new csX75::HNode(cnode[24],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[26]->change_parameters(0,2.0*cscale,0,0,0,0);

  /* Right red cuboid at top */
  cuboid_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cuboid(1.0*cscale,0.4*cscale,0.4*cscale);
  cint();
  cnode[27] = new csX75::HNode(cnode[25],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[27]->change_parameters(1.50*cscale,0,0,0,0,0);

  /* Left red cuboid at top */
  cuboid_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cuboid(1.0*cscale,0.4*cscale,0.4*cscale);
  cint();
  cnode[28] = new csX75::HNode(cnode[26],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[28]->change_parameters(-1.50*cscale,0,0,0,0,0);

  /* Right red cylinder below red cube at top */
  frustum_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.5*cscale, 0.25*cscale, 0.25*cscale);
  cint();
  cnode[29] = new csX75::HNode(cnode[27],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[29]->change_parameters(1.0*cscale,0,0,-20,-20,-80);

  /* Left red cylinder below red cube at top */
  frustum_color = glm::vec4(1.0,0.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.5*cscale, 0.25*cscale, 0.25*cscale);
  cint();
  cnode[30] = new csX75::HNode(cnode[28],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[30]->change_parameters(-1.0*cscale,0,0,-20,20,80);

  /* Right yellow cuboid (hand) */ 
  cuboid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cuboid(0.6*cscale,0.7*cscale,1.25*cscale);
  cint();
  cnode[31] = new csX75::HNode(cnode[29],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[31]->change_parameters(0,2.1*cscale,0.5*cscale,0,0,0);

  /* Left yellow cuboid (hand) */
  cuboid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cuboid(0.6*cscale,0.7*cscale,1.25*cscale);
  cint();
  cnode[32] = new csX75::HNode(cnode[30],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[32]->change_parameters(0,2.1*cscale,0.5*cscale,0,0,0);

  /* FOUR RIGHT BULLET LAUNCHERS */
  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[33] = new csX75::HNode(cnode[31],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[33]->change_parameters(0.3*cscale,0.0*cscale,1.25*cscale,90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[34] = new csX75::HNode(cnode[31],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[34]->change_parameters(-0.3*cscale,0.0*cscale,1.25*cscale,90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[35] = new csX75::HNode(cnode[31],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[35]->change_parameters(0.0*cscale,0.35*cscale,1.25*cscale,90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[36] = new csX75::HNode(cnode[31],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[36]->change_parameters(0.0*cscale,-0.35*cscale,1.25*cscale,90,0,0);

  /* LEFT RIGHT BULLET LAUNCHERS */
  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[37] = new csX75::HNode(cnode[32],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[37]->change_parameters(0.3*cscale,0.0*cscale,1.25*cscale,90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[38] = new csX75::HNode(cnode[32],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[38]->change_parameters(-0.3*cscale,0.0*cscale,1.25*cscale,90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[39] = new csX75::HNode(cnode[32],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[39]->change_parameters(0.0*cscale,0.35*cscale,1.25*cscale,90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 3.0*cscale, 0.20*cscale, 0.20*cscale);
  cint();
  cnode[40] = new csX75::HNode(cnode[32],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[40]->change_parameters(0.0*cscale,-0.35*cscale,1.25*cscale,90,0,0);

  /* Neck part above the red cube */
  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 1.5*cscale, 0.15*cscale, 0.15*cscale);
  cint();
  cnode[41] = new csX75::HNode(cnode[22],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[41]->change_parameters(0,0.6*cscale,0,0,-20,0);

  /* Yellow face at the top */
  cuboid_color = glm::vec4(1.0,1.0,0.0, 1.0);
  cuboid(0.7*cscale,1.2*cscale,0.30*cscale);
  cint();
  cnode[42] = new csX75::HNode(cnode[41],num_vertices_cuboid,v_cuboid,v_ccuboid,sizeof(v_cuboid),sizeof(v_ccuboid));
  cnode[42]->change_parameters(0,2.7*cscale,0.0*cscale,0,0,0);

  /* SIX GREEN EYES AT THE TOP */
  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.20*cscale,0.20*cscale,0.01*cscale);
  cint();
  cnode[43] = new csX75::HNode(cnode[42],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[43]->change_parameters(0.35*cscale,0.0*cscale,0.3*cscale,0,0,0);

  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.20*cscale,0.20*cscale,0.01*cscale);
  cint();
  cnode[44] = new csX75::HNode(cnode[42],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[44]->change_parameters(-0.35*cscale,0.0*cscale,0.3*cscale,0,0,0);

  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.20*cscale,0.20*cscale,0.01*cscale);
  cint();
  cnode[45] = new csX75::HNode(cnode[42],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[45]->change_parameters(0.35*cscale,0.6*cscale,0.3*cscale,0,0,0);

  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.20*cscale,0.20*cscale,0.01*cscale);
  cint();
  cnode[46] = new csX75::HNode(cnode[42],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[46]->change_parameters(-0.35*cscale,0.6*cscale,0.3*cscale,0,0,0);

  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.20*cscale,0.20*cscale,0.01*cscale);
  cint();
  cnode[47] = new csX75::HNode(cnode[42],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[47]->change_parameters(0.35*cscale,-0.6*cscale,0.3*cscale,0,0,0);

  ellipsoid_color = glm::vec4(0.0,1.0,0.0, 1.0);
  ellipsoid(0.20*cscale,0.20*cscale,0.01*cscale);
  cint();
  cnode[48] = new csX75::HNode(cnode[42],num_vertices_ellipsoid,v_ellipsoid,v_cellipsoid,sizeof(v_ellipsoid),sizeof(v_cellipsoid));
  cnode[48]->change_parameters(-0.35*cscale,-0.6*cscale,0.3*cscale,0,0,0);

  /* LEFT FOUR BLUE LEGS */
  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[49] = new csX75::HNode(cnode[21],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[49]->change_parameters(0.25*cscale,0.25*cscale,-0.2*cscale,-90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[50] = new csX75::HNode(cnode[21],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[50]->change_parameters(0.25*cscale,-0.25*cscale,-0.2*cscale,-90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[51] = new csX75::HNode(cnode[21],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[51]->change_parameters(-0.25*cscale,0.25*cscale,-0.2*cscale,-90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[52] = new csX75::HNode(cnode[21],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[52]->change_parameters(-0.25*cscale,-0.25*cscale,-0.2*cscale,-90,0,0);
  
  /* RIGHT FOUR BLUE LEGS */
  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[53] = new csX75::HNode(cnode[20],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[53]->change_parameters(0.25*cscale,0.25*cscale,-0.2*cscale,-90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[54] = new csX75::HNode(cnode[20],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[54]->change_parameters(0.25*cscale,-0.25*cscale,-0.2*cscale,-90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[55] = new csX75::HNode(cnode[20],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[55]->change_parameters(-0.25*cscale,0.25*cscale,-0.2*cscale,-90,0,0);

  frustum_color = glm::vec4(0.0,0.0,1.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 2.8*cscale, 0.10*cscale, 0.10*cscale);
  cint();
  cnode[56] = new csX75::HNode(cnode[20],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[56]->change_parameters(-0.25*cscale,-0.25*cscale,-0.2*cscale,-90,0,0);

  /* Right foot */
  frustum_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.4*cscale, 0.8*cscale, 0.8*cscale);
  cint();
  cnode[57] = new csX75::HNode(cnode[56],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[57]->change_parameters(0.25*cscale,2.8*cscale,0.25*cscale,0,0,0);

  /* Left foot */
  frustum_color = glm::vec4(0.0,1.0,0.0, 1.0);
  cycle_color_frustum_top = frustum_color;
  cycle_color_frustum_bottom = frustum_color;
  frustum(0, 0.4*cscale, 0.8*cscale, 0.8*cscale);
  cint();
  cnode[58] = new csX75::HNode(cnode[52],num_vertices_frustum,v_frustum,v_cfrustum,sizeof(v_frustum),sizeof(v_cfrustum));
  cnode[58]->change_parameters(0.25*cscale,2.8*cscale,0.25*cscale,0,0,0);
  
   /* <---------------- END OF ROBOT MODEL ----------------> */  

   /* <---------------- START OF BOX MODEL ----------------> */  

  // double x = 1, y = 0.5, delta = 0.05
  double x = 1, y = 0.5, delta = 0.065;

  vbsquare(x,0,y,delta);
  bnode[1] = new csX75::HNode(NULL,num_vertices_square,v_bsquare,v_cbsquare,sizeof(v_bsquare),sizeof(v_cbsquare));
  bnode[2] = new csX75::HNode(bnode[1],num_vertices_square,v_bsquare,v_cbsquare,sizeof(v_bsquare),sizeof(v_cbsquare));
  bnode[2]->change_parameters(x,0,-x,0,90,0);
  bnode[3] = new csX75::HNode(bnode[1],num_vertices_square,v_bsquare,v_cbsquare,sizeof(v_bsquare),sizeof(v_cbsquare));
  bnode[3]->change_parameters(0,0,-2*x,0,0,0);
  bnode[4] = new csX75::HNode(bnode[1],num_vertices_square,v_bsquare,v_cbsquare,sizeof(v_bsquare),sizeof(v_cbsquare));
  bnode[4]->change_parameters(-x,0,-x,0,90,0);

  cint();
  vbsquare(x,0,2*x,delta);
  bnode[5] = new csX75::HNode(bnode[1],num_vertices_square,v_bsquare,v_cbsquare,sizeof(v_bsquare),sizeof(v_cbsquare));
  bnode[5]->change_parameters(0,y,-2*x,75,0,0);
  bnode[5]->change_cons(-45,90,0,0,0,0,0,0,0);
  bnode[6] = new csX75::HNode(bnode[1],num_vertices_square,v_bsquare,v_cbsquare,sizeof(v_bsquare),sizeof(v_cbsquare));
  bnode[6]->change_parameters(0,0,-2*x,90,0,0);

  /* <---------------- END OF BOX MODEL ----------------> */ 

  root_node =  curr_node = node[1];
  node[1]->change_parameters(-4.5,0,0,0,0,0);
  hnode[1]->change_parameters(0,2.0*scale,0,180,0,0);
  hnode[6]->change_parameters(0,2.0*scale,0,180,0,0);
  bnode[1]->change_parameters(-0.5,-0.2,0,40,-30,0);
  cnode[1]->change_parameters(4,0.5,0,0,0,0);

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    projection_matrix = glm::frustum(-7.0, 7.0, -7.0, 7.0, 1.0, 7.0);
    //projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-7.0, 7.0, -7.0, 7.0, -5.0, 5.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);

  node[1]->render_tree();
  bnode[1]->render_tree();
  cnode[1]->render_tree();
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(1024, 1024, "The Music Box: Modelling", NULL, NULL);
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
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();
      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

