#include "Framework.h"


namespace Framework
{
	GLuint CreateShader(GLenum eShaderType,
		const std::string &strShaderFile, const std::string &strShaderName)
	{   
		GLuint shader = glCreateShader(eShaderType);
		const char *strFileData = strShaderFile.c_str();
		glShaderSource(shader, 1, &strFileData, NULL);

		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

			const char *strShaderType = NULL;
			switch(eShaderType)
			{
			    case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			    case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			    case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
			}

			fprintf(stderr, "Compile failure in %s shader named \"%s\". Error:\n%s\n",
				strShaderType, strShaderName.c_str(), strInfoLog);
			delete[] strInfoLog;
		}

		return shader;
	}

	GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
	{
		std::string strFilename = strShaderFilename;
		std::ifstream shaderFile(strFilename.c_str());
		std::stringstream shaderData;
        shaderData << SHADER_VERSION_STRING;
		shaderData << shaderFile.rdbuf();
		shaderFile.close();
		

		return CreateShader(eShaderType, shaderData.str(), strShaderFilename);
	}
	
	GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	return program;
}
    void multiplyMatrix4x4(float* A, float* B, float* C){
        
        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                float v = 0;
                for(int k = 0; k<4; k++){
                    v += A[i+4*k]*B[k + 4*j];
                }
                
                C[i + 4*j] = v;
                
            }
        }
        
        
    }
    
    float* multiplyMatrixVector4(float* A, float* b){
        float* result = new float[4];
        for(int i = 0; i<4; i++){
            float s = 0;
            for(int j = 0; j<4; j++){
                s += A[4*j + i]*b[j];
            }
            result[i] = s;
        }
        
        return result;
    }
    
    


#ifndef __APPLE__
GLuint initializeWindow(){
    
    if( glfwInit() != GL_TRUE ) {
        exit( EXIT_FAILURE );
    }
    printf("glfw initted\n");
    
    // Open an OpenGL window
    if( !glfwOpenWindow( 1024,800, 0,0,0,0,0,32, GLFW_WINDOW ) )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    glewInit();
    
    glfwSetWindowTitle("OpenGL plotting window");
    
    printf("GLSL version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("GL Version: %s", glGetString(GL_VERSION));
    
    glLoadIdentity();
    
    glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    
    glClearColor( 0.02f, 0.02f, 0.02f, 0.0f );
    glClearDepth( 1.0f );
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    printf("setup\n");
    //Load program.
    std::vector<GLuint> shaderList;
	
	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "vertex.shader"));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "fragment.shader"));
    GetError();
	return Framework::CreateProgram(shaderList);
    
}
#else

GLuint initializeWindow(){
    if( glfwInit() != GL_TRUE ) {
        exit( EXIT_FAILURE );
    }
    
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    printf("to here.\n");
    
    
    // Open an OpenGL window
    if( glfwOpenWindow(600, 400, 5, 6, 5,0, 0, 0, GLFW_WINDOW) != GL_TRUE )
    {
        printf("failed to open window.\n");
        GetError();
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    glfwSetWindowTitle("OpenGL plotting window");
    
    printf("GLSL version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("GL Version: %s", glGetString(GL_VERSION));
    
    glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    
    glClearColor( 0.02f, 0.02f, 0.02f, 0.0f );
    glClearDepth( 1.0f );
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    printf("setup\n");
    //Load program.
    std::vector<GLuint> shaderList;
	
	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "vertex.shader"));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "fragment.shader"));
    GetError();
	return Framework::CreateProgram(shaderList);
}

#endif

}
