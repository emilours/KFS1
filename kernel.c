#define VGA_ADDRESS 0xB8000   /* video memory begins here. */

/* VGA provides support for 16 colors */
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE_COLOR 15

#include <stdint.h>

unsigned short *terminal_buffer;
unsigned int vga_index;

void clear_screen(void)
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2) {
            terminal_buffer[index] = ' ';
            index += 2;
    }
}

// changer pour implementer strlen et strcmp
void print_string(char *str, unsigned char color)
{
    int index = 0;
    while (str[index]) {
            terminal_buffer[vga_index] = (unsigned short)str[index]|(unsigned short)color << 8;
            index++;
            vga_index++;
    }
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                       : "=a"(ret)
                       : "Nd"(port) );
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
} 

// Scroll and cursor functions
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * 80 + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void main(void)
{
    terminal_buffer = (unsigned short *)VGA_ADDRESS;
    vga_index = 0;

    clear_screen();
    enable_cursor(14, 15);
    print_string("Hello, I'm alive!", YELLOW);
    update_cursor(0, get_cursor_position());
    vga_index = 80;    /* next line */
    // print_string("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras accumsan erat eu aliquam condimentum. Ut hendrerit sapien id egestas rutrum. Vivamus bibendum ut tellus vitae ornare. Curabitur porttitor orci magna, quis mollis quam euismod non. Praesent eget justo volutpat, egestas elit eu, commodo ligula. In varius metus convallis nibh dictum lacinia. Curabitur vehicula euismod nunc a congue. Nam lobortis sem id ipsum volutpat, in pharetra ex sagittis. Proin et ex interdum, ultrices mauris non, laoreet est. Fusce eget porttitor tellus. Nulla eget tortor id sem tristique egestas. Morbi nec ultricies dui.Etiam maximus vulputate orci, nec sollicitudin augue. Nam et risus commodo sem imperdiet convallis non id purus. Nam ac libero sed libero cursus venenatis. Quisque nibh odio, varius sed semper et, malesuada vel metus. Etiam viverra velit nisl, sit amet vestibulum massa cursus vitae. Etiam tempus porta vulputate. Integer interdum urna et ipsum rutrum bibendum. Quisque aliquam urna lacus, sit amet pharetra risus porta ut. Fusce metus diam, euismod vel mauris eget, finibus posuere elit. In in turpis lorem. Donec efficitur mi in tellus malesuada pulvinar. Fusce eu risus purus.Suspendisse a turpis ex. Vivamus iaculis euismod tortor. Morbi in ullamcorper felis, vel imperdiet libero. Nullam vulputate sem ac tincidunt sodales. Sed a augue in felis posuere mollis. Aenean ac lacus gravida tellus bibendum porta. Donec rhoncus, turpis nec molestie mollis, libero massa commodo nisl, non dignissim elit tellus eu lectus. Sed gravida sapien eget tortor tincidunt interdum. Suspendisse eleifend condimentum orci, vel dictum enim consectetur eget. Integer tincidunt lorem vitae tempus placerat.Nulla ex justo, eleifend sed tortor mollis, scelerisque vestibulum sapien. Nulla euismod sed diam vel scelerisque. In vestibulum nunc augue, a iaculis risus pulvinar convallis. Nulla tincidunt sit amet erat imperdiet venenatis. Cras ipsum ante, lobortis sit amet nibh malesuada, molestie sollicitudin arcu. Morbi facilisis egestas urna non convallis. Quisque tempor turpis a dolor convallis tempus. Morbi tincidunt justo felis, a tincidunt eros varius et. Suspendisse at ornare dui, eu dignissim sapien. In posuere libero lacus, nec volutpat ligula varius sit amet. Phasellus hendrerit sem eros, eu consequat nibh tempor vitae. Etiam cursus bibendum augue a auctor. In maximus iaculis tellus vel aliquam. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin condimentum, sem eu pretium feugiat, ante massa dapibus nibh, in tincidunt enim nisl eu tellus. Mauris venenatis mattis urna sed mollis.In id rhoncus sem. Nulla consectetur congue mi, non elementum nulla dapibus vitae. Mauris iaculis fermentum sollicitudin. Nam vitae nisi viverra, tincidunt orci quis, vehicula enim. Nulla auctor diam quis nibh viverra mattis. Praesent enim justo, venenatis non libero vel, vestibulum gravida magna. Sed vitae mi maximus, aliquam tellus vitae, egestas libero. Vestibulum ante nibh, dapibus ac ex id, mollis feugiat metus. Sed dapibus tellus risus, vitae rutrum ante rhoncus at. Aenean nisl elit, laoreet suscipit lobortis eu, molestie at elit. Fusce vitae justo sit amet urna sagittis posuere et sit amet tellus. Sed at mauris et velit interdum ullamcorper eget vel augue. Proin turpis quam, vestibulum id ornare eget, condimentum sed mauris.Quisque non justo et eros ornare scelerisque. Vivamus ante sapien, ornare vitae dolor a, interdum faucibus felis. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. In hendrerit tristique mi commodo bibendum. Nulla facilisi. Praesent in sem a ante convallis venenatis vitae ac erat. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam placerat nisl velit, at sagittis purus commodo vel. Quisque sit amet leo laoreet, pulvinar tellus sed, laoreet ex. Fusce accumsan convallis orci nec auctor. Nulla eu accumsan nisl, at fringilla dui. Proin nisi ante, tincidunt sit amet risus non, scelerisque commodo erat. Aliquam enim elit, pharetra a volutpat vitae, pretium at est. Morbi vulputate nibh a nisi fringilla tincidunt.Mauris risus augue, interdum id fringilla ut, porta vitae quam. Curabitur eget sem vel quam ultricies consequat. Phasellus iaculis eros justo, in tempus ex tincidunt a. Nam fringilla posuere lacinia. Aenean non consequat eros. Donec et dui faucibus, congue velit ut, posuere velit. Pellentesque arcu leo, laoreet eget suscipit placerat, aliquet tempus augue. Sed semper placerat viverra. Mauris dictum felis vitae lectus commodo, eu euismod ex mollis. Proin aliquam diam a ipsum accumsan sollicitudin. Interdum et malesuada fames ac ante ipsum primis in faucibus.Quisque vel magna a augue commodo accumsan. Nam auctor nunc ac diam fermentum, eu tincidunt velit ornare. Sed dui turpis, rutrum a efficitur vel, lacinia ut augue. Donec nibh nulla, viverra vitae tempus at, gravida quis erat. Phasellus molestie condimentum enim eget semper. Nullam sit amet velit nec lacus ornare efficitur sed nec ligula. Sed id sollicitudin risus. Morbi luctus dolor magna, nec dictum urna semper sed. Pellentesque interdum in augue venenatis feugiat. Mauris viverra magna sed lacus rutrum ultricies. Morbi posuere erat at ultrices fermentum. Curabitur aliquet lobortis mi non pulvinar. Aenean libero eros, lacinia sit amet porta vitae.", RED);
    return;
}