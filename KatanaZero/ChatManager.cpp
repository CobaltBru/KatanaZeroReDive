#include "ChatManager.h"
#include "RenderManager.h"
#include "GPImage.h"
using namespace Gdiplus;

static void DrawRoundRect(Graphics* graphics,FPOINT pos, float width, float height,Color fillColor)
{
    int cornerRadius = 4;
    Gdiplus::Rect rect(pos.x, pos.y, width, height);

    GraphicsPath path;

    path.AddArc(rect.X, rect.Y, cornerRadius * 2, cornerRadius * 2, 180, 90);
    path.AddArc(rect.X + rect.Width - cornerRadius * 2, rect.Y, cornerRadius * 2, cornerRadius * 2, 270, 90);
    path.AddArc(rect.X + rect.Width - cornerRadius * 2, rect.Y + rect.Height - cornerRadius * 2, cornerRadius * 2, cornerRadius * 2, 0, 90);
    path.AddArc(rect.X, rect.Y + rect.Height - cornerRadius * 2, cornerRadius * 2, cornerRadius * 2, 90, 90);
    path.CloseFigure();

    SolidBrush brush(fillColor);
    graphics->FillPath(&brush, &path);
}

Token::Token(const wchar_t* text, FPOINT pos, APPEAR appear, OPTION option, COLORS color)
{
    globalPos = { 0.f,0.f };
	this->pos = pos;
    
	this->text = text;
    this->appear = appear;
	this->option = option;
	this->color = color;
	timer = 0;
    explodeTimer = 0;
    len = lstrlenW(text);
    currentAppear = -1;
    if (appear == APPEAR::END)
        currentAppear = len - 1;
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
    case APPEAR::EXPLODE:
        ExplodeAppear(hdc);
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
            currentAppear = max(currentAppear, i);
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
    float appearDelay = 0.02f;

    for (int i = 0; i < len; i++)
    {
        float upTime = i * appearDelay;

        if (timer >= upTime)
        {
            currentAppear = max(currentAppear, i);
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

void Token::ExplodeAppear(HDC hdc)
{
    explodeTimer += TimerManager::GetInstance()->GetDeltaTime();
    float x = pos.x + globalPos.x;
    float y = pos.y + globalPos.y;

    float gravity = 700.0f;
    float playTime = 0.7f;

    if (explodeTimer >= playTime) return;

    for (int i = 0; i <= currentAppear; i++)
    {
        float tmp;
        srand(i * 123456);
        tmp = ((float)(rand() % 1000) / 1000.f);
        float speedX = (tmp * 2 - 1.0f) * 100.f;

        srand(i * 7890);
        tmp = ((float)(rand() % 1000) / 1000.f);
        float speedY = (tmp - 1.0f) * 100.f;

        float offsetX = speedX * explodeTimer;
        float offsetY = speedY * explodeTimer + 0.5f * gravity * explodeTimer * explodeTimer;
        wchar_t letter[2] = { text[i], L'\0' };
        TextOut(hdc, x + offsetX, y + offsetY, letter, 1);

        SIZE size;
        GetTextExtentPoint32W(hdc, letter, 1, &size);
        x += size.cx;
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
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
    float appearTime = boxTime + 0.5f;
    for (int i = 0; i <= tokenIdx; i++)
    {
        appearTime += tokens[i].first;
        if(timer> appearTime) tokens[i].second.Update();
    }

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
    {
        makeExplode();
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

void Chat::makeExplode()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        tokens[i].second.setExplode();
    }
}

void OptionChat::Init(float redTime, float totalTime, vector<pair<string, Token>> selects)
{
    this->redTime = redTime;
    this->totalTime = totalTime;
    this->selects = selects;
    timeBarWidth = 500.f;
    selectWidth = 600.f;
    selectHeight = 30.f;
    selectGap = 6.f;
    timeBarPos = { WINSIZE_X / 2 - timeBarWidth / 2.f,WINSIZE_Y * (4.0f / 5.0f) - 50.f };
    selectBarPos = { WINSIZE_X / 2 ,WINSIZE_Y * (4.f / 5.0f) };
    
}

void OptionChat::Update()
{
    timer += TimerManager::GetInstance()->GetDeltaTime();
    RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
    if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
    {
        --cursor;
        if (cursor < 0)  cursor = selects.size() - 1;
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
    {
        ++cursor;
        if (cursor >= selects.size()) cursor = 0;
    }
}

void OptionChat::Render(HDC hdc)
{
    DrawTimeBar(hdc);
    DrawSelects(hdc);
}

void OptionChat::DrawTimeBar(HDC hdc)
{
    Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);
    float appearTime = 0.3f;
    float height = 12.f;
    {
        float percent = timer / appearTime;
        if (percent > 1.0f)percent = 1.0f;
        DrawRoundRect(pGraphics, timeBarPos, timeBarWidth * percent, height, Gdiplus::Color(128, 255, 255, 255));
    }
    {
        float redWidth = redTime / totalTime * timeBarWidth -2.f;
        float percent = timer / appearTime;
        if (percent > 1.0f)percent = 1.0f;
        DrawRoundRect(pGraphics, {timeBarPos.x+2.f,timeBarPos .y+2.f}, redWidth * percent, height-4.f, Gdiplus::Color(255, 255, 0, 0));
    }
    {
        if (timer > appearTime)
        {
            float percent = (timer - appearTime) / totalTime;
            if (percent > 1.0f)percent = 1.0f;
            DrawRoundRect(pGraphics, { timeBarPos.x + 2.f,timeBarPos.y + 2.f }, (timeBarWidth - 2.f) * percent, height - 4.f, Gdiplus::Color(200, 255, 255, 255));
        }
    }
    delete pGraphics;
}

void OptionChat::DrawSelects(HDC hdc)
{
    float appearTime = 0.3f;
    HPEN normalhPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN bluehPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HPEN hOldPen;
    HBRUSH hOldBrush;
    for (int i = 0; i < selects.size(); i++)
    {
        float y = selectBarPos.y + i * selectGap + i * selectHeight;
        float percent = timer / appearTime;
        if (percent > 1.0f)percent = 1.0f;

        if(cursor == i)
            hOldPen = (HPEN)SelectObject(hdc, bluehPen);
        else
            hOldPen = (HPEN)SelectObject(hdc, normalhPen);

        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        if (percent < 1.0f)
        {
            RoundRect(hdc, selectBarPos.x * percent - selectWidth / 2.f, y - selectHeight / 2.f,
                selectBarPos.x * percent + selectWidth / 2.f, y + selectHeight / 2.f,
                5.f, 5.f);
            selects[i].second.setPos({ selectBarPos.x * percent - selects[i].second.getSize(hdc).cx / 2, y-7.f });
            selects[i].second.Render(hdc);
        }
        else
        {
            RoundRect(hdc, selectBarPos.x - selectWidth / 2, y - selectHeight / 2.f,
                selectBarPos.x + selectWidth / 2, y + selectHeight / 2.f,
                5.f, 5.f);
            selects[i].second.setPos({ selectBarPos.x - selects[i].second.getSize(hdc).cx / 2, y - 7.f });
            selects[i].second.Render(hdc);
        }
            

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);

       
    }
    DeleteObject(normalhPen);
    DeleteObject(bluehPen);
    DeleteObject(hBrush);
}
