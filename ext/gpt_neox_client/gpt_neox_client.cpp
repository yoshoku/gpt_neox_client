#include "gpt_neox_client.h"

VALUE rb_cGPTNeoXClient;
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

static VALUE gpt_neox_client_initialize(int argc, VALUE* argv, VALUE self) {
  VALUE kw_args = Qnil;
  ID kw_table[3] = { rb_intern("path"), rb_intern("seed"), rb_intern("n_threads") };
  VALUE kw_values[3] = { Qundef, Qundef, Qundef };
  rb_scan_args(argc, argv, ":", &kw_args);
  rb_get_kwargs(kw_args, kw_table, 1, 2, kw_values);

  if (!RB_TYPE_P(kw_values[0], T_STRING)) {
    rb_raise(rb_eArgError, "path must be a String");
    return Qnil;
  }
  if (kw_values[1] != Qundef && !RB_NIL_P(kw_values[1]) && !RB_INTEGER_TYPE_P(kw_values[1])) {
    rb_raise(rb_eArgError, "seed must be an integer");
    return Qnil;
  }
  if (RB_INTEGER_TYPE_P(kw_values[1]) && NUM2INT(kw_values[1]) < 0) {
    rb_raise(rb_eArgError, "seed must be an integer greater than or equal to zero");
    return Qnil;
  }
  if (kw_values[2] != Qundef && !RB_NIL_P(kw_values[2]) && !RB_INTEGER_TYPE_P(kw_values[2])) {
    rb_raise(rb_eArgError, "n_threads must be an integer");
    return Qnil;
  }
  if (RB_INTEGER_TYPE_P(kw_values[2]) && NUM2INT(kw_values[2]) < 1) {
    rb_raise(rb_eArgError, "n_threads must be a positive integer");
    return Qnil;
  }

  std::string path(StringValueCStr(kw_values[0]));
  std::random_device rnd;
  const unsigned int seed = RB_INTEGER_TYPE_P(kw_values[1]) ? NUM2INT(kw_values[1]) : rnd();
  const unsigned int n_threads_ = RB_INTEGER_TYPE_P(kw_values[2]) ? NUM2INT(kw_values[2]) : 1;
  const unsigned int n_threads = std::min(n_threads_, std::thread::hardware_concurrency());

  rb_iv_set(self, "@path", kw_values[0]);
  rb_iv_set(self, "@seed", UINT2NUM(seed));
  rb_iv_set(self, "@n_threads", UINT2NUM(n_threads));

  rb_iv_set(self, "@vocab", rb_funcall(rb_const_get(rb_cGPTNeoXClient, rb_intern("Vocab")), rb_intern("new"), 0));
  rb_iv_set(self, "@model", rb_funcall(rb_const_get(rb_cGPTNeoXClient, rb_intern("Model")), rb_intern("new"), 0));

  gpt_neox_model* model = RbGPTNeoXModel::get_gpt_neox_model(rb_iv_get(self, "@model"));
  gpt_vocab* vocab = RbGPTVocab::get_gpt_vocab(rb_iv_get(self, "@vocab"));

  if (!gpt_neox_model_load(path, *model, *vocab)) {
    rb_raise(rb_eRuntimeError, "failed to load model: %s", path.c_str());
    return Qnil;
  }

  return self;
}

static VALUE gpt_neox_client_completions(int argc, VALUE* argv, VALUE self) {
  VALUE prompt_ = Qnil;
  VALUE kw_args = Qnil;
  rb_scan_args(argc, argv, "1:", &prompt_, &kw_args);

  ID kw_table[7] = { rb_intern("top_k"), rb_intern("top_p"), rb_intern("temperature"),
                     rb_intern("n_predict"), rb_intern("n_batch"),
                     rb_intern("repeat_last_n"), rb_intern("repeat_penalty") };
  VALUE kw_values[7] = { Qundef, Qundef, Qundef, Qundef, Qundef, Qundef, Qundef };
  rb_get_kwargs(kw_args, kw_table, 0, 7, kw_values);

  if (kw_values[0] != Qundef && !RB_INTEGER_TYPE_P(kw_values[0])) {
    rb_raise(rb_eArgError, "top_k must be an integer");
    return Qnil;
  }
  if (kw_values[1] != Qundef && !RB_FLOAT_TYPE_P(kw_values[1])) {
    rb_raise(rb_eArgError, "top_p must be a float");
    return Qnil;
  }
  if (kw_values[2] != Qundef && !RB_FLOAT_TYPE_P(kw_values[2])) {
    rb_raise(rb_eArgError, "temp must be a float");
    return Qnil;
  }
  if (kw_values[3] != Qundef && !RB_INTEGER_TYPE_P(kw_values[3])) {
    rb_raise(rb_eArgError, "n_predict must be an integer");
    return Qnil;
  }
  if (kw_values[4] != Qundef && !RB_INTEGER_TYPE_P(kw_values[4])) {
    rb_raise(rb_eArgError, "n_batch must be an integer");
    return Qnil;
  }
  if (kw_values[5] != Qundef && !RB_INTEGER_TYPE_P(kw_values[5])) {
    rb_raise(rb_eArgError, "repeat_last_n must be an integer");
    return Qnil;
  }
  if (kw_values[6] != Qundef && !RB_FLOAT_TYPE_P(kw_values[6])) {
    rb_raise(rb_eArgError, "repeat_penalty must be a float");
    return Qnil;
  }

  std::string prompt(StringValueCStr(prompt_));
  const int top_k = kw_values[0] != Qundef ? NUM2INT(kw_values[0]) : 40;
  const double top_p = kw_values[1] != Qundef ? NUM2DBL(kw_values[1]) : 0.9;
  const double temp = kw_values[2] != Qundef ? NUM2DBL(kw_values[2]) : 0.9;
  const int n_predict_ = kw_values[3] != Qundef ? NUM2INT(kw_values[3]) : 200;
  const int n_batch = kw_values[4] != Qundef ? NUM2INT(kw_values[4]) : 8;
  const int repeat_last_n = kw_values[5] != Qundef ? NUM2INT(kw_values[5]) : 64;
  const float repeat_penalty = kw_values[6] != Qundef ? NUM2DBL(kw_values[6]) : 1.0f;

  gpt_neox_model* model = RbGPTNeoXModel::get_gpt_neox_model(rb_iv_get(self, "@model"));
  gpt_vocab* vocab = RbGPTVocab::get_gpt_vocab(rb_iv_get(self, "@vocab"));

  std::vector<gpt_vocab::id> embd_inp = gpt_tokenize(*vocab, prompt);
  const int n_predict = std::min(n_predict_, model->hparams.n_ctx - static_cast<int>(embd_inp.size()));

  const int n_threads = NUM2INT(rb_iv_get(self, "@n_threads"));
  std::vector<float> logits;
  size_t mem_per_token = 0;
  gpt_neox_eval(*model, n_threads, 0, { 0, 1, 2, 3 }, logits, mem_per_token);

  int n_past = 0;
  int n_consumed = 0;
  int n_sampled = 0;
  std::string completions = "";
  const unsigned int seed = NUM2UINT(rb_iv_get(self, "@seed"));
  std::mt19937 rng(seed);
  std::vector<gpt_vocab::id> embd;
  std::vector<int32_t> last_n_tokens(model->hparams.n_ctx, 0);

  while (n_sampled < n_predict) {
    if (embd.size() > 0) {
      if (!gpt_neox_eval(*model, n_threads, n_past, embd, logits, mem_per_token)) {
        rb_raise(rb_eRuntimeError, "failed to predict.");
        return Qnil;
      }
      n_past += embd.size();
      embd.clear();
    }

    if (embd_inp.size() <= n_consumed) {
      gpt_vocab::id id = gpt_sample_top_k_top_p_repeat(
        *vocab,
        logits.data() + (logits.size() - model->hparams.n_vocab),
        last_n_tokens.data(), last_n_tokens.size(),
        top_k, top_p, temp,
        repeat_last_n, repeat_penalty,
        rng);
      last_n_tokens.erase(last_n_tokens.begin());
      last_n_tokens.push_back(id);
      embd.push_back(id);
      n_sampled += 1;
    } else {
      while (embd_inp.size() > n_consumed) {
        embd.push_back(embd_inp[n_consumed]);
        n_consumed += 1;
        if (embd.size() >= n_batch) break;
      }
    }

    for (auto id : embd) completions += vocab->id_to_token[id];
    if (embd.back() == 0) break;
  }

  RB_GC_GUARD(prompt_);
  return rb_utf8_str_new_cstr(completions.c_str());
}

extern "C" void Init_gpt_neox_client(void) {
  rb_cGPTNeoXClient = rb_define_class("GPTNeoXClient", rb_cObject);
  rb_define_method(rb_cGPTNeoXClient, "initialize", RUBY_METHOD_FUNC(gpt_neox_client_initialize), -1);
  rb_define_method(rb_cGPTNeoXClient, "completions", RUBY_METHOD_FUNC(gpt_neox_client_completions), -1);
  rb_define_attr(rb_cGPTNeoXClient, "path", 1, 0);
  rb_define_attr(rb_cGPTNeoXClient, "seed", 1, 0);
  rb_define_attr(rb_cGPTNeoXClient, "n_threads", 1, 0);

  RbGPTVocab::define_class(rb_cGPTNeoXClient);
  RbGPTNeoXModel::define_class(rb_cGPTNeoXClient);
}
