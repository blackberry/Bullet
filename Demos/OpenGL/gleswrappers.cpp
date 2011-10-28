#ifdef __QNX__

#include "gleswrappers.h"
#include "QnxDemoApplication.h"

#define GW_BUFFERSIZE (16384)

////////////////////////////////////////////////////////////////////////////////////////////////
// Vector math functions

// Adds the first two vectors together and stores the result in the third vector.
void add(float v1[3], float v2[3], float dst[3])
{
    float x = v1[0] + v2[0];
    float y = v1[1] + v2[1];
    float z = v1[2] + v2[2];

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}

// Computes the cross product of the first two vectors.
void cross(float v1[3], float v2[3], float dst[3])
{
    float x = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    float y = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    float z = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}

// Computes the dot product of the two given vectors.
float dot(float v1[3], float v2[3])
{
    return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

// Normalizes the given vector.
void normalize(float v[3])
{
    float n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (n == 1.0f)
        return;

    n = sqrt(n);
    if (n < 0.000001)
        return;

    n = 1.0f / n;
    v[0] *= n;
    v[1] *= n;
    v[2] *= n;
}

// Subtracts the second vector from the first and stores the result in the third vector.
void subtract(float v1[3], float v2[3], float dst[3])
{
    float x = v1[0] - v2[0];
    float y = v1[1] - v2[1];
    float z = v1[2] - v2[2];

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}

////////////////////////////////////////////////////////////////////////////////////////////////

GLenum currentMode = MODE_NONE;
float vertexBuffer[GW_BUFFERSIZE];
float texCoordBuffer[GW_BUFFERSIZE];
float colorBuffer[GW_BUFFERSIZE];
unsigned short indexBuffer[GW_BUFFERSIZE];
int vertexSize = 0;
int texCoordSize = 0;
int vertexBufferSize = 0;
int texCoordBufferSize = 0;
int colorBufferSize = 0;
int vertexModFour = 0;
int texCoordModFour = 0;
int indexBufferSize = 0;
bool useColors = false;
float currentColor[4];

////////////////////////////////////////////////////////////////////////////////////////////////

void GL_APIENTRY glBegin(GLenum mode)
{
    currentMode = mode;
    vertexSize = 0;
    texCoordSize = 0;
    vertexBufferSize = 0;
    texCoordBufferSize = 0;
    colorBufferSize = 0;
    vertexModFour = 0;
    texCoordModFour = 0;
    indexBufferSize = 0;
    useColors = false;
}

void GL_APIENTRY glEnd(void)
{
    if (indexBufferSize > 0)
    {
        switch (currentMode)
        {
        case GL_POINTS:
            glDrawElements(GL_POINTS, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_LINES:
            glDrawElements(GL_LINES, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_LINE_LOOP:
            glDrawElements(GL_LINE_LOOP, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_LINE_STRIP:
            glDrawElements(GL_LINE_STRIP, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_TRIANGLES:
            glDrawElements(GL_TRIANGLES, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_TRIANGLE_STRIP:
            glDrawElements(GL_TRIANGLE_STRIP, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_TRIANGLE_FAN:
            glDrawElements(GL_TRIANGLE_FAN, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_QUADS:
            glDrawElements(GL_TRIANGLES, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_QUAD_STRIP:
            glDrawElements(GL_TRIANGLE_STRIP, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        case GL_POLYGON:
            glDrawElements(GL_TRIANGLE_FAN, indexBufferSize, GL_UNSIGNED_SHORT, indexBuffer);
            break;
        }
    }
    else
    {
        int elementCount = 0;

        if (vertexSize > 0)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(vertexSize, GL_FLOAT, 0, vertexBuffer);
            if (vertexSize > elementCount)
            {
                elementCount = vertexBufferSize / vertexSize;
            }
        }
        if (texCoordSize > 0)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(texCoordSize, GL_FLOAT, 0, texCoordBuffer);
            if (texCoordSize > elementCount)
            {
                elementCount = texCoordBufferSize / texCoordSize;
            }
        }
        else
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        if (useColors)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, colorBuffer);
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
        }

        switch (currentMode)
        {
        case GL_POINTS:
            glDrawArrays(GL_POINTS, 0, elementCount);
            break;
        case GL_LINES:
            glDrawArrays(GL_LINES, 0, elementCount);
            break;
        case GL_LINE_LOOP:
            glDrawArrays(GL_LINE_LOOP, 0, elementCount);
            break;
        case GL_LINE_STRIP:
            glDrawArrays(GL_LINE_STRIP, 0, elementCount);
            break;
        case GL_TRIANGLES:
            glDrawArrays(GL_TRIANGLES, 0, elementCount);
            break;
        case GL_TRIANGLE_STRIP:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, elementCount);
            break;
        case GL_TRIANGLE_FAN:
            glDrawArrays(GL_TRIANGLE_FAN, 0, elementCount);
            break;
        case GL_QUADS:
            glDrawArrays(GL_TRIANGLES, 0, elementCount);
            break;
        case GL_QUAD_STRIP:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, elementCount);
            break;
        case GL_POLYGON:
            glDrawArrays(GL_TRIANGLE_FAN, 0, elementCount);
            break;
        }
    }

    currentMode = MODE_NONE;
}

void GL_APIENTRY glTexCoord2f(GLfloat s, GLfloat t)
{
    if (texCoordSize == 0)
    {
        texCoordSize = 2;
    }
    else if (texCoordSize != 2)
    {
        return;
    }

    if (currentMode == GL_QUADS)
    {
        if (texCoordModFour == 2)
        {
            GLfloat s0, s1, t0, t1;

            pushtexfloat(s);
            pushtexfloat(t);

            s0 = texCoordBuffer[texCoordBufferSize - texCoordSize * 3];
            t0 = texCoordBuffer[texCoordBufferSize - texCoordSize * 3 + 1];

            pushtexfloat(s0);
            pushtexfloat(t0);
            pushtexfloat(s);
            pushtexfloat(t);

        }
        else
        {
            pushtexfloat(s);
            pushtexfloat(t);
        }
    }
    else
    {
        pushtexfloat(s);
        pushtexfloat(t);
    }

    texCoordModFour++;
    if (texCoordModFour == 4)
    {
        texCoordModFour = 0;
    }
}

void GL_APIENTRY glVertex2f(GLfloat x, GLfloat y)
{
    if (vertexSize == 0)
    {
        vertexSize = 2;
    }
    else if (vertexSize != 2)
    {
        return;
    }

    if (currentMode == GL_QUADS)
    {
        if (vertexModFour == 2)
        {
            GLfloat x0, y0;

            pushvtxfloat(x);
            pushvtxfloat(y);

            x0 = vertexBuffer[vertexBufferSize - vertexSize * 3];
            y0 = vertexBuffer[vertexBufferSize - vertexSize * 3 + 1];

            pushvtxfloat(x0);
            pushvtxfloat(y0);
            pushvtxfloat(x);
            pushvtxfloat(y);

        }
        else
        {
            pushvtxfloat(x);
            pushvtxfloat(y);
        }
    }
    else
    {
        pushvtxfloat(x);
        pushvtxfloat(y);
    }

    vertexModFour++;
    if (vertexModFour == 4)
    {
        vertexModFour = 0;
    }

    // Add color information if we are using colors.
    if (useColors)
    {
        pushcolorfloat(currentColor[0]);
        pushcolorfloat(currentColor[1]);
        pushcolorfloat(currentColor[2]);
        pushcolorfloat(currentColor[3]);
    }
}

void GL_APIENTRY glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
    if (vertexSize == 0)
    {
        vertexSize = 3;
    }
    else if (vertexSize != 3)
    {
        return;
    }

    if (currentMode == GL_QUADS)
    {
        if (vertexModFour == 2)
        {
            GLfloat x0, y0, z0;

            pushvtxfloat(x);
            pushvtxfloat(y);
            pushvtxfloat(z);

            x0 = vertexBuffer[vertexBufferSize - vertexSize * 3];
            y0 = vertexBuffer[vertexBufferSize - vertexSize * 3 + 1];
            z0 = vertexBuffer[vertexBufferSize - vertexSize * 3 + 2];

            pushvtxfloat(x0);
            pushvtxfloat(y0);
            pushvtxfloat(z0);
            pushvtxfloat(x);
            pushvtxfloat(y);
            pushvtxfloat(z);
        }
        else
        {
            pushvtxfloat(x);
            pushvtxfloat(y);
            pushvtxfloat(z);
        }
    }
    else
    {
        pushvtxfloat(x);
        pushvtxfloat(y);
        pushvtxfloat(z);
    }

    vertexModFour++;
    if (vertexModFour == 4)
    {
        vertexModFour = 0;
    }

    // Add color information if we are using colors.
    if (useColors)
    {
        pushcolorfloat(currentColor[0]);
        pushcolorfloat(currentColor[1]);
        pushcolorfloat(currentColor[2]);
        pushcolorfloat(currentColor[3]);
    }
}

void GL_APIENTRY gluLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
        float targetPositionX, float targetPositionY, float targetPositionZ,
        float upX, float upY, float upZ)
{
    float eye[3] = { eyePositionX, eyePositionY, eyePositionZ };
    float target[3] = {targetPositionX, targetPositionY, targetPositionZ };
    float up[3] = { upX, upY, upZ };
    normalize(up);

    float zaxis[3];
    subtract(eye, target, zaxis);
    normalize(zaxis);

    float xaxis[3];
    cross(up, zaxis, xaxis);
    normalize(xaxis);

    float yaxis[3];
    cross(zaxis, xaxis, yaxis);
    normalize(yaxis);

    float m[] = {
            xaxis[0], yaxis[0], zaxis[0], 0.0f,
            xaxis[1], yaxis[1], zaxis[1], 0.0f,
            xaxis[2], yaxis[2], zaxis[2], 0.0f,
            -dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1.0f
    };

    glMultMatrixf(m);
}

// Generates object linear texture coordinates using
// the plane (1, 0, 0, 0) for the S coordinate and
// the plane (0, 0, 1, 0) for the T coordinate.
//
// Note: this overwrites all previous texture coordinates.
void GL_APIENTRY genTexCoords()
{
    texCoordBufferSize = 0;
    texCoordSize = 2;

    for (int i = 0; i < vertexBufferSize; i += 3)
    {
        pushtexfloat(vertexBuffer[i]);
        pushtexfloat(vertexBuffer[i+2]);
    }
}

void GL_APIENTRY gluPerspective(float fieldOfView, float aspectRatio,
        float zNearPlane, float zFarPlane)
{
    float f_n = 1.0f / (zFarPlane - zNearPlane);
    float factor = 1.0f / tanf(MATH_DEG_TO_RAD(fieldOfView) * 0.5f);
    float m[16] = {0};

    m[0] = (1.0f / aspectRatio) * factor;
    m[5] = factor;
    m[10] = (-(zFarPlane + zNearPlane)) * f_n;
    m[11] = -1.0f;
    m[14] = -2.0f * zFarPlane * zNearPlane * f_n;

    glMultMatrixf(m);
}

int glutmain(int argc, char** argv, int width, int height, const char* title, DemoApplication* app)
{
    QnxDemoApplication* qnxApp = (QnxDemoApplication*)app;
    qnxApp->myinit();
    qnxApp->run();
    return 0;
}

#endif // __QNX__
