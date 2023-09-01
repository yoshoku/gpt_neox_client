# GPTNeoXClient

[![Gem Version](https://badge.fury.io/rb/gpt_neox_client.svg)](https://badge.fury.io/rb/gpt_neox_client)
[![License](https://img.shields.io/badge/License-MIT-yellowgreen.svg)](https://github.com/yoshoku/gpt_neox_client/blob/main/LICENSE.txt)
[![Documentation](https://img.shields.io/badge/api-reference-blue.svg)](https://yoshoku.github.io/gpt_neox_client/doc/)

gpt_neox_client is a simple client for GPT-NeoX.

## Installation

Install the gem and add to the application's Gemfile by executing:

    $ bundle add gpt_neox_client

If bundler is not being used to manage dependencies, install the gem by executing:

    $ gem install gpt_neox_client

## Usage

GPTNeoXClient supports ggml format model. Model files need to be converted to ggml format.
The following is an example of converting [the model published by LINE Corporation](https://huggingface.co/line-corporation/japanese-large-lm-3.6b-instruction-sft):

```sh
# Clone ggml.
$ git clone https://github.com/ggerganov/ggml.git
$ cd ggml

# Install python dependencies.
$ pip install -U protobuf~=3.20.0
$ python -m pip install -r requirements.txt

# Convert downloaded model to ggml format.
$ git clone https://huggingface.co/line-corporation/japanese-large-lm-3.6b-instruction-sft
$ python examples/gpt-neox/convert-h5-to-ggml.py japanese-large-lm-3.6b-instruction-sft 1
$ ls japanese-large-lm-3.6b-instruction-sft/ggml-model-f16.bin
japanese-large-lm-3.6b-instruction-sft/ggml-model-f16.bin
```

```ruby
require "gpt_neox_client"

client = GPTNeoXClient.new('japanese-large-lm-3.6b-instruction-sft/ggml-model-f16.bin', seed: 123456789, n_threads: 4)
puts client.completions(
  'ユーザー:四国の県名を全て列挙してください。<0x0A>システム:',
  top_p: 0.9,
  top_k: 1,
  temperature: 0.7
).gsub("<0x0A>", "\n").gsub("</s>", " ")
#
# ユーザー:四国の県名を全て列挙してください。
# システム:徳島県、香川県、愛媛県、高知県
```

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/yoshoku/gpt_neox_client.
This project is intended to be a safe, welcoming space for collaboration,
and contributors are expected to adhere to the [code of conduct](https://github.com/yoshoku/gpt_neox_client/blob/main/CODE_OF_CONDUCT.md).

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Code of Conduct

Everyone interacting in the GPTNeoXClient project's codebases, issue trackers,
chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/yoshoku/gpt_neox_client/blob/main/CODE_OF_CONDUCT.md).
