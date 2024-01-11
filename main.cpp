#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <list>
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include <filesystem>
#include <glob.h>
#include <random>


#include <stdio.h>
#include <bits/stdc++.h>
#include <termios.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>



#include <epoxy/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <EGL/egl.h>
	

#include <stdio.h> /*standard i/o*/
#include <unistd.h> /*FOR USING FORK for at a time send and receive messages*/ 
#include <malloc.h> /*FOR MEMORY ALLOCATION */
#include <string.h> /*using fgets funtions for geting input from user*/




#define FAIL    -1 /*for error output == -1 */
#define BUFFER  1024  /*buffer for reading messages*/




std::string hostname="xxx.xxx.xxx.xxx";
std::string portnum="pppp";




//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "opencv2/core.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"

#include "PLYWriter.h"
#include "Context.h"
#include "Scope.h"
#include "grammar.h"
#include "Solution.h"




#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

//#include "hex_planet.h"
#include "textures.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                                                   char appname[]="ProGen3d 0.95c";                //////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




std::mt19937_64 rng;





//===========================================================================================================================

//												 Globals 

//===========================================================================================================================

Grammar *grammar;

std::string username,password;


static int SCREEN_WIDTH=1600;
static int SCREEN_HEIGHT=1200;

std::string grammar_filename("./test.grammar");

extern std::vector<Variable *> variable_list;
extern std::vector<Variable *> full_variable_list;



float angle_view=0.0f;
float scale_global=0.3f;
float elevation_view=0.0f;

std::vector<int> texture_list;
std::vector<float> texture_list_alpha;
std::vector<std::string> texture_filenames;
bool loggedin=false;

bool play=true;

void display( void );

bool setup_textures=true;
bool setup_vars=true;
bool setup_rules=true;

int img_counter=0;

int frame_counter=0;
bool init_buffer=false;



;

using glm::mat4;
using glm::vec3;
using glm::lookAt;
using glm::perspective;
using glm::rotate;
using glm::translate;
using glm::scale;

const GLchar  *VERTEX_SOURCE =
"#version 330\n"
"in vec3 position;\n"
"in vec3 normal;\n"
"in vec2 texture;\n"
"out vec3 fN;\n"
"out vec3 fV;\n"
"out vec3 fL;\n"
"out vec2 tex_coord;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"uniform vec3 lightposition;\n"
//"uniform vec3 pos;\n"
//"uniform vec3 scale_vec;\n"
"void main(){\n"
"       tex_coord.x=texture.x;	\n"
"       tex_coord.y=texture.y;	\n"
//"    fN = (model*view*vec4(normal,1.0)).xyz ;\n"
"    fN = (view*vec4(normal,1.0)).xyz ;\n"
//"    fV = - (model*view*vec4(position+pos, 1.0)).xyz;\n"
"    fV = - (view*vec4(position, 1.0)).xyz;\n"
//"    fL = lightposition.xyz - (model*view*vec4(position+pos, 1.0)).xyz ;\n"
"    fL = (view*vec4(lightposition-position, 1.0)).xyz ;\n"
//"    gl_Position = projection * view *(   (model * vec4(position+pos, 1.0)) );\n"
"    gl_Position = projection * view *model*vec4(position, 1.0);\n"
"}\n";






const GLchar *FRAGMENT_SOURCE2 =
"#version 330\n"
"in vec3 fN ;\n"
"in vec3 fL ;\n"
"in vec3 fV ;\n"
"in vec2 tex_coord;\n"
"uniform vec4 ambientproduct, diffuseproduct, specularproduct ;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform vec3 lightposition ;\n"
"uniform float shinyness ;\n"
"uniform sampler2D texture1;\n"
"uniform float alpha;\n"
"void main(){vec3 N = normalize(fN) ;\n"
"vec3 V = normalize(fV) ;\n"
"vec3 L = normalize(fL) ;\n"
"vec3 H = normalize( L + V ) ;\n"
"vec4 ambient = ambientproduct ;\n"
"float Kd = max(dot(L, N), 0.0) ;\n"
"vec4 diffuse = Kd*diffuseproduct ;\n"
"float Ks = pow(max(dot(N, H), 0.0), shinyness) ;\n"
"vec4 specular = Ks*specularproduct ;\n"
"if( dot(L, N) < 0.0 )specular = vec4(0.0, 0.0, 0.0, 1.0) ;\n"
//"if(alpha!=1.0)gl_FragColor = texture(texture1, tex_coord) ;\n"
" gl_FragColor = texture(texture1, tex_coord) *vec4((ambient.xyz + diffuse.xyz + specular.xyz),alpha);\n"
"}\n"; 


/* the GtkGLArea widget */


/* The object we are drawing */


long current_frame = 0.0;
long delta_time = 0.0;

int dt = 0.1;

static GLuint position_buffer,position_buffer1[50];
static GLuint program;
static GLuint vao,vao1[50];
int tex_count[50];





  static const EGLint configAttribs[] = {
          EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
          EGL_BLUE_SIZE, 8,
          EGL_GREEN_SIZE, 8,
          EGL_RED_SIZE, 8,
          EGL_DEPTH_SIZE, 8,
          EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
          EGL_NONE
  };    

  static const int pbufferWidth = 1024;
  static const int pbufferHeight = 1024;

  static const EGLint pbufferAttribs[] = {
        EGL_WIDTH, pbufferWidth,
        EGL_HEIGHT, pbufferHeight,
        EGL_NONE,
  };



void errorout(std::string error_str){
	std::cout<<error_str<<std::endl;
}

std::vector<std::string> breakup_into_lines(std::string input,std::string delimiter){
		std::vector<std::string> output;
	
		int pos=-1;
		
		while( (pos = input.find(delimiter))!=-1){
	
			std::string str=input.substr(0, pos);
			
			input.erase(0, pos + delimiter.length());
		
		    output.push_back(str);
		}
		output.push_back(input);
		
		
		
		return output;
}



mat4 model = mat4(1.0);

static const GLfloat vertex_data[] = {
  
  //Y
  
  1.0, -1.0, -1.0, 0.0, -1.0, 0.0, 1.0,-1.0,
    1.0, -1.0, 1.0, 0.0, -1.0, 0.0,  1.0, 1.0,
  -1.0, -1.0, 1.0, 0.0, -1.0, 0.0,-1.0, 1.0,
  
  
    -1.0, -1.0, 1.0, 0.0, -1.0, 0.0,-1.0, 1.0, 
  -1.0, -1.0,-1.0, 0.0, -1.0, 0.0,-1.0,-1.0,
  1.0, -1.0, -1.0, 0.0, -1.0, 0.0, 1.0,-1.0,
  

  -1.0, 1.0, 1.0, 0.0, 1.0, 0.0, -1.0,1.0,
   1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,1.0,
   1.0, 1.0, -1.0, 0.0, 1.0, 0.0, 1.0,-1.0,
   
1.0, 1.0, -1.0, 0.0, 1.0, 0.0, 1.0,-1.0,
  -1.0, 1.0,-1.0, 0.0, 1.0, 0.0, -1.0,-1.0,
-1.0, 1.0, 1.0, 0.0, 1.0, 0.0, -1.0,1.0,  





 //X 
  -1.0, 1.0, -1.0, -1.0, 0.0, 0.0, -1.0,1.0,
  -1.0, -1.0,-1.0, -1.0, 0.0, 0.0, -1.0,-1.0,
  -1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 1.0,-1.0,//cw
  
  
-1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 1.0,-1.0,
  -1.0, 1.0, 1.0, -1.0, 0.0, 0.0, 1.0,1.0,
-1.0, 1.0, -1.0, -1.0, 0.0, 0.0, -1.0,1.0,//cw
  
  1.0, -1.0, 1.0,1.0, 0.0, 0.0, 1.0,-1.0,
  1.0, -1.0,-1.0,1.0, 0.0, 0.0, -1.0,-1.0,
  1.0, 1.0, -1.0,1.0, 0.0, 0.0,  -1.0,1.0,//ccw
  
      
  1.0, 1.0,-1.0 ,1.0, 0.0, 0.0, -1.0,1.0,
  1.0,  1.0, 1.0 ,1.0, 0.0, 0.0, 1.0,1.0,//ccw
 1.0, -1.0, 1.0 ,1.0, 0.0, 0.0,1.0,-1.0,

//Z
  
  
  
  -1.0, 1.0, 1.0, 0.0, 0.0, 1.0, -1.0,1.0,
  -1.0, -1.0,1.0, 0.0, 0.0, 1.0, -1.0,-1.0,
  1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0,-1.0,

  
  
  1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0,-1.0,
  1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0,1.0,
  -1.0, 1.0, 1.0, 0.0, 0.0, 1.0, -1.0,1.0,
  
  
  
  -1.0, 1.0, -1.0, 0.0, 0.0, -1.0, -1.0, 1.0,
  1.0,  1.0, -1.0, 0.0, 0.0, -1.0,  1.0, 1.0,
  1.0, -1.0, -1.0, 0.0, 0.0, -1.0,  1.0,-1.0,
  
  
  1.0, -1.0, -1.0, 0.0, 0.0, -1.0,   1.0,-1.0,
  -1.0, -1.0, -1.0, 0.0, 0.0, -1.0, -1.0,-1.0,
  -1.0, 1.0, -1.0, 0.0, 0.0, -1.0,  -1.0,1.0
};




int generateTextureFromBuf(unsigned char *buf)
{
	GLuint mTexture;
    glGenTextures(1 , &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);// Bind our 2D texture so that following set up will be applied

    //Set texture wrapping parameter
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    //Set texture Filtering parameter
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    //Generate the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , 128 , 128, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    
    
    
    glBindTexture(GL_TEXTURE_2D,0); //Unbind 2D textures
    
    return mTexture;

}


int generateTexture(const char * filename)
{
	GLuint mTexture;
    glGenTextures(1 , &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);// Bind our 2D texture so that following set up will be applied

    //Set texture wrapping parameter
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    //Set texture Filtering parameter
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //Load the image
    int picWidth,picHeight,n;
    unsigned char* image = stbi_load(filename, &picWidth, &picHeight, &n,STBI_rgb_alpha);
    if (image == NULL ) {
		std::stringstream ss;
        ss<<"Failed to load image: "<<stbi_failure_reason()<<std::endl;
        errorout(ss.str());
    }
    //Generate the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , picWidth , picHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    //texture_data_list.push_back(image);
    
   // std::cout<<"Image START#####"<<picWidth<<":"<<picHeight<<":"<<picWidth*picHeight*3<<"#####################"<<std::endl;
   // for(int i=0;i<picWidth*picHeight*3;i++)std::cout<<(unsigned int)image[i]<<",";
   // std::cout<<"Image END##########################"<<std::endl;

    stbi_image_free(image);// Free the reference to the image
    glBindTexture(GL_TEXTURE_2D,0); //Unbind 2D textures
    
    return mTexture;

}



std::vector<std::string> globVector(const std::string& pattern){
	try{
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));
    
     int return_value,count=0;
    while((return_value=glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result))!=0){
		 
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if(count++>30){
			throw(1);
			
		}
	}
    
    
    std::vector<std::string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
    
}catch(...){
	errorout("glob error");
	std::vector<std::string> out;
	return out;
}
}


void   setup(){
  
    
    errorout("setup");

    
   

	srand(time(NULL));
    //std::cout << std::fixed << std::showpoint;
	std::cout.precision(2);
	
    grammar=new Grammar(grammar_filename.c_str());
    
    
    
		grammar->addContext();
	
	
	errorout("Read files...");
	
	

	std::vector<std::string> files = globVector("./textures/*.png");
	texture_filenames=files;
	std::stringstream ss;
	
	ss<<"files: "<<files.size()<<std::endl;
	errorout(ss.str());

	
	
	
	int texid;
	
	int counter=0;
    for (int i=0;i<files.size();i++){
		//std::cout<<counter;
		counter++;
		texid=generateTexture(files[i].c_str());
		texture_list.push_back(texid);
		texture_list_alpha.push_back(1.0);
		 
	img_counter++;
	    
	
	}
	/*std::vector<std::string> files2 = globVector("./*.jpg");
	
	
	std::cout<<"files: "<<files2.size()<<std::endl;
	
	
    for (int i=0;i<files2.size();i++){
		std::cout<<counter;
		counter++;
		GLuint texid=generateTexture(files2[i].c_str());
		texture_list.push_back(texid);
	
	}*/
	
	for(int i=0;i<texture_list.size();i++){
					grammar->context->loadTexture(texture_list[i]);
			
		}
		
	//back_texture=load_texture("paper.png");	
		errorout("generate primitives");
		grammar->context->genPrimitives();
		
		grammar->generateGeometry();
		errorout("Finished generating geometry...");


gtk_label_set_text((GtkLabel *)file_label,(grammar_filename+"    tokens: "+std::to_string(grammar->tokens_new.size())).c_str());

init_buffer=false;
txt_changed(mybuffer,NULL);

//grammar->context->getScene().calc_normals();
 float mydata[36*8];
    for(int i=0;i<36*8;i++){
		mydata[i]=vertex_data[i]; 
	}
	
	
	for(int i=0;i<36;i++){
		for(int j=0;j<8;j++){
			if(j==0 || j==2)mydata[i*8+j]*=0.5f;
			if(j==1){
				if(mydata[i*8+j]<0)mydata[i*8+j]=0.0f;
			}
		}
	}
	
	std::stringstream ss;
	
	ss<<"texture list size"<<texture_list.size()<<std::endl;
	errorout(ss.str());
	
	for(int i=0;i<texture_list.size();i++){
		
		float *my_vertex_data=grammar->context->calc(mydata,i);
		//tex_count[i]=texcount;
		
		  glGenVertexArrays (1, &vao1[i]);
		  glBindVertexArray (vao1[i]);

		  /* This is the buffer that holds the vertices */
		  glGenBuffers (1, &position_buffer1[i]);
		  glBindBuffer (GL_ARRAY_BUFFER, position_buffer1[i]);
		  glBufferData(GL_ARRAY_BUFFER,sizeof(float)*36*8*tex_count[i],my_vertex_data,GL_DYNAMIC_DRAW);
		  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		  glEnableVertexAttribArray (0);
		  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		  glEnableVertexAttribArray (1);
		   // texture coord attribute
		  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		  glEnableVertexAttribArray(2);
		  glBindBuffer (GL_ARRAY_BUFFER, 0);
		  delete my_vertex_data;
	  

	}



}
  
  



/* Create and compile a shader */
static GLuint
create_shader (int  type)
{
  GLuint shader;
  int status;
  shader = glCreateShader (type);
  if (type== GL_FRAGMENT_SHADER){
    glShaderSource (shader, 1, &FRAGMENT_SOURCE2, NULL);
  }
  if (type== GL_VERTEX_SHADER){
	  //std::cout<<VERTEX_SOURCE<<std::endl;
    glShaderSource (shader, 1, &VERTEX_SOURCE, NULL);
  }
  glCompileShader (shader);

  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
  {
    int log_len;
    char *buffer;
    glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_len);
    buffer = (char*)g_malloc (log_len + 1);
    glGetShaderInfoLog (shader, log_len, NULL, buffer);
    g_warning ("Compile failure in %s shader:\n%s",
               type == GL_VERTEX_SHADER ? "vertex" : "fragment",
               buffer);
    g_free (buffer);
    glDeleteShader (shader);
    return 0;
  }

  return shader;
}

void realize ()

  setup();
     
      std::string text="";
  for(int i=0;i<grammar->lines.size();i++)text+=grammar->lines[i]+"\n";
  

  gtk_text_buffer_set_text (mybuffer,text.c_str(),text.length());
     
    float mydata[36*8];
    for(int i=0;i<36*8;i++){
		mydata[i]=vertex_data[i]; 
	}
	
	
	for(int i=0;i<36;i++){
		for(int j=0;j<8;j++){
			if(j==0 || j==2)mydata[i*8+j]*=0.5f;
			if(j==1){
				if(mydata[i*8+j]<0)mydata[i*8+j]=0.0f;
			}
		}
	}
	
	
   
   /* We only use one VAO, so we always keep it bound */
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);

  /* This is the buffer that holds the vertices */
  glGenBuffers (1, &position_buffer);
  glBindBuffer (GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(float)*36*8,mydata,GL_STATIC_DRAW);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray (0);
  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray (1);
   // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindBuffer (GL_ARRAY_BUFFER, 0);

  GLuint vertex, fragment;
  int status;
  vertex = create_shader (GL_VERTEX_SHADER);

  if (vertex == 0)
    {
      return;
    }

  fragment = create_shader (GL_FRAGMENT_SHADER);

  if (fragment == 0)
    {
      glDeleteShader (vertex);
      return;
    }

  program = glCreateProgram ();
  glAttachShader (program, vertex);
  glAttachShader (program, fragment);

  glLinkProgram (program);

  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    int log_len;
    char *buffer;

    glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_len);

    buffer = (char*)g_malloc (log_len + 1);
    glGetProgramInfoLog (program, log_len, NULL, buffer);

    g_warning ("Linking failure:\n%s", buffer);

    g_free (buffer);

    glDeleteProgram (program);
    program = 0;

    glDeleteShader (vertex);
    glDeleteShader (fragment);

    return;
  }

  glDetachShader (program, vertex);
  glDetachShader (program, fragment);

glEnable(GL_DEPTH_TEST);
//glClearDepth(1.0);
glDepthFunc(GL_LESS);
//glDisable(GL_CULL_FACE);
//glCullFace(GL_BACK_AND_FRONT);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);



  
  last_frame = g_date_time_new_now_local();  
current_frame = g_date_time_get_microsecond(last_frame);
  
}
/* We should tear down the state when unrealizing */
 void unrealize ()
{
 
	for(int i=0;i<texture_list.size();i++){
		glDeleteBuffers (1, &position_buffer1[i]);
	}
  glDeleteBuffers (1, &position_buffer);
  glDeleteProgram (program);
}








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//                      BUFFER


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_buffer(float angle_cube,vec3 scale_vec,vec3 position_vec,vec3 pos,int tex_index,float texscale)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_list[tex_index]);
  /* Use our shaders */
  glUseProgram (program);
  glUniform1i(glGetUniformLocation(program, "texture1"), 0);
 
 if(angle_cube==0){
	 glDepthFunc(GL_LESS);
 }
 else {
	 glDepthFunc(GL_GREATER);
 }
 
 position_vec=vec3(position_vec.x*scale_global,position_vec.y*scale_global,position_vec.z*scale_global);
 
  model = glm::mat4(1.0);
  //model = rotate(model, angle_cube, vec3(0,1,0));
  //model = translate(model,position_vec);
  vec3 scale_vec2=vec3(scale_vec.x*scale_global,scale_vec.y*scale_global,scale_vec.z*scale_global);
  model = scale(model,scale_vec2);
  
  
  scale_vec=vec3(scale_vec.x*texscale,scale_vec.y*texscale,scale_vec.z*texscale);
  
  
  //vec3 pos(0,0,0);
  
  //model2 = rotate(model2, angle_view, vec3(0,1,0));
  //pos2=model2*pos2;
  //pos=vec3(pos2.x,pos2.y,pos2.z);
  
  
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
  
  
 
  
  vec3 position = vec3(5.0*cos(angle_view*M_PI/180.0),5.0*cos(elevation_view*M_PI/180.0),5.0*sin(angle_view*M_PI/180.0));
  
  vec3 up = vec3(0,-1,0);
  mat4 view = lookAt(position, vec3(0.0,0.0,0.0), up);
  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
  
  mat4 projection = perspective(43.0, double(GLAREA_WIDTH)/double(GLAREA_HEIGHT), 0.01, 100.0);
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);
  
  vec3 lightposition=vec3(1.1*scale_global,-60.7*scale_global,-1.3*scale_global);
  glUniform3fv(glGetUniformLocation(program,"lightposition"),1,&lightposition[0]);
  
  //pos=position_vec;
  //glUniform3fv(glGetUniformLocation(program,"pos"),1,&pos[0]);
  //glUniform3fv(glGetUniformLocation(program,"scale_vec"),1,&scale_vec[0]);  
  
  glm::vec4 ambientproduct=glm::vec4(0.8,0.8,0.8,1.0);
  glUniform4fv(glGetUniformLocation(program,"ambientproduct"),1,&ambientproduct[0]);
  
  glm::vec4 diffuseproduct=glm::vec4(0.5,0.5,0.5,1.0);
  glUniform4fv(glGetUniformLocation(program,"diffuseproduct"),1,&diffuseproduct[0]);
  
  glm::vec4 specularproduct=glm::vec4(0.1,0.1,0.1,1.0);
  glUniform4fv(glGetUniformLocation(program,"specularproduct"),1,&specularproduct[0]);
  float shinyness=16.0f;
  glUniform1f(glGetUniformLocation(program,"shinyness"),shinyness);
  glUniform1f(glGetUniformLocation(program,"alpha"),texture_list_alpha[tex_index]);
  
  
  glBindVertexArray(vao1[tex_index]);
  /* Use the vertices in our buffer */

  /* Draw the three vertices as a triangle */
  glDrawArrays (GL_TRIANGLES, 0, 36*tex_count[tex_index]);

  /* We finished using the buffers and program */
  glBindVertexArray(0);
  glDisableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glUseProgram (0);
}






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//              RENDER


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void render ()
{




  
  
  /* Clear the viewport */
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* Draw our object */
  //draw_box (angle_view,vec3(0.5,1.0,0.5),vec3(0.0,0.0,0.0),0);
  //draw_box (delta_time,vec3(1.0,0.1,1.0),vec3(0.0,0.0,0.6),1);
  
  //grammar->context->draw();
  for(int i=0;i<texture_list.size();i++){
		if(tex_count[i]>0)draw_buffer(0,vec3(1,1,1),vec3(0,0,0),vec3(0,0,0),i,1);
	}
  

}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//             MAIN



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
unsigned char SafeUCharCast(T input);

template<>
unsigned char SafeUCharCast<float>(float input)
{  
    return static_cast<unsigned char>(std::clamp<float>(input*255, 0.f, 255.f));
}

template<>
unsigned char SafeUCharCast<unsigned char>(unsigned char input)
{
    return input;
}

template<typename T>
void SaveRGBAImageToDisk(std::string fileName, size_t width, size_t height, const T* dataRaw)
{
    constexpr size_t inputDepth = 3;
    dlib::array2d<dlib::rgb_alpha_pixel> img(height, width);

    int idx = 0;
    for(auto& pix: img)
    {
        pix.red = SafeUCharCast(dataRaw[idx]);
        pix.green = SafeUCharCast(dataRaw[idx+1]);
        pix.blue = SafeUCharCast(dataRaw[idx+2]);
        pix.alpha = SafeUCharCast(dataRaw[idx+3]);
        idx += inputDepth;
    }

    dlib::save_png(img, fileName+".png");
}





int main( int argc, char* argv[] ){
	
	

EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  EGLint major, minor;

  eglInitialize(eglDpy, &major, &minor);

  // 2. Select an appropriate configuration
  EGLint numConfigs;
  EGLConfig eglCfg;

  eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);

  // 3. Create a surface
  EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, 
                                               pbufferAttribs);

  // 4. Bind the API
  eglBindAPI(EGL_OPENGL_API);

  // 5. Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, 
                                       NULL);

  eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);

  // from now on use your OpenGL context



 realize();
   GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    
    /*
     * Create a texture as color attachment.
     */
      auto CreateTexture = [](GLuint& tex, int width, int height, void* data) {
        glGenTextures(1, &tex);
        
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    };
    GLuint outputTex;
    CreateTexture(outputTex, 1024, 1024, nullptr);
    glBindTexture(GL_TEXTURE_2D, outputTex);
    /*
     * Attach the texture to the framebuffer.
     */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTex, 0);
    
 
 render();
   std::vector<unsigned char> buf(1024*1024*4);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    

    SaveRGBAImageToDisk("output", 1024,1024, buf.data());
 
 
 unrealize();


  // 6. Terminate EGL when finished
  eglTerminate(eglDpy);


  return status;
  
	
}

































