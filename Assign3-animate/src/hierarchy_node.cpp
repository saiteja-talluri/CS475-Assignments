#include "hierarchy_node.hpp"

#include <iostream>

extern GLuint vPosition,vColor,vTexCoord,vNormal,uModelViewMatrix,normalMatrix,vType;
extern std::vector<glm::mat4> matrixStack;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, glm::vec2* a_textures, glm::vec4* a_normals, 
	std::size_t v_size, std::size_t c_size, std::size_t t_size, std::size_t n_size, GLuint texture, glm::vec2 *a_type, size_t a_size){

		num_vertices = num_v;
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		tex_buffer_size = t_size;
		normals_buffer_size = n_size;
		tex = texture;
		type_buffer_size = a_size;

		// initialize vao and vbo of the object;


		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + tex_buffer_size + normals_buffer_size + type_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, tex_buffer_size, a_textures );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + tex_buffer_size, normals_buffer_size, a_normals );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + tex_buffer_size + normals_buffer_size, type_buffer_size, a_type);

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

		glEnableVertexAttribArray( vTexCoord );
		glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + color_buffer_size));

		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + color_buffer_size + tex_buffer_size));

		glEnableVertexAttribArray( vType );
		glVertexAttribPointer( vType, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + color_buffer_size + tex_buffer_size + normals_buffer_size));


		// set parent

		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		update_matrices();
	}

	void HNode::update_matrices(){

		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));


	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);

	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void HNode::change_cons(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz,bool tx,bool ty,bool tz){
		xl = atx;
		xh = aty;
		yl = atz;;
		yh = arx;
		zl = ary;
		zh = arz;
		xt = tx;
		yt = ty;
		zt =  tz;
	}

	void HNode::render(){

		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		normal_matrix = glm::transpose (glm::inverse(glm::mat3(*ms_mult)));
		glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glBindVertexArray (vao);

		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// for memory 
		delete ms_mult;

	}

	void HNode::render_tree(){
		
		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);

		render();
		for(int i=0;i<(int)children.size();i++){
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();

	}

	void HNode::inc_rx(){
		float rx1=rx+15;
		if(rx1>180){
			rx1 = -360+rx1;
		}
		if(xt^(rx1<=xh&&rx1>=xl)){
			rx = rx1;
			update_matrices();
		}
	}


	void HNode::inc_ry(){
		float rx1=ry+15;
		if(rx1>180){
			rx1 = -360+rx1;
		}
		if(yt^(rx1<=yh&&rx1>=yl)){
			ry = rx1;
			update_matrices();
		}
	}

	void HNode::inc_rz(){
		float rx1=rz+15;
		if(rx1>180){
			rx1 = -360+rx1;
		}
		if(zt^(rx1<=zh&&rx1>=zl)){
			rz = rx1;
			update_matrices();
		}
	}

	void HNode::dec_rx(){
		float rx1=rx-15;
		if(rx1<-180){
			rx1 = rx1+360;
		}
		if(xt^(rx1<=xh&&rx1>=xl)){
			rx = rx1;
			update_matrices();
		}
		
	}

	void HNode::dec_ry(){
		float rx1=ry-15;
		if(rx1<-180){
			rx1 = rx1+360;
		}
		if(yt^(rx1<=yh&&rx1>=yl)){
			ry = rx1;
			update_matrices();
		}
	}

	void HNode::dec_rz(){

		float rx1=rz-15;

		if(rx1<-180){
			rx1 = rx1+360;
		}

		if(zt^(rx1<=zh&&rx1>=zl)){
			rz = rx1;
			update_matrices();
		}
		
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<(int)matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}