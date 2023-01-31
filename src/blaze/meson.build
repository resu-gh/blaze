lib_blaze_dir = 'blaze'

lib_blaze_incd = [
  'ast',
  'compiler',
  'evaluator',
  'except',
  'lexer',
  'meta',
  'object',
  'parser',
  'random']

lib_blaze_hdrs = [
  'ast/ast.hpp',
  'compiler/compiler.hpp',
  'evaluator/evaluator.hpp',
  'except/except.hpp',
  'except/except.ipp',
  'lexer/lexer.hpp',
  'lexer/token.hpp',
  'meta/format.hpp',
  'meta/format.ipp',
  'object/object.hpp',
  'parser/parser.hpp',
  'parser/precedence.hpp',
  'random/random.hpp']

lib_blaze_srcs = [
  'ast/dice.cpp',
  'ast/float.cpp',
  'ast/infix.cpp',
  'ast/integer.cpp',
  'ast/prefix.cpp',
  'ast/roll.cpp',
  'ast/root.cpp',
  'compiler/compiler.cpp',
  'evaluator/evaluator.cpp',
  'except/except.cpp',
  'lexer/lexer.cpp',
  'lexer/token.cpp',
  'object/dice.cpp',
  'object/float.cpp',
  'object/integer.cpp',
  'object/object.cpp',
  'object/sequence.cpp',
  'parser/parser.cpp',
  'parser/precedence.cpp',
  'random/csprng.cpp',
  'random/prng.cpp']

deps = [
  dependency ('openssl')]

lib_blaze_shared = shared_library (
  'blaze',
  sources : lib_blaze_srcs,
  include_directories : lib_blaze_incd,
  dependencies : deps,
  install : true)

fs = import ('fs')

foreach h : lib_blaze_hdrs
  install_headers (
    h,
    install_dir : lib_blaze_dir / fs.parent (h))
endforeach

pkg_mod = import ('pkgconfig')

pkg_mod.generate (
  libraries : lib_blaze_shared,
  version : '0.1.0',
  name : 'libblaze',
  filebase : 'blaze',
  description : 'A C++ dice rolling library')