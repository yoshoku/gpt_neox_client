# frozen_string_literal: true

require 'mkmf'

abort 'libstdc++ is not found.' unless have_library('stdc++')

File.open("#{__dir__}/src/main.hpp", 'w') do |fout|
  File.open("#{__dir__}/src/main.cpp", 'r') do |fin|
    fin.each_line do |line|
      break if line.match?(/^int main/)

      fout.puts line
    end
  end
end

$srcs = %w[ggml.c ggml-alloc.c common-ggml.cpp gpt_neox_client.cpp]
$CFLAGS << ' -w -DNDEBUG'
$CXXFLAGS << ' -std=c++11 -DNDEBUG'
$INCFLAGS << ' -I$(srcdir)/src'
$INCFLAGS << ' -I$(srcdir)/src/ggml'
$VPATH << '$(srcdir)/src'
$VPATH << '$(srcdir)/src/ggml'

create_makefile('gpt_neox_client/gpt_neox_client')
