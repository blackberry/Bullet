#ifdef __QNX__

#ifndef GLESWRAPPERS_H
#define GLESWRAPPERS_H

#include <math.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#define MATH_DEG_TO_RAD(x)      ((x) * 0.0174532925f)

#define MODE_NONE ((unsigned int)(-1))

#define GLFLOAT_1 (1.0f)
#define GLFLOAT_0 (0.0f)

#define GW_BUFFERSIZE (16384)

#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009

#define glOrtho glOrthof
#define glFrustum glFrustumf
#define glVertex3d glVertex3f
#define glScaled glScalef
#define glNormal3d glNormal3f

#define gluOrtho2D(left, right, bottom, top) glOrthof(left, right, bottom, top, -1, 1)

//////////////////////////////////////////////////////////////////////////////

extern GLenum currentMode;
extern float vertexBuffer[GW_BUFFERSIZE];
extern float texCoordBuffer[GW_BUFFERSIZE];
extern float colorBuffer[GW_BUFFERSIZE];
extern unsigned short indexBuffer[GW_BUFFERSIZE];
extern int vertexSize;
extern int texCoordSize;
extern int vertexBufferSize;
extern int texCoordBufferSize;
extern int colorBufferSize;
extern int vertexModFour;
extern int texCoordModFour;
extern int indexBufferSize;
extern bool useColors;
extern float currentColor[4];

//////////////////////////////////////////////////////////////////////////////

void GL_APIENTRY glBegin(GLenum mode);
void GL_APIENTRY glEnd(void);

void GL_APIENTRY glTexCoord2f(GLfloat s, GLfloat t);
void GL_APIENTRY glVertex2f(GLfloat x, GLfloat y);
void GL_APIENTRY glVertex3i(GLfixed x, GLfixed y, GLfixed z);
void GL_APIENTRY glVertex3f(GLfloat x, GLfloat y, GLfloat z);

void GL_APIENTRY genTexCoords();

void GL_APIENTRY gluLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
    float targetPositionX, float targetPositionY, float targetPositionZ,
    float upX, float upY, float upZ);

void GL_APIENTRY gluPerspective(float fieldOfView, float aspectRatio,
        float zNearPlane, float zFarPlane);

// Vector math functions.
void add(float v1[3], float v2[3], float dst[3]);
void cross(float v1[3], float v2[3], float dst[3]);
float dot(float v1[3], float v2[3]);
void normalize(float v[3]);
void subtract(float v1[3], float v2[3], float dst[3]);

static __inline void GL_APIENTRY pushtexfloat(GLfloat f)
{
    texCoordBuffer[texCoordBufferSize++] = f;
}

static __inline void GL_APIENTRY pushvtxfloat(GLfloat f)
{
    vertexBuffer[vertexBufferSize++] = f;
}

static __inline void GL_APIENTRY pushcolorfloat(GLfloat f)
{
    colorBuffer[colorBufferSize++] = f;
}

static __inline void GL_APIENTRY pushindex(unsigned short idx)
{
    indexBuffer[indexBufferSize++] = idx;
}

static __inline void GL_APIENTRY glTexCoord2fv(const GLfloat *v)
{
    glTexCoord2f(v[0], v[1]);
}

static __inline void GL_APIENTRY glVertex3fv(const GLfloat *v)
{
    glVertex3f(v[0], v[1], v[2]);
}

static __inline void GL_APIENTRY glColor4fWrapper(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    // If this is called inside a glBegin/glEnd pair and colors
    // are not yet being generated, generate color data for all current
    // vertices before changing the current drawing color.
    if (currentMode != MODE_NONE && !useColors)
    {
        // Get the current color's r, g, b, and a component values.
        glGetFloatv(GL_CURRENT_COLOR, currentColor);

        for (int i = 0; i < vertexBufferSize; i += 3)
        {
            pushcolorfloat(currentColor[0]);
            pushcolorfloat(currentColor[1]);
            pushcolorfloat(currentColor[2]);
            pushcolorfloat(currentColor[3]);
        }

        useColors = true;
    }

    glColor4f(r, g, b, a);

    // Store the current color so that we don't have to
    // get it every time a new vertex is added.
    currentColor[0] = r;
    currentColor[1] = g;
    currentColor[2] = b;
    currentColor[3] = a;
}

#define glColor4f(r, g, b, a) glColor4fWrapper(r, g, b, a)

static __inline void GL_APIENTRY glColor3f(GLfloat r, GLfloat g, GLfloat b)
{
    glColor4fWrapper(r, g, b, GLFLOAT_1);
}

static __inline void GL_APIENTRY glArrayElement(GLint idx)
{
    pushindex(idx);
}

#endif // GLESWRAPPERS_H

#endif // __QNX__
