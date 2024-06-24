#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

HANDLE out_handle;

struct player_t
{
    COORD position;
    COORD velocity;
} player;

COORD wall = { 10, 10 };
COORD coin = { 6, 9 };
bool coin_is_alive = true;

void clear_screen(void)
{
    DWORD n_chars_written;
    COORD cursor_pos = { 0 };
    SetConsoleCursorPosition(out_handle, cursor_pos);

    // NOTE(lemmtopia): This escape sequence clears the console.
    WriteConsoleA(out_handle, "\x1b[2J", 14, &n_chars_written, 0);
}

void draw_char(COORD coord, char c)
{
    DWORD n_chars_written;
    SetConsoleCursorPosition(out_handle, coord);

    // NOTE(lemmtopia): This escape sequence clears the console.
    WriteConsoleA(out_handle, &c, 1, &n_chars_written, 0);
}

void move_player(char ch)
{
    player.velocity.X = 0;
    player.velocity.Y = 0;

    switch (ch)
    {
        case 'w':
            player.velocity.Y--;
            break;
        case 'a':
            player.velocity.X--;
            break;
        case 's':
            player.velocity.Y++;
            break;
        case 'd':
            player.velocity.X++;
            break;
    }

    // NOTE(lemmtopia): Collision!
    bool is_colliding_x = (player.position.X + player.velocity.X) == wall.X && player.position.Y == wall.Y;
    bool is_colliding_y = (player.position.Y + player.velocity.Y) == wall.Y && player.position.X == wall.X;

    if (is_colliding_x) player.velocity.X = 0;
    if (is_colliding_y) player.velocity.Y = 0;

    if (player.position.Y < 0) player.position.Y = 0;
    if (player.position.X < 0) player.position.X = 0;

    if (player.position.X == coin.X && player.position.Y == coin.Y)
    {
        coin.X = 0;
        coin.Y = 0;
        coin_is_alive = false;
    }

    player.position.X += player.velocity.X;
    player.position.Y += player.velocity.Y;
}

void main_loop(void)
{
    char ch;
    do
    {
        clear_screen();
        draw_char(wall, '#');
        if (coin_is_alive) draw_char(coin, 'o');
        draw_char(player.position, '@');

        ch = _getch();

        move_player(ch);
    } while (ch != 'q');
}

int main(void)
{
    out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    clear_screen();
    main_loop();

    return 0;
}