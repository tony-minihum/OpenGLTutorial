#define GLFW_DLL

#include <GL/glfw.h>
#include <cstdlib>

#if defined (_MSC_VER)
#pragma comment(lib, "GLFWDLL.lib")
#pragma comment(lib, "opengl32.lib")
#endif

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
			0.0f, 0.433f,
			-0.5f, -0.433f,
			0.5f, -0.433f,
		};
		glVertexPointer(2, GL_FLOAT, 0, vtx);	// glVertexPinter(size, type, stride, pointer);
												// size := Specifies the number of coordinates per vertex. Must be 2, 3, or 4. The initial value is 4.
		
		static const GLfloat color[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		glColorPointer(3, GL_FLOAT, 0, color);

		// Set mode
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_TRIANGLES, 0, 3);	// glDrawArrays(mode, first, count);

		// Reset mode
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glfwSwapBuffers();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

