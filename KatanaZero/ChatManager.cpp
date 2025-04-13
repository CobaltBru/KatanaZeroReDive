#include "ChatManager.h"
#include "RenderManager.h"

Token::Token(const wchar_t* text, FPOINT pos, APPEAR appear, OPTION option, COLORS color)
{
    globalPos = { 0.f,0.f };
	this->pos = pos;
    
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

    complete = false;
}

void Token::Update()
{
	timer += TimerManager::GetInstance()->GetDeltaTime();
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
        TextOut(hdc, pos.x + globalPos.x, pos.y + globalPos.y, text, len);
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
    int x = pos.x + globalPos.x;

    float appearDelay =0.1f; 
    float upTime = 0.05f;
    float pixel = 3;

    for (int i = 0; i < len; i++)
    {
        float appearTime = i * appearDelay;
        float elapsed = timer - appearTime;
        int y = pos.y + globalPos.y + pixel;

        if (timer>= appearTime)
        {
            if (elapsed >= upTime)
            {
                y = pos.y + globalPos.y;
            }
            else
            {
                float percent = elapsed / upTime;
                y = pos.y + globalPos.y + (int)(pixel * (1.0 - percent));
            }
            wchar_t letter[2] = { text[i], L'\0' };
            TextOut(hdc, x, y, letter, 1);

            SIZE size;
            GetTextExtentPoint32W(hdc, letter, 1, &size);
            x += size.cx;
            if (i == len - 1)
            {
                complete = true;
                appear = APPEAR::END;
            }
        }
        
    }
}

void Token::DoomAppear(HDC hdc)
{
    int x = pos.x + globalPos.x;
    float appearDelay = 0.1f;

    for (int i = 0; i < len; i++)
    {
        float upTime = i * appearDelay;

        if (timer >= upTime)
        {
            wchar_t letter[2] = { text[i], L'\0' };
            TextOut(hdc, x, pos.y + globalPos.y, letter, 1);
            if (i == len - 1)
            {
                complete = true;
                appear = APPEAR::END;
            }
            SIZE size;
            GetTextExtentPoint32W(hdc, &text[i], 1, &size);
            x += size.cx;
        }

    }
}

void Token::WaveEffect(HDC hdc)
{
    
    float x = pos.x + globalPos.x;

    float amplitude = 4;
    float delay = 0.5f;
    float speed = 15.f;
    for (int i = 0; i < len; i++)
    {
        float offsetY = amplitude * sin(timer * speed + i * delay);

        wchar_t letter[2] = { text[i], L'\0' };

        TextOut(hdc, x, pos.y + globalPos.y + offsetY, letter, 1);

        SIZE size;
        GetTextExtentPoint32W(hdc, letter, 1, &size);
        x += size.cx;
    }
}

void Token::ShakeEffect(HDC hdc)
{
    float x = pos.x + globalPos.x;

    int range = 3;

    for (int i = 0; i < len; i++)
    {
        float offsetX = ((rand() % (2 * range + 1)) - range)/10.0f;
        float offsetY = ((rand() % (2 * range + 1)) - range)/10.0f;

        wchar_t letter[2] = { text[i], L'\0' };
        TextOut(hdc, x + offsetX, pos.y + globalPos.y + offsetY, letter, 1);

        SIZE size;
        GetTextExtentPoint32W(hdc, letter, 1, &size);
        x += size.cx;
    }
}

void Chat::Init(string Key, vector<pair<float, Token>> tokens, float width, float height)
{
    tokenIdx = 0;
    this->key = key;
    this->tokens = tokens;
    this->width = width;
    this->height = height;
    boxTime = 0.5f;
}

void Chat::Update()
{
    timer += TimerManager::GetInstance()->GetDeltaTime();
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
    
    float appearTime = boxTime + 0.5f;
    for (int i = 0; i <= tokenIdx; i++)
    {
        appearTime += tokens[i].first;
        if(timer> appearTime) tokens[i].second.Update();
    }
}

void Chat::Render(HDC hdc)
{
    DrawBox(hdc);
    DrawTokens(hdc);
}

void Chat::DrawBox(HDC hdc)
{
    
    float percent = timer / boxTime;
    float currentHeight = height * percent;
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    if(percent<1.0f)
        RoundRect(hdc, pos.x - width / 2, pos.y, pos.x + width / 2, pos.y + currentHeight, 5.f, 5.f);
    else
        RoundRect(hdc, pos.x - width / 2, pos.y, pos.x + width / 2, pos.y + height, 5.f, 5.f);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    DeleteObject(hPen);
    DeleteObject(hBrush);
}

void Chat::DrawTokens(HDC hdc)
{
    float appearTime = boxTime + 0.5f;
    FPOINT gPos = { pos.x - width / 2 + 5.f,pos.y  + 5.f };
    for (int i = 0; i < tokens.size(); i++)
    {
        appearTime += tokens[i].first;
        if (timer >= appearTime)
        {
            tokens[i].second.setGlobalPos(gPos);
            tokens[i].second.Render(hdc);

            if (tokens[i].second.isComplete())
            {
                if(tokenIdx<tokens.size()-1)tokenIdx = i + 1;
            }
        }
        else
        {
            break;
        }
    }
}
