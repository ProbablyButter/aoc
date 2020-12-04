# bootstrap re2c
find_package(BISON)

if (BISON_FOUND)
  option(BOOTSTRAP_RE2C "Build re2c from scratch" ON)
  if (BOOTSTRAP_RE2C)
    message(STATUS "Bootstrapping re2c")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/libre2c_posix")

    # cp ../re2c/re2c/bootstrap/src/options/help.cc src/options/help.cc
    # build/re2c/re2c/src/parse/parser.[h,cc] are bootstrap/src/parse/parser.[h,cc] and src/parse/parser.[h,cc]?
    BISON_TARGET(RE2C_Parser deps/re2c/re2c/src/parse/parser.ypp "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/parser.cc"
      DEFINES_FILE "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/parser.h")

    BISON_TARGET(RE2C_Parse deps/re2c/re2c/libre2c_posix/parse.ypp "${PROJECT_BINARY_DIR}/deps/re2c/libre2c_posix/parse.cc"
      DEFINES_FILE "${PROJECT_BINARY_DIR}/deps/re2c/libre2c_posix/parse.h")

    # build re2c bootstrapped version
    add_executable(re2c_bootstrap
      deps/re2c/re2c/src/adfa/adfa.cc
      deps/re2c/re2c/src/adfa/prepare.cc
      deps/re2c/re2c/src/cfg/cfg.cc
      deps/re2c/re2c/src/cfg/compact.cc
      deps/re2c/re2c/src/cfg/dce.cc
      deps/re2c/re2c/src/cfg/freeze.cc
      deps/re2c/re2c/src/cfg/interfere.cc
      deps/re2c/re2c/src/cfg/liveanal.cc
      deps/re2c/re2c/src/cfg/normalize.cc
      deps/re2c/re2c/src/cfg/optimize.cc
      deps/re2c/re2c/src/cfg/rename.cc
      deps/re2c/re2c/src/cfg/varalloc.cc
      deps/re2c/re2c/src/codegen/bitmap.cc
      deps/re2c/re2c/src/codegen/emit_action.cc
      deps/re2c/re2c/src/codegen/emit_dfa.cc
      deps/re2c/re2c/src/codegen/go_construct.cc
      deps/re2c/re2c/src/codegen/go_destruct.cc
      deps/re2c/re2c/src/codegen/go_emit.cc
      deps/re2c/re2c/src/codegen/go_used_labels.cc
      deps/re2c/re2c/src/codegen/input_api.cc
      deps/re2c/re2c/src/codegen/label.cc
      deps/re2c/re2c/src/codegen/output.cc
      deps/re2c/re2c/src/codegen/print.cc
      deps/re2c/re2c/src/compile.cc
      deps/re2c/re2c/src/debug/dump_adfa.cc
      deps/re2c/re2c/src/debug/dump_cfg.cc
      deps/re2c/re2c/src/debug/dump_dfa.cc
      deps/re2c/re2c/src/debug/dump_interf.cc
      deps/re2c/re2c/src/debug/dump_nfa.cc
      deps/re2c/re2c/src/dfa/closure.cc
      deps/re2c/re2c/src/dfa/closure_leftmost.cc
      deps/re2c/re2c/src/dfa/closure_posix.cc
      deps/re2c/re2c/src/dfa/dead_rules.cc
      deps/re2c/re2c/src/dfa/determinization.cc
      deps/re2c/re2c/src/dfa/fallback_tags.cc
      deps/re2c/re2c/src/dfa/fillpoints.cc
      deps/re2c/re2c/src/dfa/find_state.cc
      deps/re2c/re2c/src/dfa/minimization.cc
      deps/re2c/re2c/src/dfa/posix_precedence.cc
      deps/re2c/re2c/src/dfa/tag_history.cc
      deps/re2c/re2c/src/dfa/tagver_table.cc
      deps/re2c/re2c/src/dfa/tcmd.cc
      deps/re2c/re2c/src/encoding/ebcdic/ebcdic_regexp.cc
      deps/re2c/re2c/src/encoding/enc.cc
      deps/re2c/re2c/src/encoding/range_suffix.cc
      deps/re2c/re2c/src/encoding/utf16/utf16.cc
      deps/re2c/re2c/src/encoding/utf16/utf16_regexp.cc
      deps/re2c/re2c/src/encoding/utf8/utf8.cc
      deps/re2c/re2c/src/encoding/utf8/utf8_regexp.cc
      deps/re2c/re2c/src/main.cc
      deps/re2c/re2c/src/nfa/estimate_size.cc
      deps/re2c/re2c/src/nfa/re_to_nfa.cc
      deps/re2c/re2c/src/options/msg.cc
      deps/re2c/re2c/src/options/opt.cc
      deps/re2c/re2c/src/options/warn.cc
      deps/re2c/re2c/src/parse/ast.cc
      deps/re2c/re2c/src/parse/input.cc
      deps/re2c/re2c/src/parse/normalize.cc
      deps/re2c/re2c/src/parse/scanner.cc
      deps/re2c/re2c/src/parse/unescape.cc
      deps/re2c/re2c/src/parse/validate.cc
      deps/re2c/re2c/src/regexp/ast_to_re.cc
      deps/re2c/re2c/src/regexp/default_tags.cc
      deps/re2c/re2c/src/regexp/fixed_tags.cc
      deps/re2c/re2c/src/regexp/nullable.cc
      deps/re2c/re2c/src/regexp/rule.cc
      deps/re2c/re2c/src/regexp/split_charset.cc
      deps/re2c/re2c/src/regexp/tag.cc
      deps/re2c/re2c/src/skeleton/control_flow.cc
      deps/re2c/re2c/src/skeleton/generate_code.cc
      deps/re2c/re2c/src/skeleton/generate_data.cc
      deps/re2c/re2c/src/skeleton/maxpath.cc
      deps/re2c/re2c/src/skeleton/skeleton.cc
      deps/re2c/re2c/src/util/get_dir.cc
      deps/re2c/re2c/src/util/range.cc
      deps/re2c/re2c/src/util/s_to_n32_unsafe.cc
      
      deps/re2c/re2c/bootstrap/src/options/help.cc
      deps/re2c/re2c/bootstrap/src/options/parse_opts.cc
      deps/re2c/re2c/bootstrap/src/options/ver_to_vernum.cc
      deps/re2c/re2c/bootstrap/src/parse/lex.cc
      deps/re2c/re2c/bootstrap/src/parse/lex_conf.cc
      ${BISON_RE2C_Parser_OUTPUT_SOURCE}
      #deps/re2c/re2c/bootstrap/src/parse/parser.cc
      )
    target_include_directories(re2c_bootstrap
      PRIVATE
      deps/re2c/re2c
      deps/re2c/re2c/bootstrap
      "${PROJECT_BINARY_DIR}/deps/re2c/re2c/"
      "${PROJECT_BINARY_DIR}/deps/re2c/include/")
    
    set_target_properties(re2c_bootstrap
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/bin"
      )

    # build real re2c
    add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex.cc" "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex.h"
      COMMAND "./re2c_bootstrap" "-b" "-W" "-o" "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex.cc" "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/parse/lex.re"
      DEPENDS re2c_bootstrap
      DEPENDS "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/parse/lex.re"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/bin"
      )

    add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex_conf.cc"
      COMMAND "./re2c_bootstrap" "-b" "-W" "-o" "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex_conf.cc" "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/parse/lex_conf.re"
      DEPENDS re2c_bootstrap
      DEPENDS "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/parse/lex_conf.re"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/bin"
      )

    add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options/parse_opts.cc"
      COMMAND "./re2c_bootstrap" "-b" "-W" "-o" "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options/parse_opts.cc" "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/options/parse_opts.re"
      DEPENDS re2c_bootstrap
      DEPENDS "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/options/parse_opts.re"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/bin"
      )

    add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options/ver_to_vernum.cc"
      COMMAND "./re2c_bootstrap" "-b" "-W" "-o" "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options/ver_to_vernum.cc" "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/options/ver_to_vernum.re"
      DEPENDS re2c_bootstrap
      DEPENDS "${PROJECT_SOURCE_DIR}/deps/re2c/re2c/src/options/ver_to_vernum.re"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/deps/re2c/bin"
      )
    set(RE2C_GEN_SOURCES
      "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options/parse_opts.cc"
      "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/options/ver_to_vernum.cc"
      "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex.cc"
      "${PROJECT_BINARY_DIR}/deps/re2c/re2c/src/parse/lex_conf.cc"
      ${BISON_RE2C_Parser_OUTPUT_SOURCE})
  endif()
else()
  set(BOOTSTRAP_RE2C OFF)
endif()

if(NOT BISON_FOUND OR NOT BOOTSTRAP_RE2C)
  set(RE2C_GEN_SOURCES
      "deps/re2c/re2c/bootstrap/src/options/parse_opts.cc"
      "deps/re2c/re2c/bootstrap/src/options/ver_to_vernum.cc"
      "deps/re2c/re2c/bootstrap/src/parse/lex.cc"
      "deps/re2c/re2c/bootstrap/src/parse/lex_conf.cc"
      "deps/re2c/re2c/bootstrap/src/parse/parser.cc")
endif()

configure_file("deps/re2c/re2c_config.h.in"
  "${PROJECT_BINARY_DIR}/deps/re2c/include/re2c/re2c_config.h")

add_executable(re2c
  deps/re2c/re2c/src/adfa/adfa.cc
  deps/re2c/re2c/src/adfa/prepare.cc
  deps/re2c/re2c/src/cfg/cfg.cc
  deps/re2c/re2c/src/cfg/compact.cc
  deps/re2c/re2c/src/cfg/dce.cc
  deps/re2c/re2c/src/cfg/freeze.cc
  deps/re2c/re2c/src/cfg/interfere.cc
  deps/re2c/re2c/src/cfg/liveanal.cc
  deps/re2c/re2c/src/cfg/normalize.cc
  deps/re2c/re2c/src/cfg/optimize.cc
  deps/re2c/re2c/src/cfg/rename.cc
  deps/re2c/re2c/src/cfg/varalloc.cc
  deps/re2c/re2c/src/codegen/bitmap.cc
  deps/re2c/re2c/src/codegen/emit_action.cc
  deps/re2c/re2c/src/codegen/emit_dfa.cc
  deps/re2c/re2c/src/codegen/go_construct.cc
  deps/re2c/re2c/src/codegen/go_destruct.cc
  deps/re2c/re2c/src/codegen/go_emit.cc
  deps/re2c/re2c/src/codegen/go_used_labels.cc
  deps/re2c/re2c/src/codegen/input_api.cc
  deps/re2c/re2c/src/codegen/label.cc
  deps/re2c/re2c/src/codegen/output.cc
  deps/re2c/re2c/src/codegen/print.cc
  deps/re2c/re2c/src/compile.cc
  deps/re2c/re2c/src/debug/dump_adfa.cc
  deps/re2c/re2c/src/debug/dump_cfg.cc
  deps/re2c/re2c/src/debug/dump_dfa.cc
  deps/re2c/re2c/src/debug/dump_interf.cc
  deps/re2c/re2c/src/debug/dump_nfa.cc
  deps/re2c/re2c/src/dfa/closure.cc
  deps/re2c/re2c/src/dfa/closure_leftmost.cc
  deps/re2c/re2c/src/dfa/closure_posix.cc
  deps/re2c/re2c/src/dfa/dead_rules.cc
  deps/re2c/re2c/src/dfa/determinization.cc
  deps/re2c/re2c/src/dfa/fallback_tags.cc
  deps/re2c/re2c/src/dfa/fillpoints.cc
  deps/re2c/re2c/src/dfa/find_state.cc
  deps/re2c/re2c/src/dfa/minimization.cc
  deps/re2c/re2c/src/dfa/posix_precedence.cc
  deps/re2c/re2c/src/dfa/tag_history.cc
  deps/re2c/re2c/src/dfa/tagver_table.cc
  deps/re2c/re2c/src/dfa/tcmd.cc
  deps/re2c/re2c/src/encoding/ebcdic/ebcdic_regexp.cc
  deps/re2c/re2c/src/encoding/enc.cc
  deps/re2c/re2c/src/encoding/range_suffix.cc
  deps/re2c/re2c/src/encoding/utf16/utf16.cc
  deps/re2c/re2c/src/encoding/utf16/utf16_regexp.cc
  deps/re2c/re2c/src/encoding/utf8/utf8.cc
  deps/re2c/re2c/src/encoding/utf8/utf8_regexp.cc
  deps/re2c/re2c/src/main.cc
  deps/re2c/re2c/src/nfa/estimate_size.cc
  deps/re2c/re2c/src/nfa/re_to_nfa.cc
  deps/re2c/re2c/src/options/msg.cc
  deps/re2c/re2c/src/options/opt.cc
  deps/re2c/re2c/src/options/warn.cc
  deps/re2c/re2c/src/parse/ast.cc
  deps/re2c/re2c/src/parse/input.cc
  deps/re2c/re2c/src/parse/normalize.cc
  deps/re2c/re2c/src/parse/scanner.cc
  deps/re2c/re2c/src/parse/unescape.cc
  deps/re2c/re2c/src/parse/validate.cc
  deps/re2c/re2c/src/regexp/ast_to_re.cc
  deps/re2c/re2c/src/regexp/default_tags.cc
  deps/re2c/re2c/src/regexp/fixed_tags.cc
  deps/re2c/re2c/src/regexp/nullable.cc
  deps/re2c/re2c/src/regexp/rule.cc
  deps/re2c/re2c/src/regexp/split_charset.cc
  deps/re2c/re2c/src/regexp/tag.cc
  deps/re2c/re2c/src/skeleton/control_flow.cc
  deps/re2c/re2c/src/skeleton/generate_code.cc
  deps/re2c/re2c/src/skeleton/generate_data.cc
  deps/re2c/re2c/src/skeleton/maxpath.cc
  deps/re2c/re2c/src/skeleton/skeleton.cc
  deps/re2c/re2c/src/util/get_dir.cc
  deps/re2c/re2c/src/util/range.cc
  deps/re2c/re2c/src/util/s_to_n32_unsafe.cc
  
  deps/re2c/re2c/bootstrap/src/options/help.cc
  "${RE2C_GEN_SOURCES}"
)
target_include_directories(re2c PRIVATE deps/re2c/re2c
  PUBLIC
  "${PROJECT_BINARY_DIR}/deps/re2c/include"
  )
if (BISON_FOUND AND BOOTSTRAP_RE2C)
  target_include_directories(re2c PRIVATE "${PROJECT_BINARY_DIR}/deps/re2c/re2c/")
else()
  target_include_directories(re2c PRIVATE "deps/re2c/re2c/bootstrap")
endif()

set_target_properties(re2c
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
  )
if(WIN32)
  set(AOC_RE2C_EXEC "${PROJECT_BINARY_DIR}/bin/$<CONFIG>/re2c.exe")
else()
  set(AOC_RE2C_EXEC "${PROJECT_BINARY_DIR}/bin/re2c")
endif()

add_library(whereami deps/whereami/src/whereami.c)
target_include_directories(whereami PUBLIC deps/whereami/src)

