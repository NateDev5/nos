#include <kernel/terminal.h>

#include <kernel/library/log.h>
#include <kernel/library/string.h>
#include <kernel/library/arrayutils.h>

#include <kernel/drivers/video/vga.h>
#include <kernel/drivers/io/keycodes.h>
#include <kernel/drivers/io/keymap.h>

#include <kernel/memory/mem.h>

namespace Kernel::Terminal {
    static constexpr int8 characterMapCaps [] = {
        0,
        'A','B','C','D','E','F','G','H','I','J','K','L','M', 
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    };

    static constexpr int8 characterMap [][KEYCODE_MAP_LEN] = {
        {
            0, 
            // a-z
            'a','b','c','d','e','f','g','h','i','j','k','l','m', 
            'n','o','p','q','r','s','t','u','v','w','x','y','z',

            // 0-9
            '0','1','2','3','4','5','6','7','8','9',

            // special characters
            '`','-','=','[',']','\\',';','\'',',','.','/',

            // f keys
            0,0,0,0,0,0,0,0,0,0,0,0,

            // other
            0    /* back space */, 
            ' '  /* space */,
            '\t' /* tab */,
            0,0,0,0,
            0    /* enter */,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        {
            0, 
            // a-z
            'A','B','C','D','E','F','G','H','I','J','K','L','M', 
            'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',

            // 0-9
            ')','!','@','#','$','%','^','&','*','(',

            // special characters
            '~','_','+','{','}','|',':','"','<','>','?',

            // f keys
            0,0,0,0,0,0,0,0,0,0,0,0,

            // other
            0    /* back space */, 
            ' '  /* space */,
            '\t' /* tab */,
            0,0,0,0,
            0    /* enter */,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        }
    };

    static uint16 currentOffset = 0;
    static uint16 minOffset = 0;
    static uint16 maxOffset = 0;

    static int8 terminalBuffer[TERMINAL_BUFFER_SIZE];
    static uint8 terminalBufferPos = 0;

    void run() {
        Drivers::VGA::enableCursor(Drivers::VGA::UNDER);
        newEntry();

        while (true) {
            Drivers::Keyboard::KeypressInfo keypress = Drivers::Keyboard::readKey();
            handleKeypress(keypress);
        }
    }

    void handleKeypress (IN Drivers::Keyboard::KeypressInfo keypress) {
        if(keypress.flags & Drivers::Keyboard::KEYDOWN && keypress.scancode != SCANCODE_INVALID) {
            if(keypress.keycode == KEYCODE_ENTER) {
                Library::print("\n");
                handleBuffer();
                newEntry();
                return;
            }

            if(keypress.keycode == KEYCODE_BACKSPACE) {
                if(terminalBufferPos == 0) return;

                // - 2 because at the start of every entry there is "> "
                uint16 currentCursorPos = currentOffset % SCRN_WIDTH - 2;
                terminalBufferPos = Library::removeAt(terminalBuffer, TERMINAL_BUFFER_SIZE, terminalBufferPos, currentCursorPos - 1);

                currentOffset--;

                Drivers::VGA::removecharAt(currentOffset * 2);

                Drivers::VGA::setCursorPos(currentOffset);

            }

            if(keypress.flags & Drivers::Keyboard::EXTENDED 
                && keypress.scancode == SCANCODE_ARROW_LEFT 
                && currentOffset > minOffset) {
                currentOffset--;
                Drivers::VGA::setCursorPos(currentOffset);
                return;
            }

            if(keypress.flags & Drivers::Keyboard::EXTENDED 
                && keypress.scancode == SCANCODE_ARROW_RIGHT 
                && currentOffset < maxOffset) {
                currentOffset++;
                Drivers::VGA::setCursorPos(currentOffset);
                return;
            }

            if(keypress.keycode != KEYCODE_INVALID) {
                if(keypress.flags & Drivers::Keyboard::CAPS &&
                    keypress.keycode >= KEYCODE_A && keypress.keycode <= KEYCODE_Z) {
                        displayCharacter(characterMapCaps[keypress.keycode]);
                        return;
                    }

                uint8 tableIndex = keypress.flags & Drivers::Keyboard::SHIFT ? 1 : 0;
                int8 _char = characterMap[tableIndex][keypress.keycode];
                displayCharacter(_char);
            }
        }
    }

    void displayCharacter (IN int8 _char) {
        if(_char == NULL) return;
        if(terminalBufferPos > TERMINAL_BUFFER_SIZE) return;

        Library::printc(_char);
        terminalBuffer[terminalBufferPos] = _char;
        terminalBufferPos++;
        
        currentOffset++;
        maxOffset++;
        Drivers::VGA::setCursorPos(currentOffset);
    }

    void newEntry () {
        Library::print("> ");

        currentOffset = Drivers::VGA::getCurrentOffset() / 2;
        minOffset = currentOffset;
        maxOffset = currentOffset;

        Drivers::VGA::setCursorPos(currentOffset);
    }

    void handleBuffer () {
        if(Library::strcmp(terminalBuffer, "clear"))
            Library::clear();
        else if(Library::strcmp(terminalBuffer, "vgatest"))
            Drivers::VGA::test();
        else 
            Library::fprintf_ln("'%s' is not a valid command", Drivers::VGA::LRED, terminalBuffer);

        Memory::memset(terminalBuffer, 0, TERMINAL_BUFFER_SIZE);
        terminalBufferPos = 0;
    }
}