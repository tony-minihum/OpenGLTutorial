#define GLFW_DLL

#define _USE_MATH_DEFINES

#include <GL/glfw.h>
#include <cstdlib>
#include <cmath>

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
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set vertex array.
		GLfloat vtx[20 * 2];

		for (int i = 0; i < 20; ++i) {
			GLfloat angle = static_cast<GLfloat>((M_PI * 2.0 * i) / 20);
			vtx[i * 2] = std::sin(angle) * 0.5f;
			vtx[i * 2 + 1] = std::cos(angle) * 0.6f;
		}
		glVertexPointer(2, GL_FLOAT, 0, vtx);	// glVertexPinter(size, type, stride, pointer);
												// size := Specifies the number of coordinates per vertex. Must be 2, 3, or 4. The initial value is 4.
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		// Set mode
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 20);	// glDrawArrays(mode, first, count);

		// Reset mode
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

