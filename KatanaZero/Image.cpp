#include "Image.h"

HRESULT Image::Init(int width, int height)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::Empty;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    return S_OK;   // S_OK, E_FAIL
}

HRESULT Image::Init(const wchar_t* filePath, int width, int height, 
    bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(
        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::File;

    imageInfo->maxFrameX = 0;
    imageInfo->maxFrameY = 0;
    imageInfo->frameWidth = 0;
    imageInfo->frameHeight = 0;
    imageInfo->currFrameX = imageInfo->currFrameY = 0;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;   // S_OK, E_FAIL
}

HRESULT Image::Init(const wchar_t* filePath, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(
        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::File;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;
    imageInfo->currFrameX = imageInfo->currFrameY = 0;

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;   // S_OK, E_FAIL
}

HRESULT Image::Init(const wchar_t* filePath, bool isTransparent, COLORREF transColor, int FrameWidth, int FrameHeight)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(
        g_hInstance, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    BITMAP bmp;
    GetObject(imageInfo->hBitmap, sizeof(BITMAP), &bmp);

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, bmp.bmWidth, bmp.bmHeight);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    imageInfo->width = bmp.bmWidth;
    imageInfo->height = bmp.bmHeight;
    imageInfo->loadType = IMAGE_LOAD_TYPE::File;

    imageInfo->maxFrameX = FrameWidth == 0 ? 0 : bmp.bmWidth / FrameWidth;
    imageInfo->maxFrameY = FrameHeight == 0 ? 0 :bmp.bmHeight / FrameHeight;
    imageInfo->frameWidth = FrameWidth;
    imageInfo->frameHeight = FrameHeight;
    imageInfo->currFrameX = imageInfo->currFrameY = 0;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;   // S_OK, E_FAIL
}

void Image::Render(HDC hdc, int destX, int destY, float Scale)
{
   if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->width * Scale, imageInfo->height * Scale,
            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor);
    }
    else
    {
        StretchBlt(hdc, 
            destX, destY,
            imageInfo->width * Scale, imageInfo->height * Scale,
            imageInfo->hMemDC,
            0,0,
            imageInfo->width, imageInfo->height,
            SRCCOPY
        );

        //BitBlt(
        //    hdc,                // 복사 목적지 DC
        //    destX, destY,       // 복사 목적지 위치
        //    imageInfo->width * Scale,   // 원본에서 복사될 가로크기
        //    imageInfo->height * Scale,  // 원본에서 복사될 세로크기
        //    imageInfo->hMemDC,  // 원본 DC
        //    0, 0,               // 원본 복사 시작 위치
        //    SRCCOPY             // 복사 옵션
        //);
    }
}

void Image::Render(HDC hdc, int destX, int destY, int frameIndex, bool isFlip)
{
    imageInfo->currFrameX = frameIndex;

    if (isFlip && isTransparent)
    {
        StretchBlt(imageInfo->hTempDC, 0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hMemDC,
            (imageInfo->frameWidth * imageInfo->currFrameX) + (imageInfo->frameWidth - 1),
            imageInfo->frameHeight * imageInfo->currFrameY,
            -imageInfo->frameWidth, imageInfo->frameHeight,
            SRCCOPY
        );

        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->frameWidth, imageInfo->frameHeight,

            imageInfo->hTempDC,
            0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->frameWidth, imageInfo->frameHeight,

            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,
            destX, destY,
            imageInfo->width / 9,
            imageInfo->height,
            imageInfo->hMemDC,
            imageInfo->width / 9 * frameIndex, 0,
            SRCCOPY
        );
    }
}

void Image::FrameRender(HDC hdc, int destX, int destY, 
    int frameX, int frameY, bool isFlip, bool isCenter, float Scale)
{
    int x = destX;
	int y = destY;
	if (isCenter)
	{
        x = destX - ((imageInfo->frameWidth * Scale) / 2);
        y = destY - ((imageInfo->frameHeight * Scale) / 2);
	}

    imageInfo->currFrameX = frameX;
    imageInfo->currFrameY = frameY;


    if (isFlip && isTransparent)
    {
        StretchBlt(imageInfo->hTempDC, 0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hMemDC,
            (imageInfo->frameWidth * imageInfo->currFrameX) + (imageInfo->frameWidth - 1),
            imageInfo->frameHeight * imageInfo->currFrameY,
            -imageInfo->frameWidth, imageInfo->frameHeight,
            SRCCOPY
        );

        GdiTransparentBlt(hdc,
            x, y,
            imageInfo->frameWidth * Scale, imageInfo->frameHeight * Scale,
            imageInfo->hTempDC,
            0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            x, y,
            imageInfo->frameWidth * Scale, imageInfo->frameHeight * Scale,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,
            x, y,
            imageInfo->frameWidth * Scale,
            imageInfo->frameHeight * Scale,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX, 
            imageInfo->frameHeight * imageInfo->currFrameY,
            SRCCOPY
        );
    }
}

void Image::SourFrameRenderWidth(HDC hdc, int destX, int destY, int frameX, int frameY,
    float start, float end, bool isFlip, bool isCenter, float Scale)
{
    int x = destX * Scale;
    int y = destY * Scale;
    if (isCenter)
    {
        x = destX - ((imageInfo->frameWidth * Scale) / 2);
        y = destY - ((imageInfo->frameHeight * Scale) / 2);
    }

    imageInfo->currFrameX = frameX;
    imageInfo->currFrameY = frameY;
    start = max(0.f, start);
    end = min(1.f, end);
    if (start > end)return;
    float startX = imageInfo->frameWidth * start;
    float endX = imageInfo->frameWidth * end;

    if (isFlip && isTransparent)
    {
        StretchBlt(imageInfo->hTempDC, 0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hMemDC,
            (imageInfo->frameWidth * imageInfo->currFrameX) + (imageInfo->frameWidth - 1),
            imageInfo->frameHeight * imageInfo->currFrameY,
            -imageInfo->frameWidth, imageInfo->frameHeight,
            SRCCOPY
        );

        GdiTransparentBlt(hdc,
            x + startX, y,
            (endX - startX) * Scale, imageInfo->frameHeight * Scale,

            imageInfo->hTempDC,
            0, 0,
            endX - startX, imageInfo->frameHeight,
            transColor);

    }
    else if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            x + startX, y,
            (endX - startX) * Scale, imageInfo->frameHeight * Scale,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX + startX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            endX - startX, imageInfo->frameHeight,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,
            x + startX, y,
            (endX - startX)* Scale, imageInfo->frameHeight * Scale,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX + startX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            SRCCOPY
        );
    }
}



void Image::Release()
{
    if (imageInfo)
    {
        SelectObject(imageInfo->hTempDC, imageInfo->hOldTemp);
        DeleteObject(imageInfo->hTempBit);
        DeleteDC(imageInfo->hTempDC);

        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
        DeleteObject(imageInfo->hBitmap);
        DeleteDC(imageInfo->hMemDC);

        delete imageInfo;
        imageInfo = nullptr;
    }
}
