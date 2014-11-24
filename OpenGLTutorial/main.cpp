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
		GL_TEXTURE_WRAP_T, GL_CLAMP);

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
	if (!glfwOpenWindow(640, 480, // width, height
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
	
	if (!setupTexture(texture_id, "sample.raw",256, 256)) {
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

	GLint current_framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_framebuffer);

	// Generate texture for offscreen buffer.
	GLuint fbo_texture_id;
	glGenTextures(1, &fbo_texture_id);

	glBindTexture(GL_TEXTURE_2D, fbo_texture_id);

	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGB, 256, 256,
		0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Setup framebuffer.
	GLuint framebuffer_id;
	glGenFramebuffers(1, &framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		fbo_texture_id, 0);

	float angle = 0.0f;
	// Loop while window opened.
	while (glfwGetWindowParam(GLFW_OPENED)) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

		// Setup Viewport.
		glViewport(0, 0, 256, 256);

		// Set clear color, and then, clear buffer.
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		angle += 0.02f;
		const float x_ofs = std::sin(angle) * 0.5f;

		// Set vertex array.
		const GLfloat vtx[] = {
			-0.5f + x_ofs, -0.5f,
			0.5f + x_ofs, -0.5f,
			0.5f + x_ofs, 0.5f,
			-0.5f + x_ofs, 0.5f,
		};
		glVertexPointer(2, GL_FLOAT, 0, vtx);	// glVertexPinter(size, type, stride, pointer);
											// size := Specifies the number of coordinates per vertex. Must be 2, 3, or 4. The initial value is 4.
		
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);	// glDrawArrays(mode, first, count);

		// Reset mode
		glDisableClientState(GL_VERTEX_ARRAY);

		///
		glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);

		// Setup Viewport.
		glViewport(0, 0, 640, 480);

		// Set clear color, and then, clear buffer.
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (int y = 0; y < 3; ++y) {
			for (int x = 0; x < 3; ++x) {
				float x_ofs = x * 0.55f - 0.8f;
				float y_ofs = y * 0.55f - 0.8f;
				const GLfloat vtx2[] = {
					0.0f + x_ofs, 0.0f + y_ofs,
					0.5f + x_ofs, 0.0f + y_ofs,
					0.5f + x_ofs, 0.5f + y_ofs,
					0.0f + x_ofs, 0.5f + y_ofs,
				};
				glVertexPointer(2, GL_FLOAT, 0, vtx2);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				static const GLfloat texture_uv[] = {
					0.0f, 0.0f,
					1.0f, 0.0f,
					1.0f, 1.0f,
					0.0f, 1.0f,
				};
				glTexCoordPointer(2, GL_FLOAT, 0, texture_uv);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, fbo_texture_id);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glDrawArrays(GL_QUADS, 0, 4);	// glDrawArrays(mode, first, count);

				// Reset mode
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				glDisable(GL_TEXTURE_2D);
			}
		}

		glfwSwapBuffers();
	}
	
	glDeleteTextures(1, &texture_id);
	glDeleteTextures(1, &fbo_texture_id);
	glDeleteFramebuffers(1, &framebuffer_id);

	glfwTerminate();

	return EXIT_SUCCESS;
}

