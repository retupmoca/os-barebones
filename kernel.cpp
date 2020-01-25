#include <cstddef>
#include <cstdint>
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
namespace kern {
    size_t strlen(const char* str) 
    {
        size_t len = 0;
        while (str[len])
            len++;
        return len;
    }
     
    class Terminal {
        public:
            enum class Colors {
                BLACK = 0,
                BLUE = 1,
                GREEN = 2,
                CYAN = 3,
                RED = 4,
                MAGENTA = 5,
                BROWN = 6,
                LIGHT_GREY = 7,
                DARK_GREY = 8,
                LIGHT_BLUE = 9,
                LIGHT_GREEN = 10,
                LIGHT_CYAN = 11,
                LIGHT_RED = 12,
                LIGHT_MAGENTA = 13,
                LIGHT_BROWN = 14,
                WHITE = 15,
            };
            static const size_t VGA_WIDTH = 80;
            static const size_t VGA_HEIGHT = 25;

            void init() {
                row = 0;
                col = 0;
                color = entry_color(Colors::LIGHT_GREY, Colors::BLACK);
                buffer = (uint16_t*) 0xB8000;
                for (size_t y = 0; y < VGA_HEIGHT; y++) {
                    for (size_t x = 0; x < VGA_WIDTH; x++) {
                        const size_t index = y * VGA_WIDTH + x;
                        buffer[index] = entry(' ', color);
                    }
                }
            }

            void setcolor(Colors fg, Colors bg) {
                color = entry_color(fg, bg);
            }

            void putcharat(char c, size_t col, size_t row) {
                putentryat(c, color, col, row);
            }

            void putchar(char c) {
                if (c != '\n')
                    putcharat(c, col, row);

                if (c == '\n' || ++col == VGA_WIDTH) {
                    col = 0;
                    if (++row == VGA_HEIGHT)
                        row = 0;
                }
            }

            void write(const char *data, size_t size) {
                for (size_t i = 0; i < size; i++)
                    putchar(data[i]);
            }

            void writestring(const char *data) {
                write(data, strlen(data));
            }

        private:
            size_t row;
            size_t col;
            uint8_t color;
            uint16_t *buffer;

            static uint8_t entry_color(Colors fg, Colors bg) {
                return (uint8_t)fg | (uint8_t)bg << 4;
            }

            static uint16_t entry(unsigned char uc, uint8_t color) {
                return (uint16_t) uc | (uint16_t) color << 8;
            }

            void putentryat(char c, uint8_t color, size_t x, size_t y) {
                const size_t index = y * VGA_WIDTH + x;
                buffer[index] = entry(c, color);
            }
    };
    static Terminal term;
}
 
extern "C" {
    void kernel_main(void) {
        using namespace kern;
        term.init();
        term.writestring("Hello World!\nC++!\n");
    }
}
