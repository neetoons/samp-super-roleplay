#!/usr/bin/env python3
"""
Pawn source code formatter for SRP gamemode.

Transformations:
1. Tabs → 4 spaces
2. Remove trailing whitespace
3. Space after if/for/while/switch keywords
4. Remove space before++/-- operators
5. Allman brace style (block-opening { on its own line)
6. else on its own line (Allman convention)
7. Braces around case bodies in switch
8. Break long lines (>120 chars) for conditions
"""

import re
import sys
import os


def stateful_transform(text, fn):
    """Apply fn to code segments, skipping strings and comments."""
    result = []
    i = 0
    state = 'code'
    code_start = 0

    def emit_code(end):
        if end > code_start:
            segment = text[code_start:end]
            result.append(fn(segment))
        return end

    while i < len(text):
        ch = text[i]

        if state == 'line_comment':
            if ch == '\n':
                state = 'code'
                code_start = i + 1
            result.append(ch)
            i += 1
            continue

        if state == 'block_comment':
            if ch == '*' and i + 1 < len(text) and text[i + 1] == '/':
                result.append('*/')
                i += 2
                state = 'code'
                code_start = i
                continue
            result.append(ch)
            i += 1
            continue

        if state == 'string':
            if ch == '\\':
                result.append(ch)
                i += 1
                if i < len(text):
                    result.append(text[i])
                    i += 1
                continue
            if ch == '"':
                state = 'code'
                code_start = i + 1
            result.append(ch)
            i += 1
            continue

        # state == 'code'
        if ch == '/' and i + 1 < len(text):
            if text[i + 1] == '/':
                emit_code(i)
                state = 'line_comment'
                result.append('//')
                i += 2
                continue
            if text[i + 1] == '*':
                emit_code(i)
                state = 'block_comment'
                result.append('/*')
                i += 2
                continue

        if ch == '"':
            emit_code(i)
            state = 'string'
            result.append(ch)
            i += 1
            continue

        if ch == "'":
            emit_code(i)
            result.append(ch)
            i += 1
            while i < len(text) and text[i] != "'":
                if text[i] == '\\':
                    result.append(text[i])
                    i += 1
                if i < len(text):
                    result.append(text[i])
                    i += 1
            if i < len(text):
                result.append(text[i])
                i += 1
            code_start = i
            continue

        if ch == '#':
            emit_code(i)
            j = i
            while j < len(text) and text[j] != '\n':
                if text[j] == '\\' and j + 1 < len(text) and text[j + 1] == '\n':
                    j += 2
                    continue
                if text[j] == '/' and j + 1 < len(text) and text[j + 1] in '/*':
                    break
                j += 1
            result.append(text[i:j])
            if j < len(text) and text[j] == '\n':
                result.append('\n')
                j += 1
            i = j
            code_start = i
            continue

        i += 1

    emit_code(i)
    return ''.join(result)


def step_tabs(text):
    return text.replace('\t', '    ')


def step_trailing_whitespace(text):
    return re.sub(r'[ \t]+$', '', text, flags=re.MULTILINE)


def step_blank_lines(text):
    return re.sub(r'\n{3,}', '\n\n', text)


def step_function_spacing(text):
    return re.sub(
        r'^}([ \t]*)\n(?=\S)(?!(?:else|case\s|default\s*:|catch|#|}))',
        r'}\n\n\1',
        text,
        flags=re.MULTILINE,
    )


def step_keyword_space(text):
    text = re.sub(r'\b(if|for|while|switch)\s*\(', r'\1 (', text)
    return text


def step_increment(text):
    text = re.sub(r'(\w+)\s\+\+', r'\1++', text)
    text = re.sub(r'(\w+)\s--', r'\1--', text)
    return text


def step_allman_braces(text):
    """
    Convert OTBS to Allman: move block-opening { to a new line.

    Preserves the indentation of the opening statement.

    Sequence:
    1. `} else {` → `}\\n<indent>else\\n<indent>{`
    2. `} else`  → `}\\n<indent>else`
    3. `) {`     → `)\\n<indent>{`
    4. `else {`  → `else\\n<indent>{`
    5. `do {`    → `do\\n<indent>{`
    """

    def _indent(m):
        ws = m.group(1)
        return m.group(0).replace(m.group(2), ws + m.group(2), 1)

    def transform(code):
        code = re.sub(
            r'^([ \t]*)\}[^\S\n]+(else)[^\S\n]*\{',
            lambda m: m.group(1) + '}\n' + m.group(1) + m.group(2) + '\n' + m.group(1) + '{',
            code,
            flags=re.MULTILINE,
        )
        code = re.sub(
            r'^([ \t]*)\}[^\S\n]+(else\b)(?![^\S\n]*\{)',
            lambda m: m.group(1) + '}\n' + m.group(1) + m.group(2),
            code,
            flags=re.MULTILINE,
        )
        code = re.sub(
            r'^([ \t]*)(.*)\)[^\S\n]*\{',
            lambda m: m.group(1) + m.group(2) + ')\n' + m.group(1) + '{',
            code,
            flags=re.MULTILINE,
        )
        code = re.sub(
            r'^([ \t]*)(else)[^\S\n]*\{',
            lambda m: m.group(1) + m.group(2) + '\n' + m.group(1) + '{',
            code,
            flags=re.MULTILINE,
        )
        code = re.sub(
            r'^([ \t]*)(do)[^\S\n]*\{',
            lambda m: m.group(1) + m.group(2) + '\n' + m.group(1) + '{',
            code,
            flags=re.MULTILINE,
        )
        return code

    return stateful_transform(text, transform)


def step_case_braces(text):
    """
    Wrap case bodies in braces.

    case X: stmt;  ->  case X: { stmt; }
    case X:\n  body  ->  case X: {\n  body\n}
    Leaves already-braced cases alone.
    """

    def transform(code):
        lines = code.split('\n')
        result = []
        i = 0
        while i < len(lines):
            line = lines[i]
            m = re.match(r'^(\s*)((?:case\s+[^:]+|default)\s*:\s*)(.*)', line)
            if m:
                indent = m.group(1)
                case_hdr = m.group(2)
                body = m.group(3).strip()

                if body.startswith('{'):
                    # Already braced, skip
                    result.append(line)
                    i += 1
                    continue

                if body == '' or body.startswith('//'):
                    # Body on next line(s). Collect until next case/default or closing }
                    body_lines = []
                    j = i + 1
                    while j < len(lines):
                        stripped = lines[j].strip()
                        if re.match(r'^(case\s|default\s*:)', stripped):
                            break
                        if stripped == '}' or stripped.startswith('}'):
                            break
                        body_lines.append(lines[j])
                        j += 1

                    if body_lines:
                        # If already Allman-braced (first body line is just {), skip
                        first_line = body_lines[0].strip()
                        if first_line == '{' or first_line.startswith('{'):
                            result.append(line)
                            i += 1
                            continue
                        result.append(f"{indent}{case_hdr}{{")
                        for bl in body_lines:
                            result.append(bl)
                        result.append(f"{indent}}}")
                        i = j
                        continue
                    else:
                        result.append(line)
                        i += 1
                        continue
                else:
                    # Single-line case body — leave as-is
                    result.append(line)
                    i += 1
                    continue

            result.append(line)
            i += 1
        return '\n'.join(result)

    return stateful_transform(text, transform)


def step_call_continuation_indent(text):
    def _parens_in_line(line, start_in_string=False):
        count = 0
        in_string = start_in_string
        i = 0
        while i < len(line):
            ch = line[i]
            if ch == '\\':
                i += 1
                if i < len(line) and line[i] in '\\"ntr':
                    i += 1
                continue
            if ch == '"':
                in_string = not in_string
            elif not in_string and ch == '(':
                count += 1
            elif not in_string and ch == ')':
                count -= 1
            i += 1
        return count, in_string

    lines = text.split('\n')
    result = list(lines)

    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()
        if not stripped or stripped.startswith('//') or stripped.startswith('/*') or stripped.startswith('#'):
            i += 1
            continue

        net, _ = _parens_in_line(stripped)

        if net > 0:
            base_indent = re.match(r'^(\s*)', line).group(1)
            base_len = len(base_indent)
            target_arg = ' ' * (base_len + 4)

            depth = net
            in_string = False  # track string state across lines
            j = i + 1
            while j < len(lines) and depth > 0:
                cur = lines[j]
                cur_strip = cur.strip()

                if not cur_strip or cur_strip.startswith('//') or cur_strip.startswith('/*') or cur_strip.startswith('#'):
                    j += 1
                    continue

                cur_net, in_string = _parens_in_line(cur_strip, in_string)

                cur_indent = re.match(r'^(\s*)', cur).group(1)
                rest = cur[len(cur_indent):]

                if depth + cur_net <= 0:
                    stripped_rest = rest.strip()
                    content_part = stripped_rest.rstrip(');,')
                    if content_part.strip():
                        result[j] = target_arg + stripped_rest
                    else:
                        result[j] = base_indent + stripped_rest
                else:
                    result[j] = target_arg + rest.strip()

                depth += cur_net
                j += 1

            i = j
        else:
            i += 1

    return '\n'.join(result)


def step_indent_braces(text):
    """
    Re-indent lone `{` to match the indentation of the preceding statement.

    For multi-line conditions walks back to find the control-flow keyword
    (if/for/while/switch) and uses its indentation.
    """
    lines = text.split('\n')
    result = list(lines)
    ctrl_re = re.compile(r'\b(if|for|while|switch)\s*\(')
    scope_re = re.compile(r'^[ \t]*[{}][ \t]*$')
    case_re = re.compile(r'^\s*(case\s|default\s*:)')

    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped != '{':
            continue

        j = i - 1
        while j >= 0 and lines[j].strip() == '':
            j -= 1
        if j < 0:
            continue

        indent = re.match(r'^(\s*)', lines[j]).group(1)
        indent_len = len(indent)

        # Walk back looking for a control keyword at lesser indent.
        # Stop at scope boundaries and case/default labels.
        while j > 0:
            k = j - 1
            while k >= 0 and lines[k].strip() == '':
                k -= 1
            if k < 0:
                break
            if scope_re.match(lines[k]) or case_re.match(lines[k]):
                break
            prev_indent = len(re.match(r'^(\s*)', lines[k]).group(1))
            if prev_indent <= indent_len:
                if prev_indent < indent_len and ctrl_re.search(lines[k]):
                    indent = re.match(r'^(\s*)', lines[k]).group(1)
                indent_len = prev_indent
                j = k
            else:
                break

        result[i] = indent + '{'
    return '\n'.join(result)


def step_long_lines(text, max_len=120):

    def transform(code):
        lines = code.split('\n')
        result = []
        for line in lines:
            if len(line) <= max_len:
                result.append(line)
                continue

            # Try to break long if/for/while conditions
            m = re.match(r'^(\s*(?:if|for|while)\s*\()(.+)(\)\s*\{?\s*)$', line)
            if m:
                indent = m.group(1)
                condition = m.group(2)
                suffix = m.group(3)

                parts = re.split(r'(\s*(?:&&|\|\|)\s*)', condition)
                if len(parts) > 3:
                    new_line = indent
                    for k, part in enumerate(parts):
                        if not part:
                            continue
                        if part.strip() in ('&&', '||'):
                            new_line += part
                        elif k == 1:
                            new_line += part
                        else:
                            new_line += '\n' + ' ' * len(indent) + part.strip()
                    new_line += suffix
                    if new_line.count('\n') > 0:
                        result.append(new_line)
                        continue

            # Try to break long function calls by arguments
            m = re.match(r'^(\s*)(\w[\w_]*\s*\()(.+)(\)\s*;?\s*)$', line)
            if m:
                indent = m.group(1)
                call_pre = m.group(2)
                args = m.group(3)
                call_suf = m.group(4)

                arg_list = re.split(r',\s*', args)
                if len(arg_list) > 3:
                    new_line = indent + call_pre
                    for k, arg in enumerate(arg_list):
                        if k == 0:
                            new_line += arg
                        else:
                            new_line += ',\n' + ' ' * (len(indent) + len(call_pre)) + arg
                    new_line += call_suf
                    if new_line.count('\n') > 0:
                        result.append(new_line)
                        continue

            result.append(line)
        return '\n'.join(result)

    return stateful_transform(text, transform)


def format_path(path):
    with open(path, 'rb') as f:
        raw = f.read()

    try:
        text = raw.decode('utf-8')
    except UnicodeDecodeError:
        try:
            text = raw.decode('cp1252')
        except:
            print(f"  SKIP (cannot decode): {path}", file=sys.stderr)
            return

    original = text

    text = step_tabs(text)
    text = step_trailing_whitespace(text)
    text = step_blank_lines(text)
    text = step_function_spacing(text)
    text = step_keyword_space(text)
    text = step_increment(text)
    text = step_allman_braces(text)
    text = step_case_braces(text)
    text = step_indent_braces(text)
    text = step_long_lines(text)
    text = step_call_continuation_indent(text)

    if text != original:
        with open(path, 'wb') as f:
            f.write(text.encode('utf-8'))
        print(f"  FORMATTED: {path}", file=sys.stderr)
    else:
        print(f"  unchanged: {path}", file=sys.stderr)


def format_text(raw_text: str) -> str:
    text = step_tabs(raw_text)
    text = step_trailing_whitespace(text)
    text = step_blank_lines(text)
    text = step_function_spacing(text)
    text = step_keyword_space(text)
    text = step_increment(text)
    text = step_allman_braces(text)
    text = step_case_braces(text)
    text = step_indent_braces(text)
    text = step_long_lines(text)
    text = step_call_continuation_indent(text)
    return text


if __name__ == '__main__':
    targets = sys.argv[1:]

    # stdin/stdout mode (used by conform.nvim): no args → read stdin, print formatted to stdout
    if not targets:
        try:
            raw = sys.stdin.buffer.read()
        except:
            sys.exit(1)
        try:
            text = raw.decode('utf-8')
        except UnicodeDecodeError:
            try:
                text = raw.decode('cp1252')
            except:
                sys.exit(1)
        sys.stdout.write(format_text(text))
        sys.exit(0)

    # In-place mode: file args given → modify each file on disk
    for path in targets:
        if os.path.isfile(path):
            format_path(path)
        else:
            print(f"  NOT FOUND: {path}", file=sys.stderr)
