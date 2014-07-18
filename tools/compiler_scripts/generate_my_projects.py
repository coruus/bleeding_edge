#!/usr/bin/env python
# Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
# for details. All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.

import os
import sys


def Main():
  def normjoin(*args):
    return os.path.normpath(os.path.join(*args))

  dart_dir = normjoin(__file__, '..', '..', '..')
  compiler_scripts = normjoin(dart_dir, 'tools', 'compiler_scripts')
  editor = normjoin(dart_dir, 'editor')

  locations = {
    'compiler_scripts': compiler_scripts,
    'editor': editor,
  }

  generate_source_list_calls = [
    # The paths are relative to dart/editor/
    {
        "name" : "plugin_engine_java",
        "output" : "%(editor)s/plugin_engine_sources" % locations,
        "path" : "tools/plugins/com.google.dart.engine",
    },
    {
        "name" : "plugin_command_analyze_java",
        "output" : "%(editor)s/plugin_command_analyze_sources" % locations,
        "path" : "tools/plugins/com.google.dart.command.analyze",
    },
  ]

  for call_options in generate_source_list_calls:
    command = (("python %(compiler_scripts)s/generate_source_list.py "
                % locations) +
               ("%(name)s %(output)s %(path)s" % call_options))
    exit_code = os.system(command)
    if exit_code:
      return exit_code

  if '--no-gyp' in sys.argv:
    print '--no-gyp is deprecated.'

  return 0


if __name__ == '__main__':
  sys.exit(Main())
