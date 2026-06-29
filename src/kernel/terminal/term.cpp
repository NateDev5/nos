#include <kernel/terminal/term.h>

#include <kernel/library/debug.h>

#include <kernel/drivers/video/framebuffer.h>

#include <shared/mem.h>
#include <shared/parse.h>
#include <shared/string.h>

namespace Kernel::Terminal {
static TerminalCell term_buffer_grid[TEMP_TERM_HEIGHT][TEMP_TERM_WIDTH];
static uint8_t      term_buffer_lines[TEMP_TERM_HEIGHT];

TerminalInfo info;

void init() {
    clear();
    Shared::Point font_size = Drivers::Video::Framebuffer::font_size();
    info.width              = Drivers::Video::Framebuffer::width() / font_size.x;  // width in characters
    info.height             = Drivers::Video::Framebuffer::height() / font_size.y; // height in characters

    Shared::memset(info.ansi_arguments, 0, ANSI_MAX_ARGS);
    info.ansi_argument_ptr  = 0;
    info.argument_digit_ptr = 0;

    info.forecolor = DEFAULT_FORE;
    info.backcolor = DEFAULT_BACK;

    KINFO("Terminal info:")
    KINFO("   - Width: %i", info.width)
    KINFO("   - Height: %i", info.height)
    KINFO("   - Max characters: %i\n", info.width * info.height)
}

void print_char(IN char _char) {
    if (info.ansi_sequence) {
        if (Shared::is_numeric(_char)) {
            info.ansi_arguments[info.ansi_argument_ptr][info.argument_digit_ptr] = _char;
            info.argument_digit_ptr++;

            if (info.argument_digit_ptr == 3)
                info.argument_digit_ptr = 0;

            return;
        } else if (_char == 'm') {
            apply_ansi_style();
            info.parsing_argument   = false;
            info.ansi_sequence      = false;
            info.ansi_escaped       = false;
            info.argument_digit_ptr = 0;
            return;
        }
    }

    switch (_char) {
    case ANSI_ESCAPE:
        if (!info.ansi_escaped) {
            info.ansi_escaped = true;
            return;
        }
        break;
    case ANSI_CSI:
        if (info.ansi_escaped) {
            info.parsing_argument  = true;
            info.ansi_sequence     = true;
            info.ansi_argument_ptr = 0;
            Shared::memset(info.ansi_arguments, 0, sizeof(info.ansi_arguments));
            return;
        }
        break;
    case ';':
        if (info.ansi_sequence) {
            info.ansi_argument_ptr++;
            info.argument_digit_ptr = 0;
            return;
        }
        break;
    case '\n':
        info.x = 0;
        info.y++;
        break;
    case '\b':
        modify_cell(--info.x, info.y, 0);
        break;
    default:
        modify_cell(info.x++, info.y, _char);
        break;
    }

    if (info.x >= info.width) {
        info.x = 0;
        info.y++;
    }

    if (info.y >= info.height) {
        info.x = 0;
        info.y--;
        scroll_up();
    }
}

void print(IN CONST_CHAR_PTR str) {
    for (uint32_t i = 0; str[i] != '\0'; i++)
        print_char(str[i]);
}

void modify_cell(IN uint32_t x, IN uint32_t y, IN char unicode) {
    term_buffer_grid[y][x] = {unicode, info.forecolor, info.backcolor, DIRTY};
    term_buffer_lines[y]   = DIRTY;
    render_term();
}

void render_term(IN bool redraw) {
    for (uint32_t y = 0; y < TEMP_TERM_HEIGHT; y++) {
        if (term_buffer_lines[y] == DIRTY || redraw) {
            for (uint32_t x = 0; x < TEMP_TERM_WIDTH; x++) {
                TerminalCell *cell = &term_buffer_grid[y][x];
                if (cell->dirty || redraw) {
                    Drivers::Video::Framebuffer::draw_char({x, y}, cell->unicode, cell->forecolor, cell->backcolor);
                    cell->dirty = NOT_DIRTY;
                }
            }
            term_buffer_lines[y] = NOT_DIRTY;
        }
    }
}

void scroll_up() {
    Shared::memcpy(term_buffer_grid[1], term_buffer_grid, (TEMP_TERM_WIDTH * (TEMP_TERM_HEIGHT - 1) * sizeof(TerminalCell)));
    Shared::memset(term_buffer_grid[TEMP_TERM_HEIGHT - 1], 0, TEMP_TERM_WIDTH * sizeof(TerminalCell));
    render_term(true);
}

void clear() {
    Drivers::Video::Framebuffer::clear(DEFAULT_BACK);

    for (uint32_t y = 0; y < TEMP_TERM_HEIGHT; y++) {
        for (uint32_t x = 0; x < TEMP_TERM_WIDTH; x++) {
            term_buffer_grid[y][x] = {0, DEFAULT_FORE, DEFAULT_BACK, NOT_DIRTY};
        }
        term_buffer_lines[y] = NOT_DIRTY;
    }

    info.x = 0;
    info.y = 0;
}

void apply_ansi_style() {
    uint64_t arg_index = 0;
    AnsiCmd  cmd       = Reset;
    switch (info.ansi_arguments[arg_index][0]) {
    case '0':
        cmd = Reset;
        break;
    case '1':
        cmd = Bold;
        break;
    case '2':
        cmd = Dim;
        break;
    case '3':
        if (info.ansi_arguments[arg_index][1] == '8')
            cmd = Foreground;
        else
            cmd = Italic;
        break;
    case '4':
        if (info.ansi_arguments[arg_index][1] == '8')
            cmd = Background;
        else
            cmd = Underline;
        break;
    case '5':
        cmd = Blinking;
        break;
    case '7':
        cmd = Inverse;
        break;
    case '8':
        cmd = Hidden;
        break;
    case '9':
        cmd = Strikethrough;
        break;
    }

    if (cmd == Foreground || cmd == Background) {
        if (info.ansi_arguments[++arg_index][0] != '2' || info.ansi_argument_ptr != 4)
            return;
        apply_ansi_rgb_color(cmd);

        return;
    }

    if (info.ansi_argument_ptr == 0) {
        info.forecolor = DEFAULT_FORE;
        info.backcolor = DEFAULT_BACK;
        return;
    }

    if (cmd == Reset)
        for (uint8_t i = 0; i < info.ansi_argument_ptr; i++)
            apply_ansi_preset_color(info.ansi_arguments[i + 1]);
}

void apply_ansi_rgb_color(IN AnsiCmd cmd) {
    uint32_t *color_ptr = nullptr;
    switch (cmd) {
    case Foreground:
        color_ptr = &info.forecolor;
        break;
    case Background:
        color_ptr = &info.backcolor;
        break;
    default:
        return;
    }

    uint32_t color = 0;

    for (uint8_t i = 2; i < info.ansi_argument_ptr + 1; i++) {
        int64_t parsed = Shared::parse_int(info.ansi_arguments[i]);
        if (parsed < 0 || parsed > 255)
            return;

        // in the future, check for the color model and use the shift mask for each color
        uint64_t to_shift = info.ansi_argument_ptr - i;
        color |= parsed << to_shift * 8;
    }

    *color_ptr = color;
}

void apply_ansi_preset_color(IN CHAR_PTR arg) {
    uint32_t *color_ptr   = nullptr;
    uint32_t  default_col = 0;
    switch (arg[0]) {
    case '3':
        color_ptr   = &info.forecolor;
        default_col = DEFAULT_FORE;
        break;
    case '4':
        color_ptr   = &info.backcolor;
        default_col = DEFAULT_BACK;
        break;
    }

    switch (arg[1]) {
    case '0':
        *color_ptr = BLACK;
        break;
    case '1':
        *color_ptr = RED;
        break;
    case '2':
        *color_ptr = GREEN;
        break;
    case '3':
        *color_ptr = YELLOW;
        break;
    case '4':
        *color_ptr = BLUE;
        break;
    case '5':
        *color_ptr = MAGENTA;
        break;
    case '6':
        *color_ptr = CYAN;
        break;
    case '7':
        *color_ptr = WHITE;
        break;
    case '9':
        *color_ptr = default_col;
        break;
    }
}
} // namespace Kernel::Terminal
