blaze_srcs = [
  'blaze.cpp']

blaze_incd = []
foreach d : lib_blaze_incd
  blaze_incd += '..' / lib_blaze_dir / d
endforeach

blaze = executable(
  'blaze',
  sources : blaze_srcs,
  include_directories : blaze_incd,
  link_with : lib_blaze_shared,
  install : true)
