## [Unreleased]

## [0.3.0] - 2023-09-06

- Add `embeddings` method.
  ```ruby
  require 'gpt_neox_client'

  client = GPTNeoXClient.new(path: '/path/to/ggml-model-f16.bin', seed: 123456789, n_threads: 8)
  embd = client.embeddings('Hello, world.', normalize: true)
  ```

## [0.2.0] - 2023-09-02

- Add Accelerate framework and Metal build option for macOS.
- Add OpenBLAS build option for platforms other than macOS.

## [0.1.0] - 2023-09-01

- Initial release
