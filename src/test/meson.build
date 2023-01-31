deps = [
  dependency ('gtest'),
  dependency ('libglog')]

lexer_test_srcs = [
  'lexer_test.cpp']

lexer_test_incd = []
foreach d : lib_blaze_incd
  lexer_test_incd += '..' / lib_blaze_dir / d
endforeach

lexer_test = executable(
  'lexer_test',
  sources : lexer_test_srcs,
  include_directories : lexer_test_incd,
  dependencies : deps,
  link_with : lib_blaze_shared,
  install : false)

test ('lexer test', lexer_test)

parser_test_srcs = [
  'parser_test.cpp']

parser_test_incd = []
foreach d : lib_blaze_incd
  parser_test_incd += '..' / lib_blaze_dir / d
endforeach

parser_test = executable(
  'parser_test',
  sources : parser_test_srcs,
  include_directories : parser_test_incd,
  dependencies : deps,
  link_with : lib_blaze_shared,
  install : false)

test ('parser test', parser_test)

evaluator_test_srcs = [
  'evaluator_test.cpp']

evaluator_test_incd = []
foreach d : lib_blaze_incd
  evaluator_test_incd += '..' / lib_blaze_dir / d
endforeach

evaluator_test = executable(
  'evaluator_test',
  sources : evaluator_test_srcs,
  include_directories : evaluator_test_incd,
  dependencies : deps,
  link_with : lib_blaze_shared,
  install : false)

test ('evaluator test', evaluator_test)
