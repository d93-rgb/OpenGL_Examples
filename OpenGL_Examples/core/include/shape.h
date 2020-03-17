#pragma once
#include "opengl_examples.h"
#include "GL/gl3w.h"

namespace ogl_examples
{

class Shape
{
public:
    Shape();

    ~Shape();

    virtual void draw() = 0;

protected:

};

} // namespace ogl_examples