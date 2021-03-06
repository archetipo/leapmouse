#include <iostream>

#include <time.h>

#include "Leap.h"
#include "mousemanager.h"

#ifndef ACTIVETIMEOUT
#define ACTIVETIMEOUT 300 // wait before auto-deactive (seconds)
#endif

#ifndef TOGGLE_FRAME_LIMIT
#define TOGGLE_FRAME_LIMIT 10 // frames to wait after (de-)activation before allowing another
#endif

using namespace Leap;

class SampleListener : public Listener
{
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    MouseManager* Mouse;
    int mystate;
    int clickcount;
    int pressedDelay; //adjust move object when buttin is pressed
    int pressstate;
    int side;
    int sensibility;  // value for sensibility click incrise value decrise  sensibility
    int rclick;
    bool active; // if we're currently active

    int64_t lastFrameID; // last frame processed
    int64_t lastToggle; // frame id of the last frame we toggled activity
    time_t lastEvent; // epoch time in seconds of last event

};

void SampleListener::onInit(const Controller& controller)
{
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller)
{
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);

  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller)
{
  //Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller)
{
  Mouse->close();
  delete Mouse;
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller)
{
  // This function needs to be broken up, it's way too long
  const Frame frame = controller.frame();
  //int currid = 1;

  //check if we already processed this frame
  if (frame.id() == this->lastFrameID)
    return;
  this->lastFrameID = frame.id();

  //stop activity if we've not been used for ACTIVETIMEOUT
  if ((ACTIVETIMEOUT + this->lastEvent) < time(0) && this->active)
  {
    std::cout << "auto deactivated" << std::endl;
    this->active = false;
  }

  if (frame.fingers().count() == 2)
  {
    GestureList gestures = frame.gestures();
    for (int it = 0; it < gestures.count(); ++it)
    {
      if (gestures[it].type() != Gesture::TYPE_KEY_TAP)
        continue;
      if ((this->lastToggle + TOGGLE_FRAME_LIMIT) > frame.id()) //avoid detecting the same tap twice
        continue;
      KeyTapGesture gesture = gestures[it];
      Vector v = gesture.direction();
      this->active = !this->active;
      this->lastToggle = frame.id();
      this->lastEvent = time(0);
      std::cout << (this->active ? "activated" : "deactivated") << std::endl;
      break; //make sure we don't accidentally use the same list twice
    }
  }

  if (!this->active)
    return;

  if (frame.fingers().count() == 1 & side != 2)
  {
    PointableList pointables = frame.pointables();
    InteractionBox iBox = frame.interactionBox();
    for (int p = 0; p < pointables.count(); ++p)
    {
      Pointable pointable = pointables[p];
      Vector normalizedPosition = iBox.normalizePoint(pointable.stabilizedTipPosition());
      float distance = pointable.touchDistance();
      float x = normalizedPosition.x * (Mouse->w_width+250);
      float y = (Mouse->w_height + 250) - normalizedPosition.y * (Mouse->w_height + 250);
      if (side == 0)
      {
        Mouse->move((int)x, (int)y);
        pressedDelay = 0;
      }
      if (distance < 0)
      {
        clickcount++;
        side = 1;
        if (clickcount > sensibility)
        {
          if (pressstate == 0)
          {
            Mouse->leftPress();
            pressstate = 1;
          }
          else
          {
            if (clickcount >= 30)
            {
              if (rclick == 0)
                Mouse->move((int)x,(int)y);
              std::cout << "move side: "<< side << std::endl;
              clickcount = 20;
            }
          }
        }
      }
      else if (side == 1)
      {
        Mouse->leftRelease();
        std::cout << "released side: " << side << std::endl;
        side = 0;
        pressstate = 0;
        pressedDelay = 0;
        clickcount = 0;
        rclick = 0;
      }
    }
    this->lastEvent = time(0);
  }
  else if (frame.fingers().count() > 1 & side != 1)
  {
    clickcount = 0;
    std::cout << "active side: " << side << std::endl;
    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g)
    {
      Gesture gesture = gestures[g];
        switch (gesture.type())
        {
          case Gesture::TYPE_CIRCLE :
          {
            CircleGesture circle = gesture;
            if (gesture.state() == 3 & circle.progress() > 1)
            {
              side = 0;
              rclick = 1;
              Mouse->rightPress();
              Mouse->rightRelease();
            }
            break;
          }
          case Gesture::TYPE_SWIPE :
          {
            side = 2;
            SwipeGesture swipe = gesture;
            Vector d = swipe.direction();
            //  std::cout << "Swipe id: " << d(1) << std::endl;
            if (d[1] > 0.8)
            {
              this->mystate = 1; //up
              // std::cout << "wheel up " <<std::endl;
            }
            else if (d[1] < -0.8)
            {
              this->mystate = 2; //down
              //std::cout << "wheel down " <<std::endl;
            }
            if (swipe.state() == 3)
            {
              if (this->mystate == 1)
              {
                side = 0;
                this->mystate = 0;
                Mouse->wheelUp();
              }
              else if (this->mystate == 2)
              {
                side = 0;
                this->mystate = 0;
                Mouse->wheelDown();
              }
            }
            break;
          }

          default:
          //std::cout << "Unknown gesture type." << std::endl;
          break;
        }
    }
    this->lastEvent = time(0);
  }
}

void SampleListener::onFocusGained(const Controller& controller)
{
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller)
{
  std::cout << "Focus Lost" << std::endl;
}

int main()
{
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  listener.Mouse = new MouseManager();
  listener.mystate = 0;
  listener.clickcount = 0;
  listener.pressstate = 0;
  listener.side = 0;
  listener.pressedDelay = 0;
  listener.rclick = 0;
  listener.sensibility = 8;
  listener.active = false;

  listener.lastFrameID = -1;
  listener.lastToggle = 0;
  listener.lastEvent = time(0);

  std::cout << "width: "  << listener.Mouse->w_width  << std::endl;
  std::cout << "height: " << listener.Mouse->w_height << std::endl;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
