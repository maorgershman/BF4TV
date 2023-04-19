#include <Windows.h>
#include <stdbool.h>

DWORD WINAPI is_running_loop(LPVOID lpParam);
void main_loop();

bool isOn = false;

enum State
{
  State_Negative = -1,
  State_Neutral = 0,
  State_Positive = 1,
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  CreateThread(NULL, 0, is_running_loop, NULL, 0, NULL);
  main_loop();

  return 0;
}

DWORD WINAPI is_running_loop(LPVOID lpParam)
{
  while (true)
  {
    isOn = GetKeyState(VK_INSERT);
    Sleep(10);
  }
}

void mouse_move(enum State x, enum State y)
{
  const int MAX_SHORT = 0xFFFF;

  INPUT buffer[1] = { 0 };
  buffer->type = INPUT_MOUSE;
  buffer->mi.dx = x * MAX_SHORT;
  buffer->mi.dy = -y * MAX_SHORT;
  buffer->mi.dwFlags = MOUSEEVENTF_MOVE;

  SendInput(1, buffer, sizeof(INPUT));
}

void main_loop()
{
  while (true)
  {
    if (!isOn)
    {
      Sleep(100);
      continue;
    }

    auto x = State_Neutral;
    auto y = State_Neutral;

    if (GetAsyncKeyState('W'))
      y = State_Positive;
    else if (GetAsyncKeyState('S'))
      y = State_Negative;

    if (GetAsyncKeyState('A'))
      x = State_Negative;
    else if (GetAsyncKeyState('D'))
      x = State_Positive;

    mouse_move(x, y);
    Sleep(1);
  }
}