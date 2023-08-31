#include "gpt_neox_client.h"

VALUE rb_cGPTNeoXClient;
VALUE rb_cGPTParams;
VALUE rb_cGPTVocab;
VALUE rb_cGPTNeoXModel;

class RbGPTNeoXModel {
public:
  static VALUE gpt_neox_model_alloc(VALUE self) {
    gpt_neox_model* ptr = (gpt_neox_model*)ruby_xmalloc(sizeof(gpt_neox_model));
    new (ptr) gpt_neox_model();
    return TypedData_Wrap_Struct(self, &gpt_neox_model_type, ptr);
  }

  static void gpt_neox_model_free(void* ptr) {
    ((gpt_neox_model*)ptr)->~gpt_neox_model();
    ruby_xfree(ptr);
  }

  static size_t gpt_neox_model_size(const void* ptr) {
    return sizeof(*((gpt_neox_model*)ptr));
  }

  static gpt_neox_model* get_gpt_neox_model(VALUE self) {
    gpt_neox_model* ptr = nullptr;
    TypedData_Get_Struct(self, gpt_neox_model, &gpt_neox_model_type, ptr);
    return ptr;
  }

  static void define_class(VALUE outer) {
    rb_cGPTNeoXModel = rb_define_class_under(outer, "Model", rb_cObject);
    rb_define_alloc_func(rb_cGPTNeoXModel, gpt_neox_model_alloc);
  }

private:
  static const rb_data_type_t gpt_neox_model_type;
};

const rb_data_type_t RbGPTNeoXModel::gpt_neox_model_type = {
  "RbGPTNeoXModel",
  { 0,
    RbGPTNeoXModel::gpt_neox_model_free,
    RbGPTNeoXModel::gpt_neox_model_size },
  0,
  0,
  RUBY_TYPED_FREE_IMMEDIATELY
};

class RbGPTVocab {
public:
  static VALUE gpt_vocab_alloc(VALUE self) {
    gpt_vocab* ptr = (gpt_vocab*)ruby_xmalloc(sizeof(gpt_vocab));
    new (ptr) gpt_vocab();
    return TypedData_Wrap_Struct(self, &gpt_vocab_type, ptr);
  }

  static void gpt_vocab_free(void* ptr) {
    ((gpt_vocab*)ptr)->~gpt_vocab();
    ruby_xfree(ptr);
  }

  static size_t gpt_vocab_size(const void* ptr) {
    return sizeof(*((gpt_vocab*)ptr));
  }

  static gpt_vocab* get_gpt_vocab(VALUE self) {
    gpt_vocab* ptr = nullptr;
    TypedData_Get_Struct(self, gpt_vocab, &gpt_vocab_type, ptr);
    return ptr;
  }

  static void define_class(VALUE outer) {
    rb_cGPTVocab = rb_define_class_under(outer, "Vocab", rb_cObject);
    rb_define_alloc_func(rb_cGPTVocab, gpt_vocab_alloc);
  }

private:
  static const rb_data_type_t gpt_vocab_type;
};

const rb_data_type_t RbGPTVocab::gpt_vocab_type = {
  "RbGPTVocab",
  { 0,
    RbGPTVocab::gpt_vocab_free,
    RbGPTVocab::gpt_vocab_size },
  0,
  0,
  RUBY_TYPED_FREE_IMMEDIATELY
};

class RbGPTParams {
public:
  static VALUE gpt_params_alloc(VALUE self) {
    gpt_params* ptr = (gpt_params*)ruby_xmalloc(sizeof(gpt_params));
    new (ptr) gpt_params();
    return TypedData_Wrap_Struct(self, &gpt_params_type, ptr);
  }

  static void gpt_params_free(void* ptr) {
    ((gpt_params*)ptr)->~gpt_params();
    ruby_xfree(ptr);
  }

  static size_t gpt_params_size(const void* ptr) {
    return sizeof(*((gpt_params*)ptr));
  }

  static gpt_params* get_gpt_params(VALUE self) {
    gpt_params* ptr = nullptr;
    TypedData_Get_Struct(self, gpt_params, &gpt_params_type, ptr);
    return ptr;
  }

  static void define_class(VALUE outer) {
    rb_cGPTParams = rb_define_class_under(outer, "Params", rb_cObject);
    rb_define_alloc_func(rb_cGPTParams, gpt_params_alloc);
    rb_define_method(rb_cGPTParams, "seed=", RUBY_METHOD_FUNC(_gpt_params_set_seed), 1);
    rb_define_method(rb_cGPTParams, "seed", RUBY_METHOD_FUNC(_gpt_params_get_seed), 0);
    rb_define_method(rb_cGPTParams, "n_threads=", RUBY_METHOD_FUNC(_gpt_params_set_n_threads), 1);
    rb_define_method(rb_cGPTParams, "n_threads", RUBY_METHOD_FUNC(_gpt_params_get_n_threads), 0);
    rb_define_method(rb_cGPTParams, "n_predict=", RUBY_METHOD_FUNC(_gpt_params_set_n_predict), 1);
    rb_define_method(rb_cGPTParams, "n_predict", RUBY_METHOD_FUNC(_gpt_params_get_n_predict), 0);
    rb_define_method(rb_cGPTParams, "n_batch=", RUBY_METHOD_FUNC(_gpt_params_set_n_batch), 1);
    rb_define_method(rb_cGPTParams, "n_batch", RUBY_METHOD_FUNC(_gpt_params_get_n_batch), 0);
    rb_define_method(rb_cGPTParams, "top_k=", RUBY_METHOD_FUNC(_gpt_params_set_top_k), 1);
    rb_define_method(rb_cGPTParams, "top_k", RUBY_METHOD_FUNC(_gpt_params_get_top_k), 0);
    rb_define_method(rb_cGPTParams, "top_p=", RUBY_METHOD_FUNC(_gpt_params_set_top_p), 1);
    rb_define_method(rb_cGPTParams, "top_p", RUBY_METHOD_FUNC(_gpt_params_get_top_p), 0);
    rb_define_method(rb_cGPTParams, "temp=", RUBY_METHOD_FUNC(_gpt_params_set_temp), 1);
    rb_define_method(rb_cGPTParams, "temp", RUBY_METHOD_FUNC(_gpt_params_get_temp), 0);
    rb_define_method(rb_cGPTParams, "repeat_last_n=", RUBY_METHOD_FUNC(_gpt_params_set_repeat_last_n), 1);
    rb_define_method(rb_cGPTParams, "repeat_last_n", RUBY_METHOD_FUNC(_gpt_params_get_repeat_last_n), 0);
    rb_define_method(rb_cGPTParams, "repeat_penalty=", RUBY_METHOD_FUNC(_gpt_params_set_repeat_penalty), 1);
    rb_define_method(rb_cGPTParams, "repeat_penalty", RUBY_METHOD_FUNC(_gpt_params_get_repeat_penalty), 0);
    rb_define_method(rb_cGPTParams, "model=", RUBY_METHOD_FUNC(_gpt_params_set_model), 1);
    rb_define_method(rb_cGPTParams, "model", RUBY_METHOD_FUNC(_gpt_params_get_model), 0);
    rb_define_method(rb_cGPTParams, "prompt=", RUBY_METHOD_FUNC(_gpt_params_set_prompt), 1);
    rb_define_method(rb_cGPTParams, "prompt", RUBY_METHOD_FUNC(_gpt_params_get_prompt), 0);
  }

private:
  static const rb_data_type_t gpt_params_type;

  // seed
  static VALUE _gpt_params_set_seed(VALUE self, VALUE seed) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->seed = NUM2INT(seed);
    return INT2NUM(ptr->seed);
  }

  static VALUE _gpt_params_get_seed(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return INT2NUM(ptr->seed);
  }

  // n_threads
  static VALUE _gpt_params_set_n_threads(VALUE self, VALUE n_threads) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->n_threads = NUM2INT(n_threads);
    return INT2NUM(ptr->n_threads);
  }

  static VALUE _gpt_params_get_n_threads(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return INT2NUM(ptr->n_threads);
  }

  // n_predict
  static VALUE _gpt_params_set_n_predict(VALUE self, VALUE n_predict) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->n_predict = NUM2INT(n_predict);
    return INT2NUM(ptr->n_predict);
  }

  static VALUE _gpt_params_get_n_predict(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return INT2NUM(ptr->n_predict);
  }

  // n_batch
  static VALUE _gpt_params_set_n_batch(VALUE self, VALUE n_batch) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->n_batch = NUM2INT(n_batch);
    return INT2NUM(ptr->n_batch);
  }

  static VALUE _gpt_params_get_n_batch(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return INT2NUM(ptr->n_batch);
  }

  // top_k
  static VALUE _gpt_params_set_top_k(VALUE self, VALUE top_k) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->top_k = NUM2INT(top_k);
    return INT2NUM(ptr->top_k);
  }

  static VALUE _gpt_params_get_top_k(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return INT2NUM(ptr->top_k);
  }

  // top_p
  static VALUE _gpt_params_set_top_p(VALUE self, VALUE top_p) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->top_p = NUM2DBL(top_p);
    return DBL2NUM(ptr->top_p);
  }

  static VALUE _gpt_params_get_top_p(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return DBL2NUM(ptr->top_p);
  }

  // temp
  static VALUE _gpt_params_set_temp(VALUE self, VALUE temp) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->temp = NUM2DBL(temp);
    return DBL2NUM(ptr->temp);
  }

  static VALUE _gpt_params_get_temp(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return DBL2NUM(ptr->temp);
  }

  // repeat_last_n
  static VALUE _gpt_params_set_repeat_last_n(VALUE self, VALUE repeat_last_n) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->repeat_last_n = NUM2INT(repeat_last_n);
    return INT2NUM(ptr->repeat_last_n);
  }

  static VALUE _gpt_params_get_repeat_last_n(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return INT2NUM(ptr->repeat_last_n);
  }

  // repeat_penalty
  static VALUE _gpt_params_set_repeat_penalty(VALUE self, VALUE repeat_penalty) {
    gpt_params* ptr = get_gpt_params(self);
    ptr->repeat_penalty = NUM2DBL(repeat_penalty);
    return DBL2NUM(ptr->repeat_penalty);
  }

  static VALUE _gpt_params_get_repeat_penalty(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return DBL2NUM(ptr->repeat_penalty);
  }

  // model
  static VALUE _gpt_params_set_model(VALUE self, VALUE model) {
    gpt_params* ptr = get_gpt_params(self);
    std::string model_str(StringValueCStr(model));
    ptr->model = model_str;
    RB_GC_GUARD(model);
    return rb_str_new_cstr(ptr->model.c_str());
  }

  static VALUE _gpt_params_get_model(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return rb_str_new_cstr(ptr->model.c_str());
  }

  // prompt
  static VALUE _gpt_params_set_prompt(VALUE self, VALUE prompt) {
    gpt_params* ptr = get_gpt_params(self);
    std::string prompt_str(StringValueCStr(prompt));
    ptr->prompt = prompt_str;
    RB_GC_GUARD(prompt);
    return rb_str_new_cstr(ptr->prompt.c_str());
  }

  static VALUE _gpt_params_get_prompt(VALUE self) {
    gpt_params* ptr = get_gpt_params(self);
    return rb_str_new_cstr(ptr->prompt.c_str());
  }
};

const rb_data_type_t RbGPTParams::gpt_params_type = {
  "RbGPTParams",
  { 0,
    RbGPTParams::gpt_params_free,
    RbGPTParams::gpt_params_size },
  0,
  0,
  RUBY_TYPED_FREE_IMMEDIATELY
};

extern "C" void Init_gpt_neox_client(void) {
  rb_cGPTNeoXClient = rb_define_class("GPTNeoXClient", rb_cObject);

  RbGPTParams::define_class(rb_cGPTNeoXClient);
  RbGPTVocab::define_class(rb_cGPTNeoXClient);
  RbGPTNeoXModel::define_class(rb_cGPTNeoXClient);
}
