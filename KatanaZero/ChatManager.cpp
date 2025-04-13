#include "ChatManager.h"
#include "RenderManager.h"

Token::Token(const wchar_t* text, APPEAR appear, OPTION option, COLORS color)
{
	pos = { 0.f,0.f };
	this->text = text;
    this->appear = appear;
	this->option = option;
	this->color = color;
	timer = 0;
    len = lstrlenW(text);
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
    else if (color == COLORS::GREEN)
        savedColor = RGB(127, 255, 0);
    else if (color == COLORS::SKY)
        savedColor = RGB(0, 255, 255);
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
    switch (appear)
    {
    case APPEAR::NORMAL:
        NormalAppear(hdc);
        return;
        break;
    case APPEAR::DOOM:
        DoomAppear(hdc);
        return;
        break;
    case APPEAR::END:
        break;
    }
    switch (option)
    {
    case OPTION::STOP:
        TextOut(hdc, pos.x, pos.y, text, len);
        break;
    case OPTION::SHAKE:
        ShakeEffect(hdc);
        break;
    case OPTION::WAVE:
        WaveEffect(hdc);
        break;
    }
    SetTextColor(hdc, RGB(0,0,0));
}

void Token::NormalAppear(HDC hdc)
{
    int x = pos.x;

    // 효과 파라미터
    float appearDelay =0.05f;  // 각 글자 간 지연 시간 (밀리초)
    float upTime = 0.05f; // 각 글자의 애니메이션 지속 시간 (밀리초)
    float pixel = 5;  // 글자가 시작할 때 final 위치보다 아래에 위치하는 픽셀 수

    for (int i = 0; i < len; i++)
    {
        // 각 글자는 i * letterDelay 후에 애니메이션을 시작한다.
        float appearTime = i * appearDelay;
        float elapsed = timer - appearTime;
        int y = pos.y + pixel;

        // 한 글자씩 출력
        if (timer>= appearTime)
        {
            if (elapsed >= upTime)
            {
                y = pos.y;
            }
            else
            {
                float percent = elapsed / upTime;
                y = pos.y + (int)(pixel * (1.0 - percent));
            }
            wchar_t letter[2] = { text[i], L'\0' };
            TextOut(hdc, x, y, letter, 1);

            SIZE size;
            GetTextExtentPoint32W(hdc, letter, 1, &size);
            x += size.cx;
            if (i == len - 1)
            {
                appear = APPEAR::END;
            }
        }
        
    }
}

void Token::DoomAppear(HDC hdc)
{
    int x = pos.x;
    // 각 글자가 등장하는 지연 시간 (밀리초 단위)
    float appearDelay = 0.03f;

    for (int i = 0; i < len; i++)
    {
        // i번째 글자의 등장 시작 시점
        float upTime = i * appearDelay;

        // 아직 아직 나타날 시간이 아니면 해당 글자는 그리지 않음
        if (timer >= upTime)
        {
            wchar_t letter[2] = { text[i], L'\0' };
            TextOut(hdc, x, pos.y, letter, 1);
            if (i == len - 1)
            {
                appear = APPEAR::END;
            }
            // 현재 글자의 폭을 측정하여 다음 글자의 위치 계산
            SIZE size;
            GetTextExtentPoint32W(hdc, &text[i], 1, &size);
            x += size.cx;
        }

    }
}

void Token::WaveEffect(HDC hdc)
{
    
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

void Token::ShakeEffect(HDC hdc)
{
    float x = pos.x;

    int range = 5;

    for (int i = 0; i < len; i++)
    {
        float offsetX = ((rand() % (2 * range + 1)) - range)/10.0f;
        float offsetY = ((rand() % (2 * range + 1)) - range)/10.0f;

        wchar_t letter[2] = { text[i], L'\0' };
        TextOut(hdc, x + offsetX, pos.y + offsetY, letter, 1);

        SIZE size;
        GetTextExtentPoint32W(hdc, letter, 1, &size);
        x += size.cx;
    }
}
