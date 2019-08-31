#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "gl_framework.hpp"


namespace csX75	 { 

	// A simple class that represents a node in the hierarchy tree
	class HNode {
		//glm::vec4 * vertices;
		//glm::vec4 * colors;
		GLfloat tx,ty,tz,rx,ry,rz;
		GLfloat xl=-180,xh=180,yl=-180,yh=180,zl=-180,zh=180;
		bool xt=0,yt=0,zt=0;

		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		std::size_t tex_buffer_size;
		std::size_t normals_buffer_size;
		std::size_t type_buffer_size;

		GLuint num_vertices;
		GLuint vao,vbo,tex;

		glm::mat4 rotation;
		glm::mat4 translation;
		glm::mat3 normal_matrix;
		
		std::vector<HNode*> children;
		HNode* parent;

		void update_matrices();

	  public:
		HNode (HNode*, GLuint, glm::vec4*,  glm::vec4*, glm::vec2*, glm::vec4*, std::size_t, std::size_t, std::size_t, std::size_t, GLuint, glm::vec2*, std::size_t);
		//HNode (HNode* , glm::vec4*,  glm::vec4*,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);

		void add_child(HNode*);
		void render();
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void change_cons(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,bool,bool,bool);
		void render_tree();
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );
};	

#endif