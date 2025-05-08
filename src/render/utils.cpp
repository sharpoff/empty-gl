#include "render/utils.h"
#include <GL/glew.h>

void beginDebugGroup(std::string msg, uint id)
{
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, id, msg.length(), msg.data());
}

void endDebugGroup()
{
    glPopDebugGroup();
}
