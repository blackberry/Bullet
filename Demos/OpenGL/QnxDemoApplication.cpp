#ifdef __QNX__

#include "QnxDemoApplication.h"

#include <bps/accelerometer.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/navigator.h>
#include <bps/orientation.h>
#include <bps/screen.h>
#include <gestures/double_tap.h>
#include <gestures/pinch.h>
#include <gestures/set.h>
#include <gestures/two_finger_pan.h>
#include <input/screen_helpers.h>
#include <screen/screen.h>
#include <sys/keycodes.h>
#include <time.h>

#include "btBulletDynamicsCommon.h"

struct timespec __timespec;

#define ZOOM_SCALE 0.2
#define MOVE_SCALE 1.0

static EGLenum checkErrorEGL(const char* msg)
{
    static const char* errmsg[] =
    {
        "EGL function succeeded",
        "EGL is not initialized, or could not be initialized, for the specified display",
        "EGL cannot access a requested resource",
        "EGL failed to allocate resources for the requested operation",
        "EGL fail to access an unrecognized attribute or attribute value was passed in an attribute list",
        "EGLConfig argument does not name a valid EGLConfig",
        "EGLContext argument does not name a valid EGLContext",
        "EGL current surface of the calling thread is no longer valid",
        "EGLDisplay argument does not name a valid EGLDisplay",
        "EGL arguments are inconsistent",
        "EGLNativePixmapType argument does not refer to a valid native pixmap",
        "EGLNativeWindowType argument does not refer to a valid native window",
        "EGL one or more argument values are invalid",
        "EGLSurface argument does not name a valid surface configured for rendering",
        "EGL power management event has occurred",
    };
    EGLenum error = eglGetError();
    fprintf(stderr, "%s: %s\n", msg, errmsg[error - EGL_SUCCESS]);
    return error;
}

// Convert the timespec into milliseconds.
static long timespec2millis(struct timespec *a)
{
    return a->tv_sec*1000 + a->tv_nsec/1000000;
}

static void handleGestures(gesture_base_t* gesture, mtouch_event* event, void* param, int async)
{
    QnxDemoApplication* app = (QnxDemoApplication*)param;
    app->setInputHandledAsGesture(true);

    switch (gesture->type)
    {
        case GESTURE_TWO_FINGER_PAN:
        {
            gesture_tfpan_t* tfpan = (gesture_tfpan_t*)gesture;
            if (!(tfpan->last_centroid.x == 0 && tfpan->last_centroid.y == 0))
                app->updateCameraPosition((float)(tfpan->centroid.x - tfpan->last_centroid.x), (float)(tfpan->centroid.y - tfpan->last_centroid.y));
            break;
        }
        case GESTURE_PINCH:
        {
            gesture_pinch_t* pinch = (gesture_pinch_t*)gesture;

            float distance = sqrt(pinch->distance.x*pinch->distance.x + pinch->distance.y*pinch->distance.y);
            float lastDistance = sqrt(pinch->last_distance.x*pinch->last_distance.x + pinch->last_distance.y*pinch->last_distance.y);

            if (distance != 0 && lastDistance != 0)
                app->updateCameraZoom(distance - lastDistance);
            break;
        }
        case GESTURE_DOUBLE_TAP:
        {
            gesture_double_tap_t* doubleTap = (gesture_double_tap_t*)gesture;

            app->shootBox(app->getRayTo(doubleTap->second_touch.x, doubleTap->second_touch.y));
            break;
        }
        default:
            app->setInputHandledAsGesture(false);
            break;
    }
}

static unsigned char keycodeToGLUTKey(int keycode)
{
    switch (keycode)
    {
        case KEYCODE_INSERT:
            return GLUT_KEY_INSERT;
        case KEYCODE_HOME:
            return GLUT_KEY_HOME;
        case KEYCODE_PG_UP:
            return GLUT_KEY_PAGE_UP;
        case KEYCODE_END:
            return GLUT_KEY_END;
        case KEYCODE_PG_DOWN:
            return GLUT_KEY_PAGE_DOWN;
        case KEYCODE_LEFT:
            return GLUT_KEY_LEFT;
        case KEYCODE_RIGHT:
            return GLUT_KEY_RIGHT;
        case KEYCODE_UP:
            return GLUT_KEY_UP;
        case KEYCODE_DOWN:
            return GLUT_KEY_DOWN;
        case KEYCODE_F1:
            return GLUT_KEY_F1;
        case KEYCODE_F2:
            return GLUT_KEY_F2;
        case KEYCODE_F3:
            return GLUT_KEY_F3;
        case KEYCODE_F4:
            return GLUT_KEY_F4;
        case KEYCODE_F5:
            return GLUT_KEY_F5;
        case KEYCODE_F6:
            return GLUT_KEY_F6;
        case KEYCODE_F7:
            return GLUT_KEY_F7;
        case KEYCODE_F8:
            return GLUT_KEY_F8;
        case KEYCODE_F9:
            return GLUT_KEY_F9;
        case KEYCODE_F10:
            return GLUT_KEY_F10;
        case KEYCODE_F11:
            return GLUT_KEY_F11;
        case KEYCODE_F12:
            return GLUT_KEY_F12;
        default:
            return 0;
    }
}

static unsigned char keycodeToUnsignedChar(int keycode)
{
    switch (keycode)
    {
        case KEYCODE_SPACE:
            return ' ';
        case KEYCODE_RIGHT_PAREN:
            return ')';
        case KEYCODE_ZERO:
            return '0';
        case KEYCODE_EXCLAM:
            return '!';
        case KEYCODE_ONE:
            return '1';
        case KEYCODE_AT:
            return '@';
        case KEYCODE_TWO:
            return '2';
        case KEYCODE_NUMBER:
            return '#';
        case KEYCODE_THREE:
            return '3';
        case KEYCODE_DOLLAR:
            return '$';
        case KEYCODE_FOUR:
            return '4';
        case KEYCODE_PERCENT:
            return '%';
        case KEYCODE_FIVE:
            return '5';
        case KEYCODE_CIRCUMFLEX:
            return '^';
        case KEYCODE_SIX:
            return '6';
        case KEYCODE_AMPERSAND:
            return '&';
        case KEYCODE_SEVEN:
            return '7';
        case KEYCODE_ASTERISK:
            return '@';
        case KEYCODE_EIGHT:
            return '8';
        case KEYCODE_LEFT_PAREN:
            return '(';
        case KEYCODE_NINE:
            return '9';
        case KEYCODE_EQUAL:
            return '=';
        case KEYCODE_PLUS:
            return '+';
        case KEYCODE_LESS_THAN:
            return '<';
        case KEYCODE_COMMA:
            return ',';
        case KEYCODE_UNDERSCORE:
            return '_';
        case KEYCODE_MINUS:
            return '-';
        case KEYCODE_GREATER_THAN:
            return '>';
        case KEYCODE_PERIOD:
            return '.';
        case KEYCODE_COLON:
            return ':';
        case KEYCODE_SEMICOLON:
            return ';';
        case KEYCODE_QUESTION:
            return '?';
        case KEYCODE_SLASH:
            return '/';
        case KEYCODE_GRAVE:
            return '`';
        case KEYCODE_TILDE:
            return '~';
        case KEYCODE_LEFT_BRACE:
            return '{';
        case KEYCODE_LEFT_BRACKET:
            return '[';
        case KEYCODE_BAR:
            return '|';
        case KEYCODE_BACK_SLASH:
            return '\\';
        case KEYCODE_RIGHT_BRACE:
            return '}';
        case KEYCODE_RIGHT_BRACKET:
            return ']';
        case KEYCODE_QUOTE:
            return '\"';
        case KEYCODE_APOSTROPHE:
            return '\'';
        case KEYCODE_CAPITAL_A:
            return 'A';
        case KEYCODE_A:
            return 'a';
        case KEYCODE_CAPITAL_B:
            return 'B';
        case KEYCODE_B:
            return 'b';
        case KEYCODE_CAPITAL_C:
            return 'C';
        case KEYCODE_C:
            return 'c';
        case KEYCODE_CAPITAL_D:
            return 'D';
        case KEYCODE_D:
            return 'd';
        case KEYCODE_CAPITAL_E:
            return 'E';
        case KEYCODE_E:
            return 'e';
        case KEYCODE_CAPITAL_F:
            return 'F';
        case KEYCODE_F:
            return 'f';
        case KEYCODE_CAPITAL_G:
            return 'G';
        case KEYCODE_G:
            return 'g';
        case KEYCODE_CAPITAL_H:
            return 'H';
        case KEYCODE_H:
            return 'h';
        case KEYCODE_CAPITAL_I:
            return 'I';
        case KEYCODE_I:
            return 'i';
        case KEYCODE_CAPITAL_J:
            return 'J';
        case KEYCODE_J:
            return 'j';
        case KEYCODE_CAPITAL_K:
            return 'K';
        case KEYCODE_K:
            return 'k';
        case KEYCODE_CAPITAL_L:
            return 'L';
        case KEYCODE_L:
            return 'l';
        case KEYCODE_CAPITAL_M:
            return 'M';
        case KEYCODE_M:
            return 'm';
        case KEYCODE_CAPITAL_N:
            return 'N';
        case KEYCODE_N:
            return 'n';
        case KEYCODE_CAPITAL_O:
            return 'O';
        case KEYCODE_O:
            return 'o';
        case KEYCODE_CAPITAL_P:
            return 'P';
        case KEYCODE_P:
            return 'p';
        case KEYCODE_CAPITAL_Q:
            return 'Q';
        case KEYCODE_Q:
            return 'q';
        case KEYCODE_CAPITAL_R:
            return 'R';
        case KEYCODE_R:
            return 'r';
        case KEYCODE_CAPITAL_S:
            return 'S';
        case KEYCODE_S:
            return 's';
        case KEYCODE_CAPITAL_T:
            return 'T';
        case KEYCODE_T:
            return 't';
        case KEYCODE_CAPITAL_U:
            return 'U';
        case KEYCODE_U:
            return 'u';
        case KEYCODE_CAPITAL_V:
            return 'V';
        case KEYCODE_V:
            return 'v';
        case KEYCODE_CAPITAL_W:
            return 'W';
        case KEYCODE_W:
            return 'w';
        case KEYCODE_CAPITAL_X:
            return 'X';
        case KEYCODE_X:
            return 'x';
        case KEYCODE_CAPITAL_Y:
            return 'Y';
        case KEYCODE_Y:
            return 'y';
        case KEYCODE_CAPITAL_Z:
            return 'Z';
        case KEYCODE_Z:
            return 'z';
        default:
            return '\0';
    }
}

QnxDemoApplication::QnxDemoApplication()
    : _inputHandledAsGesture(false)
{
    initDisplay();
    initGestures();
}

QnxDemoApplication::~QnxDemoApplication()
{
    if (_set != NULL)
    {
        gestures_set_free(_set);
        _set = NULL;
    }

    if (_eglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    if (_eglSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(_eglDisplay, _eglSurface);
        _eglSurface = EGL_NO_SURFACE;
    }

    if (_eglContext != EGL_NO_CONTEXT)
    {
        eglDestroyContext(_eglDisplay, _eglContext);
        _eglContext = EGL_NO_CONTEXT;
    }

    if (_eglDisplay != EGL_NO_DISPLAY)
    {
        eglTerminate(_eglDisplay);
        _eglDisplay = EGL_NO_DISPLAY;
    }

    if (_screenWindow)
    {
        screen_destroy_window(_screenWindow);
        _screenWindow = NULL;
    }

    if (_screenEvent)
    {
        screen_destroy_event(_screenEvent);
        _screenEvent = NULL;
    }

    if (_screenContext)
    {
        screen_destroy_context(_screenContext);
        _screenContext = NULL;
    }
}

void QnxDemoApplication::getAccelerometerPitchAndRoll(float* pitch, float* roll)
{
    double tx, ty, tz;
    accelerometer_read_forces(&tx, &ty, &tz);

    // Adjust for the orientation (landscape/portrait).
    if (_orientationAngle == 0)
    {
        tx = -tx;
        ty = -ty;
        tz = -tz;
    }

    if (pitch != NULL)
        *pitch = atan(ty / sqrt(tx * tx + tz * tz)) * 180.0f * M_1_PI;
    if (roll != NULL)
        *roll = atan(tx / sqrt(ty * ty + tz * tz)) * 180.0f * M_1_PI;
}

void QnxDemoApplication::run()
{
    int rc;
    int eventType;
    int flags;
    int value;
    int domain;
    mtouch_event_t touchEvent;

    // Store the initial time.
    clock_gettime(CLOCK_REALTIME, &__timespec);
    _timeStart = timespec2millis(&__timespec);
    _timeAbsolute = 0L;

    reshape(_screenWindowSize[0], _screenWindowSize[1]);

    // Initialize the gestures library.
    initGestures();

    // Main message loop.
    bool done = false;
    while (!done)
    {
        bps_event_t* event = NULL;
        rc = bps_get_event(&event, 5);
        assert(rc == BPS_SUCCESS);

        if (event)
        {
            domain = bps_event_get_domain(event);
            if (domain == screen_get_domain())
            {
                _screenEvent = screen_event_get_event(event);
                screen_get_event_property_iv(_screenEvent, SCREEN_PROPERTY_TYPE, &eventType);

                switch (eventType)
                {
                    case SCREEN_EVENT_MTOUCH_TOUCH:
                    case SCREEN_EVENT_MTOUCH_MOVE:
                    case SCREEN_EVENT_MTOUCH_RELEASE:
                        _inputHandledAsGesture = false;

                        screen_get_mtouch_event(_screenEvent, &touchEvent, 0);
                        gestures_set_process_event(_set, &touchEvent, this);

                        // If the input was not handled as a gesture,
                        // process it to simulate mouse input.
                        if (!_inputHandledAsGesture)
                        {
                            switch (eventType)
                            {
                                case SCREEN_EVENT_MTOUCH_TOUCH:
                                    mouseFunc(0, 0, touchEvent.x, touchEvent.y);
                                    break;
                                case SCREEN_EVENT_MTOUCH_RELEASE:
                                    mouseFunc(0, 1, touchEvent.x, touchEvent.y);
                                    break;
                                case SCREEN_EVENT_MTOUCH_MOVE:
                                    mouseMotionFunc(touchEvent.x, touchEvent.y);
                                    break;
                            }
                        }
                        break;
                    case SCREEN_EVENT_KEYBOARD:
                        screen_get_event_property_iv(_screenEvent, SCREEN_PROPERTY_KEY_FLAGS, &flags);
                        screen_get_event_property_iv(_screenEvent, SCREEN_PROPERTY_KEY_SYM, &value);

                        handleKeyboard(value, (flags & KEY_DOWN));
                        break;
                }
            }
            else if (domain == navigator_get_domain())
            {
                if (bps_event_get_code(event) == NAVIGATOR_EXIT)
                    done = true;
            }

            bps_event_destroy(event);
        }

        moveAndDisplay();
    }

    screen_stop_events(_screenContext);
    bps_shutdown();
}

void QnxDemoApplication::swapBuffers()
{
    if (eglSwapBuffers(_eglDisplay, _eglSurface) != EGL_TRUE)
    {
        perror("eglSwapBuffers");
    }
}

void QnxDemoApplication::updateCameraPosition(int dx, int dy)
{
    m_azi += -dx * btScalar(0.2);
    m_azi = fmodf(m_azi, btScalar(360.f));
    m_ele += dy * btScalar(0.2);
    m_ele = fmodf(m_ele, btScalar(180.f));

    updateCamera();
}

void QnxDemoApplication::updateCameraZoom(int delta)
{
    m_cameraDistance -= btScalar((float)delta * ZOOM_SCALE * m_zoomStepSize);
    updateCamera();
    if (m_cameraDistance < btScalar(0.1))
        m_cameraDistance = btScalar(0.1);
}

void QnxDemoApplication::glutSwapBuffers()
{
    swapBuffers();
}

void QnxDemoApplication::initDisplay()
{
    int rc = 0;
    int screenFormat = SCREEN_FORMAT_RGBA8888;
    int screenUsage = SCREEN_USAGE_OPENGL_ES1;
    int screenSwapInterval = 1;
    int screenTransparency = SCREEN_TRANSPARENCY_NONE;

    // Hard-coded to fullscreen.
    _screenWindowSize[0] = -1;
    _screenWindowSize[1] = -1;

    // Hard-coded to (0,0).
    int windowPosition[] = { 0, 0 };

    EGLint eglConfigCount;

    // Hard-coded to 32-bit/OpenGL ES 1.1.
    const EGLint eglConfigAttrs[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_NONE };

    const EGLint eglSurfaceAttrs[] = { EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE };

    bps_initialize();
    accelerometer_set_update_frequency(FREQ_40_HZ);

    // Get the application's orientation (used for accelerometer adjustment).
    orientation_direction_t direction;
    orientation_get(&direction, &_orientationAngle);

    // Create the screen context.
    rc = screen_create_context(&_screenContext, 0);
    if (rc)
    {
        perror("screen_create_context");
        return;
    }

    // Create the screen window.
    rc = screen_create_window(&_screenWindow, _screenContext);
    if (rc)
    {
        perror("screen_create_window");
        return;
    }

    // Set/get any window prooperties.
    rc = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_FORMAT, &screenFormat);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)");
        return;
    }

    rc = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_USAGE, &screenUsage);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_USAGE)");
        return;
    }

    if (_screenWindowSize[0] > 0 && _screenWindowSize[1] > 0)
    {
        rc = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_SIZE, _screenWindowSize);
        if (rc)
        {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_SIZE)");
            return;
        }
    }
    else
    {
        rc = screen_get_window_property_iv(_screenWindow, SCREEN_PROPERTY_SIZE, _screenWindowSize);
        if (rc)
        {
            perror("screen_get_window_property_iv(SCREEN_PROPERTY_SIZE)");
            return;
        }
    }

    if (windowPosition[0] != 0 || windowPosition[1] != 0)
    {
        rc = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_POSITION, windowPosition);
        if (rc)
        {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_POSITION)");
            return;
        }
    }

    rc = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_TRANSPARENCY, &screenTransparency);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_TRANSPARENCY)");
        return;
    }

    rc = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_SWAP_INTERVAL, &screenSwapInterval);
    if (rc)
    {
        perror("screen_set_window_property_iv(SCREEN_PROPERTY_SWAP_INTERVAL)");
        return;
    }

    // Double buffered.
    rc = screen_create_window_buffers(_screenWindow, 2);
    if (rc)
    {
        perror("screen_create_window_buffers");
        return;
    }

    // Create the screen event object.
    rc = screen_create_event(&_screenEvent);
    if (rc)
    {
        perror("screen_create_event");
        return;
    }

    screen_request_events(_screenContext);

    // Get the EGL display and initialize.
    _eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (_eglDisplay == EGL_NO_DISPLAY)
    {
        perror("eglGetDisplay");
        return;
    }
    if (eglInitialize(_eglDisplay, NULL, NULL) != EGL_TRUE)
    {
        perror("eglInitialize");
        return;
    }

    if (eglChooseConfig(_eglDisplay, eglConfigAttrs, &_eglConfig, 1, &eglConfigCount) != EGL_TRUE
            || eglConfigCount == 0)
    {
        checkErrorEGL("eglChooseConfig");
        return;
    }

    _eglContext = eglCreateContext(_eglDisplay, _eglConfig, EGL_NO_CONTEXT, NULL);
    if (_eglContext == EGL_NO_CONTEXT)
    {
        checkErrorEGL("eglCreateContext");
        return;
    }

    _eglSurface = eglCreateWindowSurface(_eglDisplay, _eglConfig, _screenWindow, eglSurfaceAttrs);
    if (_eglSurface == EGL_NO_SURFACE)
    {
        checkErrorEGL("eglCreateWindowSurface");
        return;
    }

    if (eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext) != EGL_TRUE)
    {
        checkErrorEGL("eglMakeCurrent");
        return;
    }

    // Set vsync.
    eglSwapInterval(_eglDisplay, screenSwapInterval);
}

void QnxDemoApplication::initGestures()
{
    _set = gestures_set_alloc();
    if (_set != NULL)
    {
        tfpan_gesture_alloc(NULL, handleGestures, _set);
        pinch_gesture_alloc(NULL, handleGestures, _set);
        double_tap_gesture_alloc(NULL, handleGestures, _set);
    }
}

void QnxDemoApplication::handleKeyboard(int key, bool isPressed)
{
    // Generate keyboard events at the center of the screen.
    int x = m_glutScreenWidth / 2;
    int y = m_glutScreenHeight / 2;

    int glutKey = keycodeToGLUTKey(key);
    if (glutKey != 0)
    {
        if (isPressed)
            specialKeyboard(glutKey, x, y);
        else
            specialKeyboardUp(glutKey, x, y);
    }
    else
    {
        if (isPressed)
            this->keyboardCallback(keycodeToUnsignedChar(key), x, y);
        else
            this->keyboardUpCallback(keycodeToUnsignedChar(key), x, y);
    }
}

#endif // __QNX__
