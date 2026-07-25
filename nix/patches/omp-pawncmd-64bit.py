#!/usr/bin/env python3
"""Patch Pawn.CMD script.cc for 64-bit pointer safety."""

with open('src/script.cc', 'r') as f:
    src = f.read()

# Add handle map after include
src = src.replace(
    '#include "main.h"\n',
    '#include "main.h"\n\nstatic std::unordered_map<cell, CmdArrayPtr> g_array_handles;\nstatic cell g_next_handle_ = 1;\n'
)

# Patch NewCmdArray (first occurrence)
src = src.replace(
    '  cmd_arrays_.insert(arr);\n\n  return reinterpret_cast<cell>(arr.get());',
    '  cmd_arrays_.insert(arr);\n  cell handle = g_next_handle_++;\n  g_array_handles[handle] = arr;\n  return handle;',
    1
)

# Patch NewAliasArray (second occurrence)
src = src.replace(
    '  cmd_arrays_.insert(arr);\n\n  return reinterpret_cast<cell>(arr.get());',
    '  cmd_arrays_.insert(arr);\n  cell handle = g_next_handle_++;\n  g_array_handles[handle] = arr;\n  return handle;',
    1
)

# Patch DeleteArray
src = src.replace(
    'void Script::DeleteArray(cell arr) { cmd_arrays_.erase(GetCmdArray(arr)); }',
    'void Script::DeleteArray(cell arr) {\n'
    '  auto it = g_array_handles.find(arr);\n'
    '  if (it != g_array_handles.end()) {\n'
    '    cmd_arrays_.erase(it->second);\n'
    '    g_array_handles.erase(it);\n'
    '  }\n'
    '}'
)

# Patch GetCmdArray
old_get = (
    'const CmdArrayPtr &Script::GetCmdArray(cell ptr) {\n'
    '  const auto iter = std::find_if(\n'
    '      cmd_arrays_.begin(), cmd_arrays_.end(),\n'
    '      [ptr](const auto &p) { return reinterpret_cast<cell>(p.get()) == ptr; });\n'
    '\n'
    '  if (iter == cmd_arrays_.end()) {\n'
    '    throw std::runtime_error{"Invalid array handle"};\n'
    '  }\n'
    '\n'
    '  return *iter;\n'
    '}'
)

new_get = (
    'const CmdArrayPtr &Script::GetCmdArray(cell ptr) {\n'
    '  auto handle_it = g_array_handles.find(ptr);\n'
    '  if (handle_it == g_array_handles.end()) {\n'
    '    throw std::runtime_error{"Invalid array handle"};\n'
    '  }\n'
    '  const auto iter = cmd_arrays_.find(handle_it->second);\n'
    '  if (iter == cmd_arrays_.end()) {\n'
    '    throw std::runtime_error{"Invalid array handle"};\n'
    '  }\n'
    '  return *iter;\n'
    '}'
)

src = src.replace(old_get, new_get)

with open('src/script.cc', 'w') as f:
    f.write(src)

print('script.cc patched for 64-bit handles')
