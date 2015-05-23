#ifndef OPENGLCONTEXT_H__
#define OPENGLCONTEXT_H__

#include "define.h"

#include <string>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include "array2d.h"


// Documentation de SFML: http://www.sfml-dev.org/documentation/index-fr.php
class OpenglContext
{
public:
    enum MOUSE_BUTTON {
        MOUSE_BUTTON_NONE       = 0x00,
        MOUSE_BUTTON_LEFT       = 0x01,
        MOUSE_BUTTON_MIDDLE     = 0x02,
        MOUSE_BUTTON_RIGHT      = 0x04,
        MOUSE_BUTTON_WHEEL_UP   = 0x08,
        MOUSE_BUTTON_WHEEL_DOWN = 0x10
    };
    OpenglContext();
    virtual ~OpenglContext();

    virtual void Init() = 0;
    virtual void DeInit() = 0;
    virtual void LoadResource() = 0;
    virtual void UnloadResource() = 0;
    virtual void Render(float elapsedTime) = 0;
    virtual void KeyPressEvent(unsigned char key) = 0;
    virtual void KeyReleaseEvent(unsigned char key) = 0;
    virtual void MouseMoveEvent(int x, int y) = 0;
    virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y) = 0;
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y) = 0;


    bool Start(const std::string& title, int width = 0, int height = 0);
    bool Stop();

    int Width() const;
    int Height() const;

    void SetMaxFps(int maxFps);
    int GetMaxFps() const;

    void SetFullscreen(bool fullscreen);
    bool IsFullscreen() const;

protected:
    void CenterMouse();
    void MakeRelativeToCenter(int& x, int& y) const;

    void ShowCursor();
    void HideCursor();
    void ShowCrossCursor() const;


private:
    MOUSE_BUTTON ConvertMouseButton(sf::Mouse::Button button) const;
	void InitWindow(int width = 0, int height = 0);


protected:
	sf::Window	m_app;
	int			m_renderDistance;
	float		m_cross_color_r, m_cross_color_g, m_cross_color_b;
	float		m_mouse_sensibility;

private:
    int			m_maxFps;
    bool		m_fullscreen;
	
    std::string m_title;
	float m_lastFrameTime;


};

#endif // OPENGLCONTEXT_H__
