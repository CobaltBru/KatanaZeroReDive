#include "ChatManager.h"
#include "RenderManager.h"

Token::Token(const wchar_t* text, OPTION option, COLORS color)
{
	pos = { 0.f,0.f };
	this->text = text;
	this->option = option;
	this->color = color;
	timer = 0;

    if (color == COLORS::WHITE)
        savedColor = RGB(255, 255, 255);
    else if (color == COLORS::RED)
        savedColor = RGB(255, 0, 0);
    else if (color == COLORS::YELLOW)
        savedColor = RGB(255, 215, 0);
    else if (color == COLORS::PURPLE)
        savedColor = RGB(148, 0, 211);
    else if (color == COLORS::ORANGE)
        savedColor = RGB(255, 140, 0);
}

void Token::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Token::Render(HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, savedColor);
    switch (option)
    {
    case OPTION::NORMAL:

        break;
    case OPTION::SHAKE:

        break;
    case OPTION::WAVE:
        WaveEffect(hdc);
        break;
    case OPTION::DOOM:

        break;
    }
    SetTextColor(hdc, RGB(0,0,0));
}

void Token::WaveEffect(HDC hdc)
{
    
    int len = lstrlenW(text);
    float x = pos.x;

    float amplitude = 4;
    float delay = 0.5f;
    float speed = 15.f;
    for (int i = 0; i < len; i++)
    {
        float offsetY = amplitude * sin(timer * speed + i * delay);

        wchar_t letter[2] = { text[i], L'\0' };

        TextOut(hdc, x, pos.y + offsetY, letter, 1);

        SIZE size;
        GetTextExtentPoint32W(hdc, letter, 1, &size);
        x += size.cx;
    }
}
