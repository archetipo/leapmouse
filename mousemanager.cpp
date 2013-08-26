

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mousemanager.h"

MouseManager::MouseManager()
{

	this->dpy= XOpenDisplay(NULL);

    xScreen = DefaultScreenOfDisplay( dpy );
    w_width=xScreen->width;
	w_height=xScreen->height;
	//~ XCloseDisplay(this->dpy);
	//XGetNormalHints(dpy,root,wsize);
	//w_width=wsize->width;
	}
void MouseManager::move(int x ,int y )
{
	//this->dpy= XOpenDisplay(NULL);
	XTestFakeMotionEvent(this->dpy, -1, x, y, 0);
	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);
	}

void MouseManager::rightPress()
{
	//~ this->dpy= XOpenDisplay(NULL);
	XTestFakeButtonEvent(this->dpy, Button3, true, 0);
	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);

	}
void MouseManager::rightRelease()
{
	//~ this->dpy= XOpenDisplay(NULL);
	XTestFakeButtonEvent(this->dpy, Button3, false, 0);
	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);
	}

void MouseManager::leftPress()
{
	//~ this->dpy= XOpenDisplay(NULL);
	XTestFakeButtonEvent(this->dpy, Button1, true, 0);
	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);

	}
void MouseManager::leftRelease()
{
	//~ this->dpy= XOpenDisplay(NULL);
	XTestFakeButtonEvent(this->dpy, Button1, false, 0);
	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);
	}
void MouseManager::wheelUp()
{
	//~ this->dpy= XOpenDisplay(NULL);
	XTestFakeButtonEvent(this->dpy, Button4, true, 0);
	XTestFakeButtonEvent(this->dpy, Button4, false, 0);
	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);
	}
void MouseManager::wheelDown()
{
	//~ this->dpy= XOpenDisplay(NULL);
	XTestFakeButtonEvent(this->dpy, Button5, true, 0);
	XTestFakeButtonEvent(this->dpy, Button5, false, 0);

	XFlush(this->dpy);
	//~ XCloseDisplay(this->dpy);
	}

void MouseManager::close()
{

	XCloseDisplay(this->dpy);

	}
