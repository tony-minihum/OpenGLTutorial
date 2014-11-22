#define GLFW_DLL

#define GLEW_STATIC
#if defined (_MSC_VER)
#include <GL/glew.h>
#endif

#include <GL/glfw.h>
#include <cstdlib>

#include <fstream>
#include <vector>

#if defined (_MSC_VER)
#pragma comment(lib, "GLFWDLL.lib")
#pragma comment(lib, "opengl32.lib")
#ifdef _DEBUG
#pragma comment(lib, "glew32sd.lib")
#else
#pragma comment(lib, "glew32s.lib")
#endif
#endif

bool setupTexture(const GLuint id, const char* file, const int width, const int height)
{
	std::ifstream fstr(file, std::ios::binary);

	if (!fstr) return false;

	const size_t file_size = static_cast<size_t>(fstr.seekg(0, fstr.end).tellg());

	fstr.seekg(0, fstr.beg);

	std::vector<char> texture_buffer(file_size);

	fstr.read(&texture_buffer[0], file_size);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D,        // glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA, width, height,     //   0, pixFormat, width, height, (width and height must be a power of 2)
		0, GL_RGBA, GL_UNSIGNED_BYTE,  //   0, pixFormat, GL_UNSIGNED_BYTE,
		&texture_buffer[0]);           //   pointer)

	// Set filter for enlarging the texture.
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // GL_LINEAR, GL_NEAREST
	
	// Set filter for reducing size of the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Set texture wrap mode.
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat border_color[] = {
		1.0f, 0.0f, 0.0f, 1.0f
	};
	glTexParameterfv(GL_TEXTURE_2D,
		GL_TEXTURE_BORDER_COLOR, border_color);

	return true;
}

int main()
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	// Initialize OpenGL and create window
	if (!glfwOpenWindow(0, 0, // width, height
		0, 0, 0,              // number of R bit, G, B  // when set 0, default value is used.
		0,                    // number of A bit
		0, 0,                 // number of depth bit, stencil
		GLFW_WINDOW))
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	
	if (!setupTexture(texture_id, "sample2.raw", 32, 32)) {
		glDeleteTextures(1, &texture_id);
		glfwTerminate();
		return EXIT_FAILURE;
	}

#if defined (_MSC_VER)
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return EXIT_FAILURE;
	}
#endif

	// 
	glfwSwapInterval(1);

	// Loop while window opened.
	while (glfwGetWindowParam(GLFW_OPENED))
	{
		// Set clear color, and then, clear buffer.
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set vertex array.
		static const GLfloat vtx[] = {
			-0.8f, -0.8f,
			0.8f, -0.8f,
			0.8f, 0.8f,
			-0.8f, 0.8f,
		};
		glVertexPointer(2, GL_FLOAT, 0, vtx);	// glVertexPinter(size, type, stride, pointer);
											// size := Specifies the number of coordinates per vertex. Must be 2, 3, or 4. The initial value is 4.
		
		static const GLfloat texture_uv[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		glTexCoordPointer(2, GL_FLOAT, 0, texture_uv);

		// Set mode
		glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);	// glDrawArrays(mode, first, count);

		// Reset mode
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glfwSwapBuffers();
	}

	glDeleteTextures(1, &texture_id);

	glfwTerminate();

	return EXIT_SUCCESS;
}

