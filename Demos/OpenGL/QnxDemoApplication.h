#ifndef QNX_DEMO_APPLICATION_H
#define QNX_DEMO_APPLICATION_H

#include "DemoApplication.h"

#include <gestures/set.h>
#include <screen/screen.h>

class QnxDemoApplication : public DemoApplication
{
public:

    QnxDemoApplication();
    virtual ~QnxDemoApplication();

    void getAccelerometerPitchAndRoll(float* pitch, float* roll);
    void run();
    void setInputHandledAsGesture(bool value) { _inputHandledAsGesture = value; }
    void swapBuffers();
    void updateCameraPosition(int dx, int dy);
    void updateCameraZoom(int delta);
	void updateModifierKeys() {};

	// Defined to be compliant with GLUT code.
	void glutSwapBuffers();

private:
	void initDisplay();
	void initGestures();
    void handleKeyboard(int key, bool isPressed);

	long _timeStart;
	long _timeAbsolute;
	screen_context_t _screenContext;
	screen_window_t _screenWindow;
	screen_event_t _screenEvent;
	int _screenWindowSize[2];
	EGLDisplay _eglDisplay;
	EGLContext _eglContext;
	EGLSurface _eglSurface;
	EGLConfig _eglConfig;
	gestures_set* _set;
	bool _inputHandledAsGesture;
	int _orientationAngle;
};

#endif // QNX_DEMO_APPLICATION_H
