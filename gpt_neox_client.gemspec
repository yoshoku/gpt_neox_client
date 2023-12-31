# frozen_string_literal: true

require_relative 'lib/gpt_neox_client/version'

Gem::Specification.new do |spec|
  spec.name = 'gpt_neox_client'
  spec.version = GPTNeoXClient::VERSION
  spec.authors = ['yoshoku']
  spec.email = ['yoshoku@outlook.com']

  spec.summary = 'gpt_neox_client is a simple client for GPT-NeoX.'
  spec.description = 'gpt_neox_client is a simple client for GPT-NeoX.'
  spec.homepage = 'https://github.com/yoshoku/gpt_neox_client'
  spec.license = 'MIT'

  spec.metadata['homepage_uri'] = spec.homepage
  spec.metadata['source_code_uri'] = spec.homepage
  spec.metadata['changelog_uri'] = "#{spec.homepage}/blob/main/CHANGELOG.md"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(__dir__) do
    `git ls-files -z`.split("\x0")
                     .reject { |f| f.match(%r{\A(?:(?:bin|doc|test|spec|node_modules|pkg|tmp|\.git|\.github|\.husky)/)}) }
                     .select { |f| f.match(/\.(?:rb|rbs|h|hpp|c|cpp|cu|m|metal|md|txt)$/) }
  end
  spec.files.append('ext/gpt_neox_client/src/LICENSE')
  spec.bindir = 'exe'
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']
  spec.extensions = ['ext/gpt_neox_client/extconf.rb']
  spec.metadata['rubygems_mfa_required'] = 'true'
end
