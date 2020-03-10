#include "renderingdevice.h"
#include "guiparameter.h"

int main(int argc, char *argv[])
{
	ogl_examples::RenderingDevice rdev(std::make_shared<ogl_examples::GUIParameter>());

	rdev.run();
	return 0;
}
