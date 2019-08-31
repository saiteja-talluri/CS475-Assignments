#version 130

in vec3 normal;
in vec4 eye;
in vec4 Color;
in vec2 tex;
in vec4 ecPos;
in float Type;

uniform mat4 viewMatrix;
uniform sampler2D texture;

out vec4 frag_color;

in float s1;
in float s2;

void main () 
{
  
  vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0);
  vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
  vec4 specular = vec4(0.2, 0.2, 0.2, 1.0);
  float shininess = 1.0;
  vec4 spec = vec4(0.5);

  vec4 texPic = Color;
  
  if(Type == 1.0){
    texPic = texture2D(texture, tex);
    // texPic = texture(texture, tex);
  }


  // First Light
  vec4 lightPos1 = vec4(10.0,40, 10.0, 0.0);
  vec3 lightDir1 = vec3(viewMatrix * lightPos1);
  lightDir1 = normalize(lightDir1);

  // Second light
  vec4 lightPos2 = vec4(20.0, 10.0, 20.0, 0.0);
  vec3 lightDir2 = vec3(viewMatrix * lightPos2);
  lightDir2 = normalize(lightDir2);

  vec4 color = vec4(0.0);
  vec3 n = normalize(vec3(normal));

  if(s1 > 0.5) {
      
      float dotProduct = dot(n, lightDir1);
      float intensity =  max( dotProduct, 0.0);

      if(intensity > 0.0) {
          vec3 e = normalize(vec3(eye));
          vec3 h = normalize(lightDir1 + e );
          float intSpec = max(dot(h,n), 0.0);
          spec = specular * pow(intSpec, shininess);
      }

      color += (intensity * diffuse  + spec)*texPic;
  }

  if(s2 > 0.5) {
      
      float dotProduct = dot(n, lightDir2);
      float intensity =  max( dotProduct, 0.0);

      if(intensity > 0.0) {
          vec3 e = normalize(vec3(eye));
          vec3 h = normalize(lightDir2 + e );
          float intSpec = max(dot(h,n), 0.0);
          spec = specular * pow(intSpec, shininess);
      }

      color += (intensity * diffuse  + spec)*texPic;
  }

  color = max(color, ambient*texPic);

  frag_color = color;
}
