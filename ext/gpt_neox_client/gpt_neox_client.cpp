#include "gpt_neox_client.h"

VALUE rb_mGPTNeoX;
VALUE rb_mGPTNeoXClient;
VALUE rb_cGPTParams;
VALUE rb_cGPTVocab;

class GPTVocabWrapper {
public:
  struct gpt_vocab vocab;
  GPTVocabWrapper() {}
  ~GPTVocabWrapper() {}
};

class RbGPTVocab {
public:
  static VALUE gpt_vocab_alloc(VALUE self) {
    GPTVocabWrapper* ptr = (GPTVocabWrapper*)ruby_xmalloc(sizeof(GPTVocabWrapper));
    new (ptr) GPTVocabWrapper();
    return TypedData_Wrap_Struct(self, &gpt_vocab_type, ptr);
  }

  static void gpt_vocab_free(void* ptr) {
    ((GPTVocabWrapper*)ptr)->~GPTVocabWrapper();
    ruby_xfree(ptr);
  }

  static size_t gpt_vocab_size(const void* ptr) {
    return sizeof(*((GPTVocabWrapper*)ptr));
  }

  static GPTVocabWrapper* get_gpt_vocab(VALUE self) {
    GPTVocabWrapper* ptr = nullptr;
    TypedData_Get_Struct(self, GPTVocabWrapper, &gpt_vocab_type, ptr);
    return ptr;
  }

  static void define_class(VALUE outer) {
    rb_cGPTVocab = rb_define_class_under(outer, "GPTVocab", rb_cObject);
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

class GPTParamsWrapper {
public:
  struct gpt_params params;
  GPTParamsWrapper() {}
  ~GPTParamsWrapper() {}
};

class RbGPTParams {
public:
  static VALUE gpt_params_alloc(VALUE self) {
    GPTParamsWrapper* ptr = (GPTParamsWrapper*)ruby_xmalloc(sizeof(GPTParamsWrapper));
    new (ptr) GPTParamsWrapper();
    return TypedData_Wrap_Struct(self, &gpt_params_type, ptr);
  }

  static void gpt_params_free(void* ptr) {
    ((GPTParamsWrapper*)ptr)->~GPTParamsWrapper();
    ruby_xfree(ptr);
  }

  static size_t gpt_params_size(const void* ptr) {
    return sizeof(*((GPTParamsWrapper*)ptr));
  }

  static GPTParamsWrapper* get_gpt_params(VALUE self) {
    GPTParamsWrapper* ptr = nullptr;
    TypedData_Get_Struct(self, GPTParamsWrapper, &gpt_params_type, ptr);
    return ptr;
  }

  static void define_class(VALUE outer) {
    rb_cGPTParams = rb_define_class_under(outer, "GPTParams", rb_cObject);
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
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.seed = NUM2INT(seed);
    return INT2NUM(ptr->params.seed);
  }

  static VALUE _gpt_params_get_seed(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return INT2NUM(ptr->params.seed);
  }

  // n_threads
  static VALUE _gpt_params_set_n_threads(VALUE self, VALUE n_threads) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.n_threads = NUM2INT(n_threads);
    return INT2NUM(ptr->params.n_threads);
  }

  static VALUE _gpt_params_get_n_threads(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return INT2NUM(ptr->params.n_threads);
  }

  // n_predict
  static VALUE _gpt_params_set_n_predict(VALUE self, VALUE n_predict) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.n_predict = NUM2INT(n_predict);
    return INT2NUM(ptr->params.n_predict);
  }

  static VALUE _gpt_params_get_n_predict(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return INT2NUM(ptr->params.n_predict);
  }

  // n_batch
  static VALUE _gpt_params_set_n_batch(VALUE self, VALUE n_batch) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.n_batch = NUM2INT(n_batch);
    return INT2NUM(ptr->params.n_batch);
  }

  static VALUE _gpt_params_get_n_batch(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return INT2NUM(ptr->params.n_batch);
  }

  // top_k
  static VALUE _gpt_params_set_top_k(VALUE self, VALUE top_k) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.top_k = NUM2INT(top_k);
    return INT2NUM(ptr->params.top_k);
  }

  static VALUE _gpt_params_get_top_k(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return INT2NUM(ptr->params.top_k);
  }

  // top_p
  static VALUE _gpt_params_set_top_p(VALUE self, VALUE top_p) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.top_p = NUM2DBL(top_p);
    return DBL2NUM(ptr->params.top_p);
  }

  static VALUE _gpt_params_get_top_p(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return DBL2NUM(ptr->params.top_p);
  }

  // temp
  static VALUE _gpt_params_set_temp(VALUE self, VALUE temp) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.temp = NUM2DBL(temp);
    return DBL2NUM(ptr->params.temp);
  }

  static VALUE _gpt_params_get_temp(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return DBL2NUM(ptr->params.temp);
  }

  // repeat_last_n
  static VALUE _gpt_params_set_repeat_last_n(VALUE self, VALUE repeat_last_n) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.repeat_last_n = NUM2INT(repeat_last_n);
    return INT2NUM(ptr->params.repeat_last_n);
  }

  static VALUE _gpt_params_get_repeat_last_n(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return INT2NUM(ptr->params.repeat_last_n);
  }

  // repeat_penalty
  static VALUE _gpt_params_set_repeat_penalty(VALUE self, VALUE repeat_penalty) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    ptr->params.repeat_penalty = NUM2DBL(repeat_penalty);
    return DBL2NUM(ptr->params.repeat_penalty);
  }

  static VALUE _gpt_params_get_repeat_penalty(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return DBL2NUM(ptr->params.repeat_penalty);
  }

  // model
  static VALUE _gpt_params_set_model(VALUE self, VALUE model) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    std::string model_str(StringValueCStr(model));
    ptr->params.model = model_str;
    RB_GC_GUARD(model);
    return rb_str_new_cstr(ptr->params.model.c_str());
  }

  static VALUE _gpt_params_get_model(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return rb_str_new_cstr(ptr->params.model.c_str());
  }

  // prompt
  static VALUE _gpt_params_set_prompt(VALUE self, VALUE prompt) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    std::string prompt_str(StringValueCStr(prompt));
    ptr->params.prompt = prompt_str;
    RB_GC_GUARD(prompt);
    return rb_str_new_cstr(ptr->params.prompt.c_str());
  }

  static VALUE _gpt_params_get_prompt(VALUE self) {
    GPTParamsWrapper* ptr = get_gpt_params(self);
    return rb_str_new_cstr(ptr->params.prompt.c_str());
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
  rb_mGPTNeoX = rb_define_module("GPTNeoX");
  rb_mGPTNeoXClient = rb_define_module("GPTNeoXClient");

  RbGPTParams::define_class(rb_mGPTNeoX);
  RbGPTVocab::define_class(rb_mGPTNeoX);
}
