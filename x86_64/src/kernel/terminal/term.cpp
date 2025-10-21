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
    TODO("Add ANSI support")
    TODO("Render characters more efficently")
    Shared::Point font_size = Drivers::Video::Framebuffer::font_size();
    info.width  = Drivers::Video::Framebuffer::width() / font_size.x;  // width in characters
    info.height = Drivers::Video::Framebuffer::height() / font_size.y; // height in characters

    Shared::memset(info.ansi_arguments, 0, ANSI_MAX_ARGS);
    info.ansi_argument_ptr  = 0;
    info.argument_digit_ptr = 0;

    info.forecolor = DEFAULT;
    info.backcolor = BLACK;

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
            Shared::memset(info.ansi_arguments, 0, ANSI_MAX_ARGS);
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
    Drivers::Video::Framebuffer::clear();

    for (uint32_t y = 0; y < TEMP_TERM_HEIGHT; y++) {
        for (uint32_t x = 0; x < TEMP_TERM_WIDTH; x++) {
            term_buffer_grid[y][x] = {0, DEFAULT, BLACK, NOT_DIRTY};
        }
        term_buffer_lines[y] = NOT_DIRTY;
    }

    info.x = 0;
    info.y = 0;
}

void apply_ansi_style() {
    // Make this better, not my best code
    // Support RGB

    /*
    AnsiCmd cmd;
    switch (info.ansi_arguments[0][0]) {
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
        cmd = Italic;
        break;
    case '4':
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
    */

    if (info.ansi_argument_ptr == 0) {
        info.forecolor = DEFAULT;
        info.backcolor = BLACK;
        return;
    }

    for (uint8_t i = 0; i < info.ansi_argument_ptr; i++)
        parse_and_apply_color(info.ansi_arguments[i + 1]);
}

void parse_and_apply_color(IN CHAR_PTR arg) {
    // Fix this horrible code (This was just temporary for a working prototype)
    uint32_t* color_ptr = nullptr;
    switch (arg[0]) {
    case '3':
        color_ptr = &info.forecolor;
        break;
    case '4':
        color_ptr = &info.backcolor;
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
        *color_ptr = DEFAULT;
        break;
    }
}
} // namespace Kernel::Terminal
