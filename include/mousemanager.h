
#ifndef __MOUSEMANAGER_H__
#define __MOUSEMANAGER_H__

#include <X11/Xlib.h>


#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>


class MouseManager
{
	public:
		MouseManager();
		virtual ~MouseManager(){};
		void move(int x,int y);
		void rightPress();
		void rightRelease();
		void leftPress();
		void leftRelease();
		void wheelUp(); //4
		void wheelDown();  //5
		void close();
		Screen* xScreen;
		int w_width;
		int w_height;
	private:
		Display *dpy;
		Window root;

};



#endif
