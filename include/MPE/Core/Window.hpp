#ifndef MPE_WINDOW_HPP
#define MPE_WINDOW_HPP

#include <string>

struct GLFWwindow;

namespace mpe
{
	//Window for realtime graphics
	class Window
	{
	public:
		//Initializes a window making it the current graphics context
		Window(const std::string& title, int width, int height);
		//Calls Window::Destroy()
		//Either uninitialize the graphics API before destruction
		//or call Window::SetAsGraphicsContext() on another Window!
		~Window();
		//Let graphics API work on this window
		void SetAsGraphicsContext();
		//Returns true if the window was correctly initialized and not destroyed
		bool IsValid() const;
		//Destroys the window if it's a valid one.
		//Either uninitialize the graphics API before calling Window::Destroy()
		//or call Window::SetAsGraphicsContext() on another Window!
		void Destroy();
		//Returns the width of the window
		int Width() const;
		//Returns the height of the window
		int Height() const;
		//Must be called to draw the current frame
		void Draw();
		//Returns true if the window is going to close
		bool ShouldClose() const;
		//Returns the underlying implementation
		GLFWwindow* GetNativeWindow();
		//Returns the underlying implementation
		const GLFWwindow* GetNativeWindow() const;
		//Sets the background default color
		void SetClearColor(float r, float g, float b);
		//Not available yet
		Window(const Window&) = delete;
		//Not available yet
		Window(Window&&) = delete;
		//Not available yet
		Window& operator =(const Window&) = delete;
		//Not available yet
		Window& operator =(Window&&) = delete;
		//Returns the Window set as graphics context
		static Window* GetActive();
	private:
		GLFWwindow* win;
		int w;
		int h;
		static int initialized_windows;
	};
}

#endif