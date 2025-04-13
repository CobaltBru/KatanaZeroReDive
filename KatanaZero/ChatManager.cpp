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

    // ȿ�� �Ķ����
    float appearDelay =0.05f;  // �� ���� �� ���� �ð� (�и���)
    float upTime = 0.05f; // �� ������ �ִϸ��̼� ���� �ð� (�и���)
    float pixel = 5;  // ���ڰ� ������ �� final ��ġ���� �Ʒ��� ��ġ�ϴ� �ȼ� ��

    for (int i = 0; i < len; i++)
    {
        // �� ���ڴ� i * letterDelay �Ŀ� �ִϸ��̼��� �����Ѵ�.
        float appearTime = i * appearDelay;
        float elapsed = timer - appearTime;
        int y = pos.y + pixel;

        // �� ���ھ� ���
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
    // �� ���ڰ� �����ϴ� ���� �ð� (�и��� ����)
    float appearDelay = 0.03f;

    for (int i = 0; i < len; i++)
    {
        // i��° ������ ���� ���� ����
        float upTime = i * appearDelay;

        // ���� ���� ��Ÿ�� �ð��� �ƴϸ� �ش� ���ڴ� �׸��� ����
        if (timer >= upTime)
        {
            wchar_t letter[2] = { text[i], L'\0' };
            TextOut(hdc, x, pos.y, letter, 1);
            if (i == len - 1)
            {
                appear = APPEAR::END;
            }
            // ���� ������ ���� �����Ͽ� ���� ������ ��ġ ���
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
