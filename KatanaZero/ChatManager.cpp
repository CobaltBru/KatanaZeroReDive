#include "ChatManager.h"
#include "RenderManager.h"
#include "GPImage.h"
#include "ScrollManager.h"
#include "CommonFunction.h"
#include <fstream>
using json = nlohmann::json;
using namespace Gdiplus;


Token::Token(wstring wtext, FPOINT pos, APPEAR appear, OPTION option, COLORS color)
{
    globalPos = { 0.f,0.f };
	this->pos = pos;
    
	//this->wtext = wtext;
    this->wtext.assign(wtext);
    this->appear = appear;
	this->option = option;
	this->color = color;
	timer = 0;
    explodeTimer = 0;
    len = wtext.size();
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
        TextOut(hdc, pos.x + globalPos.x, pos.y + globalPos.y, wtext.c_str(), len);
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
            wchar_t letter[2] = { wtext.c_str()[i], L'\0' };
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
            wchar_t letter[2] = { wtext.c_str()[i], L'\0' };
            TextOut(hdc, x, pos.y + globalPos.y, letter, 1);
            if (i == len - 1)
            {
                complete = true;
                appear = APPEAR::END;
            }
            SIZE size;
            GetTextExtentPoint32W(hdc, &wtext.c_str()[i], 1, &size);
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

        srand(i * 789);
        tmp = ((float)(rand() % 1000) / 1000.f);
        float speedY = (tmp - 1.0f) * 200.f - 50.f;

        float offsetX = speedX * explodeTimer;
        float offsetY = speedY * explodeTimer + 0.5f * gravity * explodeTimer * explodeTimer;
        wchar_t letter[2] = { wtext.c_str()[i], L'\0' };
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

        wchar_t letter[2] = { wtext.c_str()[i], L'\0' };

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

        wchar_t letter[2] = { wtext.c_str()[i], L'\0' };
        TextOut(hdc, x + offsetX, pos.y + globalPos.y + offsetY, letter, 1);

        SIZE size;
        GetTextExtentPoint32W(hdc, letter, 1, &size);
        x += size.cx;
    }
}



void Chat::Init(vector<pair<float, Token>> &tokens, float width, float height)
{
    tokenIdx = 0;
    this->tokens.assign(tokens.begin(), tokens.end());
    //this->tokens = tokens;
    this->width = width;
    this->height = height;
    boxTime = 0.5f;
    statusFlag = 0;
}

void Chat::Update()
{
    statusFlag = 0;
    timer += TimerManager::GetInstance()->GetDeltaTime();
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
    const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

    float percent = timer / boxTime;
    float currentHeight = height * percent;
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    if(percent<1.0f)
        RoundRect(hdc, (pos.x - width / 2) + Scroll.x, pos.y + Scroll.y, (pos.x + width / 2) + Scroll.x, (pos.y + currentHeight) + Scroll.y, 5.f, 5.f);
    else
        RoundRect(hdc, (pos.x - width / 2) + Scroll.x, pos.y + Scroll.y, (pos.x + width / 2) + Scroll.x, (pos.y + height) + Scroll.y, 5.f, 5.f);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    DeleteObject(hPen);
    DeleteObject(hBrush);
}

//void Chat::DrawTokens(HDC hdc)
//{
//    const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();
//
//    float appearTime = boxTime + 0.5f;
//    FPOINT gPos = { pos.x - width / 2 + 5.f,pos.y  + 5.f };
//    for (int i = 0; i < tokens.size(); i++)
//    {
//        appearTime += tokens[i].first;
//        if (timer >= appearTime)
//        {
//            tokens[i].second.setGlobalPos({ gPos.x + Scroll.x,gPos.y + Scroll.y });
//            tokens[i].second.Render(hdc);
//
//            if (tokens[i].second.isComplete())
//            {
//                if (tokenIdx < tokens.size() - 1)tokenIdx = i + 1;
//                else statusFlag = 2;
//            }
//        }
//        else
//        {
//            break;
//        }
//    }
//}
void Chat::DrawTokens(HDC hdc)
{
    const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

    float baseAppearTime = boxTime + 0.5f;
    float scheduledTime = baseAppearTime;

    FPOINT gPos = { pos.x - width / 2 + 5.f, pos.y + 5.f };

    for (int i = 0; i < tokens.size(); i++)
    {
        if (i > 0)
        {
            if (tokens[i - 1].second.isComplete())
            {
                scheduledTime += tokens[i].first;
            }
            else
            {
               break;
            }
        }

        if (timer >= scheduledTime)
        {
            tokens[i].second.setGlobalPos({ gPos.x + Scroll.x, gPos.y + Scroll.y });
            tokens[i].second.Render(hdc);

            if (tokens[i].second.isComplete())
            {
                if (tokenIdx < tokens.size() - 1)
                    tokenIdx = i + 1;
                else
                    statusFlag = 2;
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

void Chat::calcSizes(HDC hdc)
{
    float x = 0.f;
    float y = 0.f;
    for (auto tok : tokens)
    {
        tok.second.setPos({ x,y });

    }
}

void OptionChat::Init(vector <pair<float, Token >> &tokens, float width, float height, 
    float redTime, float totalTime, 
    vector<pair<string, Token>> &redSelects,
    vector<pair<string, Token>> &normalSelects)
{
    __super::Init(tokens, width, height);
    this->redTime = redTime;
    this->totalTime = totalTime;

    /*this->redSelects = redSelects;
    this->normalSelects = normalSelects;*/
    this->redSelects.assign(redSelects.begin(), redSelects.end());
    this->normalSelects.assign(normalSelects.begin(), normalSelects.end());


    state = OptionState::RED;
    animDuration = 0.3f; // ���� ��� 0.3�� ���� ����/���� �ִϸ��̼� ����

    cursor = 0;

    timeBarWidth = 500.f;
    selectWidth = 600.f;
    selectHeight = 30.f;
    selectGap = 6.f;
    timeBarPos = { WINSIZE_X / 2 - timeBarWidth / 2.f,WINSIZE_Y * (4.0f / 5.0f) - 50.f };
    selectBarPos = { WINSIZE_X / 2 ,WINSIZE_Y * (4.f / 5.0f) };
    
}

void OptionChat::Update()
{
    __super::Update();
    float dt = TimerManager::GetInstance()->GetDeltaTime();
    timer += dt;

    if (state == OptionState::RED)
    {
        statusFlag = 2;
        if (animDuration >= timer)
        {
            float percent = timer / animDuration;
            currentSelectBarX = selectBarPos.x * percent;
        }
        else if (redTime >= timer)
        {
            currentSelectBarX = selectBarPos.x;
        }
        else if (timer >= redTime + animDuration)
        {
            statusFlag = 0;
            state = OptionState::NORMAL;
            currentSelectBarX = 0;
        }
        else if (timer >= redTime)
        {
            float percent = (timer - redTime) / animDuration;
            currentSelectBarX = selectBarPos.x + selectBarPos.x * percent;
        }
        
    }
    else if (state == OptionState::NORMAL)
    {
        statusFlag = 1;
        if (redTime + animDuration + animDuration >= timer)
        {
            float percent = (timer - redTime - animDuration) / animDuration;
            currentSelectBarX = selectBarPos.x * percent;
        }
        else
        {
            currentSelectBarX = selectBarPos.x;
        }
    }
    
}

void OptionChat::Render(HDC hdc)
{
    __super::Render(hdc);
    DrawTimeBar(hdc);
    if (state == OptionState::RED)
        DrawRedSelects(hdc);
    else if (state == OptionState::NORMAL)
        DrawNormalSelects(hdc);
    //DrawSelects(hdc);
}

void OptionChat::DrawTimeBar(HDC hdc)
{
    Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hdc);
    float appearTime = 0.3f;
    float height = 12.f;
    //���
    {
        float percent = timer / appearTime;
        if (percent > 1.0f)percent = 1.0f;
        DrawRoundRect(pGraphics, timeBarPos, timeBarWidth * percent, height, Gdiplus::Color(128, 255, 255, 255));
    }
    //���� ��
    {
        float redWidth = redTime / totalTime * timeBarWidth -2.f;
        float percent = timer / appearTime;
        if (percent > 1.0f)
        {
            percent = 1.0f;
        }
        DrawRoundRect(pGraphics, {timeBarPos.x+2.f,timeBarPos .y+2.f}, redWidth * percent, height-4.f, Gdiplus::Color(255, 255, 0, 0));
    }
    //�Ͼ� ��
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
    // �������� ��Ÿ�� ���� �ð� (�ִϸ��̼� ���� �ð�)
    float appearTime = 0.3f;  // ������ �ִϸ��̼� �⺻ ���� �ð� (��)

    // for���� ���� �� ������ ������
    // �������� ���� ���¿� ���� �޶����ϴ�.
    vector<pair<string, Token>>& currentSelects =
        (state == OptionState::RED || state == OptionState::TRANSITION) ? redSelects : normalSelects;
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    for (int i = 0; i < currentSelects.size(); i++)
    {
        // �⺻ y ��ġ: ���������� selectBarPos���� ���� �������� ��ġ
        float y = selectBarPos.y + i * (selectHeight + selectGap);

        // �ִϸ��̼� ���� ���� ���
        float percent = timer / appearTime;
        if (percent > 1.0f) percent = 1.0f;

        float offsetX = 0.f;
        // ���¿� ���� x ������ ���
        if (state == OptionState::RED)
        {
            // ���� �������� ���ʿ��� ����� ��Ÿ���ϴ�.
            // target x ��ġ�� selectBarPos.x, ���� ��ġ�� selectBarPos.x - selectWidth (���� ������)
            offsetX = (1.0f - percent) * (-selectWidth);
        }
        else if (state == OptionState::TRANSITION)
        {
            // TRANSITION ���¿����� ���� �������� ���������� �������,
            // �Ϲ� �������� ���ʿ��� ���ɴϴ�.
            // ���⼭�� ������ ���� �������� �� �������� ȥ���մϴ�.
            // ���� ���, ���� �������� (percent) ������ ���������� �̵��ϵ���,
            // �Ϲ� �������� (1 - percent) ������ ���ʿ��� �������� ����մϴ�.
            // �߰� �ܰ迡���� �� ���� ��� ��� ������ ���� �ֽ��ϴ�.
            // �Ʒ��� �����Դϴ�.
            float redOffset = percent * (selectWidth / 2); // ���������� �̵�
            float normalOffset = (1.0f - percent) * (-selectWidth / 2); // ���ʿ��� ����
            // ���⼭ �츮�� �� ���� �� ��ȯ ���̹Ƿ�, �켱 ���� �������� �������,
            // normal �������� ��Ÿ������ normalOffset�� ����մϴ�.
            offsetX = normalOffset;
        }
        else // NORMAL_STATE
        {
            // �Ϲ� �������� ������ �� �ڸ��� ��ġ�մϴ�.
            offsetX = 0;
        }

        // ��� �簢����, ������ �ڽ��� selectBarPos.x + offsetX�� �������� �����̵��
        RECT rect;
        rect.left = (LONG)(selectBarPos.x + offsetX - selectWidth / 2);
        rect.top = (LONG)(y - selectHeight / 2);
        rect.right = rect.left + (LONG)selectWidth;
        rect.bottom = rect.top + (LONG)selectHeight;

        // ������ �ڽ��� �׸��� (�ձ� �簢��; ���⼭ ��� ���� ���Ƿ� ����)
        RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 5, 5);

        // �� �������� �ش��ϴ� Token�� ��ġ�� �ڽ� �߾ӿ� ����ϴ�.
        // (Token Ŭ������ getSize�� �ؽ�Ʈ ũ�⸦ ��ȯ)
        SIZE textSize = currentSelects[i].second.getSize(hdc);
        FPOINT tokenPos;
        tokenPos.x = rect.left + ((selectWidth - textSize.cx) / 2.0f);
        tokenPos.y = y - 7.f; // �ణ ���� ����

        // ������ Token�� ��ġ ���� �� ������
        currentSelects[i].second.setPos(tokenPos);
        currentSelects[i].second.Render(hdc);

        // (Ŀ�� ǥ�ø� ����, ���� cursor == i, �ܰ����� �ٸ��� �׸� �� ����)
        // ���õ� �׸��� �β��� �Ķ� ������ �ܰ����� �׸���.
        if (cursor == i)
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            
            RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 5, 5);
            SelectObject(hdc, hOldPen);
            
            DeleteObject(hPen);
            
        }
    }
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

void OptionChat::DrawRedSelects(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    
    for (int i = 0; i < redSelects.size(); i++)
    {
        float y = selectBarPos.y + i * (selectHeight + selectGap);
        int left = (int)(currentSelectBarX - selectWidth / 2.0f);
        int top = (int)(y - selectHeight / 2.0f);
        int right = left + (int)selectWidth;
        int bottom = top + (int)selectHeight;
        
        if (cursor == i)
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
            HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
            RoundRect(hdc, left, top, right, bottom, 5, 5);
            SelectObject(hdc, oldPen);
            DeleteObject(hPen);
        }
        else
        {
            RoundRect(hdc, left, top, right, bottom, 5, 5);
        }

        SIZE textSize = redSelects[i].second.getSize(hdc);
        FPOINT tokenPos;
        tokenPos.x = currentSelectBarX - textSize.cx / 2.0f;
        tokenPos.y = y - textSize.cy / 2.0f;
        redSelects[i].second.setPos(tokenPos);
        redSelects[i].second.Render(hdc);
    }
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

void OptionChat::DrawNormalSelects(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    for (int i = 0; i < normalSelects.size(); i++)
    {
        float y = selectBarPos.y + i * (selectHeight + selectGap);
        int left = (int)(currentSelectBarX - selectWidth / 2.0f);
        int top = (int)(y - selectHeight / 2.0f);
        int right = left + (int)selectWidth;
        int bottom = top + (int)selectHeight;
        
        if (cursor == i)
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
            HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
            RoundRect(hdc, left, top, right, bottom, 5, 5);
            SelectObject(hdc, oldPen);
            DeleteObject(hPen);
        }
        else
        {
            RoundRect(hdc, left, top, right, bottom, 5, 5);
        }

        SIZE textSize = normalSelects[i].second.getSize(hdc);
        FPOINT tokenPos;
        tokenPos.x = currentSelectBarX - textSize.cx / 2.0f;
        tokenPos.y = y - textSize.cy / 2.0f;
        normalSelects[i].second.setPos(tokenPos);
        normalSelects[i].second.Render(hdc);
    }

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

void OptionChat::moveCursor(int way)
{
    cursor += way;
    if (cursor < 0)  cursor = (state == OptionState::RED || state == OptionState::TRANSITION ? redSelects.size() : normalSelects.size()) - 1;
    if (cursor >= (state == OptionState::RED || state == OptionState::TRANSITION ? redSelects.size() : normalSelects.size()))
        cursor = 0;
}

string OptionChat::selectCursor()
{
    vector<pair<string, Token>>& currentSelects =
        (state == OptionState::RED || state == OptionState::TRANSITION) ? redSelects : normalSelects;
    return currentSelects[cursor].first;
}

void ChatManager::Push(string key, string next, int pos, Chat* chat)
{
    chat->setPos(poses[pos]);
    chatMap.insert(make_pair(key, make_pair(chat, next)));
}

void ChatManager::startChat(string key)
{
    if (key == "END")
    {
        currentKey = "";
        currentChat = nullptr;
        nextChat = "END";
        return;
    }
    auto iter = chatMap.find(key);
    if (iter != chatMap.end())
    {
        currentKey = key;
        currentChat = (*iter).second.first;
        nextChat = (*iter).second.second;
    }
    else
    {
        currentKey = "";
        currentChat = nullptr;
        nextChat = "END";
    }
}



void ChatManager::Update()
{
    
    if (currentChat)
    {
        if (explodeFlag)
        {
            timer += TimerManager::GetInstance()->GetDeltaTime();
            if (timer >= 0.7f)
            {
                explodeFlag = false;
                timer = 0.f;
                startChat(tmpChat);
                if (currentChat == nullptr) return;
            }
        }
        currentChat->Update();
        if (currentChat->getStatus() != 0)
        {
            if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
            {
                currentChat->moveCursor(-1);
            }
            else if (KeyManager::GetInstance()->IsOnceKeyDown('S'))
            {
                currentChat->moveCursor(1);
            }
        }
        if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
        {
            if (currentChat->getStatus() != 0)
            {
                if (currentChat->getStatus() == 2)
                {
                    explodeFlag = true;
                    currentChat->makeExplode();
                    tmpChat = currentChat->selectCursor();
                    timer = 0;
                }
                else startChat(currentChat->selectCursor());
            }
            else
            {
                startChat(nextChat);
            }
        }
        RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::UI, this);
    }
    else
    {
        currentKey = "";
    }
    
}

void ChatManager::Render(HDC hdc)
{
    if (currentChat)
    {
        currentChat->Render(hdc);
    }
}

void ChatManager::Release()
{
    for (auto mp : chatMap)
    {
        delete mp.second.first;
        mp.second.first = nullptr;
    }
    chatMap.clear();
}

void ChatManager::LoadChat(const std::string& path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open()) return;
    json arr;
    ifs >> arr;

    for (auto& entry : arr) {
        int posIndex = entry["posIndex"].get<int>();

        // 2) Ű/�ؽ�Ʈ
        std::string key = entry["key"].get<std::string>();
        std::string next = entry["next"].get<std::string>();

        // 3) chat �ڽ� ũ��
        auto& cj = entry["chat"];
        float boxW = cj["boxWidth"].get<float>();
        float boxH = cj["boxHeight"].get<float>();

        // 4) tokens �Ľ�
        std::vector<std::pair<float, Token>> tokens;
        float timecalc = 0.f;
        for (auto& tj : cj["tokens"]) {
            float delay = tj["delay"].get<float>();
            std::string txt = tj["text"].get<std::string>();
            
            float x = tj["x"].get<float>();
            float y = tj["y"].get<float>();
            int appear_i = tj["appear"].get<int>();
            int option_i = tj["option"].get<int>();
            int color_i = tj["colors"].get<int>();

            // UTF-8 -> wide
            std::wstring wtxt = Utf8ToWstring(txt);
            timecalc += wtxt.size() * 0.08f;
            timecalc += delay;
            Token t(
                wtxt,
                { x, y },
                static_cast<Token::APPEAR>(appear_i),
                static_cast<Token::OPTION>(option_i),
                static_cast<Token::COLORS>(color_i)
            );
            tokens.emplace_back(delay, std::move(t));
        }

        // 5) ������ �Ľ� (norm / red)
        std::vector<std::pair<std::string, Token>> norm, red;
        for (auto& s : entry["norm"]) {
            std::string selNext = s["next"].get<std::string>();
            std::wstring wtxt = Utf8ToWstring(s["text"].get<std::string>());
            Token t(wtxt, { 0,0 }, Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::WHITE);
            norm.emplace_back(selNext, std::move(t));
        }
        for (auto& s : entry["red"]) {
            std::string selNext = s["next"].get<std::string>();
            std::wstring wtxt = Utf8ToWstring(s["text"].get<std::string>());
            Token t(wtxt, { 0,0 }, Token::APPEAR::END, Token::OPTION::STOP, Token::COLORS::RED);
            red.emplace_back(selNext, std::move(t));
        }

        timecalc += 0.7f;
        float redTime = entry.value("redTime", max(1.5f, timecalc));
        float totalTime = entry.value("totalTime", max(10.f, timecalc * 2.f));

        // 7) Chat vs OptionChat �б�
        if (norm.empty() && red.empty()) {
            Chat* ch = new Chat();
            ch->Init(tokens, boxW+10.f, boxH+10.f);
            Push(key, next, posIndex, ch);
        }
        else {
            OptionChat* oc = new OptionChat();
            oc->Init(tokens, boxW+10.f, boxH+10.f, redTime, totalTime, red, norm);
            Push(key, next, posIndex, oc);
        }
    }
}
