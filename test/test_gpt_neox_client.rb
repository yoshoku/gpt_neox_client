# frozen_string_literal: true

require 'test_helper'

class TestGPTNeoXClient < Minitest::Test
  def test_version_numbers
    refute_nil(::GPTNeoXClient::VERSION)
  end
end
