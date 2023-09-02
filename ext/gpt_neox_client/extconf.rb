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

$srcs = %w[ggml.c ggml-alloc.c common-ggml.cpp common.cpp gpt_neox_client.cpp]
$CFLAGS << ' -w -DNDEBUG'
$CXXFLAGS << ' -std=c++11 -DNDEBUG'
$INCFLAGS << ' -I$(srcdir)/src'
$INCFLAGS << ' -I$(srcdir)/src/ggml'
$VPATH << '$(srcdir)/src'
$VPATH << '$(srcdir)/src/ggml'

if RUBY_PLATFORM.match?(/darwin|linux|bsd/) && try_compile('#include <stdio.h>', '-pthread')
  $CFLAGS << ' -pthread'
  $CXXFLAGS << ' -pthread'
end

if RUBY_PLATFORM.match?(/darwin/)
  if have_framework('Accelerate')
    $CFLAGS << ' -DGGML_USE_ACCELERATE'
  else
    warning 'Accelerate framework is not found.'
  end
end

$CFLAGS << ' -DGGML_USE_OPENBLAS' if !RUBY_PLATFORM.match?(/darwin/) && (have_library('openblas') && have_header('cblas.h'))

create_makefile('gpt_neox_client/gpt_neox_client')

if RUBY_PLATFORM.match?(/darwin/)
  File.open('Makefile', 'a') do |f|
    f.puts "\nggml-metal.o: ggml-metal.m ggml-metal.h"
    f.puts "\t$(CC) $(CFLAGS) -c $< -o $@"
  end

  metal_path = File.expand_path("#{__dir__}/src/ggml/ggml-metal.metal")
  dest_path = File.expand_path("#{__dir__}/../../lib/gpt_neox_client/")
  FileUtils.cp(metal_path, dest_path)
end
