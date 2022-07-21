#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(const wchar_t* pFilename, Graphics* pGraphics)
{
	this->pGraphics = pGraphics;
	pBitmap = NULL;

	spritesAccross = 0;

	IWICImagingFactory* pWicFactory = NULL;

	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pWicFactory)))
	{
		
	}

	IWICBitmapDecoder* pWicDecoder = NULL;

	if (FAILED(pWicFactory->CreateDecoderFromFilename(pFilename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pWicDecoder)))
	{

	}

	IWICBitmapFrameDecode* pWicFrame = NULL;

	if (FAILED(pWicDecoder->GetFrame(0, &pWicFrame)))
	{

	}

	IWICFormatConverter* pWicConverter = NULL;

	if (FAILED(pWicFactory->CreateFormatConverter(&pWicConverter)))
	{

	}

	if (FAILED(pWicConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom)))
	{

	}

	if (FAILED(pGraphics->pGetRenderTarget()->CreateBitmapFromWicBitmap(pWicConverter, NULL, &pBitmap)))
	{

	}

	if (pWicFactory)pWicFactory->Release();
	if (pWicDecoder)pWicDecoder->Release();
	if (pWicConverter)pWicConverter->Release();
	if (pWicFrame)pWicFrame->Release();

	if (pBitmap)
	{
		spriteWidth = pBitmap->GetSize().width;
		spriteHeight = pBitmap->GetSize().height;
	}
	else
	{
		spriteWidth = spriteHeight = 0;
	}
}

SpriteSheet::SpriteSheet(const wchar_t* pFilename, Graphics* pGraphics, float spriteWidth, float spriteHeight) : SpriteSheet(pFilename, pGraphics)
{
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
	this->spritesAccross = static_cast<int>(pBitmap->GetSize().width / spriteWidth);
}

SpriteSheet::~SpriteSheet()
{
	if (pBitmap)pBitmap->Release();
}

void SpriteSheet::Draw()
{
	pGraphics->pGetRenderTarget()->DrawBitmap(
		pBitmap,
		D2D1::RectF(0.0f, 0.0f,
			pBitmap->GetSize().width, pBitmap->GetSize().height),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0.0f, 0.0f,
			pBitmap->GetSize().width, pBitmap->GetSize().height));
}

void SpriteSheet::Draw(int index, float x, float y)
{
	D2D_RECT_F src = D2D1::RectF(
		(float)((index % spritesAccross) * spriteWidth),
		(float)((index / spritesAccross) * spriteHeight),
		(float)((index % spritesAccross) * spriteWidth) + spriteWidth,
		(float)((index / spritesAccross) * spriteHeight) + spriteHeight);

	D2D_RECT_F dest = D2D1::RectF(x, y, x + spriteWidth, y + spriteHeight);

	pGraphics->pGetRenderTarget()->DrawBitmap(
		pBitmap,
		dest,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		src);
}