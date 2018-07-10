#include "TBillboard.h"

void TBillboard::beginDraw()
{
    if(texture != nullptr)
    {
        texture->draw();

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_POINTS, 0, 1);

        glDisableVertexAttribArray(0);

    }
}