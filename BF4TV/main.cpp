#include <Windows.h>
#include <iostream>
#include <thread>

void is_running_loop() noexcept;
void main_loop() noexcept;

std::atomic<bool> isOn = false;

enum class State
{
  Negative = -1,
  Neutral = 0,
  Positive = 1,
};

int main()
{
  std::thread(is_running_loop).detach();
  main_loop();
}

void is_running_loop() noexcept
{
  while (true)
  {
    isOn = GetKeyState(VK_INSERT);
    std::cout << "\33[2K\r" << (isOn ? "Running" : "Not running");

    Sleep(10);
  }
}

void mouse_move(State x, State y) noexcept
{
  constexpr int MAX_SHORT = 0xFFFF;

  INPUT buffer[1] = { 0 };
  buffer->type = INPUT_MOUSE;
  buffer->mi.dx = static_cast<int>(x) * MAX_SHORT;
  buffer->mi.dy = -static_cast<int>(y) * MAX_SHORT;
  buffer->mi.dwFlags = MOUSEEVENTF_MOVE;

  SendInput(1, buffer, sizeof(INPUT));
}

void main_loop() noexcept
{
  constexpr int vKey_W = 0x57;
  constexpr int vKey_A = 0x41;
  constexpr int vKey_S = 0x53;
  constexpr int vKey_D = 0x44;

  while (true)
  {
    if (!isOn)
    {
      Sleep(100);
      continue;
    }

    auto x = State::Neutral;
    auto y = State::Neutral;

    if (GetAsyncKeyState(vKey_W))
      y = State::Positive;
    else if (GetAsyncKeyState(vKey_S))
      y = State::Negative;

    if (GetAsyncKeyState(vKey_A))
      x = State::Negative;
    else if (GetAsyncKeyState(vKey_D))
      x = State::Positive;

    mouse_move(x, y);
    Sleep(1);
  }
}