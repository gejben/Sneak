

#include "Graphics.hpp"
#include "Engine.hpp"
#include <cassert>

namespace GejbEngine {

	//private variables
	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 device = NULL;
	LPDIRECT3DSURFACE9 backbuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;
	std::string programTitle = "";
	LPD3DXFONT font = NULL;
	int colordepth = 32;
	bool transparent = true;
	D3DCOLOR ambientcolor = NULL;
	std::vector<LPDIRECT3DTEXTURE9> textureList(NULL);
	D3DCOLOR clearColor;

	void ShutdownGraphics() {
		if(spriteHandler) {
			spriteHandler->Release();
		}
		if(device) {
			device->Release();
		}
		if(d3d) {
			d3d->Release();
		}
	}

	bool InitGraphics(HWND hWnd, int width, int height, bool fullscreen,
										int colordepth) {
		ambientcolor = D3DCOLOR_RGBA(255, 255, 255, 0);
		clearColor = D3DCOLOR_RGBA(0, 0, 0, 255);
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(d3d == NULL) {

			FatalError("Failed to initialize Direct3D");
			return false;
		}

		//get default display-adapter
		D3DDISPLAYMODE dm;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

		//setup present-parameters
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = !fullscreen;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.hDeviceWindow = hWnd;

		//create device
		HRESULT result = d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&device);

		if(result != D3D_OK) {
			FatalError("Error creating Direct3D device");
			return false;
		}

		device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
		device->Clear(0, NULL, D3DCLEAR_TARGET, 0xFAFF, 1.0f, 0);

		//create font (arial)
		D3DXFONT_DESC FontDesc = { 24,
			0,
			400,
			0,
			false,
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_PITCH,
			"Arial"
		};

		D3DXCreateFontIndirect(device, &FontDesc, &font);

		//Create sprite-handler
		D3DXCreateSprite(device, &spriteHandler);
		if(result != D3D_OK) {
			return false;
		}

		return true;
	}

	bool BeginGraphics() {
		return device->BeginScene();
	}

	void EndGraphics() {
		device->EndScene();
		HRESULT hr = device->Present(0, 0, 0, 0);
		if(FAILED(hr)) {
			if(hr == D3DERR_DEVICELOST) {
				Reset();
			}
		}
	}

	bool Reset() {
		HRESULT hr;

		hr = device->TestCooperativeLevel();
		if(FAILED(hr) && hr != D3DERR_DEVICENOTRESET) {
			return false;
		}

		hr = spriteHandler->OnLostDevice();
		assert(SUCCEEDED(hr));

		hr = font->OnLostDevice();
		assert(SUCCEEDED(hr));

		D3DPRESENT_PARAMETERS presentParameters = { 0 };
		presentParameters.BackBufferWidth = getScreenWidth();
		presentParameters.BackBufferHeight = getScreenHeight();
		presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
		presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentParameters.Windowed = !getFullscreen();

		hr = device->Reset(&presentParameters);
		if(FAILED(hr)) {
			assert(false);

			return false;
		}

		if(!getFullscreen()) {
			AdjustWindow();
		}

		hr = spriteHandler->OnResetDevice();
		assert(SUCCEEDED(hr));

		hr = font->OnResetDevice();
		assert(SUCCEEDED(hr));

		return true;
	}

	void AdjustWindow() {
		DWORD windowStyle = GetWindowLong(getWindowHandle(), GWL_STYLE);

		RECT size = { 0, 0, getScreenWidth(), getScreenHeight() };
		AdjustWindowRect(&size, windowStyle, false);

		WINDOWPLACEMENT placement;
		GetWindowPlacement(getWindowHandle(), &placement);

		placement.rcNormalPosition.right =
			placement.rcNormalPosition.left + (size.right - size.left);
		placement.rcNormalPosition.bottom =
			placement.rcNormalPosition.top + (size.bottom - size.top);

		SetWindowPlacement(getWindowHandle(), &placement);
	}

	void ClearScreen(D3DCOLOR color) {
		device->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
	}

	void DrawString(std::string text, int x, int y) {

		RECT FontPosition;

		FontPosition.left = x;
		FontPosition.top = y;
		FontPosition.right = x + 20 * text.length();
		FontPosition.bottom = y + 30;
		font->DrawText(NULL,
									 text.c_str(),
									 -1,
									 &FontPosition,
									 DT_LEFT,
									 0xffffffff);
	}


	bool LoadSprite(std::string filename, Texture &texture, D3DCOLOR transcolor) {
		LPDIRECT3DTEXTURE9 d9texture = NULL;

		D3DXIMAGE_INFO info;
		HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
		if(result != D3D_OK) {
			return false;
		}

		result = D3DXCreateTextureFromFileEx(device, filename.c_str(), info.Width,
																				 info.Height, 1, D3DPOOL_DEFAULT, 
																				 D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
																				 D3DX_DEFAULT,D3DX_DEFAULT,
																				 transcolor, &info, NULL, &d9texture);

		if(result != D3D_OK) {
			return false;
		}

		textureList.push_back(d9texture);

		//ts up parameters in sprite for successfull load and for being a 
		//single-frame sprite that will not be animated
		texture.height = info.Height;
		texture.width = info.Width;
		texture.textureIndex = textureList.size() - 1;
		texture.imageLoaded = true;

		return true;
	}
	void ReleaseSprite(Texture &texture) {
		if(textureList.size() == 0) {
			return;   //workaround
		}

		if(textureList[texture.textureIndex]) {
			textureList[texture.textureIndex]->Release();
		}
		texture.textureIndex = 0;
		texture.imageLoaded = 0;
	}

	void DrawSprite(const Sprite *sprite) {
		if(sprite->getImageLoaded() == false) {
			return;
		}

		int textureIndex = sprite->getTextureIndex(); //debug

		//texture position
		D3DXVECTOR3 pos(sprite->getX(), sprite->getY(), 0);

		//source rectangle in image (e->g get right frame from sprite sheet)
		int fx = (sprite->getCurrentColumn() * sprite->getFrameWidth());
		int fy = (sprite->getCurrentRow() * sprite->getFrameHeight());
		RECT srcRect = { fx, fy, fx + sprite->getFrameWidth(), fy + sprite->getFrameHeight() };

		//draw sprite with or without alpha-channel transparency
		if(transparent) {
			spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		} else {
			spriteHandler->Begin(NULL);
		}

		TransformSprite(sprite);

		spriteHandler->Draw(textureList[sprite->getTextureIndex()], &srcRect, NULL, &pos,
												sprite->getColor());

		spriteHandler->End();
	}
	void TransformSprite(const Sprite *sprite) {
		D3DXMATRIX matrix;
		D3DXVECTOR2 scale(sprite->getScale(), sprite->getScale());
		D3DXVECTOR2 center(sprite->getX() + (sprite->getFrameWidth()*sprite->getScale()) / 2
											 , sprite->getY() + (sprite->getFrameHeight()*sprite->getScale()) / 2);
		D3DXVECTOR2 trans((float)sprite->getX(), (float)sprite->getY());
		D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, &center,
															 sprite->getRotation(), NULL/*&trans*/);

		spriteHandler->SetTransform(&matrix);
	}


	bool LoadTexture(std::string filename, Texture *te, D3DCOLOR transcolor) {
		LPDIRECT3DTEXTURE9 texture = NULL;

		D3DXIMAGE_INFO info;
		HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
		if(result != D3D_OK) {
			return false;
		}

		result = D3DXCreateTextureFromFileEx(device, filename.c_str(), info.Width,
																				 info.Height, 1, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, 
																				 D3DPOOL_DEFAULT, D3DX_DEFAULT,
																				 D3DX_DEFAULT, transcolor, &info, NULL, &texture);

		if(result != D3D_OK) {
			return false;
		}

		textureList.push_back(texture);

		te->height = info.Height;
		te->width = info.Width;
		te->textureIndex = textureList.size() - 1;
		te->imageLoaded = true;
		te->color = 0xffffffff;

		return true;
	}

	void ReleaseTexture(Texture *texture) {
		if(textureList[texture->textureIndex]) {
			textureList[texture->textureIndex]->Release();
		}
		texture->textureIndex = 0;
		texture->imageLoaded = false;
	}

	void DrawTexture(Texture *texture, int x, int y) {
		if(texture->imageLoaded == false) {
			return;
		}

		D3DXVECTOR3 pos(x, y, 0);
		//draw sprite with or without alpha-channel transparency
		if(transparent) {
			spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		} else {
			spriteHandler->Begin(NULL);
		}

		TransformTexture();

		spriteHandler->Draw(textureList[texture->textureIndex], NULL, NULL, &pos,
												texture->color);

		spriteHandler->End();
	}

	void TransformTexture() {
		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(&matrix, NULL, 0, NULL, NULL, 0, NULL);

		spriteHandler->SetTransform(&matrix);
	}

	int  getColorDepth() { return colordepth; }
	void setColorDepth(int value) { colordepth = value; }
	void setProgramTitle(std::string title) { programTitle = title; }
	void setTransparency(bool transparency) { transparent = transparency; }
	DWORD getClearColor() { return clearColor; }
	void setClearColor(DWORD value) { clearColor = value; }
}