#include <iostream>
#include "GLGECore/GLGECore.h"

void func() 
{
    printf("Hi\n");
}

int main()
{
    Message msg = {
        .identifier = {
            .number = 1,
        },
        .contents = { 0 },
        .callback = {
            .identifier = GLGE_MSG_CALLBACK_FUNC_RET_VOID_INP_VOID,
            .func = {
                .func_retVoid_inpVoid = func
            }
        }
    };

    MessageListener listener;
    listener.sendMessage(msg);
    listener.sendMessage(msg);

    Message message;
    if (!listener.pull(&message)) {return 1;}
    (*message.callback.func.func_retVoid_inpVoid)();
    if (!listener.peek(&message)) {return 0;}
    return 2;
}