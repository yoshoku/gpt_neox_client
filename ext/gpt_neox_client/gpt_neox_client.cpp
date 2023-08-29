#include "gpt_neox_client.h"

VALUE rb_mGPTNeoXClient;

extern "C" void Init_gpt_neox_client(void)
{
  rb_mGPTNeoXClient = rb_define_module("GPTNeoXClient");
}
